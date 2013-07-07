/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.5 $
 * $Date: 2009/03/12 11:56:19 $
 * $Id: dchk.c,v 1.5 2009/03/12 11:56:19 pfedick Exp $
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

#define _GNU_SOURCE
#ifdef HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef HAVE_STDIO_H
	#include <stdio.h>
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

#ifdef HAVE_ERRNO_H
	#include <errno.h>
#endif

#include <zlib.h>

#include "dchk.h"
#include "compat.h"

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



DCHK_DOMAINLIST *dchk_NewDomainList()
/*!\brief Create a new domain list
 *
 * This function creates a new domain list, which is used to check status of multiple domains
 * in bundled requests.
 *
 * \returns On success a pointer to a new DCHK_DOMAINLIST is returned, otherwise NULL. The function could
 * only fail when the process is out of memory.
 *
 * \example
 * \code
	// Create the domain list
	DCHK_DOMAINLIST *list=dchk_NewDomainList();
	// Assume all program parameters are domain names
	int i;
	for (i=1;i<=argc;i++) dchk_AddDomain(list,argv[i]);
	// Do something with the list
	...
	// Free the domain list
	dchk_FreeDomainList(list);

 * \endcode
 *
 * \see
 * Use \ref dchk_FreeDomainList to free the list and \ref dchk_AddDomain to add domains.
 * \ingroup DCHK_API_DCHK
 */
{
	DCHK_DOMAINLIST *q=(DCHK_DOMAINLIST*)malloc(sizeof(DCHK_DOMAINLIST));
	if (!q) return NULL;
	q->first=NULL;
	q->last=NULL;
	q->num=0;
	return q;
}

void dchk_FreeDomainList(DCHK_DOMAINLIST *list)
/*!\brief Frees a domain list
 *
 * This function frees a previously created domain list and all it's contents.
 *
 * \param[in] list pointer to a DCHK_DCHK_DOMAINLIST object
 *
 * \example
 * See \ref dchk_NewDomainList
 *
 * \see
 * Use \ref dchk_NewDomainList to create the list and \ref dchk_AddDomain to add domains.
 *
 * \ingroup DCHK_API_DCHK
 */
{
	if (!list) return;
	DCHK_RECORD *r,*first=list->first;
	while (first) {
		r=first;
		first=r->next;
		if (r->domain) free(r->domain);
		if (r->status) free(r->status);
		if (r->lastdatabaseupdate) free(r->lastdatabaseupdate);
		free(r);
	}
	free(list);
}

int dchk_AddDomain(DCHK_DOMAINLIST *list, const char *domain)
/*!\brief Add a domain to a domain list
 *
 * This function adds a domain name to the \p list
 *
 * \param[in] list pointer to a DCHK_DCHK_DOMAINLIST object, which was created with dchk_NewDomainList
 * \param[in] domain pointer to a domain name, which must not be longer than 512 bytes
 * \returns On success, the function returns 1, otherwise 0. An error could only occur, when one
 * of the parameters was NULL, not enough memory is available or the domain name is longer
 * that 512 bytes.
 *
 * \example
 * See \ref dchk_NewDomainList
 *
 * \see
 * Use \ref dchk_NewDomainList to create the list and \ref dchk_FreeDomainList to free the list.
 *
 * \ingroup DCHK_API_DCHK
 */
{
	if (!list) return 0;
	if (!domain) return 0;
	// We don't allow domain names longer than 512 bytes
	if (strlen(domain)>512) {
		return 0;
	}
	DCHK_RECORD *r=(DCHK_RECORD*)malloc(sizeof(DCHK_RECORD));
	if (!r) return 0;
	r->next=NULL;
	r->domain=strdup(domain);
	r->lastdatabaseupdate=NULL;
	r->status=NULL;
	if (!list->first) {
		list->first=r;
		list->last=r;
	} else {
		list->last->next=r;
		list->last=r;
	}
	list->num++;
	return 1;
}

