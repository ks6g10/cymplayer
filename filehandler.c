#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>

#define UNLENGTH (strlen(USERNAME))

const char * SETTINGS_PATH = "./.cymplayer/settings";
const char * DIR_PATH = "./.cymplayer";
const char * USERNAME = "Username=";

char * getUsername() 
{
	size_t buffer = 200; 
	size_t * buffptr = &buffer;

	char * input = (char *) (malloc((sizeof(char))*buffer));
	char ** inputptr = &input;
	FILE *settings;
	DIR * settings_dir;
	int ret = 0;

	if(settings == NULL)
	{
		printf("File %s does not exist\n",SETTINGS_PATH);
		free(input);
		return NULL;
	}
	else
	{
		int i =0;
		int ret;
		ret = getline(inputptr,buffptr,settings);
		if(strncmp(*inputptr,USERNAME,UNLENGTH) == 0) {
			printf("Your username = ");
			for(i = UNLENGTH;i < ret; i++)
				printf("%c",*(&(*inputptr)[i]));
		}
	}

	fclose(settings);
	return 0;
}

void writeSetting(char * username) {
	FILE * settings;
	DIR * settings_dir;
	settings_dir = opendir(DIR_PATH);
//	settings = fopen(path,"r");
	if(settings_dir == NULL)
	{
		fprintf(stdout,"Directory %s does not exist\n",DIR_PATH);
		ret = mkdir(DIR_PATH,S_IRWXU);
		if(ret == -1) {
			fprintf(stderr,"Could not create directory, quits\n");
			exit(EXIT_FAILURE);	
		}
	}
	closedir(settings_dir);
		
	settings = fopen(SETTINGS_PATH,&WRITE);
	if(settings == NULL) {
		fprintf(stderr,"Could not write file %s\n",SETTINGS_PATH);
		exit(EXIT_FAILURE);
	}
	fprintf(settings,"%s%s\n",USERNAME,"pojk3n");
	fclose(settings);
}
