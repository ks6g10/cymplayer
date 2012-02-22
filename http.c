#include<stdio.h>
#include<curl/curl.h>
#include<string.h>
#include<stdlib.h>
#include"http.h"
#define YOUTUBELEN strlen(YOUTUBE)
#define CURLINITFAIL 2
#define PRELEN strlen(PREFIX)
#define JPGLEN strlen(JPG)

const char * NEWSUB = "/newsubscriptionvideos?v=2";
const char * FULL = "http://gdata.youtube.com/feeds/api/users/p0jk3n/newsubscriptionvideos?v=2";
const char * ERR = "Something went wrong while fetching files, make sure your internet connection is working\n";
const char * NAME = "newsub.xml";
const char * FEEDUSER = "http://gdata.youtube.com/feeds/api/users/";
const char * YOUTUBE = "http://www.youtube.com/watch?v=";
const char * JPG = ".jpg";
const char * YTIMGPRE = "http://i4.ytimg.com/vi/";
const char * DEF = "/default.jpg";
const char * VIDEOFILE = "tmpvideo.html";

const char * getNewsub(char * username);


/*@youtubeid the id of a youtube video, get it from entry->fields[ID]
 *@return either the const videofile or NULL if error occur.
 *It is used to download the html file of a youtube video.
 */
const char * getYoutubeHtml(char * youtubeid)
{
	char * url = malloc(strlen(youtubeid)+YOUTUBELEN+1);
	sprintf(url,"%s%s",YOUTUBE,youtubeid);
//	strcpy(url,YOUTUBE);
//	strcpy((url+YOUTUBELEN),youtubeid);
	printf("%s\n",url);
//	return NULL;
	const char * filename = VIDEOFILE;
	CURL *curl;
	CURLcode retur;
	FILE * file;

	
	
	curl = curl_easy_init();
	if(!curl)
	{
		return (char *) NULL;
	}
	file = fopen(filename,"w");
	curl_easy_setopt(curl,CURLOPT_URL,YOUTUBE);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,file);
	retur = curl_easy_perform(curl);
	if(retur != 0) {
		fprintf(stderr,ERR);
		filename = (const char * ) NULL;
	}
	
	fclose(file);
	curl_easy_cleanup(curl);
	
	free(url);
	return (const char *) filename;
}




char * getfilename(entry * argentry)
{
	const char * PREFIX = "./";
	const char * JPG = ".jpg";
	char * id = argentry->fields[ID];
	size_t idlen = strlen(id);
	char * filename = malloc(idlen+JPGLEN+PRELEN+sizeof(char));
	sprintf(filename,"%s%s%s",PREFIX,id,JPG);
//	strcpy(filename,PREFIX);
//	strcpy((filename+PRELEN),id);
//	strcpy((filename+PRELEN+idlen),JPG);	
	return (char *) filename;
}


/*@rootentry The root struct entry to download the thumb from, needs the youtube id.
 *@return should return different depending on if it sucessfull, not checked right now
 *Downloads thumb images for each video. Type jpeg.
 */

int getThumbs(entry * rootentry)
{
	const size_t YTIMGLEN = strlen(YTIMGPRE);
	const size_t DEFLEN = strlen(DEF);
	
	CURL *curl;
	CURLcode retur;
	size_t idlen;
	entry * current = rootentry;
	char * filename;// = strcat(rootentry->fields[ID],".jpg");
	char * link = malloc(DEFLEN+YTIMGLEN+15*sizeof(char));
	FILE * file;
	

	curl = curl_easy_init();
	if(!curl)
		return CURLINITFAIL;
	while(current->next != NULL)
	{
		idlen = strlen(current->fields[ID]);
		filename = getfilename(current);

		if((file = fopen(filename,"r"))) 
		{
			fclose(file);
			current = current->next;
			continue;	
		} 
		
		file = fopen(filename,"w");

		sprintf(link,"%s%s%s",YTIMGPRE,current->fields[ID],DEF);
		fprintf(stdout, "Fetching %s from %s\n",filename,link);
		
		curl_easy_setopt(curl,CURLOPT_URL,link);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,file);
		retur = curl_easy_perform(curl);
		fclose(file);
		free(filename);
		current = current->next;
	}
	curl_easy_cleanup(curl);
	free(link);
	return 0;
}


/* Downloads the xml file for new subscriptions
 *@username the youtube username to fetch data from.
 *@return returns NULL if there is problem donwloading, else returns the const char * to NAME
 */

const char * getNewsub(char * username) 
{
	
	CURL *curl;
	CURLcode retur;
	FILE* file;
	
	curl = curl_easy_init();
	if(curl) 
	{
		fprintf(stdout,"Downloading the newsubscription xml\n");
		file  = fopen(NAME, "w");
		curl_easy_setopt(curl,CURLOPT_URL,FULL);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,file);
		retur = curl_easy_perform(curl);
		if(retur != 0) {
			fclose(file);
			fprintf(stderr,ERR);
			return (const char *) NULL;
		}
	}
	fclose(file);
	curl_easy_cleanup(curl);
	return (const char *) NAME;
}
