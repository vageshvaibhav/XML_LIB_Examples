#include <iostream>
#include "lib_xml2_example.hpp"
#include "boost_property_tree_example.hpp"

int main(int argc, char **argv) {

    /***********************1. LibXML2 Example*****************/
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

    /***********************4.Boost Property Tree Example*****************/
    debug_settings deb;
    std::string init[] = {"C++","Java","Python","Scala"};
    std::set<std::string> modules(init,init+sizeof(init)/sizeof(init[0]));
    deb.m_file = "abc.log";
    deb.m_level = 10;
    deb.m_modules = modules;
    deb.save("test.xml");

    return 0;
}
