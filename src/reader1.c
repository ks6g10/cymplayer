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

/*
 * reader1.c
 *
 * Copyright (C) 2012 - Kim Svensson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */



#include <stdio.h>
#include <libxml/xmlreader.h>
#include <string.h>
#include <ctype.h>
#include "reader1.h"
#ifdef LIBXML_READER_ENABLED


char * format_duration(char * argdur);


/**
 * nodetype: 
 * @parent: parent node.
 * Determines if and what kind of node type it is, according to what we want.
 */
int 
nodetype(char *parent) 
{
	const char * CHARY = (const char *) "y";
	const char * CHARM = (const char *) "m";
	if(strncmp(parent,CHARY,1)==0) 
	{
		if(strcmp(parent,"yt:uploaded") == 0) 
		{ /* if the parent name was yt:uploaded */
			return UPLOADED;
		}
		else if(strcmp(parent,"yt:videoid") == 0)  
		{
			return ID;
		}
	} 
	
	if(strcmp(parent,"media:title") == 0) 
	{
		return TITLE;
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
process_node(xmlTextReaderPtr reader,char *parent, entry *argEntry) 
{
	
	short type, depth;
	depth = xmlTextReaderDepth(reader);
	type =  xmlTextReaderNodeType(reader);
	
	if(depth == 4 && type == 3) 
	{ /* if the value is a child node */
		short ret = nodetype(parent);
		
		if(__builtin_expect(ret == 0,0)) //if no match
			return;
		//If there is more than one credit field, only take the first.
//		if(argEntry->fields[ret] !=NULL) {
//			return;
//		}
		const char *value;
		char* ptr;
		
		value = (const char *) xmlTextReaderConstValue(reader);
		ptr = (char *)  malloc(strlen(value)+sizeof(char) ); //alloc space for string
		strcpy(ptr,value);
		argEntry->fields[ret] = ptr;		
		
	} 
	else if(depth == 3 && type == 1) 
	{   /* if the value is not a child but a attribute */
		char *name;
		name =(char *) xmlTextReaderConstName(reader);
		
		if(strcmp(name,"media:credit") == 0)
		{
			char * author = (char *) xmlTextReaderGetAttributeNo(reader,2);
			argEntry->fields[AUTHOR] = malloc(strlen(author)+sizeof(char));
			strcpy(argEntry->fields[AUTHOR],author);
			free(author);
		}
		
		if(__builtin_expect(strcmp(name,"yt:duration") == 0,0)) 
		{     /* gives you the duration */
			char *duration;
			duration =(char *) xmlTextReaderGetAttributeNo(reader,0);
			argEntry->fields[DURATION] = format_duration(duration);
			free(duration);

		}

    
	} /* end if */
  
} /* end parse node */

char * 
format_duration(char * argdur)
{
	// 99:59:59 max format for duration, 8 chars + end of string char = 9. 
	//00:01 min format for duration, 5 chars + end of string char = 6.
	//Source K&R page 43 for algorithm of turning string to integer value

	int n=0;
	int index;
	char * dur;
	char * tmp;

	for(index = 0; isdigit(argdur[index]); ++index) 
		n = 10 * n + (argdur[index] - '0');
	
	if(n >= 3600) //duration one hour or more
	{
		dur = (char *) malloc(9*sizeof(char));
		tmp = dur;
		int hours = n/3600;
		*dur++ = (char) '0' + hours/10;
		*dur++ = (char) '0' + hours%10;
		*dur++ = ':';
		n %= 3600;
	}
	else
	{
		dur = (char *)  malloc(6*sizeof(char));
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

	return (char *) tmp;	
}

void 
print_entryarray(entry *argRoot)
{
	const entry *tmp = argRoot;
	while(tmp->next != NULL)
	{
		printf("%s -> ", tmp->fields[AUTHOR]);
		printf("%s", tmp->fields[TITLE]);
		printf(" %s\n", tmp->fields[DURATION]);
		tmp =(entry *) tmp->next;
	}
}

void 
free_entryarray(entry *argRoot)
{

 	entry *tmp = argRoot;
	entry *prevtmp = NULL;
	while(tmp != NULL)
	{
		free(tmp->fields[AUTHOR]);
		free(tmp->fields[UPLOADED]);
		free(tmp->fields[TITLE]);
		free(tmp->fields[ID]);
		free(tmp->fields[DURATION]);
		prevtmp = tmp;
		tmp =(entry *) tmp->next;
		free(prevtmp);
	}

//	free(prevtmp);// free the last entry as the while loop will exit at last.
}

/**
 * streamFile:
 * @filename: the file name to parse
 *
 * Parse and print information about an XML file.
 */
entry *
stream_file(const char *filename) 
{	
	xmlTextReaderPtr reader;
	reader = xmlReaderForFile(filename, NULL, 0);
	// if there is nothing to read
	if (__builtin_expect(reader == NULL,0)) 
	{
		fprintf(stderr, "Unable to open %s\n", filename);
		return (entry *) NULL;
	}

	entry *root = (entry *) malloc(sizeof( entry));
	entry *current = root;
	int ret;
	char *cname,*parent;
	unsigned char inside = 0;
	ret = xmlTextReaderRead(reader);	

	while (__builtin_expect(ret == 1,1)) 
	{ /* while there still is something to read */
		cname = (char *)  xmlTextReaderConstName(reader); /* gets the name of the current node */
		
		if(strcmp(cname,"media:group") == 0 && !(inside ^=1)) 
		{ /* if the node is of media:group, toggle inside */
			current->next = (entry *) calloc(1,sizeof(entry)); //set new pointer to next entry
			if(__builtin_expect(current->next == NULL,0))
			{
				fprintf(stderr, "Malloc failed in reader1.c:streamFile:~244\n");
				return (entry *) NULL;
			}

			current =(entry *) current->next; //change entry to new one.

		}
		else if(inside == 1) 
		{
			process_node(reader, parent, current);
			parent = cname;//(char *)  xmlTextReaderConstName(reader);
		}

		ret = xmlTextReaderRead(reader);		
	}

	if (__builtin_expect(ret,0)) 
	{
		fprintf(stderr, "%s : failed to parse\n", filename);
	}
	xmlFreeTextReader(reader);	
	return (entry *) root;
}



entry * get_rootentry(char * filename)
{

	entry * rootentry = (entry *) NULL;
	/*
	 * this initialize the library and check potential ABI mismatches
	 * between the version it was compiled for and the actual shared
	 * library used.
	 */

	LIBXML_TEST_VERSION
		rootentry = stream_file(filename);

	if(rootentry == NULL)
	{
		fprintf(stderr, "malloc failed, exit\n");
		exit(1);
	}
//	rootentry 
//	printEntryArray(rootentry);
//	freeEntryArray( rootentry);

	/*
	 * Cleanup function for the XML library.
	 */
	xmlCleanupParser();
	/*
	 * this is to debug memory for regression tests
	 */
	xmlMemoryDump();
//	printf("root title %s",rootentry->fields[TITLE]);
	return rootentry;

}

/* int main(int argc, char **argv) { */
/* 	if(argc < 2) { */
/* //		printf("asd"); */
/* 		return run(); */
/* 	} */
/* 	if (argc != 2) */
/* 		return(1); */
/* 	entry * rootentry = (entry *) NULL; */
/* 	/\* */
/* 	 * this initialize the library and check potential ABI mismatches */
/* 	 * between the version it was compiled for and the actual shared */
/* 	 * library used. */
/* 	 *\/ */
/* 	printf("%s",argv[1]); */
/* 	LIBXML_TEST_VERSION */
/* 		rootentry = streamFile(argv[1]); */

/* 	if(rootentry == NULL) */
/* 	{ */
/* 		fprintf(stderr, "malloc failed, exit\n"); */
/* 		exit(1); */
/* 	} */

/* 	printEntryArray(rootentry); */
/* 	freeEntryArray( rootentry); */

/* 	/\* */
/* 	 * Cleanup function for the XML library. */
/* 	 *\/ */
/* 	xmlCleanupParser(); */
/* 	/\* */
/* 	 * this is to debug memory for regression tests */
/* 	 *\/ */
/* 	xmlMemoryDump(); */
	


/* 	return(0); */
/* }  */

int run() {

	entry * rootentry = (entry *) NULL;
	//rootentry = getRootentry();
	print_entryarray(rootentry);
	free_entryarray( rootentry);

	return 0;
	}

#else
int main(void) {
	fprintf(stderr, "XInclude support not compiled in\n");
	exit(1);
}
#endif

