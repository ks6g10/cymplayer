#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>

#define USERLEN (strlen(USERNAME))

#define SETTINGLEN (strlen(SETTINGS))

const char * SETTINGS = "/settings";
const char * USERNAME = "Username=";
static char * homedir;
static char * thumbdir;
static char * settingsdir;


char * getusername();
void writesetting(char * username);


char * get_settingsdir()
{
	return settingsdir;
}

void init_filehandler()
{

	uid_t userid = getuid();
	struct passwd * userpasswd = getpwuid(userid);
	homedir = userpasswd->pw_dir;
	if(!(homedir))
	{
		fprintf(stderr,"Could not aquire home directory, quits\n");
		exit(2);

	}
	
}

char * get_homedir()
{
	if(!settingsdir)
		init_filehandler();
	return homedir;
}

char * get_thumbdir()
{

	return thumbdir;

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


	fullpath = calloc(1,strlen(settingsdir)+strlen(SETTINGS)+sizeof(char));
	sprintf(fullpath,"%s%s",settingsdir,SETTINGS);

	settings = fopen(fullpath,"r");
	
	if(!(settings))
	{
		printf("File %s does not exist\n",fullpath);
		free(fullpath);
		return (char * ) NULL;
	}

	input = (char *) malloc((sizeof(char))*buffer);
	inputptr = &input;

	
	ssize_t retsize = 0;
	char * username;
	retsize = getline(inputptr,buffptr,settings);

	/*New Line Symbol*/
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


/*
 *@arghomedir e.g. /home/ks6g10
 *@argsuffix suffix of the directory such as /.cymplayer in /home/ks6g10/.cymplayer
 */
char * touch_directory(char * arghomedir,const char * argsuffix)
{

	char * fullpath = calloc(1,strlen(arghomedir)+strlen(argsuffix)+1);
	DIR * settings_dir;
	int ret;
	sprintf(fullpath,"%s%s",arghomedir,argsuffix);

	settings_dir = opendir(fullpath);
	
	if(settings_dir == NULL)
	{
		fprintf(stdout,"Directory %s does not exist\n",fullpath);
		ret = mkdir(fullpath,S_IRWXU);
		if(ret == -1) {
			fprintf(stderr,"Could not create directory %s, quits\n",fullpath);
			exit(EXIT_FAILURE);	
		}
		fprintf(stdout,"Directory %s created\n",fullpath);
	}
	closedir(settings_dir);

	return fullpath;
}

void write_settings(char * username) 
{
	FILE * settings;
	char * fullpath;
	
	const char * CYMPLAYERDIR = "/.cymplayer";
	const char * THUMBDIR = "/.cymplayer/thumbs";
	if(!(homedir))
		init_filehandler();
	
	settingsdir = touch_directory(homedir,CYMPLAYERDIR);
	thumbdir = touch_directory(homedir,THUMBDIR);
//	settings = fopen(path,"r");
	
	fullpath = calloc(1,strlen(settingsdir)+strlen(SETTINGS)+sizeof(char));
	sprintf(fullpath,"%s%s",settingsdir,SETTINGS);
	

	settings = fopen(fullpath,"w");
	if(settings == NULL) {
		fprintf(stderr,"Could not write to file %s\n",fullpath);
		exit(EXIT_FAILURE);
	}
	fprintf(settings,"%s%s\n",USERNAME,username);
	free(fullpath);
	fclose(settings);
}
