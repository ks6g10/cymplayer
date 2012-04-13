
/*Returns the youtube username stored in the settings file, if no file/setting return NULL*/
char * get_username();

/*Writes the username to the settings file, also creates directories if not present*/
void write_settings(char * username);

/*Returns e.g. /home/ks6g10/.cymplayer */
char * get_settingsdir();
/*Returns e.g. /home/ks6g10/.cymplayer/thumbs */
char * get_thumbdir();