int dchk_AddnDomain(DCHK_DOMAINLIST *list, const char *domain, int n)
/*!\brief Add a portion of a string as domain to a domain list
 *
 * This function adds a domain name to the \p list
 *
 * \param[in] list pointer to a DCHK_DCHK_DOMAINLIST object, which was created with dchk_NewDomainList
 * \param[in] domain pointer to a domain name, which must not be longer than 512 bytes
 * \param[in] n Size of the \p domain string. Only the first \n bytes of the \p domain string will be
 * copied into the list. If \p domain is shorter than \n, the complete string is copied.
 * \returns On success, the function returns 1, otherwise 0. An error could only occur, when one
 * of the parameters was NULL, not enough memory is available or the domain name is longer
 * that 512 bytes.
 *
 * \example
 * See \ref dchk_NewDomainList
 *
 * \see
 * Use \ref dchk_NewDomainList to create the list and \ref dchk_FreeDomainList to free the list.
 *
 * \ingroup DCHK_API_DCHK
 */
{
	if (!list) return 0;
	if (!domain) return 0;
	// We don't allow domain names longer than 512 bytes
	if (strlen(domain)>512) {
		return 0;
	}
	DCHK_RECORD *r=(DCHK_RECORD*)malloc(sizeof(DCHK_RECORD));
	if (!r) return 0;
	r->next=NULL;
	r->domain=strndup(domain,n);
	r->lastdatabaseupdate=NULL;
	r->status=NULL;
	if (!list->first) {
		list->first=r;
		list->last=r;
	} else {
		list->last->next=r;
		list->last=r;
	}
	list->num++;
	return 1;
}

int dchk_ExplodeDomain(DCHK_DOMAINLIST *list, const char *string, const char *delimiter)
/*!\brief Add multiple domains to the list
 *
 * With this function you can add multiple domains to the list, which are
 * all in one string and separated by a specific delimiter.
 *
 * \param[in] list pointer to a DCHK_DCHK_DOMAINLIST object, which was created with dchk_NewDomainList
 * \param[in] string Pointer to a text string containing the domain names
 * \param[in] delimiter Pointer to a string, which contains the delimiter
 * if the domains. This could be a single char (like space or tab), but also a
 * longer string.
 * \returns On success, the function returns the number of domains added to the
 * list, otherwise 0.
 */
{
	if (!list) return 0;
	if (!string) return 0;
	if (!delimiter) return 0;
	size_t l;
	if ((l=strlen(string))==0) return 0;
	size_t dwsize=strlen(delimiter);
	if (dwsize==0) return 0;
	const char *tokfind;
	while (1) {
		// Skip empty tokens
		while ((tokfind=strstr(string,delimiter))==string) string+=dwsize;
		if(tokfind>string) {
			if (!dchk_AddnDomain(list,string,tokfind-string)) return 0;
			string=tokfind+dwsize;
		} else {
			if (strlen(string)>0) {
				if (!dchk_AddDomain(list,string)) return 0;
			}
			break;
		}
	}
	return 1;
}


static int dchk_DoQuery(IRISLWZ_HANDLE *handle, DCHK_DOMAINLIST *domainset, char *payload)
/*!\brief Perform an actual query to the DCHK server
 *
 * \internal
 * This function is used internally by \ref dchk_QueryDomainList. It sends a single query with
 * an already bundled domain list to the server and parses the result back into the domain list.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in,out] domainset Pointer to an DCHK_DOMAINLIST object in which the results are stored
 * \param[in] payload Pointer to the already constructed XML request
 *
 * \returns
 * On success the function returns 1 and updates the domains in the \p domainset. In case of an error
 * it returns 0.
 *
 * \ingroup DCHK_API_DCHK
 */
{
	if (!handle) return 0;
	if (!domainset) {
		irislwz_SetError(handle,74025,"int dchk_DoQuery(IRISLWZ_HANDLE *handle,==> DCHK_DOMAINLIST *domainset <==, char *payload)");
		return 0;
	}
	if (!payload) {
		irislwz_SetError(handle,74025,"int dchk_DoQuery(IRISLWZ_HANDLE *handle,DCHK_DOMAINLIST *domainset,==> char *payload <==)");
		return 0;
	}
	if (!irislwz_Query(handle,payload)) return 0;
	// Check if we have size or other information
	if (!irislwz_CheckResponse(handle)) return 0;
	return dchk_XML_ParseResult(handle,domainset);
}


