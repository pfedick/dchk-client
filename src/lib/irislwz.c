/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.19 $
 * $Date: 2011/01/26 12:34:43 $
 * $Id: irislwz.c,v 1.19 2011/01/26 12:34:43 pfedick Exp $
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
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x501
	#endif
	#include <winsock2.h>
	#include <Ws2tcpip.h>
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
#ifdef HAVE_RESOLV_H
	#include <resolv.h>
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


typedef struct {
#ifdef _WIN32
	SOCKET	sd;
#else
	int sd;
#endif
} DCHK_SOCKET;



static void irislwz_debugout(int level, const char *text, void *privatedata)
{
	printf("%s\n",text);
}

static void VerboseOut(IRISLWZ_HANDLE *handle, const char *subject, const char *content)
{
	if (!handle) return;
	if (handle->verboseflag&64) {
	} else {
		printf ("[%s]\n%s\n",(subject?subject:""),(content?content:""));
	}
}

static char *ExplainHeader(IRISLWZ_PACKET *packet)
{
	if (!packet) return NULL;
	char *buffer=(char*)malloc(2048);
	if (!buffer) return NULL;

	int tid=ntohs(packet->header->transactionId);
	int mrl=0;
	char *authority=NULL;
	int h=packet->header->header;
	int version=(h&192)>>6;
	int auth_len=0;
	const char *type;
	if (h&32) {
		type="Response";
		authority=strdup("-");
	} else {
		type="Request";
		auth_len=packet->header->authority_length;
		authority=strndup(packet->packet+6,packet->header->authority_length);
		mrl=ntohs(packet->header->max_response_length);
	}
	const char *plt="";
	switch (h&3) {
		case 0: plt="XML";
				break;
		case 1: plt="Version Info";
				break;
		case 2: plt="Size Info";
				break;
		case 3: plt="Other Info";
				break;
	}

	sprintf(buffer,	"Packet size:             %i Bytes, Type: %s\n"
					"Version:                 %i\n"
					"Transaction Id:          %i\n"
					"Maximum Response Length: %i Bytes\n"
					"Authority Length:        %i Bytes, Authority: %s\n"
					"Payload deflated:        %s\n"
					"Deflate supported:       %s\n"
					"Payload-Type:            %s\n"
			, packet->size, type,
			version,
			tid,mrl,auth_len, (authority?authority:""),
			(h&16?"yes":"no"),
			(h&8?"yes":"no"),
			plt
			);
	if (authority) free(authority);
	return buffer;
}


static int rfc1951_deflate(Bytef *dst, uLongf *dstlen, const Bytef *src, uLong sourceLen, int level)
{
	int ret;
	z_stream z;
	memset(&z,0,sizeof(z));
	z.next_in=(Bytef*)src;
	z.avail_in=sourceLen;
	z.next_out=dst;
	z.avail_out=*dstlen;
	z.data_type=Z_ASCII;
	ret=deflateInit2(&z, level,Z_DEFLATED,-15,8,Z_DEFAULT_STRATEGY);
	if (ret!=Z_OK) {
		return ret;
	}
	ret=deflate(&z, Z_FINISH);
	/*
	if (ret==Z_STREAM_END) printf ("deflate meldet: Z_STREAM_END\n");
	else if (ret==Z_OK) printf ("deflate meldet: Z_OK\n");
	else if (ret==Z_BUF_ERROR) printf ("deflate meldet: Z_BUF_ERROR\n");
	else if (ret==Z_STREAM_ERROR) printf ("deflate meldet: Z_STREAM_ERROR\n");
	else printf ("deflate meldet: anderer Fehler: %i\n",ret);
	*/

	if (ret!=Z_STREAM_END) {
		if (ret==Z_OK) ret=Z_BUF_ERROR;
		deflateEnd(&z);
		return ret;
	}
	*dstlen=z.total_out;
	ret=deflateEnd(&z);
	return ret;
}

static int rfc1951_inflate(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen)
{
	int ret;
	z_stream z;
	memset(&z,0,sizeof(z));
	z.next_in=(Bytef*)source;
	z.avail_in=sourceLen;
	z.next_out=dest;
	z.avail_out=*destLen;
	ret=inflateInit2(&z,-15);
	if (ret!=Z_OK) {
		return ret;
	}
	ret=inflate(&z, Z_FINISH);
	/*
	if (ret==Z_STREAM_END) printf ("inflate meldet: Z_STREAM_END\n");
	else if (ret==Z_OK) printf ("inflate meldet: Z_OK\n");
	else if (ret==Z_BUF_ERROR) printf ("inflate meldet: Z_BUF_ERROR\n");
	else if (ret==Z_STREAM_ERROR) printf ("inflate meldet: Z_STREAM_ERROR\n");
	else printf ("inflate meldet: anderer Fehler: %i\n",ret);
	*/

	if (ret!=Z_STREAM_END) {
		if (ret==Z_OK) ret=Z_BUF_ERROR;
		inflateEnd(&z);
		return ret;
	}
	*destLen=z.total_out;
	ret=inflateEnd(&z);
	return ret;
}

