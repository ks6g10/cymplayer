#include<stdio.h>
#include<curl/curl.h>
#include<string.h>
#include "reader1.h"

#define YOUTUBELEN strlen(YOUTUBE)

static const char * FEEDUSER = "http://gdata.youtube.com/feeds/api/users/";
static const char * YOUTUBE = "http://www.youtube.com/watch?v=";

FILE * getYoutubeHtml(char * youtubeid)
{
	char * url = malloc(strlen(youtubeid)+YOUTUBELEN+1);
	strcpy(url,YOUTUBE);
	strcpy((url+YOUTUBELEN),youtubeid);
	printf("%s\n",url);
	return NULL;
	const char * Videofile = "tmpvideo.html";
	CURL *curl;
	CURLcode retur;
	FILE * file;

	curl = curl_easy_init();
	if(curl)
	{
		file = fopen(Videofile,"w");
		curl_easy_setopt(curl,CURLOPT_URL,YOUTUBE);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,file);
		retur = curl_easy_perform(curl);
		fclose(file);
		curl_easy_cleanup(curl);
	}
}

void main() {
	FILE * newsub = getNewsub("p0jk3n");
	entry * rootentry = getRootentry(newsub);

}

void getThumbs(entry * rootentry)
{
	CURL *curl;
	CURLcode retur;
	entry * current = rootentry;
	char * name = strcat(rootentry->fields[ID],".jpg");
	curl = curl_easy_init();
	while(current->next != NULL)
	{
		

	}

}

FILE * getNewsub(char * username) 
{
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
