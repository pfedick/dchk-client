/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.2 $
 * $Date: 2009/03/12 11:56:19 $
 * $Id: resolver.c,v 1.2 2009/03/12 11:56:19 pfedick Exp $
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
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x501
	#endif

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Windns.h>

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


static char *dchk_ParseNAPTRRecord(IRISLWZ_HANDLE *handle, u_char *data)
/*!\brief This function parses a raw NAPTR record
 *
 * This function parses a raw NAPTR record, described in RFC 2915.
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object. In this function it is only
 *            used to store the error code, if an error occurs.
 * \param[in] data Pointer to the raw NAPTR record
 * \returns On success, the function returns a pointer to the replacement data of the NAPTR
 * record, which can be used to make a SRV query to the DNS server. After usage the client
 * needs to free the allocated memory by calling "free". In case of an error, it returns NULL
 * and an errorcode ist set in the \ref IRISLWZ_HANDLE structure.
 *
 * A typical NAPTR record for DCHK looks like this:
\code
de.
;;        order  pref  flags service               regexp replacement
IN NAPTR  100    10    "s"   "DCHK1:iris.lwz"      ""     _iris-lwz._udp.de.
\endcode
 */
{
	const u_char *cp=data+4;			// Skip order an pref
	cp+=cp[0]+1;    					// skip flags
	if (cp[0]!=14 || strncmp((const char*)cp+1,(const char*)"DCHK1:iris.lwz",14)!=0) {
		// Service name has not the length of "DCHK1:iris.lwz" (14 Byte), or has not the
		// name "DCHK1:iris.lwz"
		irislwz_SetError(handle,74057,NULL);
		return NULL;
	}
	// Ok, here we have a valid DCHK NAPTR record
	cp+=cp[0]+1;    // jump over service
	cp+=cp[0]+1;    // jump over regexp
	// We need to allocate a buffer for the replacement name
	const u_char *save=cp;
	// Lets calculate the required amount of bytes
	int count=0;
	while (cp[0]) {
		if (count) count++;		// We need to add a dot
		count+=cp[0];			// Add the size of this name
		cp+=cp[0]+1;			// next lable
	}
	count++;					// Add a final zero byte
	cp=save;
	// now lets build the complete name
	char *buffer=(char*)malloc(count);
	if (!buffer) {
		irislwz_SetError(handle,74024,NULL);
		return NULL;
	}
	buffer[0]=0;
	while (cp[0]) {
		if (buffer[0]!=0) strcat(buffer,".");
		strncat(buffer,(const char*)cp+1,cp[0]);
		cp+=cp[0]+1;    // next lable
	}
	return buffer;
}

static char *dchk_GetNAPTR(IRISLWZ_HANDLE *handle, const char *tld)
/*!\brief Finds NAPTR record for DCHK1
 */
{
#ifdef _WIN32
	PDNS_RECORD r;
	PIP4_ARRAY pSrvList = NULL;
	char *query=(char*)malloc(2+strlen(tld));
	char *buffer=NULL;
	if (query==NULL) {
		irislwz_SetError(handle,74024,NULL);
		return NULL;
	}
	sprintf(query,"%s.",tld);
	DNS_STATUS ret=DnsQuery("de.",DNS_TYPE_NAPTR,DNS_QUERY_BYPASS_CACHE,pSrvList,&r,NULL);
	if (ret) {
		//printf ("NAPTR lookup failed: ret=%i\n",ret);
		irislwz_SetError(handle,74004,"%s",tld);
		free(query);
		return NULL;
	}
	PDNS_RECORD c=r;

	while (c) {
		if (strcmp(c->pName,tld)==0 && c->wType==DNS_TYPE_NAPTR) {	// NAPTR record?
			buffer=dchk_ParseNAPTRRecord(handle,(u_char*)&c->Data);
			if (buffer) {
				// We have found our NAPTR record for DCHK
				free(query);
				DnsRecordListFree(r, DnsFreeRecordList);
				return buffer;
			}
		}
		c=c->pNext;
	}
	// We have not found a NAPTR record
	free(query);
	DnsRecordListFree(r, DnsFreeRecordList);
	irislwz_SetError(handle,74005,"%s",tld);
	return NULL;
#else
	int res,i;
	// First we search for NAPTR-Records for the given TLD
	unsigned char *answer=(unsigned char *)malloc(PACKETSZ);
	char *buffer=NULL;
	if (!answer) {
		irislwz_SetError(handle,74024,NULL);
		return NULL;
	}
	res=res_query(tld,C_IN,ns_t_naptr,answer, PACKETSZ);                //
	if (res<0) {
		if (h_errno==1) irislwz_SetError(handle,74002,"%s",tld);
		else if (h_errno==4) irislwz_SetError(handle,74004,"%s",tld);
		else irislwz_SetError(handle,74001,"%i: %s",h_errno,hstrerror(h_errno));
		free(answer);
		return NULL;
	}
	//irislwz_HexDump(answer,res);
	dchk_ns_msg msg;
	res=dchk_bind_ns_initparse(answer,res,&msg);
	if (res<0) {
		irislwz_SetError(handle,74003,"%s",hstrerror(h_errno));
		free(answer);
		return NULL;
	}
	int nr=dchk_bind_ns_msg_count(msg,ns_s_an);
	if (!nr) {
		irislwz_SetError(handle,74004,"%s",tld);
		free(answer);
		return NULL;
	}
	// parsing NAPTR records
	ns_rr rr;
	for (i=0;i<nr;i++) {
		res=dchk_bind_ns_parserr(&msg,ns_s_an,i,&rr);
		if (res==0) {
			// we search for a NAPTR record for service "DCHK1:iris.lwz"
			if (ns_rr_type(rr)==ns_t_naptr) {
				buffer=dchk_ParseNAPTRRecord(handle,(u_char*)ns_rr_rdata(rr));
				if (buffer) {
					// We have found our NAPTR record for DCHK
					free(answer);
					return buffer;
				}
			}
		}
	}
	irislwz_SetError(handle,74005,"%s",tld);
	free(answer);
	return NULL;
#endif
}



