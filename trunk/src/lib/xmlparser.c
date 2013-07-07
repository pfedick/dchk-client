/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pafe $
 * $Revision: 1.6 $
 * $Date: 2010/08/04 18:31:43 $
 * $Id: xmlparser.c,v 1.6 2010/08/04 18:31:43 pafe Exp $
 *
 *
 * Copyright (C) 2008/2009 Patrick Fedick <dchk@pfp.de>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * SoftwareFoundation; either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef _MSC_VER
#include "msvc-config.h"
#else
#include "config.h"
#endif

#ifdef HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
	#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif


#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#ifdef HAVE_UNISTD_H
    #include <unistd.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
    #include <sys/socket.h>
#endif
#ifdef HAVE_SYS_POLL_H
    #include <sys/poll.h>
#endif
#ifdef HAVE_NETINET_IN_H
    #include <netinet/in.h>
#endif
#ifdef HAVE_NETDB_H
    #include <netdb.h>
#endif
#ifdef HAVE_ARPA_INET_H
    #include <arpa/inet.h>
#endif
#ifdef HAVE_ARPA_NAMESER_H
	#include <arpa/nameser.h>
#endif
#ifdef HAVE_RESOLV_H
	#include <resolv.h>
#endif
#ifdef HAVE_ERRNO_H
	#include <errno.h>
#endif

#include <zlib.h>

#include "dchk.h"
#include "compat.h"
#ifndef _WIN32
	#include "ns_parse.h"
#endif

#ifdef HAVE_XML2
#ifndef LIBXML_STATIC
	#define LIBXML_STATIC
	#define LIBXSLT_STATIC
	#define XMLSEC_STATIC
#endif
	#include <libxml/parser.h>
	#include <libxml/tree.h>
	#include <libxml/xpath.h>
	#include <libxml/xpathInternals.h>
#endif

void irislwz_xmlGenericErrorFunc (void * ctx, const char * msg, ...)
{
	return;
}

