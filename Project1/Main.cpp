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


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::stringstream*)userp)->write((char*)contents, size * nmemb);
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

int main(void)
{
	CURL *curl;
	CURLcode res;
	std::stringstream readBuffer;

	propt::ptree root;

	

	const char *URL = "https://stream.twitter.com/1.1/statuses/sample.json";
	std::string consumer_key = "ief6hfiCDXFt8qxHpftdRQZK8"; // Key from Twitter
	std::string consumer_secret = "DzR9AOhz4PC2dYiBoP3qaIb6vPefXBCYPqyfEOTt1jJWrE6U5I"; // Secret from Twitter
	std::string request_token_url = "https://api.twitter.com/oauth/request_token";
	std::string request_token_query_args = "oauth_callback=oob";
	std::string authorize_url = "https://api.twitter.com/oauth/authorize";
	std::string access_token_url = "https://api.twitter.com/oauth/access_token";



	OAuth::Consumer consumer(consumer_key, consumer_secret);
	OAuth::Client oauth(&consumer);

	std::string base_request_token_url = request_token_url + (request_token_query_args.empty() ? std::string("") : (std::string("?") + request_token_query_args));
	std::string oAuthQueryString = oauth.getURLQueryString(OAuth::Http::Get, base_request_token_url);


	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		propt::read_json(readBuffer, root);

		print(root);

		std::cin.get();
	}
	return 0;
}