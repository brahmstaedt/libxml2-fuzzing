#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

/**
 * example3Func:
 * @content: the content of the document
 * @length: the length in bytes
 *
 * Parse the in memory document and free the resulting tree
 */
static void
example3Func(const char *content, int length) {
    xmlDocPtr doc; /* the resulting document tree */

    /*
     * The document being in memory, it have no base per RFC 2396,
     * and the "noname.xml" argument will serve as its base.
     */
    doc = xmlReadMemory(content, length, "noname.xml", NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse document\n");
        return;
    }
    xmlFreeDoc(doc);
}

int main(void) {
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /*
     * Read line from stdin
     * TODO: allow more than one line
     */
    std::string line;
    std::getline(std::cin, line);

    /*
     * Convert string to char[]
     */
    int n = line.length();
    char document[n + 1];
    strcpy(document, line.c_str());

    /*
     * Invoke libxml2 parser
     */
    example3Func(document, n);

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();

}