int irislwz_Validate(IRISLWZ_HANDLE *handle, const char *xml, char **beautified)
/*!\brief Do an XML validation against a text
 *
 * This function takes the given \p xml test an tries to validate it with the XML
 * library (xmlReadMemory). If the XML is valid, a \p beautified version, which can
 * be better read by humans, can be stored optionally.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] xml pointer to the text which should be validated
 * \param[out] beautified optional pointer to which a beautified version of the xml
 * is written. The Memory for this is allocated by the library, but must be freed by the
 * client using \p free.
 *
 * \returns If the XML is valid, the function returns 1, otherwise 0.
 *
 * \example
\code
const char *xml="<iris1:request xmlns:iris1=\"urn:ietf:params:xml:ns:iris1\">"
		"<iris1:searchSet><iris1:lookupEntity registryType=\"dchk1\" "
		"entityClass=\"domain-name\" entityName=\"denic.de\"/>"
		"</iris1:searchSet></iris1:request>";
char *beautified=NULL;
if (irislwz_Validate(handle,xml,&beautified)) {
	printf ("Original XML code:\n");
	printf ("%s\n",xml);
	printf ("Beautified version:\n");
	printf ("%s\n",beautified);
}
if (beautified) free(beautified);
\endcode
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	xmlSetGenericErrorFunc(handle,irislwz_xmlGenericErrorFunc);
    xmlDoc *doc = NULL;
	//xmlNode *root_element = NULL;
    int size=(int)strlen(xml);
    //printf ("Query: >>%s<<\n",xml);
    //printf ("Size: %i Bytes\n",size);
    doc = xmlReadMemory (xml, size,
				 NULL,
				 "UTF-8",
				 XML_PARSE_NONET);
	if (doc == NULL) {
		irislwz_SetError(handle,74044,NULL);
		return 0;
	}
	if (beautified) {
		xmlChar *mem;
		int size;
		xmlKeepBlanksDefault(0);
		xmlDocDumpFormatMemoryEnc(doc, &mem, &size, handle->localcharset,1);
		*beautified=strdup((char*)mem);
		xmlFree(mem);
	}
	xmlFreeDoc(doc);
	return 1;

}


int dchk_XML_ParseResult(IRISLWZ_HANDLE *handle,DCHK_DOMAINLIST *domainset)
/*!\brief Parse DCHK response
 *
 * This function is internally used. It parses the response of a DCHK query and stores various
 * information in the DCHK_DOMAINLIST.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in,out] domainset pointer to a DCHK_DOMAINLIST
 * \returns On success, the function returns 1, otherwise 0.
 *
 * \ingroup DCHK_API_DCHK
 */
{
	xmlDoc *doc = NULL;
	xmlXPathContext *xpathCtx;
	xmlXPathObject  *xpathObj;
	doc = xmlReadMemory (handle->response.uncompressed_payload,
			handle->response.size_uncompressed, NULL, NULL, 0);
	if (doc == NULL) {
		irislwz_SetError(handle,74014,NULL);
		return 0;
	}
	//printf ("answer: >>>%s<<<\n",handle->response.uncompressed_payload);
    //Create xpath evaluation context
    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
    	irislwz_SetError(handle,74015,NULL);
        xmlFreeDoc(doc);
        return(0);
    }
    /* do register namespace */
    if (xmlXPathRegisterNs(xpathCtx, (const xmlChar*)"iris1",(const xmlChar*)"urn:ietf:params:xml:ns:iris1") != 0) {
    	irislwz_SetError(handle,74018,"iris1");
    	xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return(0);
    }
    if (xmlXPathRegisterNs(xpathCtx, (const xmlChar*)"dchk1",(const xmlChar*)"urn:ietf:params:xml:ns:dchk1") != 0) {
    	irislwz_SetError(handle,74018,"dchk1");
    	xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return(0);
    }
    if (xmlXPathRegisterNs(xpathCtx, (const xmlChar*)"urn",(const xmlChar*)"urn:ietf:params:xml:ns:dchk1") != 0) {
    	irislwz_SetError(handle,74018,"urn");
    	xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return(0);
    }

    int ret=0;
    int i;
    char expr[1024];
    DCHK_RECORD *r=domainset->first;

    // resultSets are guaranteed to come in the same order as searchSets
    for (i=1;i<=domainset->num;i++,r=r->next) {
    	// check nameNotFound
    	sprintf(expr,"//iris1:response/iris1:resultSet[%i]/iris1:nameNotFound",i);
    	xpathObj = xmlXPathEvalExpression((const xmlChar *)expr, xpathCtx);
    	if(xpathObj!=NULL && xpathObj->nodesetval && xpathObj->nodesetval->nodeNr>0) {
    		xmlXPathFreeObject(xpathObj);
    		r->status=strdup("free");
    		continue;
    	}
    	xmlXPathFreeObject(xpathObj);
    	// check invalidName
    	sprintf(expr,"//iris1:response/iris1:resultSet[%i]/iris1:invalidName",i);
    	xpathObj = xmlXPathEvalExpression((const xmlChar *)expr, xpathCtx);
    	if(xpathObj!=NULL && xpathObj->nodesetval && xpathObj->nodesetval->nodeNr>0) {
    		xmlXPathFreeObject(xpathObj);
    		r->status=strdup("invalid");
    		continue;
    	}
    	xmlXPathFreeObject(xpathObj);
    	// check answer/domain
    	sprintf(expr,"//iris1:response/iris1:resultSet[%i]/iris1:answer/urn:domain",i);
    	xpathObj = xmlXPathEvalExpression((const xmlChar *)expr, xpathCtx);
    	if(xpathObj!=NULL && xpathObj->nodesetval && xpathObj->nodesetval->nodeNr>0) {
    		ret=dchk_XML_ParseDomain(handle,xpathObj->nodesetval->nodeTab[0],r);
    		xmlXPathFreeObject(xpathObj);
    		continue;
    	}
    	xmlXPathFreeObject(xpathObj);
    }
    /* Cleanup of XPath data */
    xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);

	return 1;

}