const char *irislwz_GetVersion()
/*!\brief Returns a pointer to a string containing the version of the library
 *
 * This function returns a pointer to a string containing the version of the library.
 *
 * \returns Pointer to version string
 *
 * \see
 * - \ref irislwz_GetCopyrightString
 * - \ref irislwz_GetReleaseDate
 * - \ref irislwz_CheckMinVersion
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	return DCHK_VERSION;
}

const char *irislwz_GetCopyrightString()
/*!\brief Returns a pointer to a string containing the copyright of the library
 *
 * This function returns a pointer to a string containing the copyright of the library.
 *
 * \returns Pointer to copyright string
 *
 * \see
 * - \ref irislwz_GetVersion
 * - \ref irislwz_GetReleaseDate
 * - \ref irislwz_CheckMinVersion
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	return DCHK_COPYRIGHT;
}

const char *irislwz_GetReleaseDate()
/*!\brief Returns a pointer to a string containing the release date of the library
 *
 * This function returns a pointer to a string containing the release date of the library.
 * The format of the date is "dd.mm.yyyy".
 *
 * \returns Pointer to release date string
 *
 * \see
 * - \ref irislwz_GetVersion
 * - \ref irislwz_GetCopyrightString
 * - \ref irislwz_CheckMinVersion
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	return DCHK_RELEASEDATE;
}

int irislwz_CheckMinVersion (int major, int minor, int build)
/*!\brief Checks for a specific version of the library
 *
 * With this function you can check if the library has a specific version or is below or above.
 *
 * \param[in] major Major version of the library
 * \param[in] minor Minor version
 * \param[in] build Build number
 * \returns The function returns one of the following three values:
 * - 0: the library has exact the same version you specified
 * - -1: the libraries version is below the version you specified
 * - +1: the library has a higher version than the version you specified
 *
 * \remarks
 * If the version you want to check is "1.2.3" then "1" is the major version, "2" the minor version
 * and "3" the build.
 *
 * \see
 * - \ref irislwz_GetVersion
 * - \ref irislwz_GetCopyrightString
 * - \ref irislwz_GetReleaseDate
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	int lib=((DCHK_VERSION_MAJOR<<16)+(DCHK_VERSION_MINOR<<8)+DCHK_VERSION_BUILD);
	int req=((major<<16)+(minor<<8)+build);
	if (lib==req) return 0;
	if (lib>req) return 1;
	return -1;
}



int irislwz_Init(IRISLWZ_HANDLE *handle)
/*!\brief Initialization of IRISLWZ_HANDLE object
 *
 * Use this function to initialize a newly created IRISLWZ_HANDLE object.
 * This function must be called before any other irislwz_*- or dchk_*-Function.
 *
 * \param[in] handle pointer to an existing IRISLWZ_HANDLE object
 * \returns Returns 0, when the \p handle parameter points to NULL, otherwise
 * returns 1 to indicate a successful initialization.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	memset(handle, 0, sizeof(IRISLWZ_HANDLE));
	handle->port=715;
	handle->initialized=1;
	handle->authority=NULL;
	handle->localcharset=strdup("UTF-8");
	handle->maximumResponseLength=1500;
	handle->timeout_seconds=5;
	handle->timeout_useconds=0;
	handle->socket=(DCHK_SOCKET*)malloc(sizeof(DCHK_SOCKET));
	((DCHK_SOCKET*)handle->socket)->sd=0;
	handle->DebugHandler=irislwz_debugout;
	return 1;
}

int irislwz_Exit(IRISLWZ_HANDLE *handle)
/*!\brief Deinitialize of IRISLWZ_HANDLE object
 *
 * Use this function on a IRISLWZ_HANDLE object, if you don't need it any longer.
 * The function will deinitialize the object an free all memory associated
 * to it.
 *
 * \param[in] handle pointer to an existing IRISLWZ_HANDLE object
 * \returns Returns 0, when the \p handle parameter points to NULL, otherwise
 * returns 1 to indicate a successful deinitialization.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	if (!handle->initialized) return 1;
	if (handle->errortext) free(handle->errortext);
	if (handle->authority) free(handle->authority);
	if (handle->server) free(handle->server);
	if (handle->localcharset) free(handle->localcharset);
	if (handle->socket) free(handle->socket);
	irislwz_FreePacket(&handle->request);
	irislwz_FreePacket(&handle->response);
	memset(handle, 0, sizeof(IRISLWZ_HANDLE));
	return 1;
}

void irislwz_SetDebugLevel(IRISLWZ_HANDLE *handle, int level)
/*!\brief Sets debug level
 *
 * This functions sets the debug level for debug output. By default the library does not
 * output any debug (level=0). The higher the level, the more output you will get.
 * You can specify a debug handler if you want the output anywhere else than STDOUT
 * (see \ref irislwz_SetDebugHandler).
 *
 * \param[in] handle pointer to an existing IRISLWZ_HANDLE object
 * \param[in] level debug level. 0=no debug
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return;
	handle->debuglevel=level;
}

void irislwz_SetDebugHandler(IRISLWZ_HANDLE *handle, void (*DebugHandler)(int, const char *, void *), void *privatedata)
/*!\brief Sets debug handler
 *
 * If you set a debug level higher than 0, you will get debug output on STDOUT. If you want
 * the output somewhere else, you can specify a debug handler or callback function.
 * The function must have the following syntax:
 *
 * \code
 * void debughandler(int level, const char *text, void *privatedata)
 * {
 * 	// Do something, e.g. printf("%s\n",text);
 * }
 * \endcode
 *
 * \param[in] handle pointer to an existing IRISLWZ_HANDLE object
 * \param[in] DebugHandler Pointer to the function that will receive the debug output
 * \param[in] privatedata Pointer to some optional data, which is passed to the
 * callback function
 * \see
 * \ref irislwz_SetDebugLevel
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return;
	handle->DebugHandler=DebugHandler;
	handle->debugdata=privatedata;
}

void irislwz_Debug(IRISLWZ_HANDLE *handle, int level, const char *fmt, ...)
/*!\brief Print to debug handler
 *
 * This function is used to print to the debug handler
 *
 * \param[in] handle Pointer to the DCHK-Object
 * \param[in] level Debug level
 * \param[in] fmt Format string for an additional error text. The syntax of this
 * string is identical to sprintf/printf.
 * \param[in] ... Optional additional parameters for the format string
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return;
	if (level>handle->debuglevel || handle->DebugHandler==NULL) return;
	char *buff=NULL;
	va_list args;
	va_start(args, fmt);
	if (vasprintf(&buff,(char*)fmt,args)<0) {
		buff=NULL;
	}
	va_end(args);
	if (!buff) return;
	handle->DebugHandler(level,buff,handle->debugdata);
	free (buff);
}

int irislwz_SetTimeout(IRISLWZ_HANDLE *handle, int seconds, int useconds)
/*!\brief Set timeout for communication with an IRISLWZ server
 *
 * With this function you can set a timeout for the communication
 * with a server. After sending a request packet to the server the client
 * has to wait for the response. As UDP is an unreliable protocol the client
 * can not detect if the request packet reached it's destination. It is also possible,
 * that the answer packet got lost. By default the library has a timeout of 5 seconds.
 *
 * With this function you can define a shorter timeout.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] seconds Number of seconds. If you pass 0 seconds, the library uses no
 * timeout. If no response arrives, the library blocks until the operating system issues a
 * timeout, which could be several minutes.
 * \param[in] useconds Number of microseconds
 * \returns Always returns 1 except the pointer to the IRISLWZ_HANDLE object \p handle was NULL.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->timeout_seconds=seconds;
	handle->timeout_useconds=useconds;
	return 1;
}

int irislwz_SetAuthority(IRISLWZ_HANDLE *handle, const char *authority)
/*!\brief Sets the authority for IRISLWZ requests
 *
 * With this function you can define the authority which is used in IRISLWZ request packets.
 * The default is  not to use an authority.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] authority Pointer to an authority string. The string must not be longer than 255 octets.
 * \returns Returns 1 on success or 0 when an error occurred.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	if (handle->authority) free(handle->authority);
	handle->authority=NULL;
	if (!authority) return 1;
	if (strlen(authority)>255) {
		irislwz_SetError(handle,74027,"%i",strlen(authority));
		return 0;
	}
	handle->authority=strdup(authority);
	if (handle->authority) return 1;
	irislwz_SetError(handle,74024,NULL);
	return 0;
}

int irislwz_SetAuthorityByDomain(IRISLWZ_HANDLE *handle, const char *domain)
/*!\brief Sets authority for IRISLWZ requests by using the TLD of a domain
 *
 * This function uses the TLD of the given \p domain to set the authority for
 * further requests.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] domain pointer to a domain name
 * \returns Returns 1 on success or 0 when an error occurred.
 *
 * \note
 * If the TLD is set to "arpa", a special handling takes place. At the moment the
 * library only checks if the domain ends with "9.4.e164.arpa" (german ENUM domains). If this is
 * the case "9.4.e164.arpa" is used as authority name. Other arpa zones are not implemented.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	if (!domain) {
		irislwz_SetError(handle,74025,"int irislwz_SetAuthorityByDomain(IRISLWZ_HANDLE *handle, ==> const char *domain <==)");
	}
	const char *tld=strrchr(domain,'.');		// Find last occurrance of '.'
	if (tld) tld++;								// Point to first char after '.'
	if (!tld) {
		irislwz_SetError(handle,74058,domain);
		return 0;
	}
	if (strcasecmp(tld,"arpa")==0) {
		// We need to do some more work. For now only support german ENUM domains
		// TODO: How can we implement a generic support?
		if (strstr(domain,"9.4.e164.arpa")) tld="9.4.e164.arpa";
	}
	return irislwz_SetAuthority(handle,tld);
}

int irislwz_SetAuthorityByDomainList(IRISLWZ_HANDLE *handle, DCHK_DOMAINLIST *list)
/*!\brief Sets authority for IRISLWZ requests by using a domain list
 *
 * This function uses the first domain in the domain \p list to set an authority name for
 * further requests.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] list pointer to a DCHK_DOMAINLIST object
 * \returns Returns 1 on success or 0 when an error occurred.
 *
 * \remarks
 * The function actually only picks the first domain name from the list and calls the function
 * irislwz_SetAuthorityByDomain.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	if (!list) {
		irislwz_SetError(handle,74025,"int irislwz_SetAuthorityByDomainList(IRISLWZ_HANDLE *handle, ==> DCHK_DOMAINLIST *list <==)");
		return 0;
	}
	if (list->num==0 || list->first==NULL) {
		irislwz_SetError(handle,74042,"");
		return 0;
	}
	return irislwz_SetAuthorityByDomain(handle,(const char*)list->first->domain);
}

int irislwz_UseRFC1950(IRISLWZ_HANDLE *handle, int flag)
/*!\brief Use RFC 1950 for compression
 *
 * The IRIS LWZ RFC (<a href="http://tools.ietf.org/html/rfc4993">RFC 4993</a>) describes to use
 * the deflate algorithm described in <a href="http://tools.ietf.org/html/rfc4993">RFC 1951</a>
 * for compression. But the first version of DENIC's DCHK server, client and also this client
 * wrongly used <a href="http://tools.ietf.org/html/rfc4993">RFC 1950</a>, which is a zlib
 * wrapper around <a href="http://tools.ietf.org/html/rfc4993">RFC 1951</a>. Until the bug is fixed
 * in the server, you can use this option to keep the wrong compression.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] flag Use "0" to use <a href="http://tools.ietf.org/html/rfc4993">RFC 1950</a> as it was
 * meant to, or "1" to use the wrong compression.
 * \returns Always returns 1 except the pointer to the IRISLWZ_HANDLE object \p handle was NULL.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->rfc1950_workaround=(char)flag&1;
	return 1;
}

int irislwz_SetMaximumResponseLength(IRISLWZ_HANDLE *handle, int bytes)
/*!\brief Sets the maximum acceptable packet size for answer packets
 *
 * By default the client accepts a maximum packet size of 1500 octets (Bytes), which is the
 * recommend size by the <a href="http://tools.ietf.org/html/rfc4993">Lightweight UDP Transfer Protocol RFC</a>,
 * when the path MTU is unknown.
 * With this function you can set any other size.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] bytes number of bytes
 * \returns Always returns 1 (success).
 *
 * \remarks
 * Please keep in mind, that even if you specify a bigger packet size, the DCHK server will
 * not exceed 4000 octets. If the answer does not fit into the answer packet, the server will send
 * a "size information".
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->maximumResponseLength=bytes;
	return 1;
}

int irislwz_SetHost(IRISLWZ_HANDLE *handle, const char *host, int port)
/*!\brief Set hostname an port of an IRISLWZ server
 *
 * With this function you can specify hostname or IP address and port of IRISLWZ server
 * manually. If you don't know the hostname, you can use the function \ref dchk_SetHostByDNS
 * and let the DCHK library find out the server name by using DNS.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] host name or IP address of the server which should be used
 * \param[in] port port of the server (default=715)
 * \returns Returns 1 on success or 0 when an error occurred.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	if (!host) {
		irislwz_SetError(handle,74025,"int irislwz_SetHost(IRISLWZ_HANDLE *handle, ==> const char *host <==, int port)");
		return 0;
	}
	if (handle->server) free(handle->server);
	handle->server=strdup(host);
	if (!handle->server) {
		irislwz_SetError(handle,74024,NULL);
		return 0;
	}
	if (port) handle->port=port;
	else handle->port=715;
	return 1;
}


const char *irislwz_GetHostname(IRISLWZ_HANDLE *handle)
/*!\brief Returns pointer to the hostname of a previous set server
 *
 * This function returns a pointer to the previous set hostname. This could
 * have been done by explicit calling the irislwz_SetHost function or automatically by
 * calling dchk_SetHostByDNS.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \returns Returns a pointer to the hostname or NULL, if the hostname was not set yet.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return NULL;
	if (!handle->server) {
		irislwz_SetError(handle,74026,NULL);
		return NULL;
	}
	return handle->server;
}

int irislwz_GetPort(IRISLWZ_HANDLE *handle)
/*!\brief Returns port of a previous set server
 *
 * This function returns the port of a previous set IRISLWZ server. This could
 * have been done by explicit calling the irislwz_SetHost function or automatically by
 * calling dchk_SetHostByDNS.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \returns Returns port of server or 0 if no server was set, yet.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	if (handle->server==NULL || handle->port==0) {
		irislwz_SetError(handle,74026,NULL);
		return 0;
	}
	return handle->port;
}

int irislwz_AcceptDeflated(IRISLWZ_HANDLE *handle, int flag)
/*!\brief Accept compressed answers
 *
 * Tell the client if it should accept zlib compressed answers or not
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] flag set to 1, if we accept compressed answers, set to 0 if not
 * \returns Always returns 1.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->acceptDeflated=(char) flag;
	return 1;
}

int irislwz_SendDeflated(IRISLWZ_HANDLE *handle, int flag)
/*!\brief Send compressed requests
 *
 * Tell the client if it should send zlib compressed requests or not
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] flag set to 1, if we want to send compressed requests, set to 0 if not
 * \returns Always returns 1.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->sendDeflated=(char) flag;
	return 1;
}

int irislwz_IgnoreInvalidXML(IRISLWZ_HANDLE *handle, int flag)
/*!\brief Tell the library to ignore invalid XML
 *
 * Normally the client would stop and raise a validation error if the request payload
 * contains invalid XML. With this function you can disable this check, so it is
 * possible to test how the server reacts to an invalid XML request.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] flag set to 1, if we want to ignore invalid XML, set to 0 if not
 * \returns Always returns 1.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->ignoreInvalidXML=(char) flag;
	return 1;
}


int irislwz_FlagSizeInfo(IRISLWZ_HANDLE *handle, int flag)
/*!\brief Set SI-Flag
 *
 * This function sets the SI-flag for IRISLWZ requests.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] flag Should be "1" to set the flag or "0" to unset it.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->flag_si=(char) flag;
	return 1;
}

int irislwz_FlagOtherInfo(IRISLWZ_HANDLE *handle, int flag)
/*!\brief Set OI-Flag
 *
 * This function sets the OI-flag for IRISLWZ requests.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] flag Should be "1" to set the flag or "0" to unset it.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->flag_oi=(char) flag;
	return 1;
}

int irislwz_TestSetVersion(IRISLWZ_HANDLE *handle, int version)
/*!\brief Override LWZ version
 *
 * With this function you can override the version bits in the LWZ header. It is meant for
 * testing purposes only.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] version Should be a value between 0 and 3.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->test_flags|=1;
	handle->test_version=version&3;
	return 1;
}

int irislwz_TestSetTransactionId(IRISLWZ_HANDLE *handle, int tid)
/*!\brief Override random transaction id
 *
 * With this function you can override the default random transaction id in the LWZ header.
 * It is meant for testing purposes only.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] tid A value between 0 and 65535
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->test_flags|=4;
	handle->test_tid=tid&0xffff;
	return 1;
}

int irislwz_TestSetHeader(IRISLWZ_HANDLE *handle, int header)
/*!\brief Override header byte in LWZ header
 *
 * With this function you can completely override the header-byte in the LWZ header.
 * It is meant for testing purposes only.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] header A value between 0 and 255
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->test_flags|=2;
	handle->test_header=header&0xff;
	return 1;
}

int irislwz_TestTruncatePacket(IRISLWZ_HANDLE *handle, int bytes)
/*!\brief Truncate the request packet
 *
 * With this function you can truncate the request packet at any position, to corrupt the
 * request.
 * It is meant for testing purposes only.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] bytes A value between 0 and 65535
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->test_flags|=16;
	handle->test_truncate=bytes;
	return 1;
}

int irislwz_TestSetAuthorityLength(IRISLWZ_HANDLE *handle, int bytes)
/*!\brief Override value for authority length
 *
 * With this function you can override the value for authority length in the LWZ header,
 * to corrupt the request.
 * It is meant for testing purposes only.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] bytes A value between 0 and 255
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->test_flags|=8;
	handle->test_authlen=bytes;
	return 1;
}

int irislwz_TestCorruptCompression(IRISLWZ_HANDLE *handle, int flag)
/*!\brief Corrupt compressed request packets
 *
 * When calling this function, a compressed request packet will be corrupted by
 * changing some values of the compressed stream with random values.
 * It is meant for testing purposes only.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] flag 1=enable or 0=disable
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	if (flag) handle->test_flags|=32;
	else handle->test_flags&=0xFFDF;
	return 1;
}

int irislwz_TestCleanup(IRISLWZ_HANDLE *handle)
/*!\brief Disable all special test options
 *
 * By calling this function all special test options get disabled.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	handle->test_flags=0;
	return 1;
}


int irislwz_Connect(IRISLWZ_HANDLE *handle)
/*!\brief Creates an UDP socket to an IRISLWZ server
 *
 * This function creates an UDP socket to an IRISLWZ server. As UDP is a connection less
 * protocol, there is no connection made to the server, but the hostname gets
 * resolved and the TCP stack checks, if the destination host is reachable.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \returns On success, this function returns 1, on error 0.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
#ifdef _WIN32
	SOCKET	sockfd;
#else
	int sockfd;
#endif
	if (!handle) return 0;
	if (((DCHK_SOCKET*)handle->socket)->sd>0) {
		irislwz_SetError(handle,74031,NULL);
		return 0;
	}

	struct timeval tv;
	tv.tv_sec=handle->timeout_seconds;
	tv.tv_usec=handle->timeout_useconds;

	// Resolve Hostname
	struct protoent *proto=getprotobyname("UDP");
	int n;
	struct addrinfo hints, *res, *ressave;
	bzero(&hints,sizeof(struct addrinfo));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_DGRAM;
	if (proto) {
		hints.ai_protocol=proto->p_proto;
	} else {
		hints.ai_protocol=17;	// UDP
	}
	if ((n=getaddrinfo(handle->server,NULL,&hints,&res))!=0) {
#ifdef _WIN32
		irislwz_SetError(handle,irislwz_TranslateSocketError(),NULL);
#else
		irislwz_SetError(handle,irislwz_TranslateGaiError(n),"%s",handle->server);
#endif
		return 0;
	}
	ressave=res;

	int e=0, conres=-1;

	do {
		sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if (sockfd<0) continue;		// Error, try next one
		((struct sockaddr_in*)res->ai_addr)->sin_port=htons(handle->port);
		conres=connect(sockfd,res->ai_addr,res->ai_addrlen);
		if (conres==0) break;
		e=irislwz_TranslateSocketError();
		#ifdef _WIN32
			shutdown(sockfd,2);
			closesocket(sockfd);
		#else
			shutdown(sockfd,2);
			close(sockfd);
		#endif
	} while ((res=res->ai_next)!=NULL);
	freeaddrinfo(ressave);
	if (conres<0) {
		irislwz_SetError(handle,74012,"%s, %i:%s",handle->server,e,irislwz_GetSocketError(e));
		return 0;
	}
	if (handle->timeout_seconds>0 || handle->timeout_useconds>0) {
		#ifdef WIN32
			if (setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv))!=0) {
				e=irislwz_TranslateSocketError();
				closesocket(sockfd);
				irislwz_SetError(handle,74033,"%i: %s", e, irislwz_GetSocketError(e));
		#else
			if (setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(void*)&tv,sizeof(tv))!=0) {
				e=irislwz_TranslateSocketError();
				irislwz_SetError(handle,74033,"%i: %s", e, irislwz_GetSocketError(e));
				close (sockfd);
		#endif

				return 0;
			}
	}
	((DCHK_SOCKET*)handle->socket)->sd=sockfd;
	return 1;
}

int irislwz_Disconnect(IRISLWZ_HANDLE *handle)
/*!\brief Destroys socket to IRISLWZ server
 *
 * This function destroys the socket to IRISLWZ server.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \returns On success, this function returns 1, on error 0.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	if (((DCHK_SOCKET*)handle->socket)->sd<=0) {
		irislwz_SetError(handle,74032,NULL);
		return 0;
	}
#ifdef _WIN32
	closesocket(((DCHK_SOCKET*)handle->socket)->sd);
#else
	// close (((DCHK_SOCKET*)handle->socket)->sd);
	shutdown(((DCHK_SOCKET*)handle->socket)->sd,SHUT_RDWR);
#endif
	((DCHK_SOCKET*)handle->socket)->sd=0;
	return 1;
}

void irislwz_FreePacket(IRISLWZ_PACKET *p)
/*!\brief Frees the memory used by an IRISLWZ_PACKET
 *
 * This function frees the memory allocated within an IRISLWZ_PACKET object.
 * The object itself is not freed.
 *
 * \param[in] p Pointer to an IRISLWZ_PACKET
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!p) return;
	if (p->packet) free(p->packet);
	if (p->uncompressed_payload!=NULL && p->uncompressed_payload!=p->payload) free(p->uncompressed_payload);
	if (p->beautified) free(p->beautified);
	bzero(p,sizeof(IRISLWZ_PACKET));
}


int irislwz_PrepareQuery(IRISLWZ_HANDLE *handle)
/*!\brief Prepare IRISLWZ Header for next query
 *
 * This function initializes the IRISLWZ header in the request structure
 * of the IRISLWZ_HANDLE. It uses the data and flags of the IRISLWZ_HANDLE object and
 * sets the values and bits in the IRISLWZ_HEADER.
 * The packet and header pointer must have been already
 * allocated with enough space to hold the 6 byte header and the authority name.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \returns On success, this function returns 1, on error 0.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	IRISLWZ_HEADER *header=handle->request.header;
	header->header=0;
	header->transactionId=htons(irislwz_rand(1,65534));
	header->max_response_length=htons(handle->maximumResponseLength);
	if (handle->authority) {
		header->authority_length=strlen(handle->authority);
		memcpy(handle->request.packet+6,handle->authority,header->authority_length);
	} else header->authority_length=0;
	if (handle->acceptDeflated) header->header|=8;
	if (handle->sendDeflated) header->header|=16;
	if (handle->flag_si) header->header|=2;
	if (handle->flag_oi) header->header|=3;
	return 1;
}

int irislwz_Talk(IRISLWZ_HANDLE *handle, IRISLWZ_PACKET *request, IRISLWZ_PACKET *response)
/*!\brief Talk with an IRISLWZ server
 *
 * This function is used for communication with an IRISLWZ-server. The server must be
 * connected previously with \ref irislwz_Connect.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] request pointer to an IRISLWZ_PACKET object, which holds the request
 * \param[out] response pointer to an IRISLWZ_PACKET object, in which the answer is stored
 *
 * \returns On success, this function returns 1, on error 0.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	int e, ret;
	if (!handle) return 0;
	if (((DCHK_SOCKET*)handle->socket)->sd<=0) {
		irislwz_SetError(handle,74032,NULL);
		return 0;
	}
	if (!request) {
		irislwz_SetError(handle,74025,"int irislwz_Talk(IRISLWZ_HANDLE *handle, ==> IRISLWZ_PACKET *request <==, IRISLWZ_PACKET *response)");
		return 0;
	}
	if (!response) {
		irislwz_SetError(handle,74025,"int irislwz_Talk(IRISLWZ_HANDLE *handle, IRISLWZ_PACKET *request, ==> IRISLWZ_PACKET *response <==)");
		return 0;
	}

	// Testing options
	if (handle->test_flags&8) request->header->authority_length=handle->test_authlen;
	if (handle->test_flags&1) {
		request->header->header&=0x3f;
		request->header->header|=(handle->test_version<<6);
	}
	if (handle->test_flags&2) {
		request->header->header=handle->test_header;
	}
	if (handle->test_flags&4) request->header->transactionId=htons(handle->test_tid);
	if ((handle->test_flags&16) && handle->test_truncate<request->size) request->size=handle->test_truncate;


	if (handle->verboseflag&1) VerboseOut(handle,"Request",request->uncompressed_payload);
	if (handle->verboseflag&2) VerboseOut(handle,"Request beautified",request->beautified);

	if (handle->verboseflag&4) {
		char *buffer=ExplainHeader(&handle->request);
		if (buffer) {
			VerboseOut(handle,"Request Header",buffer);
			free(buffer);
		}
	}

	ssize_t bytes=send(((DCHK_SOCKET*)handle->socket)->sd, request->packet, request->size,0);
	if (bytes<0) {
		e=irislwz_TranslateSocketError();
		//irislwz_SetError(handle,74009,"%i: %s",e,irislwz_GetSocketError(e));
		irislwz_SetError(handle,e,"%s:%i",handle->server,handle->port);
		return 0;
	}
	size_t maxsize=handle->maximumResponseLength+8192;
	void *buffer=malloc(maxsize);
	if (!buffer) {
		irislwz_SetError(handle,74024,NULL);
		return 0;
	}
	bytes=recv(((DCHK_SOCKET*)handle->socket)->sd,(char*)buffer,maxsize,0);
	if (bytes<=0) {
		e=irislwz_TranslateSocketError();
		if (e==1006) e=1031;
		irislwz_SetError(handle,e,"%s:%i, Bytes: %i",handle->server,handle->port,bytes);
		free(buffer);
		return 0;
	}
	if (bytes<3) {
		irislwz_SetError(handle,74036,"%i Bytes",bytes);
		free(buffer);
		return 0;
	}
	response->size=bytes;
	response->packet=(char*)malloc(bytes+1);
	if (!response->packet) {
		irislwz_SetError(handle,74024,NULL);
		free(buffer);
		return 0;
	}
	memcpy(response->packet,buffer,bytes);
	free(buffer);
	response->header=(IRISLWZ_HEADER*)response->packet;
	response->payload=response->packet+3;
	response->size_payload=bytes-3;
	response->payload[response->size_payload]=(unsigned char)0;
	if (request->header->transactionId!=response->header->transactionId && response->header->transactionId!=0xffff) {
		// TransactionId does not fit
		irislwz_SetError(handle,74103,NULL);
		return 0;
	}
	// is response compressed?
	if ((response->header->header)&16) {
		// DCHK answers compress really good, so we have to allocate a big
		// buffer for decompression. We use 10 times the compressed size plus a bit extra
		uLongf dstlen=(bytes-3)*10+1024;
		//printf ("Bytes= %i, dstlen=%i\n",bytes,dstlen);
		int retrycounter=0;
		while (1) {
			response->uncompressed_payload=(char*)malloc(dstlen);
			uLongf srclen=bytes-3;
			if (handle->rfc1950_workaround) ret=uncompress((Bytef*)response->uncompressed_payload,&dstlen,
					(const Bytef*) response->payload,srclen);
			else ret=rfc1951_inflate((Bytef*)response->uncompressed_payload,&dstlen,
					(const Bytef*) response->payload,srclen);
			if (ret!=Z_OK) {
				if (ret==Z_BUF_ERROR && retrycounter<5) {
					// We increase the buffer for uncompressed data
					free(response->uncompressed_payload);
					dstlen+=dstlen;
					//printf ("Increasing decompression-Buffer to %i Bytes\n",dstlen);
					response->uncompressed_payload=(char*)malloc(dstlen);
					retrycounter++;
					continue;
				}
				if (handle->verboseflag&32) {
					char *buffer=ExplainHeader(&handle->response);
					if (buffer) {
						VerboseOut(handle,"Response Header",buffer);
						free(buffer);
						//irislwz_HexDump(response->payload,srclen);
					}
				}
				irislwz_SetError(handle,74037,NULL);
				return 0;
			} else {
				break;
			}
		}
		response->size_uncompressed=dstlen;
	} else {
		response->uncompressed_payload=response->payload;
		response->size_uncompressed=response->size_payload;
	}
	if (handle->verboseflag&32) {
		char *buffer=ExplainHeader(&handle->response);
		if (buffer) {
			VerboseOut(handle,"Response Header",buffer);
			free(buffer);
		}
	}

	if ((handle->verboseflag&8) && (response->uncompressed_payload)) VerboseOut(handle,"Response",response->uncompressed_payload);

	response->uncompressed_payload[response->size_uncompressed]=0;
	if (irislwz_Validate(handle,response->uncompressed_payload,&response->beautified)) {
		if (handle->verboseflag&16) VerboseOut(handle,"Response beautified",response->beautified);
		return 1;
	}
	if (handle->error==74044) handle->error=74046;
	return 0;
}

int irislwz_QueryVersion(IRISLWZ_HANDLE *handle)
/*!\brief Send a version request to the server
 *
 * This function sends a version request to the server
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \returns On success, this function returns 1, on error 0.
 *
 * To query the version, the Version-Bit is set in the IRISLWZ_HEADER. The server itself
 * answers with a version information in the payload, which looks like this:
 * \code
<?xml version="1.0" encoding="UTF-8"?>
<versions xmlns="urn:ietf:params:xml:ns:iris-transport">
  <transferProtocol protocolId="iris.lwz">
    <application protocolId="urn:ietf:params:xml:ns:iris1">
      <dataModel protocolId="urn:ietf:params:xml:ns:dchk1"/>
    </application>
  </transferProtocol>
</versions>

 * \endcode
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	irislwz_FreePacket(&handle->request);
	irislwz_FreePacket(&handle->response);

	handle->request.size=6;
	if (handle->authority) handle->request.size+=strlen(handle->authority);
	handle->request.packet=(char*)malloc(handle->request.size);
	if (!handle->request.packet) {
		irislwz_SetError(handle,10024,NULL);
		return 0;
	}
	handle->request.header=(IRISLWZ_HEADER*)handle->request.packet;
	irislwz_PrepareQuery(handle);
	if (handle->authority) handle->request.authority=handle->request.packet+6;
	handle->request.header->header|=1;			// Version request

	return irislwz_Talk(handle,&handle->request, &handle->response);
}

int irislwz_GetCompressedSize(const char *buffer, int size, int rfc1950)
/*!\brief Calculate the zlib compressed size of a buffer
 *
 * This function compresses the given \p buffer with zlib and returns the required
 * amount of Bytes. It is used internally by some functions, but can be used externally, too.
 *
 * \param[in] buffer Pointer to the beginning of data, which should be compressed
 * \param[in] size Amount of bytes in the \p buffer
 * \param[in] rfc1950 If set to "1", this function will use RFC 1950 instead of RFC 1951 for
 * compression.
 * \returns On success, this function returns the amount of bytes needed to compress
 * the data, otherwise it will return 0.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	int suberr;
	if (!buffer) return 0;
	if (!size) return 0;
	uLongf dstlen=size+64;
	void *dst=malloc(dstlen);
	if (!dst) return 0;
	if (rfc1950==1) suberr=compress2((Bytef*)dst,(uLongf *)&dstlen,(const Bytef*)buffer,size,Z_BEST_COMPRESSION);
	else suberr=rfc1951_deflate((Bytef*)dst,(uLongf *)&dstlen,(const Bytef*)buffer,size,Z_BEST_COMPRESSION);
	if (suberr!=Z_OK) {
		return 0;
	}
	free(dst);
	return (int)dstlen;
}

int irislwz_Query(IRISLWZ_HANDLE *handle, const char *payload)
/*!\brief Send a request to an IRISLWZ server
 *
 * This function is used to send a request to an IRISLWZ server and wait for response.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] payload Request, which should be send to IRIS server
 * \returns On success, this function returns 1 and the response is saved in the response structure of
 * \p handle. The request structure is filled, too. In case of an error, 0 will be returned.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	if (!handle) return 0;
	if (!payload) {
		irislwz_SetError(handle,74025,"int irislwz_Query(IRISLWZ_HANDLE *handle, ==> char *payload <==)");
		return 0;
	}
	return irislwz_Query2(handle,payload,strlen(payload));
}

int irislwz_Query2(IRISLWZ_HANDLE *handle, const char *payload, int bytes)
/*!\brief Send a request to an IRISLWZ server
 *
 * This function is used to send a request to an IRISLWZ server and wait for response.
 * In addition to irislwz_Query, this function needs a third parameter indicating the
 * size of the payload.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] payload Request, which should be send to IRIS server
 * \param[in] bytes Size of the payload
 * \returns On success, this function returns 1 and the response is saved in the response structure of
 * \p handle. The request structure is filled, too. In case of an error, 0 will be returned.
 *
 * \since
 * Version 0.5.4
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	int c,p,suberr;
	if (!handle) return 0;
	if (!payload) {
		irislwz_SetError(handle,74025,"int irislwz_Query2(IRISLWZ_HANDLE *handle, ==> char *payload <==, int bytes)");
		return 0;
	}
	if (bytes<1) {
		irislwz_SetError(handle,74025,"int irislwz_Query2(IRISLWZ_HANDLE *handle, char *payload, ==> int bytes <==)");
		return 0;
	}
	irislwz_FreePacket(&handle->request);
	irislwz_FreePacket(&handle->response);

	// We copy the uncompressed payload into the request structure
	handle->request.uncompressed_payload=(char*)malloc(bytes+1);
	if (!handle->request.uncompressed_payload) {
		irislwz_SetError(handle,74024,NULL);
		return 0;
	}
	memcpy(handle->request.uncompressed_payload,payload,bytes);
	handle->request.uncompressed_payload[bytes]=0;
	handle->request.size_uncompressed=bytes;
	// beautify it
	if (handle->dontValidateInput==0) {
		if (!irislwz_Validate(handle,handle->request.uncompressed_payload,&handle->request.beautified)) {
			if (handle->error==74044) handle->error=74045;
			if (!handle->ignoreInvalidXML) return 0;
		}
	}
	// Do we want to deflate the request packet?
	size_t size;
	if (handle->sendDeflated) {
		size=irislwz_GetCompressedSize(payload,handle->request.size_uncompressed, handle->rfc1950_workaround);
		if (!size) {
			irislwz_SetError(handle,74039,NULL);
			return 0;
		}
	} else {
		size=handle->request.size_uncompressed;
	}
	handle->request.size_payload=size;
	handle->request.size=6;
	if (handle->authority) handle->request.size+=strlen(handle->authority);
	handle->request.size+=size;
	handle->request.packet=(char*)malloc(handle->request.size+16);
	if (!handle->request.packet) {
		irislwz_SetError(handle,74024,NULL);
		return 0;
	}
	handle->request.header=(IRISLWZ_HEADER*)handle->request.packet;
	irislwz_PrepareQuery(handle);
	handle->request.payload=handle->request.packet+(handle->request.size-size);
	if (handle->authority) handle->request.authority=handle->request.packet+6;

	// Send deflated?
	if (handle->sendDeflated) {
		uLongf dstlen=size+16;
		// Compress with zlib
		if (handle->rfc1950_workaround) suberr=compress2((Bytef*)handle->request.payload,(uLongf *)&dstlen,(const Bytef*)payload,strlen(payload),Z_BEST_COMPRESSION);
		else suberr=rfc1951_deflate((Bytef*)handle->request.payload,(uLongf *)&dstlen,(const Bytef*)payload,strlen(payload),Z_BEST_COMPRESSION);
		if (suberr!=Z_OK) {
			irislwz_SetError(handle,74039,NULL);
			return 0;
		}
		// Corrupt compression?
		if (handle->test_flags&32) {
			for (c=0;c<20;c++) {
				p=irislwz_rand(0,dstlen);
				handle->request.payload[p]=irislwz_rand(0,255);
			}
		}

	} else {
		memcpy(handle->request.payload,payload,size);
	}
	return irislwz_Talk(handle,&handle->request, &handle->response);
}

int irislwz_CheckResponse(IRISLWZ_HANDLE *handle)
/*!\brief Check response if it contains size or other information
 *
 * This function can be used to check if a response packet contains size information or other
 * information.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 *
 * \returns
 * The function returns 1, if there is no flag indicating size or other information. Otherwise it returns 0
 * and an apropriate errorcode is set. It can also return 0, if the \p handle parameter is NULL.
 *
 * \since
 * Version 0.5.4
 *
 * \ingroup DCHK_API_IRISLWZ
 *
 */
{
	if (!handle) return 0;
	// Size Information
	if ((handle->response.header->header&3)==2) {
		int octets=dchk_XML_ParseSize(handle);
		if (octets) irislwz_SetError(handle,74040,"required: %i bytes, maximum set: %i bytes",octets,
				handle->maximumResponseLength);
		return 0;
	}
	// Other Information, usually error messages from the server
	if ((handle->response.header->header&3)==3) {
		if (dchk_XML_ParseOther(handle)) return 0;
		irislwz_SetError(handle,74047,NULL);
		return 0;
	}
	return 1;
}