int dchk_EscapeSpecialChars(const char *string, char *buffer, int buffersize)
/*!\brief Escape some special characters, which are not allowed in XML
 *
 * This function translates some some special characters, which are not allowed within XML:
 * - '&' (ampersand) becomes '&amp;'
 * - '"' (double quote) becomes '&quot;'
 * - ''' (single quote) becomes '&#039;'
 * - '<' (less than) becomes '&lt;'
 * - '>' (greater than) becomes '&gt;'
 *
 * \param[in] string Pointer to a 0-terminated unescaped string
 * \param[in,out] buffer Pointer to a result buffer, which must be big enough to receive the escaped string
 *            In worst case, this could be 6 times the original string length
 * \param[in] buffersize Size of the result buffer
 *
 * \returns
 * On success the function returns 1 and the converted string can be found in \p buffer. If an error occurs,
 * it returns 0. This could happen, if the buffer is not big enough.
 * \since
 * This function was added in version 0.5.4
 *
 * \ingroup DCHK_API_DCHK
 */
{
	if (!string) return 0;
	if (!buffer) return 0;
	if (buffersize==0) return 0;
	unsigned char *p=(unsigned char*)string;
	int t=0;
	int l;
	unsigned char c;
	char *translate;
	while ((c=p[0])) {
		translate=NULL;
		switch (c) {
			case '&':
				translate="&amp;";
				break;
			case '\"':
				translate="&quot;";
				break;
			case '\'':
				translate="&#039t;";
				break;
			case '<':
				translate="&lt;";
				break;
			case '>':
				translate="&gt;";
				break;
		}
		if (translate) {
			l=strlen(translate);
			if (t+l<buffersize) {
				strncpy(buffer+t,translate,l);
				t+=l;
			} else {
				return 0;
			}
		} else {
			if (t+1<buffersize) buffer[t]=c;
			else return 0;
			t++;
		}
		p++;
	}
	buffer[t]=0;
	return 1;
}


int dchk_QueryDomain(IRISLWZ_HANDLE *handle, const char *domain, char **status, char **dbupdate)
/*!\brief Checks status of a single domain
 *
 * This function takes a single domain name and checks it's status against the DCHK server.
 * The result is returned in \p status and \p dbupdate.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] domain pointer to the domain name
 * \param[out] status pointer to a status variable, where the status of the domain is stored
 * \param[out] dbupdate pointer to the last update date of the DCHK database
 * \returns On success, the function returns 1 and \p status and \p dbupdate get updated as long
 * as their pointers are not NULL. Please note that you have to free the memory used by
 * \p status and \p dbupdate manually!
 * If an error occurred, it returns 0 and the value of status and dbupdate is unchanged.
 *
 * \ingroup DCHK_API_DCHK
 */
{
	if (!handle) return 0;
	if (!domain) {
		irislwz_SetError(handle,74025,"");
		return 0;
	}
	DCHK_DOMAINLIST list;
	DCHK_RECORD r;
	r.domain=(char*)domain;
	r.status=NULL;
	r.lastdatabaseupdate=NULL;
	r.next=NULL;
	list.first=list.last=&r;
	list.num=1;
	int ret=dchk_QueryDomainList(handle,&list,0);
	if (ret) {
		if (status) *status=r.status;
		if (dbupdate) *status=r.lastdatabaseupdate;
	} else {
		if (r.status) free(r.status);
		r.status=NULL;
		if (r.lastdatabaseupdate) free(r.lastdatabaseupdate);
		r.lastdatabaseupdate=NULL;
	}
	return ret;
}



