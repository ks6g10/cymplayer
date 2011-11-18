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
#ifdef LIBXML_READER_ENABLED

/**
 * processNode:
 * @reader: the xmlReader
 *
 * Dump information about the current node
 */
typedef
struct entrystr {
  char *title;
  char *author;
  char *duration;
  char *id;
  char *upploaded;
} entry;

void
processNode(xmlTextReaderPtr reader, xmlChar *parent, entry argentry) {
  const xmlChar *name, *value;
  short type, depth;
  
  depth = xmlTextReaderDepth(reader);
  name = xmlTextReaderConstName(reader);
  value = xmlTextReaderConstValue(reader);
  type =  xmlTextReaderNodeType(reader);
  /*
     printf("%d %d %s %d %s"
	    ,depth
	    ,type
	    ,name
	    ,xmlTextReaderAttributeCount(reader)
	    ,xmlTextReaderGetAttributeNo(reader,0));
  */
  
  //if the value is not a child but a attribute
     if(depth == 3 && type == 1) {
       //gives you the duration
       if(strncmp(name,"yt:duration",8) == 0)
	  printf("%s\n",xmlTextReaderGetAttributeNo(reader,0));
	//	else if(strncmp(name,"media:thumbnail",8) == 0)
  	//  printf("%s\n",xmlTextReaderGetAttributeNo(reader,0));
	
     }
     //if the value is a child node
     if(depth == 4 && type == 3) {
       if(strncmp(parent,"yt:uploaded",5) == 0) 
	 printf("%s\n",value);
       else if(strncmp(parent,"media:credit",10) == 0) 
	 printf("%s\n",value);
       else if(strncmp(parent,"media:title",12) == 0) 
	 printf("%s\n",value);
       else if(strncmp(parent,"yt:videoid",5) == 0) 
	 printf("%s\n",value);
       
     }     
}

/**
 * streamFile:
 * @filename: the file name to parse
 *
 * Parse and print information about an XML file.
 */
static void
streamFile(const char *filename) {
  xmlTextReaderPtr reader;
  int ret, cnt;
  xmlChar *cname,*parent;
  unsigned short inside = 0;
  entry entrya[25];
  reader = xmlReaderForFile(filename, NULL, 0);
  // if there is something to read
  if (reader != NULL) {
    ret = xmlTextReaderRead(reader);
    //while there still is something to read
    while (ret == 1) {
      //gets the name of the current child
      cname = xmlTextReaderConstName(reader);
      
      //if the child is of group, toggle insisde
      if(strncmp(cname,"media:group",8) == 0) {
	inside ^= 1;
	printf("\n");
	if(inside == 0)
	  cnt++;
      }
      
      if(inside == 1)
	processNode(reader, parent,entrya[cnt]);
      parent = xmlTextReaderConstName(reader);
      ret = xmlTextReaderRead(reader);
      
    }
    xmlFreeTextReader(reader);
    if (ret != 0) {
      fprintf(stderr, "%s : failed to parse\n", filename);
    }
  } else {
    fprintf(stderr, "Unable to open %s\n", filename);
  }
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
