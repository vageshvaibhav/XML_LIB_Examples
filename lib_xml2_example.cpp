#include "lib_xml2_example.hpp"

/**
 * usage:
 * @name:		the program name.
 *
 * Prints usage information.
 */
void usage(const char *name)
{
    assert(name);

    fprintf(stderr, "Usage: %s <xml-file> <xpath-expr> [<known-ns-list>]\n", name);
    fprintf(stderr, "where <known-ns-list> is a list of known namespaces\n");
    fprintf(stderr, "in \"<prefix1>=<href1> <prefix2>=href2> ...\" format\n");
}

/**
 * execute_xpath_expression:
 * @filename:		the input XML filename.
 * @xpathExpr:		the xpath expression for evaluation.
 * @nsList:		the optional list of known namespaces in 
 *			"<prefix1>=<href1> <prefix2>=href2> ..." format.
 *
 * Parses input XML file, evaluates XPath expression and prints results.
 *
 * Returns 0 on success and a negative value otherwise.
 */
int execute_xpath_expression(const char *filename, const xmlChar *xpathExpr, const xmlChar *nsList)
{
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;

    assert(filename);
    assert(xpathExpr);

    /* Load XML document */
    doc = xmlParseFile(filename);
    if (doc == NULL)
    {
        fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
        return (-1);
    }

    /* Create xpath evaluation context */
    xpathCtx = xmlXPathNewContext(doc);
    if (xpathCtx == NULL)
    {
        fprintf(stderr, "Error: unable to create new XPath context\n");
        xmlFreeDoc(doc);
        return (-1);
    }

    /* Register namespaces from list (if any) */
    if ((nsList != NULL) && (register_namespaces(xpathCtx, nsList) < 0))
    {
        fprintf(stderr, "Error: failed to register namespaces list \"%s\"\n", nsList);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return (-1);
    }

    /* Evaluate xpath expression */
    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if (xpathObj == NULL)
    {
        fprintf(stderr, "Error: unable to evaluate xpath expression \"%s\"\n", xpathExpr);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return (-1);
    }

    /* Print results */
    print_xpath_nodes(xpathObj->nodesetval, stdout);

    /* Cleanup */
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return (0);
}

/**
 * register_namespaces:
 * @xpathCtx:		the pointer to an XPath context.
 * @nsList:		the list of known namespaces in 
 *			"<prefix1>=<href1> <prefix2>=href2> ..." format.
 *
 * Registers namespaces from @nsList in @xpathCtx.
 *
 * Returns 0 on success and a negative value otherwise.
 */
int register_namespaces(xmlXPathContextPtr xpathCtx, const xmlChar *nsList)
{
    xmlChar *nsListDup;
    xmlChar *prefix;
    xmlChar *href;
    xmlChar *next;

    assert(xpathCtx);
    assert(nsList);

    nsListDup = xmlStrdup(nsList);
    if (nsListDup == NULL)
    {
        fprintf(stderr, "Error: unable to strdup namespaces list\n");
        return (-1);
    }

    next = nsListDup;
    while (next != NULL)
    {
        /* skip spaces */
        while ((*next) == ' ')
            next++;
        if ((*next) == '\0')
            break;

        /* find prefix */
        prefix = next;
        next = (xmlChar *)xmlStrchr(next, '=');
        if (next == NULL)
        {
            fprintf(stderr, "Error: invalid namespaces list format\n");
            xmlFree(nsListDup);
            return (-1);
        }
        *(next++) = '\0';

        /* find href */
        href = next;
        next = (xmlChar *)xmlStrchr(next, ' ');
        if (next != NULL)
        {
            *(next++) = '\0';
        }

        /* do register namespace */
        if (xmlXPathRegisterNs(xpathCtx, prefix, href) != 0)
        {
            fprintf(stderr, "Error: unable to register NS with prefix=\"%s\" and href=\"%s\"\n", prefix, href);
            xmlFree(nsListDup);
            return (-1);
        }
    }

    xmlFree(nsListDup);
    return (0);
}

/**
 * print_xpath_nodes:
 * @nodes:		the nodes set.
 * @output:		the output file handle.
 *
 * Prints the @nodes content to @output.
 */
void print_xpath_nodes(xmlNodeSetPtr nodes, FILE *output)
{
    xmlNodePtr cur;
    int size;
    int i;

    assert(output);
    size = (nodes) ? nodes->nodeNr : 0;

    fprintf(output, "Result (%d nodes):\n", size);
    for (i = 0; i < size; ++i)
    {
        assert(nodes->nodeTab[i]);

        if (nodes->nodeTab[i]->type == XML_NAMESPACE_DECL)
        {
            xmlNsPtr ns;

            ns = (xmlNsPtr)nodes->nodeTab[i];
            cur = (xmlNodePtr)ns->next;
            if (cur->ns)
            {
                fprintf(output, "= namespace \"%s\"=\"%s\" for node %s:%s\n",
                        ns->prefix, ns->href, cur->ns->href, cur->name);
            }
            else
            {
                fprintf(output, "= namespace \"%s\"=\"%s\" for node %s\n",
                        ns->prefix, ns->href, cur->name);
            }
        }
        else if (nodes->nodeTab[i]->type == XML_ELEMENT_NODE)
        {
            cur = nodes->nodeTab[i];
            if (cur->ns)
            {
                fprintf(output, "= element node \"%s:%s\"\n",
                        cur->ns->href, cur->name);
            }
            else
            {
                fprintf(output, "= element node \"%s\"\n",
                        cur->name);
            }
        }
        else
        {
            cur = nodes->nodeTab[i];
            fprintf(output, "= node \"%s\": type %d\n", cur->name, cur->type);
        }
    }
}
