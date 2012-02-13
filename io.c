#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>

int main() 
{
	const char SETTINGS_PATH[] = "./.cymplayer/settings";
	const char DIR_PATH[] = "./.cymplayer";
	const char READ = 'r';
	const char WRITE = 'w';
	const char USERNAME[] = "Username=";
	unsigned int *  charcnt; 
	*charcnt = 200;
	char * path = &DIR_PATH;
	char * input = (char *) (malloc(sizeof(char)*charcnt));
	char ** inputptr = &input;
	FILE *settings;
	DIR * settings_dir;
	int ret = 0;
	settings_dir = opendir(path);
//	settings = fopen(path,"r");

	if(settings_dir == NULL)
	{
		printf("Directory %s does not exist\n",DIR_PATH);
		ret = mkdir(path,S_IRWXU);
		if(ret == -1) {
			printf("Could not create directory, quits");
			exit(2);	
		}
	}
	closedir(settings_dir);
		
	path = &SETTINGS_PATH;
	settings = fopen(path,&READ);
	if(settings == NULL)
	{
		printf("File %s does not exist\n",SETTINGS_PATH);
		settings = fopen(path,&WRITE);
		if(settings == NULL) {
			printf("Could not write file %s\n",SETTINGS_PATH);
			exit(2);
		}
		fprintf(settings,"%s\n",USERNAME);

	}
	else
	{
		getline(inputptr,charcnt,settings);
	}

	fclose(settings);
	return 0;


}
