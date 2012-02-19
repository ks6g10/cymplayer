#include<stdio.h>
#include<curl/curl.h>
#include<string.h>



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
		curl_easy_setopt(curl,CURLOPT_URL,YOUTUBE);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,file);
		retur = curl_easy_perform(curl);
		fclose(file);
		curl_easy_cleanup(curl);
	}
	
	file = fopen(Videofile,"r");
	if(file == NULL)
		return;
	size_t buff = 300;
	size_t * buffptr = &buff;
	char * line = malloc(sizeof(char)*buff);
	char ** lineptr = &line;
	int i = 0;
	while(getline(lineptr,buffptr,file) > 0)
	{
		/* if(strncmp(*lineptr,"\n",1) == 0) */
		/*    continue; */
		/* if(strncmp(*lineptr,"<",1) == 0) */
		/*    continue; */
		/* if(strncmp(*lineptr,"'",1) == 0) */
		/*    continue; */
		/* if(strncmp(*lineptr,"        i",9) != 0) */
		/*    continue; */
		i++;

		char * src = strstr(*lineptr,"url_encoded_fmt_stream_map");
		if(src == NULL)
			continue;
		src = strstr(src,"itag");
//		tmp = strstr(tmp,"itag");
		 char * next = src;

		while((next = strstr(src,"itag")) != NULL) 
		{
			while(src != next)
				putchar(*src++);
			putchar('\n');
			putchar('\n');
			src += 4;
		}
//		   if(strcmp(*lineptr,"</html>"))
//		      return;
	       
	}
	printf("\n count %d\n",i);
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
