/**
 * @file
 * @author Anders Henja (Swedish Meteorological and Hydrological Institute, SMHI)
 * @date 2009-06-15

Lists the nodes in an HDF5 file.

<b>hllist [-hdv]</b> hdf5file

<b>[-h]</b> Prints a help text.

<b>[-d]</b> Prints debugging information.

<b>[-v]</b> Prints the version number.

<b>hdf5file</b> Is the HDF5 file to be listed.
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "hlhdf.h"

static char* ProcessName;
static int hlenc_debug=0;

static void PrintHelp(int all);
static void PrintVersion(void);

/**
 * Main function
 */
int main(int argc, char** argv)
{
  extern int optind, opterr;
  int c, i;
  int nNodes = 0;

  HL_NodeList* nodelist = NULL;

  HL_init();
  HL_setDebugMode(0);

  ProcessName = strdup(argv[0]);

  opterr = 0;

  while ((c = getopt(argc, argv, "hdv")) != EOF) {
    switch (c) {
    case 'h':
      PrintHelp(1);
      exit(0);
      break;
    case 'd':
      hlenc_debug = 1;
      HL_setDebugMode(2);
      break;
    case 'v':
      PrintVersion();
      exit(0);
      break;
    default:
      PrintVersion();
      PrintHelp(0);
      exit(1);
      break;
    }
  }

  if (argc - 1 != optind) {
    PrintHelp(0);
    exit(1);
  }

  if (!(nodelist = HLNodeList_read(argv[optind]))) {
    fprintf(stderr, "Failed to read HDF5 file '%s'\n", argv[optind]);
    goto fail;
  }

  if ((nNodes =  HLNodeList_getNumberOfNodes(nodelist)) < 0) {
    fprintf(stderr, "Failed to get number of nodes\n");
    goto fail;
  }

  for (i = 0; i < nNodes; i++) {
    HL_Node* node = NULL;
    if ((node = HLNodeList_getNodeByIndex(nodelist, i)) == NULL) {
      fprintf(stderr, "Error occured when fetching node at index %d", i);
      goto fail;
    }
    printf("%-40s is ", HLNode_getName(node));
    switch (HLNode_getType(node)) {
    case ATTRIBUTE_ID:
      printf("an attribute");
      break;
    case DATASET_ID:
      printf("a dataset");
      break;
    case TYPE_ID:
      printf("a type");
      break;
    case GROUP_ID:
      printf("a group");
      break;
    case REFERENCE_ID:
      printf("a reference");
      break;
    default:
      printf("of an unreckognized type");
      break;
    }
    printf("\n");
  }

  HLNodeList_free(nodelist);

  exit(0);
  return 0;
fail:
  HLNodeList_free(nodelist);
  exit(1);
  return 1;
}

static void PrintHelp(int all)
{
  fprintf(stderr, "Usage: %s [-hdv] hdffile\n", ProcessName);
  if (!all)
    return;

  fprintf(stderr, "\t[-h]    Prints this help.\n");
  fprintf(stderr, "\t[-d]    Turns on debugging.\n");
  fprintf(stderr, "\t[-v]    Prints the version.\n");
  fprintf(stderr, "\thdffile The name of the HDF5 file that should be listed\n");
}

static void PrintVersion()
{
  fprintf(stderr, "HLLIST: Version 0.1\n");
}
