#include<stdio.h>
#include<curl/curl.h>
#include<string.h>
#include<stdlib.h>
#include"http.h"
#include"filehandler.h"
#define YOUTUBELEN strlen(YOUTUBE)
#define CURLINITFAIL 2
#define PRELEN strlen(PREFIX)
#define JPGLEN strlen(JPG)
#define FILEPATHLEN (strlen(VIDEOFILE)+SETTINGSPATHLEN)
#define SETTINGSPATHLEN (strlen(settingsdir))
const char * NEWSUB = "/newsubscriptionvideos?v=2";
const char * GDATA = "http://gdata.youtube.com/feeds/api/users/";
const char * ERR = "Something went wrong while fetching files, make sure your internet connection is working\n";
const char * XMLFILE = "/newsub.xml";
const char * FEEDUSER = "http://gdata.youtube.com/feeds/api/users/";
const char * YOUTUBE = "http://www.youtube.com/watch?v=";
const char * JPG = ".jpg";
const char * YTIMGPRE = "http://i4.ytimg.com/vi/";
const char * DEF = "/default.jpg";
const char * VIDEOFILE = "tmpvideo.html";
static char * settingsdir;

char * get_newsub(char * username);

/*@youtubeid the id of a youtube video, get it from entry->fields[ID]
 *@return either the const videofile or NULL if error occur.
 *It is used to download the html file of a youtube video.
 */
const char * get_youtubehtml(char * youtubeid)
{
	if(!settingsdir)
		settingsdir = get_settingsdir();
	
	char * url = malloc(strlen(youtubeid)+YOUTUBELEN+1);
	sprintf(url,"%s%s",YOUTUBE,youtubeid);
	printf("%s\n",url);

	char * filename = calloc(1,FILEPATHLEN+sizeof(char));
	sprintf(filename,"%s%s",settingsdir,VIDEOFILE);
	CURL *curl;
	CURLcode retur;
	FILE * file;

	printf("%s\n",url);

	
	curl = curl_easy_init();
	if(!curl)
	{
		return (char *) NULL;
	}
	file = fopen(filename,"w");
	curl_easy_setopt(curl,CURLOPT_URL,url);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,file);
	retur = curl_easy_perform(curl);
	if(retur != 0) {
		fprintf(stderr,ERR);
		filename = (const char * ) NULL;
	}
	
	fclose(file);
	curl_easy_cleanup(curl);
	
	free(url);
	return (char *) filename;
}




void get_thumb_filename(entry * argentry)
{
	if(!settingsdir)
		settingsdir = get_settingsdir();	
	const char * PREFIX = "/";
	const char * JPG = ".jpg";
	char * id = argentry->fields[ID];
	size_t idlen = strlen(id);
	char * filename = malloc(SETTINGSPATHLEN+PRELEN+idlen+JPGLEN+sizeof(char));
	sprintf(filename,"%s%s%s%s",settingsdir,PREFIX,id,JPG);
	argentry->fields[THUMBLOC] = filename;
//	strcpy(filename,PREFIX);
//	strcpy((filename+PRELEN),id);
//	strcpy((filename+PRELEN+idlen),JPG);	
}


/*@rootentry The root struct entry to download the thumb from, needs the youtube id.
 *@return should return different depending on if it sucessfull, not checked right now
 *Downloads thumb images for each video. Type jpeg.
 */

int get_thumbs(entry * rootentry)
{
	const size_t YTIMGLEN = strlen(YTIMGPRE);
	const size_t DEFLEN = strlen(DEF);
	
	CURL *curl;
	CURLcode retur;
	size_t idlen;
	entry * current = rootentry;
	char * link = malloc(DEFLEN+YTIMGLEN+15*sizeof(char));
	FILE * file;
	printf("title %s",rootentry->fields[TITLE]);

	curl = curl_easy_init();
	if(!curl)
		return CURLINITFAIL;
	while(current->next != NULL)
	{
		idlen = strlen(current->fields[ID]);
		get_thumb_filename(current);

		if((file = fopen(current->fields[THUMBLOC],"r"))) 
		{
			fclose(file);
			current = current->next;
			continue;	
		} 
		
		file = fopen(current->fields[THUMBLOC],"w");

		sprintf(link,"%s%s%s",YTIMGPRE,current->fields[ID],DEF);
		fprintf(stdout, "Fetching %s from %s\n",current->fields[THUMBLOC],link);
		
		curl_easy_setopt(curl,CURLOPT_URL,link);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,file);
		retur = curl_easy_perform(curl);
		fclose(file);
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

char * get_newsub(char * username) 
{
	if(!settingsdir)
		settingsdir = get_settingsdir();	
		
	CURL *curl;
	CURLcode retur;
	FILE* file;
	char * filepath =(char *) malloc((strlen(settingsdir)+strlen(XMLFILE)+sizeof(char)));
	sprintf(filepath,"%s%s",settingsdir,XMLFILE);
	printf("filepath %s\n",filepath);
	char * fullurl = (char *) malloc(strlen(GDATA)+strlen(username)+strlen(NEWSUB)+sizeof(char));
	sprintf(fullurl,"%s%s%s",GDATA,username,NEWSUB);

	curl = curl_easy_init();
	if(curl) 
	{
		fprintf(stdout,"Downloading the newsubscription xml %s\n",fullurl);
		file  = fopen(filepath, "w");
		curl_easy_setopt(curl,CURLOPT_URL,fullurl);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,file);
		retur = curl_easy_perform(curl);
		if(retur != 0) {
			fclose(file);
			fprintf(stderr,ERR);
			return ( char *) NULL;
		}
	}
	fclose(file);
	curl_easy_cleanup(curl);
	return (char *) filepath;
}
