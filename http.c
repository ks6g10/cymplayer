#include<stdio.h>
#include<curl/curl.h>

void main() {

	const char * URL = "http://gdata.youtube.com/feeds/api/users/";
	const char * NEWSUB = "/newsubscriptionvideos?v=2";
	const char * FULL = "http://gdata.youtube.com/feeds/api/users/p0jk3n/newsubscriptionvideos?v=2";
	const char * NAME = "newsub.xml";
	CURL *curl;
	CURLcode retur;
	FILE* file;
	
	curl = curl_easy_init();
	if(curl) 
	{
		file  = fopen(NAME, "w");
		curl_easy_setopt(curl,CURLOPT_URL,FULL);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,file);
		retur = curl_easy_perform(curl);
		fclose(file);
		curl_easy_cleanup(curl);
	}



}
