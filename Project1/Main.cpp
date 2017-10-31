#include <iostream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <liboauthcpp\liboauthcpp.h>
#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace propt = boost::property_tree;




static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	if (userp && contents) {

		((std::string*)userp)->assign(contents, (size * nmemb));
	}
	return size * nmemb;
}



void print(propt::ptree const& pt)
{
	using boost::property_tree::ptree;
	ptree::const_iterator end = pt.end();
	for (ptree::const_iterator it = pt.begin(); it != end; ++it) {
		std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
		print(it->second);
	}
}

std::string getUserString(std::string prompt) {
	std::cout << prompt << " ";

	std::string res;
	std::cin >> res;
	std::cout << std::endl;
	return res;
}

int main(void)
{
	CURL *curl;
	CURLcode res;
	std::string* response;

	std::stringstream readBuffer;

	propt::ptree root;



	std::string URL = "https://stream.twitter.com/1.1/statuses/sample.json?track=VR";
	std::string consumer_key = "ief6hfiCDXFt8qxHpftdRQZK8"; // Key from Twitter
	std::string consumer_secret = "DzR9AOhz4PC2dYiBoP3qaIb6vPefXBCYPqyfEOTt1jJWrE6U5I"; // Secret from Twitter
	std::string request_token_url = "https://api.twitter.com/oauth/request_token";
	std::string request_token_query_args = "oauth_callback=oob";
	std::string authorize_url = "https://api.twitter.com/oauth/authorize";
	std::string access_token_url = "https://api.twitter.com/oauth/access_token";
	std::string access_token_key = "79369522-keYvdBfHdH1EKzXRrzUxCHKaY9iFNd98zDDZ4AKxO";
	std::string access_token_secret = "ILsjrudsMGfyPYDgZetZwuluNhg6eAluvbvlASTOvbVQf";

	
	OAuth::Token access_token = OAuth::Token(access_token_key, access_token_secret);
	OAuth::Consumer consumer(consumer_key, consumer_secret);
	OAuth::Client oauth = OAuth::Client(&consumer, &access_token);
	std::string curlURL = oauth.getHttpHeader(OAuth::Http::Get, URL, std::string(""));
	
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (!curl) {
		std::cout << "[ERROR] curl_easy_init" << std::endl;
		curl_global_cleanup();
		return 0;
	}
	curl_easy_setopt(curl, CURLOPT_URL, curlURL);

	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		// ==== Execute
	res = curl_easy_perform(curl);
		/* Check for errors */
	if (res != CURLE_OK)
		std::cout << "\n\n[ERROR] curl_easy_perform() failed: " << res << std::endl;

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	readBuffer << *response;
	propt::read_json(readBuffer, root);

	print(root);

	std::cin.get();
	return 0;

}