int dchk_XML_ParseSize(IRISLWZ_HANDLE *handle)
/*!\brief Parse size information
 *
 * This function is internally used to parse a size information answer.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \returns If size information is found in the XML, it's value (octets) is returned, otherwise 0.
 *
The payload of a size information looks like this:
\code
<?xml version="1.0" encoding="UTF-8"?>
<size xmlns="urn:ietf:params:xml:ns:iris-transport">
  <response>
    <octets>371</octets>
  </response>
</size>
\endcode

 * \ingroup DCHK_API_DCHK
 */
{
	xmlDoc *doc = NULL;
	doc = xmlReadMemory (handle->response.uncompressed_payload,
			handle->response.size_uncompressed, NULL, NULL, 0);
	if (doc == NULL) {
		irislwz_SetError(handle,74014,NULL);
		return 0;
	}
	/* TODO: I wanted to use XPath to capture the octets value, but for some reason
	 * it didn't work. xmlXPathEvalExpression returned a value, but the nodesetval
	 * always pointed to NULL.
    sprintf(expr,"//response/octets");
	xpathObj = xmlXPathEvalExpression((const xmlChar *)"//size/response/octets", xpathCtx);
	if(xpathObj!=NULL && xpathObj->nodesetval && xpathObj->nodesetval->nodeNr>0) {
		printf ("Debug....\n");
		...
	}
	*/
	xmlNode *node1, *node2, *node3, *node4 = NULL;
	for (node1 = doc->children; node1; node1 = node1->next) {
		if (node1->type==XML_ELEMENT_NODE && strcmp((char*)node1->name,"size")==0) {
			for (node2 = node1->children; node2; node2 = node2->next) {
				if (node2->type==XML_ELEMENT_NODE && strcmp((char*)node2->name,"response")==0) {
					for (node3 = node2->children; node3; node3 = node3->next) {
						if (node3->type==XML_ELEMENT_NODE && strcmp((char*)node3->name,"octets")==0) {
							for (node4 = node3->children; node4; node4 = node4->next) {
								if (node4->type==XML_TEXT_NODE) {
									int octets=atoi((char*)node4->content);
									xmlFreeDoc(doc);
									return octets;
								}
							}
						}
					}
				}
			}
		}
	}
	xmlFreeDoc(doc);
	irislwz_SetError(handle,74041,NULL);
	return 0;
}

