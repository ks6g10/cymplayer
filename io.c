#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>

int main() 
{
	const char SETTINGS_PATH[] = "./.cymplayer/settings";
	const char DIR_PATH[] = "./.cymplayer";
	const char READ = 'r';
	char * operation = &READ;
	char * path = &DIR_PATH;
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
	settings = fopen(path,operation);
	if(settings == NULL){
		printf("File %s does not exist\n",SETTINGS_PATH);
		

	}

	return 0;


}
