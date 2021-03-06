/**
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-15

Decodes an HDF5 file into a binary data file and an ASCII info file.

<b>hldec [</b>-hdv<b>]</b> -i inputfile -f fieldname -o outputprefix

<b>[-h]</b> Prints an help text.

<b>[-d]</b> Prints debugging information.

<b>[-v]</b> Prints the version number.

<b>-i inputfile</b> Specifies the HDF5 file to be decoded.

<b>-f fieldname</b> Specifies the fieldname to be decoded, e.g. '/dataset1'.

<b>-o outputprefix</b> Specifies the prefix for the output files, the files that will be
generated are &lt;outputprefix&gt;.info and &lt;outputprefix&gt;.data.

The file with extension .info will get the following apperance:
DATATYPE: <b>[</b>ATTRIBUTE or DATASET<b>]</b>
FIELDNAME: <b>[</b>name of the field, e.g. '/attr1'<b>]</b>
DATASIZE: <b>[</b>size of the datatype in bytes<b>]</b>
DATAFORMAT: <b>[</b>string representation of the datatype, e.g. int<b>]</b>
DIMS: <b>[</b>the dimension of the data embraced by [], e.g. [10,10]<b>]</b>

The file with extension .data will be saved in byteformat with native byte order.
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "hlhdf.h"

static char* ProcessName;
static int hldec_debug=0;

static void PrintHelp(int all);
static void PrintVersion(void);

/**
 * Main function.
 */
int main(int argc, char** argv)
{
   extern char* optarg;
   extern int opterr;
   int c,i;
   char* inputfile=NULL;
   char* outputprefix=NULL;
   char* fieldname=NULL;
   size_t npts;

   HL_NodeList* nodelist=NULL;
   HL_Node* node=NULL;

   FILE* info_fp=NULL;
   FILE* data_fp=NULL;

   char info_filename[512];
   char data_filename[512];

   HL_init();
   HL_setDebugMode(0);

   ProcessName = strdup(argv[0]);

   opterr=0;

   while((c=getopt(argc,argv,"hdvi:o:f:"))!=EOF) {
      switch(c) {
      case 'h':
	 PrintHelp(1);
	 exit(0);
	 break;
      case 'd':
	 hldec_debug=1;
	 HL_setDebugMode(2);
	 break;
      case 'v':
	 PrintVersion();
	 exit(0);
	 break;
      case 'i':
	 if(optarg)
	    inputfile=strdup(optarg);
	 break;
      case 'o':
	 if(optarg)
	    outputprefix=strdup(optarg);
	 break;
      case 'f':
	 if(optarg)
	    fieldname=strdup(optarg);
	 break;
      default:
	 PrintVersion();
	 PrintHelp(0);
	 exit(1);
      }
   }

   if(!inputfile || !outputprefix || !fieldname) {
      PrintHelp(0);
      exit(1);
   }

   if(!(nodelist = HLNodeList_read(inputfile))) {
      fprintf(stderr,"Failed to open HDF file.\n");
      goto fail;
   }

   if(!HLNodeList_selectNode(nodelist,fieldname)) {
      fprintf(stderr,"Failed to select field '%s' from file '%s'\n",fieldname,inputfile);
      goto fail;
   }

   if(!HLNodeList_fetchMarkedNodes(nodelist)) {
      fprintf(stderr,"Failed to fetch data\n");
      goto fail;
   }

   if(!(node=HLNodeList_getNodeByName(nodelist,fieldname))) {
      fprintf(stderr,"Failed to get node '%s' from inputfile\n",fieldname);
      goto fail;
   }

   if(HLNode_getType(node) != DATASET_ID && HLNode_getType(node) != ATTRIBUTE_ID) {
      fprintf(stderr,"Specified field was not a dataset or attribute.\n");
      goto fail;
   }

   sprintf(info_filename,"%s.info",outputprefix);
   sprintf(data_filename,"%s.data",outputprefix);

   if(!(info_fp=fopen(info_filename,"w"))) {
      fprintf(stderr,"Failed to open info file '%s' for writing\n",info_filename);
      goto fail;
   }
   if(!(data_fp=fopen(data_filename,"wb"))) {
      fprintf(stderr,"Failed to open data file '%s' for writing\n",data_filename);
      goto fail;
   }

   if(HLNode_getType(node)==DATASET_ID) {
      fprintf(info_fp,"DATATYPE: DATASET\n");

   } else {
      fprintf(info_fp,"DATATYPE: ATTRIBUTE\n");
   }
   fprintf(info_fp,"FIELDNAME: %s\n",HLNode_getName(node));
   fprintf(info_fp,"DATASIZE: %d\n",(int)HLNode_getDataSize(node));
   fprintf(info_fp,"DATAFORMAT: %s\n",HLNode_getFormatName(node));
   if(HLNode_getRank(node)>0) {
      fprintf(info_fp,"DIMS: [");
      for(i = 0; i < HLNode_getRank(node); i++) {
	 fprintf(info_fp,"%d",(int)HLNode_getDimension(node, i));
	 if(i < HLNode_getRank(node) - 1)
	    fprintf(info_fp,",");
      }
      fprintf(info_fp,"]\n");
   } else {
      fprintf(info_fp,"DIMS: [0]\n");
   }

   npts=(size_t)HLNode_getNumberOfPoints(node);

   if(fwrite(HLNode_getData(node), HLNode_getDataSize(node), npts, data_fp) != npts) {
      fprintf(stderr,"Failed to write datafield\n");
      goto fail;
   }

   if(info_fp) fclose(info_fp);
   if(data_fp) fclose(data_fp);
   HLNodeList_free(nodelist);
   free(inputfile);
   free(outputprefix);
   free(fieldname);

   exit(0);
   return 0;
 fail:
   if(info_fp) fclose(info_fp);
   if(data_fp) fclose(data_fp);
   HLNodeList_free(nodelist);
   free(inputfile);
   free(outputprefix);
   free(fieldname);

   exit(1);
   return 1;
}

static void PrintHelp(int all)
{
   fprintf(stderr,"Usage: %s [-hdv] -i <inputfile> -o <outputprefix> -f <fieldname\n",
	   ProcessName);
   if(!all)
      return;
   fprintf(stderr,"\t[-h]    Prints this help.\n");
   fprintf(stderr,"\t[-d]    Turns on debugging.\n");
   fprintf(stderr,"\t[-v]    Prints the version.\n");
   fprintf(stderr,"\t-i inputfile\n");
   fprintf(stderr,"\t        Specify the hdf file that should be read from.\n");
   fprintf(stderr,"\t-o outputprefix\n");
   fprintf(stderr,"\t        Specify the prefix for the output files, the files.\n");
   fprintf(stderr,"\t        that will be generated are <outputprefix>.data and\n");
   fprintf(stderr,"\t        <outputprefix>.info\n");
   fprintf(stderr,"\t-f fieldname\n");
   fprintf(stderr,"\t        Specify the fieldname that should be dumped to file,\n");
   fprintf(stderr,"\t        should either be a dataset or attribute.\n");
}

static void PrintVersion()
{
   fprintf(stderr,"HLDEC: Version 0.1\n");
}
