#include <iostream>
#include "lib_xml2_example.hpp"

int main(int argc, char **argv) {
    /* Parse command line and process file */
    if((argc < 3) || (argc > 4)) {
	fprintf(stderr, "Error: wrong number of arguments.\n");
	usage(argv[0]);
	return(-1);
    } 
    
    /* Init libxml */     
    xmlInitParser();
    LIBXML_TEST_VERSION

    /* Do the main job */
    if(execute_xpath_expression(argv[1], BAD_CAST argv[2], (argc > 3) ? BAD_CAST argv[3] : NULL) < 0) {
	usage(argv[0]);
	return(-1);
    }

    /* Shutdown libxml */
    xmlCleanupParser();
    
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    return 0;
}
