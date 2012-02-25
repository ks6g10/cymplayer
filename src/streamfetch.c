/*
 * io2.c
 *
 * Copyright (C) 2012 - Kim Svensson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include"streamfetch.h"
#include"http.h"
#define URLLEN strlen(URL)
#define ITAGLEN strlen(ITAG)
#define COMMALEN strlen(COMMA)
#define ARGLEN strlen(argString)
const char * ITAG = "itag%3D";
const char * URL = "url%3D";
const char * COMMA = "%2C";
const char * REG = "url_encoded_fmt_stream_map";
void put_url_stream(streamstruct * argStruct, int resolution, char * url);

/*str_replace derived from jmucchiello and Samuel_xL at stackoverflow
 * http://stackoverflow.com/questions/779875/what-is-the-function-to-replace-string-in-c
 * Modified 2012-Feb
 */
// You must free the result if result is non-NULL.
void str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    char *start;
    size_t len_rep;  // length of rep
    size_t len_with; // length of with
    size_t len_front; // distance between rep and end of last rep
    uint8_t count;    // number of replacements
//    printf("NULL%d\n",(orig==NULL));
    if (!orig)
	    return;
    if (!rep || !(len_rep = strlen(rep)))
        return;
    if (!(ins = strstr(orig, rep))) 
        return;
    if (!with)
        with = "";
    len_with = strlen(with);
    start=orig;

    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = realloc(tmp,strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
	    return;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strncpy(tmp, with,len_with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    strcpy(start,result);
    free(result);
//    free(orig);
}


int get_resolution(char * argString)
{
	char * itagnum;
	size_t itaglen = ITAGLEN;
	uint8_t ret = 0;
	if(argString == NULL)
		return 0;
	size_t numlen = strlen(argString)-ITAGLEN-COMMALEN;
	itagnum = malloc((numlen+1)*sizeof(char));
	strncpy(itagnum,(argString+ITAGLEN),numlen);
	*(itagnum+numlen) = 0;
	ret = atoi(itagnum);
	free(itagnum);
	return ret;
}

char * cut_string(char * orig, const char * cutfrom) {
	char * cutfromptr = strstr(orig,cutfrom);
	size_t size = cutfromptr-orig;
	char * newstring =(char *) calloc(1,(size+1));
	strncpy(newstring,orig,size);
	free(orig);
	return newstring;
}





streamstruct *  get_urlstruct(const char * filename)
{


	const char * Videofile = filename;
	size_t buff = 4096;
	size_t * buffptr = &buff;
	char * line = malloc(sizeof(char)*buff);
	char ** lineptr = &line;
	FILE * file;
	streamstruct * streamptr =(streamstruct *) malloc(sizeof(streamstruct));
	int i = 0;
	file = fopen(Videofile,"r");

	while(getline(lineptr,buffptr,file) > 0)
	{

		char * next;
		char * tmp;// = malloc(sizeof(char)*buff);
		char * src;
		int ret;
		if(strncmp(*lineptr,"\n",1) == 0)
			continue; 
		if(strncmp(*lineptr,"<",1) == 0) 
			continue;
		/* if(strncmp(*lineptr,"'",1) == 0) */
		/*    continue; */
		i++;

		src = strstr(*lineptr,REG);

		if(src == NULL)
			continue;
		src += strlen(REG);
		src = strstr(src,"url");
//		tmp = strstr(tmp,"itag");
		next  = src;

		
		while((next = strstr(src,URL)) != NULL) 
		{

			tmp = calloc(1,next-src+1);
			strncpy(tmp,src,next-src);
			src = next;
			if(*tmp == 0)
			{
				src++;
				free(tmp);
				continue;
			}
			
			str_replace(tmp,"rl%3D","");
			str_replace(tmp,"%252F","/");
			str_replace(tmp,"%25252C",",");
			str_replace(tmp,"%253D","=");
			str_replace(tmp,"%253A",":");
			str_replace(tmp,"%253F","?");
			str_replace(tmp,"%2526","&");

			ret = get_resolution(strstr(tmp,ITAG));
			tmp = cut_string(tmp,"%26quality");
//			printf("%s  %d\n\n",tmp,ret);
			put_url_stream(streamptr,ret,tmp);
			//free(tmp);
			src += URLLEN;
		}
//		printf("count %d \n",i);
		free(line);
		fclose(file);
		return streamptr;	       
	}
}

void put_url_stream(streamstruct * argStruct, int resolution, char * url)
{
//	printf("insideput\n");
		
	if(resolution == MP480)
	{
//		printf("inside 480p");
		argStruct->mp480 = url;
	} 
	else if(resolution == MP720)
	{
		argStruct->mp720 = url;
	}
	else if(resolution == MP1080)
	{
		argStruct->mp1080 = url;
	}
}
