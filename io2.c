#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char *replace_str(char *str, char *orig, char *rep);

void main()
{
	const char * Videofile = "youtube.html";
	size_t buff = 4096;
	size_t * buffptr = &buff;
	char * line = malloc(sizeof(char)*buff);
	char ** lineptr = &line;
	FILE * file;

	int i = 0;
	file = fopen(Videofile,"r");
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
		const char * REG = "url_encoded_fmt_stream_map";
		char * src = strstr(*lineptr,REG);
		if(src == NULL)
			continue;
		src = strstr(src,"itag");
//		tmp = strstr(tmp,"itag");
		char * next = src;
		char * tmp;// = malloc(sizeof(char)*buff);
		char * tmp2;

		while((next = strstr(src,",")) != NULL) 
		{
			if(!strstr(src,"itag")) {
				src = next;
				src++;
				continue;
			}

			tmp = calloc(1,next-src+1);
			//while(src != next)
			//	putchar(*src++);
			strncpy(tmp,src,next-src);
			src = next;
			tmp2 = tmp;
			tmp2 = replace_str(tmp2,"url=http%3A%2F%2F", "http://");
			tmp2 = replace_str(tmp2,"%3F","?");
			//			tmp2 = replace_str(tmp2,"%2F","/");
			//	tmp2 = replace_str(tmp2,"%3D","=");
			//	tmp2 = replace_str(tmp2,"%26","&");

			printf("%s",tmp2);
			printf("\nnewline\n");
			putchar('\n');
			free(tmp);
			src += 1;
		}
//		   if(strcmp(*lineptr,"</html>"))
//		      return;
	       
	}
//	printf("\n count %d\n",i);
}


char *replace_str(char *str, char *orig, char *rep)
{
	static char buffer[4096];
	char *ptrfirst;
	char * src;
	src = str;
	char * buffptr = &buffer;
	char * tmpptr;
	size_t sizer = strlen(rep);
	size_t sizeo = strlen(orig);
	size_t move = 0;

	while((ptrfirst = strstr(src, orig)))  // Is 'orig' even in 'str'?
	{
		move = ptrfirst-src;
//		printf("%ld\n",move);
		strncpy(buffptr,src,move);
		buffptr += move;
		src += move+sizeo;
		strncpy(buffptr,rep,sizer);
		buffptr += sizer;
		tmpptr = ptrfirst;
	}	
	strncpy(buffptr,src,strlen(str)-(str-src));
	
	
  return buffer;
}
