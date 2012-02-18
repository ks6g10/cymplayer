#include<stdio.h>
#include<curl/curl.h>




static const char * FEEDUSER = "http://gdata.youtube.com/feeds/api/users/";

void main() {
	const char * YOUTUBE = "http://www.youtube.com/watch?v=23BULaahgZY";
	const char * Videofile = "tmpvideo.html";
	CURL *curl;
	CURLcode retur;
	FILE * file;

	curl = curl_easy_init();
	if(curl)
	{
		file = fopen(Videofile,"w");



	}
	
	



}


FILE * getnewsub() {
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