int dchk_XML_ParseOther(IRISLWZ_HANDLE *handle)
/*!\brief Parse other information
 *
 * This function is internally used to parse an "other information" answer.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \returns Returns 1, if other information could be parsed and sets an
 * appropriate error code (see below), otherwise 0.
 *
The payload of a size information looks like this:
\code
<?xml version="1.0" encoding="UTF-8"?>
<other xmlns="urn:ietf:params:xml:ns:iris-transport" type="payload-error">
  <description language="en">Decoding or parsing of payload failed</description>
</other>
\endcode

<a href="http://tools.ietf.org/html/rfc4993">RFC 4993</a> describes the following
types:
<ul>
<li><b>'descriptor-error'</b> - indicates there was an error decoding the
  descriptor.  Servers SHOULD send a descriptor error in the
  following cases:<ol>
      <li>When a request is received with a payload type indicating size
          information (i.e., the PT field is 'si').</li>
      <li>When a request is received with a payload type indicating
          other information (i.e., the PT field is 'oi').</li>
      <li> When a request is sent with a transaction ID of 0xFFFF (which
          is reserved for server use).</li>
      <li>When a request is received with an incomplete or truncated
          payload descriptor.</li>
      <li>When reserved bits in the payload descriptor are set to values
          other than zero.</li>
   </ol>
   If this type occurs, this function will raise error 74048.
</li>
<li><b>'payload-error'</b> - indicates there was an error interpreting the
      payload.  Servers MUST send a payload error if they receive XML
      (i.e., the PT field is set to 'xml') and the XML cannot be parsed.
      If this type occurs, this function will raise error 74049.
</li>
<li><b>'system-error'</b> - indicates that the receiver cannot process the
      request due to a condition not related to this protocol.  Servers
      SHOULD send a system-error when they are capable of responding to
      requests but not capable of processing requests.
      If this type occurs, this function will raise error 74050.
</li>
<li><b>'authority-error'</b> - indicates that the intended authority
      specified in the corresponding request is not served by the
      receiver.  Servers SHOULD send an authority error when they
      receive a request directed to an authority other than those they
      serve.
      If this type occurs, this function will raise error 74051.
</li>
<li><b>'no-inflation-support-error'</b> - indicates that the receiver does
      not support payloads that have been compressed with DEFLATE [1].
      Servers MUST send this error when they receive a request that has
      been compressed with DEFLATE but they do not support inflation.
      If this type occurs, this function will raise error 74051.
</li>
</ul>

 * \ingroup DCHK_API_DCHK
 */
{
	xmlDoc *doc = NULL;
	doc = xmlReadMemory (handle->response.uncompressed_payload,
			handle->response.size_uncompressed, NULL, NULL, 0);
	if (doc == NULL) {
		irislwz_SetError(handle,74014,NULL);
		return 0;
	}
	char *type=NULL;
	char *desc=NULL;
	xmlNode *node1, *node2, *node3 = NULL;
	xmlAttr *attr;
	for (node1 = doc->children; node1; node1 = node1->next) {
		if (node1->type==XML_ELEMENT_NODE && strcmp((char*)node1->name,"other")==0) {
			// We need to find the type property
			for (attr = node1->properties; attr; attr = attr->next) {
				if (attr->type==XML_ATTRIBUTE_NODE && strcmp((char*)attr->name,"type")==0) {
					// And finaly the text node
					for (node3 = attr->children; node3; node3 = node3->next) {
						if (node3->type==XML_TEXT_NODE && strcmp((char*)node3->name,"text")==0) {
							type=(char*)node3->content;
							break;
						}
					}
					break;
				}
			}
			// Find description
			for (node2 = node1->children; node2; node2 = node2->next) {
				if (node2->type==XML_ELEMENT_NODE && strcmp((char*)node2->name,"description")==0) {
					// Find the text node
					for (node3 = node2->children; node3; node3 = node3->next) {
						if (node3->type==XML_TEXT_NODE && strcmp((char*)node3->name,"text")==0) {
							desc=(char*)node3->content;
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}
	if (strcasecmp(type,"descriptor-error")==0) irislwz_SetError(handle,74048,desc);
	else if (strcasecmp(type,"payload-error")==0) irislwz_SetError(handle,74049,desc);
	else if (strcasecmp(type,"system-error")==0) irislwz_SetError(handle,74050,desc);
	else if (strcasecmp(type,"authority-error")==0) irislwz_SetError(handle,74051,desc);
	else if (strcasecmp(type,"no-inflation-support-error")==0) irislwz_SetError(handle,74052,desc);
	else {
		xmlFreeDoc(doc);
		irislwz_SetError(handle,74053,NULL);
		return 0;
	}
	xmlFreeDoc(doc);
	return 1;
}

int dchk_XML_ParseDomain(IRISLWZ_HANDLE *handle, void *node, DCHK_RECORD *r)
/*!\brief Parse a domain node
 *
 * This function is internally called to parse a domain node of an answer section. It searches for
 * status, domainName and lastDatabaseUpdateDateTime and stores the result in the DCHK_RECORD \p r.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] node pointer to the XML node which should be inspected
 * \param[in,out] r pointer to a DCHK_RECORD object, in which the result should be stored.
 * \returns On success the function returns 1, in case of an error it returns 0.
 *
 * \ingroup DCHK_API_DCHK
 */
{
	xmlNode *cur_node = NULL;
	xmlNode *status=NULL;
	xmlNode *domainName=NULL;
	xmlNode *idn=NULL;
	xmlNode *lastDatabaseUpdateDateTime=NULL;
	if (!handle) return 0;
	if (!node) {
		irislwz_SetError(handle,74019,NULL);
		return 0;
	}
	for (cur_node = ((xmlNode*)node)->children; cur_node; cur_node = cur_node->next) {
		//printf ("Node: %s\n",cur_node->name);
		if (cur_node->type==XML_ELEMENT_NODE) {
			if (strcmp((const char*)cur_node->name,"status")==0) {
				status=cur_node->children;
			} else if (strcmp((const char*)cur_node->name,"domainName")==0) {
				domainName=cur_node->children;
			} else if (strcmp((const char*)cur_node->name,"idn")==0) {
				idn=cur_node->children;
			} else if (strcmp((const char*)cur_node->name,"lastDatabaseUpdateDateTime")==0) {
				lastDatabaseUpdateDateTime=cur_node->children;
			}
		}
	}
	if ((!status) || (!domainName)) {
		irislwz_SetError(handle,74017,NULL);
		return 0;
	}
	if (idn) domainName=idn;
	const char *dbDomain=NULL;
	const char *dbUpdate=NULL;
	const char *dbStatus=NULL;
	// find domain name
	for (cur_node = domainName; cur_node; cur_node = cur_node->next) {
		if (cur_node->type==XML_TEXT_NODE) {
			dbDomain=(const char*)cur_node->content;
		}
	}
	// find lastDatabaseUpdate
	for (cur_node = lastDatabaseUpdateDateTime; cur_node; cur_node = cur_node->next) {
		if (cur_node->type==XML_TEXT_NODE) {
			dbUpdate=(const char*)cur_node->content;
		}
	}
	// find Status
	for (cur_node = status; cur_node; cur_node = cur_node->next) {
		if (cur_node->type==XML_ELEMENT_NODE) {
			dbStatus=(const char*)cur_node->name;
		}
	}
	if (dbStatus) r->status=strdup(dbStatus);
	if (dbUpdate) r->lastdatabaseupdate=strdup(dbUpdate);
	return 1;
}

