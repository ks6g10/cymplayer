#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>

#define UNLENGTH (strlen(USERNAME))

int main() 
{
	const char * SETTINGS_PATH = "./.cymplayer/settings";
	const char * DIR_PATH = "./.cymplayer";
	const char READ = 'r';
	const char WRITE = 'w';
	const char * USERNAME = "Username=";
	size_t buffer = 200; 
	size_t * buffptr = &buffer;

	char * input = (char *) (malloc((sizeof(char))*buffer));
	char ** inputptr = &input;
	FILE *settings;
	DIR * settings_dir;
	int ret = 0;
	settings_dir = opendir(DIR_PATH);
//	settings = fopen(path,"r");

	if(settings_dir == NULL)
	{
		printf("Directory %s does not exist\n",DIR_PATH);
		ret = mkdir(DIR_PATH,S_IRWXU);
		if(ret == -1) {
			printf("Could not create directory, quits");
			exit(EXIT_FAILURE);	
		}
	}
	closedir(settings_dir);
		

	settings = fopen(SETTINGS_PATH,&READ);
	if(settings == NULL)
	{
		printf("File %s does not exist\n",SETTINGS_PATH);
		settings = fopen(SETTINGS_PATH,&WRITE);
		if(settings == NULL) {
			printf("Could not write file %s\n",SETTINGS_PATH);
			exit(EXIT_FAILURE);
		}
		fprintf(settings,"%s%s\n",USERNAME,"pojk3n");

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
