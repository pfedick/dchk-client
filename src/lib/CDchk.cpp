/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.6 $
 * $Date: 2009/07/14 10:16:08 $
 * $Id: CDchk.cpp,v 1.6 2009/07/14 10:16:08 pfedick Exp $
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

#ifdef HAVE_NETINET_IN_H
	#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_NAMESER_H
#include <arpa/nameser.h>
#endif
#ifdef HAVE_RESOLV_H
#include <resolv.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#ifdef HAVE_STRING_H
	#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif


#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>  //windows
#endif

#include "dchk.h"
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


#ifdef _WIN32
#include <windns.h>   //DNS api's
#include <winsock.h>  //winsock

#else
#include "ns_parse.h"
#endif
#include "compat.h"

namespace dchk {

/*!\class CError
 * \brief Base class for storing errors
 *
 * This is a base class used by all other classes in this library for storing errors
 *
 * \ingroup DCHK_CPP_API
 */

/*!\var CError::code
 * \brief Holds the last error code
 *
 * Holds the last error code
 */

/*!\var CError::errortext
 * \brief Holds the last additional error text if any
 *
 * Holds the last additional error text if any
 */

CError::CError()
/*!\brief constructor of the class
 *
 * Initializes error code and text
 */
{
	code=0;
	errortext=NULL;
}

CError::~CError()
/*!\brief destructor of the class
 *
 * Frees all memory used by this class
 */
{
	if (errortext) free(errortext);
	errortext=NULL;
	code=0;
}

void CError::SetError(int code, const char *fmt, ...)
/*!\brief Set an error on this class
 *
 * This function is used internally to store an error in this class
 *
 * \param[in] code Error code
 * \param[in] fmt Format string for an additional error text. The syntax of this
 * string is identical to sprintf/printf.
 * \param[in] ... Optional additional parameters for the format string
 *
 */
{
	this->code=code;
	if (errortext) free(errortext);
	errortext=NULL;
	if (fmt==NULL) return;
	va_list args;
	va_start(args, fmt);
	if (vasprintf(&errortext,(char*)fmt,args)<0) errortext=NULL;
	va_end(args);
}

void CError::PrintError() const
/*!\brief Print last error to STDOUT
 *
 * This function prints the last error occurred in this class to STDOUT (Screen).
 */
{
	printf("ERROR %i: %s",code, irislwz_GetError(code));
	if (errortext) printf (" [%s]",errortext);
	printf ("\n");

}

int CError::GetErrorCode() const
/*!\brief Return last error code
 *
 * Return last error code
 * \returns last error code
 * \remarks
 * Please keep in mind that a non zero value does not necessarily mean that the
 * previous function was not successful. If a function returns a value, you
 * should always check if it is valid or indicates an error. Most functions return
 * 0 or NULL when an error occurred, and only if this is the case, you will get
 * the correct error.
 */
{
	return code;
}

const char *CError::GetErrorText() const
/*!\brief Return text of last error code
 *
 * Return text of last error code
 *
 * \returns Pointer to the error text.
 */
{
	const char *ret;
	ret=irislwz_GetError(code);
	if (!ret) ret="unknown";
	return ret;
}

const char *CError::GetAdditionalErrorText() const
/*!\brief Return additional information about last error
 *
 * Return additional information about last error.
 *
 * \returns Pointer to the additional error text, or NULL if there is no text.
 */
{
	if (errortext) return (const char*) errortext;
	return NULL;
}

void CError::CopyError(IRISLWZ_HANDLE *handle)
/*!\brief Copies an error from an IRISLWZ_HANDLE into this class
 *
 * This function is used internally to copy an error from an IRISLWZ_HANDLE
 * object into this class.
 *
 * \param[in] handle Pointer to a IRISLWZ_HANDLE object
 */
{
	code=0;
	if (errortext) free(errortext);
	errortext=NULL;
	if (!handle) return;
	if (handle->error) {
		code=handle->error;
		if (handle->errortext) errortext=strdup(handle->errortext);
	}
}


/*!\class CDomainList
 * \brief A class to hold a list of domains
 *
 * This class implements a list of domains stored in DCHK_RECORD structures. It is
 * used for bundled DCHK requests.
 *
 * \ingroup DCHK_CPP_API
 */


CDomainList::CDomainList()
/*!\brief constructor of the class
 *
 * Initializes the list.
 */
{
	list.first=list.last=ptr=NULL;
	list.num=0;
}

CDomainList::~CDomainList()
/*!\brief destructor of the class
 *
 * Frees all memory used by the class an the elements of the list.
 */
{
	Clear();
}

void CDomainList::Clear()
/*!\brief destructor of the class
 *
 * Frees all memory used by the class an the elements of the list.
 */
{
	while (list.first) {
		ptr=list.first;
		list.first=ptr->next;
		if (ptr->domain) free(ptr->domain);
		if (ptr->status) free(ptr->status);
		if (ptr->lastdatabaseupdate) free(ptr->lastdatabaseupdate);
		delete ptr;
	}
	list.first=list.last=ptr=NULL;
	list.num=0;
}

int CDomainList::Count() const
/*!\brief Returns number of elements in this list
 *
 * This function returns the number of elements in the list.
 *
 * \returns Number of elements
 */
{
	return list.num;
}

int CDomainList::AddDomain(const char *domain, int n)
/*!\brief Add a domain to the list
 *
 * With this member function you can add a domain name to the list
 *
 * \param[in] domain pointer to domain name, which must not be longer than 512 bytes
 * \param[in] n optional parameter containing the size of the \p domain string.
 * Only the first \n bytes of the \p domain string will be copied into the list.
 * If \p domain is shorter than \n, the complete string is copied.
 *
 * \returns On success, the function returns 1, otherwise 0. An error could only occur, when
 * the \p domain parameter points to NULL, when the process is out of memory or when the
 * domain name is longer than 512 bytes.
 */
{
	if (!domain) {
		SetError(74025);
		return 0;
	}
	// We don't allow domain names longer than 512 bytes
	if (strlen(domain)>512) {
		SetError(74054,"%i",512);
		return 0;
	}

	DCHK_RECORD *r=new DCHK_RECORD;
	if (!r) return 0;
	r->next=NULL;
	r->lastdatabaseupdate=NULL;
	if (n>0) {
		r->domain=strndup(domain,n);
	} else {
		r->domain=strdup(domain);
	}
	r->status=NULL;
	if (!list.first) {
		list.first=list.last=r;
		ptr=r;
	} else {
		list.last->next=r;
		list.last=r;
	}
	list.num++;
	return 1;
}

int CDomainList::Explode(const char *string, const char *delimiter)
/*!\brief Add multiple domains to the list
 *
 * With this function you can add multiple domains to the list, which are
 * all in one string and separated by a specific delimiter.
 *
 * \param[in] string Pointer to a text string containing the domain names
 * \param[in] delimiter Pointer to a string, which contains the delimiter
 * if the domains. This could be a single char (like space or tab), but also a
 * longer string.
 * \returns On success, the function returns the number of domains added to the
 * list, otherwise 0.
 */
{
	if (!string) {
		SetError(74025,"int CDomainList::Explode(==>const char *string<==, const char *delimiter)");
		return 0;
	}
	if (!delimiter) {
		SetError(74025,"int CDomainList::Explode(const char *string, ==> const char *delimiter <==)");
		return 0;
	}
	size_t l;
	if ((l=strlen(string))==0) {
		SetError(74055);
		return 0;
	}
	size_t dwsize=strlen(delimiter);
	if (dwsize==0) {
		SetError(74056);
		return 0;
	}
	const char *tokfind;
	int count=0;
	while (1) {
		// Skip empty tokens
		while ((tokfind=strstr(string,delimiter))==string) string+=dwsize;
		if(tokfind>string) {
			if (!AddDomain(string,tokfind-string)) return 0;
			count++;
			string=tokfind+dwsize;
		} else {
			if (strlen(string)>0) {
				if (!AddDomain(string)) return 0;
				count ++;
			}
			break;
		}
	}
	if (!count) {
		SetError(74055);
	}
	return count;
}


DCHK_RECORD *CDomainList::GetFirst()
/*!\brief returns pointer to first element in this list
 *
 * This function returns a \ref DCHK_RECORD pointer to the first element in the list.
 *
 * \returns Returns a \ref DCHK_RECORD pointer or NULL, if the list contains no entries.
 */
{
	if (!list.first) {
		SetError(74042);
		return NULL;
	}
	ptr=list.first;
	return GetNext();
}

void CDomainList::Reset()
/*!\brief Resets the pointer to the first element
 *
 * This function resets the internal pointer to the first element in the
 * list. Next call to CDomainList::GetNext will return the first element.
 *
 */
{
	ptr=list.first;
}

DCHK_RECORD *CDomainList::GetNext()
/*!\brief returns pointer to next element in this list
 *
 * This function returns a \ref DCHK_RECORD pointer to the next element in the list.
 *
 * \returns Returns a \ref DCHK_RECORD pointer or NULL, if the end of the
 * list was reached.
 */
{
	if (!ptr) {
		SetError(74043,"%i",list.num);
		return NULL;
	}
	DCHK_RECORD *r=ptr;
	ptr=ptr->next;
	return r;
}

/*!\class CDchk
 * \brief DCHK Implementation
 *
 * This class implements the Domain Availability Check (DCHK) Registry Type
 * for the Internet Registry Information Service (IRIS) as described in
 * <a href="http://tools.ietf.org/html/rfc5144">RFC 5144</a>. It is derived
 * from the class CIrisLWZ.
 *
 * In contrast to the CIrisLWZ base class this class enables compression by default.
 *
 * \ingroup DCHK_CPP_API
 *
 */


CDchk::CDchk()
/*!\brief Constructor of the class
 *
 * The constructor initializes the internal structures and sets some defaults.
 */
{
	maxdomainsperquery=20;
	SendDeflated(false);
	AcceptDeflated(true);
}

CDchk::~CDchk()
/*!\brief Destructor of the class
 *
 * The destructor of this class does actually nothing
 */
{
}

void CDchk::SetMaxDomainsPerQuery(int count)
/*!\brief Sets the maximum number of domains per query
 *
 * This function sets the maximum number of domains, which can be bundled
 * into one request.
 *
 * \param[in] count Number of domains. A value of "0" means unlimited.
 *
 */
{
	maxdomainsperquery=count;
}

int CDchk::SetHostByDNS(const char *tld)
/*!\brief Set hostname of DCHK server by using DNS
 *
 * If you don't know the hostname of the DCHK server you can use this function to
 * make a DNS lookup for the specified TLD. The function will query the name server for
 * the given TLD and check if there is a NAPTR- and a SRV record for "DCHK1:iris.lwz".
 *
 * \param[in] tld pointer to a string with the desired TLD.
 * \returns If the function can find a suitable DCHK server, the server is automatically
 * configured by calling \ref CDchk::SetHost and the function returns 1 (success). You can use
 * the functions \ref CDchk::GetHostname and \ref CDchk::GetPort to get the results. If no DCHK
 * server can be found or if an error occurred, the function will return 0.
 *
 * \remarks
 * This function will only succeed if the registry for the specified TLD has a NAPTR record
 * for "DCHK1:iris.lwz" and a SRV record in it's zonefile.
 */
{
	int ret=dchk_SetHostByDNS(&handle, tld);
	if (!ret) CopyError(&handle);
	return ret;
}

int CDchk::QueryDomainList(CDomainList &list)
/*!\brief Checks status of domains in domain list
 *
 * This function takes a list of domains and checks them against the DCHK server.
 * Multiple domains are bundled to one request. If dchk_SetMaxDomainsPerQuery was called,
 * then it will bundle up to the number of domains configured, otherwise it will bundle as
 * many domains as fit into the request payload. For each bundled request it will call DoQuery
 * to do the actual DCHK call. The result for each domain is stored back in the list.
 *
 * \param[in,out] list Reference to a CDomainList object
 * \returns On success, the function returns 1 and the \p domain list is updated. If an
 * error occurred, it returns 0.
 *
 */
{
	int ret=dchk_QueryDomainList(&handle, &list.list, maxdomainsperquery);
	if (!ret) CopyError(&handle);
	return ret;

}

int CDchk::QueryDomain(const char *domain, char **status, char **dbupdate)
/*!\brief Checks status of a single domain
 *
 * This function takes a single domain name and checks it's status against the DCHK server.
 * The result is returned in \p status and \p dbupdate.
 *
 * \param[in] domain pointer to the domain name
 * \param[out] status pointer to a status variable, where the status of the domain is stored
 * \param[out] dbupdate pointer to the last update date of the DCHK database
 * \returns On success, the function returns 1 and \p status and \p dbupdate get updated as long
 * as their pointers are not NULL. Please note that you have to free the memory used by
 * \p status and \p dbupdate manually!
 * If an error occurred, it returns 0 and the value of status and dbupdate is unchanged.
 *
 */
{
	int ret=dchk_QueryDomain(&handle, domain, status, dbupdate);
	if (!ret) CopyError(&handle);
	return ret;
}

int CDchk::PrintResults(CDomainList &list)
/*!\brief Print the contents of a DCHK_DOMAINLIST to STDOUT
 *
 * This function prints the contents of a DCHK_DOMAINLIST to STDOUT. If the status of a
 * domain in the list was already checked, it's status and timestamp of last database
 * update is printed, otherwise it prints "unchecked".
 *
 * \param[in] list Referencer to a CDomainList object
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
 */
{
	int ret=dchk_PrintResults(&list.list);
	if (!ret) CopyError(&handle);
	return ret;
}


} // EOF namespace dchk