int dchk_QueryDomainList(IRISLWZ_HANDLE *handle, DCHK_DOMAINLIST *list, int maxdomainsperquery)
/*!\brief Checks status of domains in domain list
 *
 * This function takes a list of domains and checks them against the DCHK server.
 * Multiple domains are bundled to one request. If dchk_SetMaxDomainsPerQuery was called,
 * then it will bundle up to the number of domains configured, otherwise it will bundle as
 * many domains as fit into the request payload. For each bundled request it will call DoQuery
 * to do the actual DCHK call. The result for each domain is stored back in the list.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in,out] list pointer to DCHK_DOMAINLIST, which was created with dchk_NewDomainList
 * and filled with dchk_AddDomain
 * \param[in] maxdomainsperquery maximum number of domains which can be bundled to one request
 * A value of "0" means unlimited.
 * \returns On success, the function returns 1 and the domain \p list is updated. If an
 * error occurred, it returns 0.
 *
 * \ingroup DCHK_API_DCHK
 */
{
	if (!handle) return 0;
	if (!list) {
		irislwz_SetError(handle,74025,"int dchk_QueryDomainList(IRISLWZ_HANDLE *handle, ==> DCHK_DOMAINLIST *list <==)");
		return 0;
	}
	if (list->num==0) {
		irislwz_SetError(handle,74038,NULL);
		return 0;
	}
	if (maxdomainsperquery<0) maxdomainsperquery=0;
	// We need a buffer for the request payload
	// We allocate 5000 bytes for complete uncompressed payload an 1000 bytes
	// for a single searchSet
	char *payload=(char*)malloc(7000);
	// A buffer for the searchSet
	char *searchSet=payload+5000;
	// And a buffer for escaping special characters in domain name
	char *escaped=payload+6000;
	int newlen,len=0;
	if (!payload) {
		irislwz_SetError(handle,74024,NULL);
		return 0;
	}
	payload[0]=0;
	DCHK_RECORD *r=list->first;
	DCHK_DOMAINLIST domainset;
	domainset.first=r;
	int count=0;
	int i;
	char *entityClass;
	while (r) {
		if (len==0) {			// Do we already have data?
			sprintf(payload,"<iris1:request xmlns:iris1=\"urn:ietf:params:xml:ns:iris1\">");
			len=strlen(payload);
		}
		// For plain ASCII domains, entityClass is "domain-name"
		entityClass="domain-name";
		// if we have non ASCII chars in domain, we use entityClass "idn"
		for (i=0;r->domain[i]!=0;i++) {
			if ((r->domain[i])&128) {
				entityClass="idn";
				break;
			}
		}
		// We have to handle some special characters, which are not allowed in XML: & < >
		if (!dchk_EscapeSpecialChars(r->domain,escaped,1000)) {
			free(payload);
			irislwz_SetError(handle,74106,NULL);
			return 0;
		}

		// Build the searchSet. The "AddDomain" functions take already care that a domain name
		// must not be longer than 512 bytes, therefore we don't need to check the size here
		sprintf(searchSet,"<iris1:searchSet>"
			"<iris1:lookupEntity registryType=\"dchk1\" entityClass=\"%s\" "
			"entityName=\"%s\"/>"
			"</iris1:searchSet>", entityClass,escaped);
		// does query fit in request packet size?
		if (strlen(payload)+strlen(searchSet)+17>=5000) {
			// We have only allocated 5000 bytes for uncompressed request packet and
			// we have reached this limit
			newlen=99999;
		} else {
			strcat(payload,searchSet);
			strcat(payload,"</iris1:request>");
			newlen=strlen(payload);
			// Support compression?
			if (handle->sendDeflated) {
				newlen=irislwz_GetCompressedSize(payload,newlen,handle->rfc1950_workaround);
				if (!newlen) {
					irislwz_SetError(handle,74039,NULL);
					free(payload);
					return 0;
				}
			}
		}
		if (newlen<5000 && (count<maxdomainsperquery || maxdomainsperquery==0)) {
			// Request does fit, we add it permanently to payload
			payload[len]=0;
			strcat(payload,searchSet);
			len=strlen(payload);
			count++;
		} else {
			// Request does not fit or we reached maximum number of domains per query
			// Remove next searchSet
			payload[len]=0;
			// finalize request
			strcat(payload,"</iris1:request>");
			domainset.num=count;
			if (!dchk_DoQuery(handle,&domainset,payload)) {
				free(payload);
				return 0;
			}
			domainset.first=r;
			sprintf(payload,"<iris1:request xmlns:iris1=\"urn:ietf:params:xml:ns:iris1\">");
			strcat(payload,searchSet);
			len=strlen(payload);
			count=1;
		}
		domainset.last=r;
		r=r->next;
	}
	if (count) {
		strcat(payload,"</iris1:request>");
		domainset.num=count;
		if (!dchk_DoQuery(handle,&domainset,payload)) {
			free(payload);
			return 0;
		}
	}
	free(payload);
	return 1;
}


int dchk_PrintResults(DCHK_DOMAINLIST *list)
/*!\brief Print the contents of a DCHK_DOMAINLIST to STDOUT
 *
 * This function prints the contents of a DCHK_DOMAINLIST to STDOUT. If the status of a
 * domain in the list was already checked, it's status and timestamp of last database
 * update is printed, otherwise it prints "unchecked".
 *
 * \param[in] list Pointer to a DCHK_DOMAINLIST
 * \returns The function returns 0, if the parameter \p list is NULL, otherwise it returns 1.
 *
 * \example
 * This is an example output of the function:
 * \code
denic.de: active [2008-06-10T10:55:00+02:00]
opsblau.de: active [2008-06-10T10:55:00+02:00]
example.de: active [2008-06-10T10:55:00+02:00]
fdefdw3e.de: free
urgs#123.de: invalid
\endcode
 *
 * \ingroup DCHK_API_DCHK
 */
{
	if (!list) {
		return 0;
	}
	DCHK_RECORD *r=list->first;
	while (r) {
		printf("%s: %s",r->domain,(r->status?r->status:"unchecked"));
		if (r->lastdatabaseupdate) printf(" [%s]",r->lastdatabaseupdate);
		printf("\n");
		r=r->next;
	}
	return 1;
}

