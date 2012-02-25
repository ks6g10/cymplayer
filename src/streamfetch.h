



typedef struct url {
	char * mp480; // itag 18
	char * mp720; // itag 22
	char * mp1080;// itag 37
}streamstruct;

enum streamtags {
	MP720=22,
	MP1080=37,
	MP480 =18
};


streamstruct * get_urlstruct(const char * filename);