int dchk_SetHostByDNS(IRISLWZ_HANDLE *handle, const char *tld)
/*!\brief Set hostname of DCHK server by using DNS
 *
 * If you don't know the hostname of the DCHK server you can use this function to
 * make a DNS lookup for the specified TLD. The function will query the nameserver for
 * the given TLD and check if there is a NAPTR- and a SRV record for "DCHK1:iris.lwz".
 *
 * \param[in] handle pointer to an IRISLWZ_HANDLE object
 * \param[in] tld pointer to a string with the desired TLD.
 * \returns If the function can find a suitable DCHK server, the server is automatically
 * configured by calling \ref irislwz_SetHost and the function returns 1 (success). You can use
 * the functions \ref irislwz_GetHostname and \ref irislwz_GetPort to get the results. If no DCHK
 * server can be found or if an error occured, the function will return 0.
 *
 * \remarks
 * This function will only succeed if the registry for the specified TLD has a NAPTR record
 * for "DCHK1:iris.lwz" and a SRV record in it's zonefile.
 *
 * \ingroup DCHK_API_DCHK
 */
{
	if (!handle) return 0;
	if (!tld) {
		irislwz_SetError(handle,74021,NULL);
		return 0;
	}
	char *name=dchk_GetNAPTR(handle,tld);
	if (!name) {
		return 0;
	}

#ifdef _WIN32
	int a;
	PDNS_RECORD r;
	DNS_STATUS ret=DnsQuery((const char*)name,DNS_TYPE_SRV,DNS_QUERY_STANDARD,NULL,&r,NULL);
	if (ret) {
		free(name);
		irislwz_SetError(handle,74007,"%s",name);
		return 0;
	}
	a=irislwz_SetHost(handle,(const char*)r->Data.SRV.pNameTarget,r->Data.SRV.wPort);
	//printf ("server: %s:%i\n",r->Data.SRV.pNameTarget,r->Data.SRV.wPort);
	DnsRecordListFree(r, DnsFreeRecordList);
	return a;
#else
    int res=res_init();
    if (res<0) {
    	free(name);
    	irislwz_SetError(handle,74000,"%s",hstrerror(h_errno));
    	return 0;
    }
    // we search for SRV-Records for the given name
    unsigned char *answer=(unsigned char *)malloc(PACKETSZ);
    res=res_query(name,C_IN,ns_t_srv,answer, PACKETSZ);		//
    if (res<0) {
    	if (h_errno==1) irislwz_SetError(handle,74006,"%s",name);
    	else if (h_errno==4) irislwz_SetError(handle,74007,"%s",name);
    	else irislwz_SetError(handle,74007,"%i: %s",h_errno,hstrerror(h_errno));
    	free(name);
    	free(answer);
    	return 0;
    }

    dchk_ns_msg msg;
    res=dchk_bind_ns_initparse(answer,res,&msg);
    if (res<0) {
    	irislwz_SetError(handle,74003,"%s",hstrerror(h_errno));
    	free(answer);
    	free(name);
    	return 0;
    }
    int nr=dchk_bind_ns_msg_count(msg,ns_s_an);
    if (!nr) {
    	irislwz_SetError(handle,74008,"%s",name);
    	free(answer);
    	free(name);
    	return 0;
    }

    // parsing SRV records
    ns_rr rr;
    const u_char *cp;
    int i;
    int port;
    char *host=NULL;
    int hostbuffer=0;
    for (i=0;i<nr;i++) {
    	res=dchk_bind_ns_parserr(&msg,ns_s_an,i,&rr);
    	if (res==0) {
    		// we search for a SRV record
    		if (ns_rr_type(rr)==ns_t_srv) {
    			cp=ns_rr_rdata(rr);
    			//ppl6::HexDump((void*)cp,ns_rr_rdlen(rr));
    			cp+=4;	// Skip prev and weight
    			port=ns_get16(cp);		// port
    			cp+=2;	// jump over port
   	    		// build hostname
    			if (host) {
    				free(host);
    				host=NULL;
    				hostbuffer=0;
    			}
    	    	while (cp[0]) {
    	    		if (hostbuffer<cp[0]+1) {
    	    			hostbuffer=cp[0]+256;
    	    			if(host) host=(char*)realloc(host,strlen(host)+hostbuffer);
    	    			else {
    	    				host=(char*)malloc(hostbuffer);
    	    				host[0]=0;
    	    			}
    	    		}
    	    		if(strlen(host)) {
    	    			strcat(host,".");
    	    			hostbuffer--;
    	    		}
    	    		strncat(host,(const char*)cp+1,cp[0]);
    	    		hostbuffer-=cp[0];
    	    		cp+=cp[0]+1;	// next lable
    	    	}
    	    	free(answer);
    	    	free(name);
    	    	i=irislwz_SetHost(handle,(const char *)host,port);
    	    	free(host);
    	    	return i;
    		}
    	}
    }
    irislwz_SetError(handle,74008,"%s",name);
    free(answer);
    return 0;
#endif
}

