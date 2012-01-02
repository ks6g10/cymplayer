/**
 * section: xmlReader
 * synopsis: Parse an XML file with an xmlReader
 * purpose: Demonstrate the use of xmlReaderForFile() to parse an XML file
 *          and dump the informations about the nodes found in the process.
 *          (Note that the XMLReader functions require libxml2 version later
 *          than 2.6.)
 * usage: reader1 <filename>
 * test: reader1 test2.xml > reader1.tmp ; diff reader1.tmp reader1.res ; rm reader1.tmp
 * author: Daniel Veillard
 * copy: see Copyright for the status of this software.
 */

#include <stdio.h>
#include <libxml/xmlreader.h>
#include <string.h>
#include <ctype.h>
#ifdef LIBXML_READER_ENABLED


typedef struct entrystr {
	char *title;
	char *author;
	char *duration;
	char *id;
	char *uploaded;
	struct entrystr *next;
} entry;

//void printEntryArray(entry *argEntryA,short argCount);
char * format_duration(char argdur[]);
int 
correctNode(const xmlChar *parent) 
{
	if(strcmp(parent,"yt:uploaded") == 0) 
	{ /* if the parent name was yt:uploaded */
		return 1;
	}
	else if(strcmp(parent,"media:credit") == 0) 
	{
		return 2;
	}
	else if(strcmp(parent,"media:title") == 0) 
	{
		return 3;
	}
	else if(strcmp(parent,"yt:videoid") == 0)  
	{
		return 4;
	}
	return 0;
}

/**
 * processNode:
 * @reader: the xmlReader
 * @parent: the parent node
 * Dump information about the current node
 */
void
processNode(xmlTextReaderPtr reader,const xmlChar *parent, entry *argEntry) 
{
	
	short type, depth;
	depth = xmlTextReaderDepth(reader);
	type =  xmlTextReaderNodeType(reader);
	/*
	  printf("%d %d %s %d %s"
	  ,depth
	  ,type
	  ,name
	  ,xmlTextReaderAttributeCount(reader)
	  ,xmlTextReaderGetAttributeNo(reader,0));
	*/
	if(depth == 4 && type == 3) 
	{ /* if the value is a child node */
		unsigned short ret = correctNode(parent);
		if(ret == 0) //if no match
			return;
		const xmlChar *value;
		char* ptr;
		value = xmlTextReaderConstValue(reader);
		ptr = malloc((strlen(value)+1)*(sizeof (char))); //alloc space for string
		strcpy(ptr,value);
		switch(ret)
		{
		case 1: 
			argEntry->uploaded = ptr;
			break;
		case 2: 
			argEntry->author = ptr;
			break;
		case 3:
			argEntry->title = ptr;
			break;
		case 4:
			argEntry->id = ptr;
			break;
		default:
			free(ptr);
			break;
		}
		
	} 
	else if(depth == 3 && type == 1) 
	{   /* if the value is not a child but a attribute */
		const xmlChar *name;
		name = xmlTextReaderConstName(reader);

		if(strcmp(name,"yt:duration") == 0) 
		{     /* gives you the duration */
			xmlChar *duration;
			char *ptr;
			duration = xmlTextReaderGetAttributeNo(reader,0);
			argEntry->duration = format_duration(duration);

		}
    
	} /* end if */
  
} /* end parse node */

char * 
format_duration(char argdur[])
{
	// 99:59:59 max format for duration, 8 chars + end of string char = 9. 
	//00:01 min format for duration, 5 chars + end of string char = 6.
	//Source K&R page 43 for algorithm of turning string to integer value

	int n=0;
	int i;
	char * dur;
	char * tmp;

	for(i = 0; isdigit(argdur[i]); ++i) 
		n = 10 * n + (argdur[i] - '0');
	
	if(n >= 3600)
	{
		dur = malloc(9*sizeof(char));
		tmp = dur;
		int hours = n/3600;
		*dur++ = (char) '0' + hours/10;
		*dur++ = (char) '0' + hours%10;
		*dur++ = ':';
		n %= 3600;
	}
	else
	{
		dur = malloc(6*sizeof(char));
		tmp = dur;
	}
	
	int minute = n/60;
	*dur++ = (char) '0' + minute/10; //get most segnificant minute
	*dur++ = (char) '0' + minute%10; // get least segnificant minute
	*dur++ = ':';
	
	n %= 60;
	*dur++ = (char) '0' + n/10; //get deca seconds
	*dur++ = (char) '0' + n%10; // get single seconds
	*dur = '\0';

	return tmp;	
}

void 
printEntryArray(entry *argRoot)
{

	entry *tmp = argRoot;
	while(argRoot != tmp->next)
	{
		printf("%s -> ", tmp->author);
		printf("%s\n", tmp->title);
		printf("%s\n", tmp->duration);
		tmp = tmp->next;
	}
}

/**
 * streamFile:
 * @filename: the file name to parse
 *
 * Parse and print information about an XML file.
 */
void
streamFile(const char *filename) {	
	xmlTextReaderPtr reader;
	reader = xmlReaderForFile(filename, NULL, 0);
	// if there is nothing to read
	if (reader == NULL) {
		fprintf(stderr, "Unable to open %s\n", filename);
		return;
	}

	entry *root = malloc(sizeof(entry));
	entry *next = root;
	int ret;
	const xmlChar *cname,*parent;
	unsigned short inside = 0;
	ret = xmlTextReaderRead(reader);	

	while (ret == 1) 
	{ /* while there still is something to read */
		cname = xmlTextReaderConstName(reader); /* gets the name of the current node */
		if(strcmp(cname,"media:group") == 0) 
		{ /* if the node is of media:group, toggle inside */
			inside ^= 1; //toggle inside
			if(inside == 0)
			{
//				entry *tmp = 
				next->next = malloc(sizeof(entry)); //set new pointer to next entry
				next = next->next; //change entry to new one.
				next->next = root; //set the new entry to point to root
			}
		}
		
		if(inside == 1)
			processNode(reader, parent, next);
		parent = xmlTextReaderConstName(reader);
		ret = xmlTextReaderRead(reader);
	}
	if (ret != 0) {
		fprintf(stderr, "%s : failed to parse\n", filename);
	}
	printEntryArray(root);
	xmlFreeTextReader(reader);	
}



int main(int argc, char **argv) {
	if (argc != 2)
		return(1);
  
	/*
	 * this initialize the library and check potential ABI mismatches
	 * between the version it was compiled for and the actual shared
	 * library used.
	 */
	LIBXML_TEST_VERSION
		
		streamFile(argv[1]);
	
	/*
	 * Cleanup function for the XML library.
	 */
	xmlCleanupParser();
	/*
	 * this is to debug memory for regression tests
	 */
	xmlMemoryDump();
	return(0);
}

#else
int main(void) {
	fprintf(stderr, "XInclude support not compiled in\n");
	exit(1);
}
#endif
