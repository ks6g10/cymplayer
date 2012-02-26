#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>

#define USERLEN (strlen(USERNAME))
#define HOMEDIRLEN (strlen(homedir)+strlen(DIR_PATH))
#define SETTINGLEN (strlen(SETTINGS))

const char * SETTINGS = "/settings";
const char * DIR_PATH = "/.cymplayer";
const char * USERNAME = "Username=";
static char * homedir;
static char * settingsdir;

char * getusername();
void writesetting(char * username);


void init_filehandler()
{

	uid_t userid = getuid();
	struct passwd * userpasswd = getpwuid(userid);
	homedir = userpasswd->pw_dir;
	settingsdir = (char *) calloc(1,HOMEDIRLEN+sizeof(char));
	sprintf(settingsdir,"%s%s",homedir,DIR_PATH);
	if(!(homedir))
	{
		fprintf(stderr,"Could not aquire homedir directory");
		exit(2);

	}
	
}

char * get_settingsdir()
{
	if(!settingsdir)
		init_filehandler();
	return settingsdir;
}

char * get_username() 
{
	size_t buffer = 200; 
	size_t * buffptr = &buffer;
	char * input;
	char ** inputptr;
	FILE * settings;
	char * fullpath;

	if(!(homedir))
		init_filehandler();


	fullpath = malloc(HOMEDIRLEN+SETTINGLEN+sizeof(char));
	sprintf(fullpath,"%s%s%s",homedir,DIR_PATH,SETTINGS);

	settings = fopen(fullpath,"r");
	
	if(!(settings))
	{
		printf("File %s does not exist\n",fullpath);
		free(fullpath);
		return (char * ) NULL;
	}

	input = (char *) malloc((sizeof(char))*buffer);
	inputptr = &input;

	int i =0;
	ssize_t retsize = 0;
	char * username;
	retsize = getline(inputptr,buffptr,settings);
	retsize--;
	if(strncmp(*inputptr,USERNAME,USERLEN) == 0) {
		username = (char *) calloc(1,retsize-USERLEN+sizeof(char));
		strncpy(username,(*inputptr+USERLEN),retsize-USERLEN);
	}
	
	fclose(settings);
	free(input);
	free(fullpath);

	return (char *) username;
}

void write_settings(char * username) 
{
	FILE * settings;
	DIR * settings_dir;
	char * dirpath = malloc(HOMEDIRLEN+sizeof(char));
	char * fullpath;
	int ret;

	if(!(homedir))
		init_filehandler();

	sprintf(dirpath,"%s%s",homedir,DIR_PATH);
	fprintf(stdout,"Opening %s\n",dirpath);
	settings_dir = opendir(dirpath);
//	settings = fopen(path,"r");
	if(settings_dir == NULL)
	{
		fprintf(stdout,"Directory %s does not exist\n",dirpath);
		ret = mkdir(dirpath,S_IRWXU);
		if(ret == -1) {
			fprintf(stderr,"Could not create directory, quits\n");
			exit(EXIT_FAILURE);	
		}
		fprintf(stdout,"Directory %s created\n",dirpath);
	}
	closedir(settings_dir);

	fullpath = malloc(HOMEDIRLEN+SETTINGLEN+sizeof(char));
	sprintf(fullpath,"%s%s",dirpath,SETTINGS);
	free(dirpath);

	settings = fopen(fullpath,"w");
	if(settings == NULL) {
		fprintf(stderr,"Could not write to file %s\n",fullpath);
		exit(EXIT_FAILURE);
	}
	fprintf(settings,"%s%s\n",USERNAME,username);
	free(fullpath);
	fclose(settings);
}
