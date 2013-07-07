/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.7 $
 * $Date: 2009/07/14 10:16:08 $
 * $Id: CIrisLWZ.cpp,v 1.7 2009/07/14 10:16:08 pfedick Exp $
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
#include <windows.h>
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



namespace dchk {


/*!\class CIrisLWZ
 * \brief Implementation of Lightweight UDP Transfer Protocol for the Internet Registry Information Service
 *
 * This class implements a Lightweight UDP Transfer Protocol for the Internet Registry Information Service.
 * (see RFC 4993: http://tools.ietf.org/html/rfc4993)
 *
 * \ingroup DCHK_CPP_API
 *
 */

/*!\var CIrisLWZ::handle
 * \brief Basic data structure for underlying C-API
 *
 * Basic data type for all DCHK C-API functions. It is automatically
 * initialized by the constructor of this class an deinitialized by it's destructor.
 * You can get a pointer on this structure by calling CIrisLWZ::GetHandle()
 *
 * \see \ref IRISLWZ_HANDLE
 */

CIrisLWZ::CIrisLWZ()
/*!\brief Constructor of the class
 *
 * The constructor initializes the internal structures and sets some defaults.
 */
{
	handle.initialized=0;
	irislwz_Init(&handle);
}

CIrisLWZ::~CIrisLWZ()
/*!\brief Destructor of the class
 *
 * The destructor frees all memory used by this class.
 */
{
	irislwz_Exit(&handle);
	handle.initialized=0;
}

int CIrisLWZ::SetTimeout(int seconds, int useconds)
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
 * \param[in] seconds Number of seconds. If you pass 0 seconds, the library uses no
 * timeout. If no response arrives, the library blocks until the operating system issues a
 * timeout, which could be several minutes.
 * \param[in] useconds Number of microseconds
 * \returns Always returns 1
 */
{
	int ret=irislwz_SetTimeout(&handle,seconds,useconds);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::SetAuthority(const char *authority)
/*!\brief Sets the authority for IRISLWZ requests
 *
 * With this function you can define the authority which is used in IRISLWZ request packets.
 * The default is not to use an authority.
 *
 * \param[in] authority Pointer to an authority string. The string must not be longer than 255 octets.
 * \returns Returns 1 on success or 0 when an error occurred.
 *
 */
{
	int ret=irislwz_SetAuthority(&handle,authority);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::SetAuthorityByDomain(const char *domain)
/*!\brief Sets authority for IRISLWZ requests by using the TLD of a domain
 *
 * This function uses the TLD of the given \p domain to set the authority for
 * further requests.
 *
 * \param[in] domain pointer to a domain name
 * \returns Returns 1 on success or 0 when an error occurred.
 *
 * \note
 * If the TLD is set to "arpa", a special handling takes place. At the moment the
 * library only checks if the domain ends with "9.4.e164.arpa" (german ENUM domains). If this is
 * the case "9.4.e164.arpa" is used as authority name. Other arpa zones are not implemented.
 *
 */
{
	int ret=irislwz_SetAuthorityByDomain(&handle,domain);
	if (!ret) CopyError(&handle);
	return ret;
}

int	CIrisLWZ::SetAuthorityByDomainList(CDomainList &list)
/*!\brief Sets authority for IRISLWZ requests by using a domain list
 *
 * This function uses the first domain in the domain \p list to set an authority name for
 * further requests.
 *
 * \param[in] list Reference to a CDomainList object
 * \returns Returns 1 on success or 0 when an error occurred.
 *
 * \remarks
 * The function actually only picks the first domain name from the list and calls the function
 * irislwz_SetAuthorityByDomain.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	DCHK_RECORD *r=list.GetFirst();
	if (r==NULL || r->domain==NULL) {
		SetError(74042,"");
		return 0;
	}
	return SetAuthorityByDomain((const char*)r->domain);
}


int CIrisLWZ::SetMaximumResponseLength(int bytes)
/*!\brief Sets the maximum acceptable packet size for answer packets
 *
 * By default the client accepts a maximum packet size of 1500 octets (Bytes), which is the
 * recommend size by the <a href="http://tools.ietf.org/html/rfc4993">Lightweight UDP Transfer Protocol RFC</a>,
 * when the path MTU is unknown.
 * With this function you can set any other size.
 *
 * \param[in] bytes number of bytes
 * \returns Always returns 1 (success).
 *
 * \remarks
 * Please keep in mind, that even if you specify a bigger packet size, the DCHK server will
 * not exceed 4000 octets. If the answer does not fit into the answer packet, the server will send
 * a "size information".
 */
{
	int ret=irislwz_SetMaximumResponseLength(&handle,bytes);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::SetHost(const char *host, int port)
/*!\brief Set hostname an port of an IRISLWZ server
 *
 * With this function you can specify hostname or IP address and port of IRISLWZ server
 * manually. If you don't know the hostname, you can use the function \ref CDchk::SetHostByDNS
 * and let the DCHK library find out the server name by using DNS.
 *
 * \param[in] host name or IP address of the server which should be used
 * \param[in] port port of the server (default=715)
 * \returns Returns 1 on success or 0 when an error occurred.
 */
{
	int ret=irislwz_SetHost(&handle,host,port);
	if (!ret) CopyError(&handle);
	return ret;
}

const char *CIrisLWZ::GetHostname()
/*!\brief Returns pointer to the hostname of a previous set server
 *
 * This function returns a pointer to the previous set hostname. This could
 * have been done by explicit calling the CIrisLWZ::SetHost function or automatically by
 * calling CDchk::SetHostByDNS.
 *
 * \returns Returns a pointer to the hostname or NULL, if the hostname was not set yet.
 */
{
	const char *ret=irislwz_GetHostname(&handle);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::GetPort()
/*!\brief Returns port of a previous set server
 *
 * This function returns the port of a previous set IRISLWZ server. This could
 * have been done by explicit calling the CIrisLWZ::SetHost function or automatically by
 * calling CDchk::SetHostByDNS.
 *
 * \returns Returns port of server or 0 if no server was set, yet.
 */
{
	int ret=irislwz_GetPort(&handle);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::IgnoreInvalidXML(int flag)
/*!\brief Tell the library to ignore invalid XML
 *
 * Normally the client would stop and raise a validation error if the request payload
 * contains invalid XML. With this function you can disable this check, so it is
 * possible to test how the server reacts to an invalid XML request.
 *
 * \param[in] flag set to 1, if we want to ignore invalid XML, set to 0 if not
 * \returns Always returns 1.
 */
{
	int ret=irislwz_IgnoreInvalidXML(&handle,flag);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::AcceptDeflated(int flag)
/*!\brief Accept compressed answers
 *
 * Tell the client if it should accept zlib compressed answers or not
 *
 * \param[in] flag set to 1, if we accept compressed answers, set to 0 if not
 * \returns Always returns 1.
 */
{
	int ret=irislwz_AcceptDeflated(&handle,flag);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::SendDeflated(int flag)
/*!\brief Send compressed requests
 *
 * Tell the client if it should send zlib compressed requests or not
 *
 * \param[in] flag set to 1, if we want to send compressed requests, set to 0 if not
 * \returns Always returns 1.
 */
{
	int ret=irislwz_SendDeflated(&handle,flag);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::UseRFC1950(int flag)
/*!\brief Use RFC 1950 for compression
 *
 * The IRIS LWZ RFC (<a href="http://tools.ietf.org/html/rfc4993">RFC 4993</a>) describes to use
 * the deflate algorithm described in <a href="http://tools.ietf.org/html/rfc4993">RFC 1951</a>
 * for compression. But the first version of DENIC's DCHK server, client and also this client
 * wrongly used <a href="http://tools.ietf.org/html/rfc4993">RFC 1950</a>, which is a zlib
 * wrapper around <a href="http://tools.ietf.org/html/rfc4993">RFC 1951</a>. Until the bug is fixed
 * in the server, you can use this option to keep the wrong compression.
 *
 * \param[in] flag Use "0" to use <a href="http://tools.ietf.org/html/rfc4993">RFC 1950</a> as it was
 * meant to, or "1" to use the wrong compression.
 * \returns Always returns 1 except the pointer to the IRISLWZ_HANDLE object \p handle was NULL.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
	int ret=irislwz_UseRFC1950(&handle,flag);
	if (!ret) CopyError(&handle);
	return ret;
}


int CIrisLWZ::FlagSizeInfo(int flag)
/*!\brief Set SI-Flag
 *
 * This function sets the SI-flag for IRISLWZ requests.
 *
 * \param[in] flag Should be "1" to set the flag or "0" to unset it.
 */
{
	int ret=irislwz_FlagSizeInfo(&handle,flag);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::FlagOtherInfo(int flag)
/*!\brief Set OI-Flag
 *
 * This function sets the OI-flag for IRISLWZ requests.
 *
 * \param[in] flag Should be "1" to set the flag or "0" to unset it.
 */
{
	int ret=irislwz_FlagOtherInfo(&handle,flag);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::TestSetVersion(int version)
/*!\brief Override LWZ version
 *
 * With this function you can override the version bits in the LWZ header. It is meant for
 * testing purposes only.
 *
 * \param[in] version Should be a value between 0 and 3.
 */
{
	int ret=irislwz_TestSetVersion(&handle,version);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::TestSetTransactionId(int tid)
/*!\brief Override random transaction id
 *
 * With this function you can override the default random transaction id in the LWZ header.
 * It is meant for testing purposes only.
 *
 * \param[in] tid A value between 0 and 65535
 */
{
	int ret=irislwz_TestSetTransactionId(&handle,tid);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::TestSetHeader(int header)
/*!\brief Override header byte in LWZ header
 *
 * With this function you can completely override the header-byte in the LWZ header.
 * It is meant for testing purposes only.
 *
 * \param[in] header A value between 0 and 255
 */
{
	int ret=irislwz_TestSetHeader(&handle,header);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::TestTruncatePacket(int bytes)
/*!\brief Truncate the request packet
 *
 * With this function you can truncate the request packet at any position, to corrupt the
 * request.
 * It is meant for testing purposes only.
 *
 * \param[in] bytes A value between 0 and 65535
 */
{
	int ret=irislwz_TestTruncatePacket(&handle,bytes);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::TestSetAuthorityLength(int bytes)
/*!\brief Override value for authority length
 *
 * With this function you can override the value for authority length in the LWZ header,
 * to corrupt the request.
 * It is meant for testing purposes only.
 *
 * \param[in] bytes A value between 0 and 255
 */
{
	int ret=irislwz_TestSetAuthorityLength(&handle,bytes);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::TestCorruptCompression(int flag)
/*!\brief Corrupt compressed request packets
 *
 * When calling this function, a compressed request packet will be corrupted by
 * changing some values of the compressed stream with random values.
 * It is meant for testing purposes only.
 *
 * \param[in] flag 1=enable or 0=disable
 */
{
	int ret=irislwz_TestCorruptCompression(&handle,flag);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::TestCleanup()
/*!\brief Disable all special test options
 *
 * By calling this function all special test options get disabled.
 */
{
	int ret=irislwz_TestCleanup(&handle);
	if (!ret) CopyError(&handle);
	return ret;
}


int CIrisLWZ::Connect()
/*!\brief Creates an UDP socket to an IRISLWZ server
 *
 * This function creates an UDP socket to an IRISLWZ server. As UDP is a connection less
 * protocol, there is no connection made to the server, but the hostname gets
 * resolved and the TCP stack checks, if the destination host is reachable.
 *
 * \returns On success, this function returns 1, on error 0.
 */
{
	int ret=irislwz_Connect(&handle);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::Disconnect()
/*!\brief Destroys socket to IRISLWZ server
 *
 * This function destroys the socket to IRISLWZ server.
 *
 * \returns On success, this function returns 1, on error 0.
 */
{
	int ret=irislwz_Disconnect(&handle);
	if (!ret) CopyError(&handle);
	return ret;
}

void CIrisLWZ::SetDebugLevel(int level)
/*!\brief Sets debug level
 *
 * This functions sets the debug level for debug output. By default the library does not
 * output any debug (level=0). The higher the level, the more output you will get.
 * You can specify a debug handler if you want the output anywhere else than STDOUT
 * (see \ref irislwz_SetDebugHandler).
 *
 * \param[in] level debug level. 0=no debug
 */
{
	irislwz_SetDebugLevel(&handle,level);
}

void CIrisLWZ::SetDebugHandler(void (*DebugHandler)(int, const char *, void *), void *privatedata)
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
 * \param[in] DebugHandler Pointer to the function that will receive the debug output
 * \param[in] privatedata Pointer to some optional data, which is passed to the
 * callback function
 * \see
 * \ref irislwz_SetDebugLevel
 */
{
	irislwz_SetDebugHandler(&handle,DebugHandler,privatedata);
}


int CIrisLWZ::PrepareQuery()
/*!\brief Prepare IRISLWZ Header for next query
 *
 * This function initializes the IRISLWZ header in the request structure
 * of the IRISLWZ_HANDLE. It uses the data and flags of the IRISLWZ_HANDLE object and
 * sets the values and bits in the IRISLWZ_HEADER.
 * The packet and header pointer must have been already
 * allocated with enough space to hold the 6 byte header and the authority name.
 *
 * \returns On success, this function returns 1, on error 0.
 */
{
	int ret=irislwz_PrepareQuery(&handle);
	if (!ret) CopyError(&handle);
	return ret;
}


int CIrisLWZ::Talk(IRISLWZ_PACKET *request, IRISLWZ_PACKET *response)
/*!\brief Talk with an IRISLWZ server
 *
 * This function is used for communication with an IRISLWZ-server. The server must be
 * connected previously with \ref irislwz_Connect.
 *
 * \param[in] request pointer to an IRISLWZ_PACKET object, which holds the request
 * \param[out] response pointer to an IRISLWZ_PACKET object, in which the answer is stored
 *
 * \returns On success, this function returns 1, on error 0.
 */
{
	int ret=irislwz_Talk(&handle, request, response);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::QueryVersion()
/*!\brief Send a version request to the server
 *
 * This function sends a version request to the server
 *
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
 */
{
	int ret=irislwz_QueryVersion(&handle);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::Validate(const char *xml, char **beautified)
/*!\brief Do an XML validation against a text
 *
 * This function takes the given \p xml test an tries to validate it with the XML
 * library (xmlReadMemory). If the XML is valid, a \p beautified version, which can
 * be better read by humans, can be stored optionally.
 *
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
CIrisLWZ iris;
if (iris.Validate(xml,&beautified)) {
	printf ("Original XML code:\n");
	printf ("%s\n",xml);
	printf ("Beautified version:\n");
	printf ("%s\n",beautified);
}
if (beautified) free(beautified);
\endcode
 *
 */
{
	int ret=irislwz_Validate(&handle, xml, beautified);
	if (!ret) CopyError(&handle);
	return ret;

}

void CIrisLWZ::FreePacket(IRISLWZ_PACKET *p)
/*!\brief Frees the memory used by an IRISLWZ_PACKET
 *
 * This function frees the memory allocated within an IRISLWZ_PACKET object.
 * The object itself is not freed.
 *
 * \param[in] p Pointer to an IRISLWZ_PACKET
 */
{
	irislwz_FreePacket(p);
}

int CIrisLWZ::GetCompressedSize(const char *buffer, int size)
/*!\brief Calculate the zlib compressed size of a buffer
 *
 * This function compresses the given \p buffer with zlib and returns the required
 * amount of Bytes. It is used internally by some functions, but can be used externally, too.
 *
 * \param[in] buffer Pointer to the beginning of data, which should be compressed
 * \param[in] size Amount of bytes in the \p buffer
 * \returns On success, this function returns the amount of bytes needed to compress
 * the data, otherwise it will return 0.
 */
{
	int ret=irislwz_GetCompressedSize(buffer,size,handle.rfc1950_workaround);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::Query(const char *payload)
/*!\brief Send a request to an IRISLWZ server
 *
 * This function is used to send a request to an IRISLWZ server and wait for response.
 *
 * \param[in] payload Request, which should be send to IRIS server
 * \returns On success, this function returns 1 and the response is saved in the response structure of
 * \p handle. The request structure is filled, too. In case of an error, 0 will be returned.
 */
{
	int ret=irislwz_Query(&handle, payload);
	if (!ret) CopyError(&handle);
	return ret;
}

int CIrisLWZ::Query2(const char *payload, int bytes)
/*!\brief Send a request to an IRISLWZ server
 *
 * This function is used to send a request to an IRISLWZ server and wait for response.
 * In addition to CIrisLWZ::Query, this function needs a second parameter indicating the
 * size of the payload.
 *
 * \param[in] payload Request, which should be send to IRIS server
 * \param[in] bytes Size of the payload
 * \returns On success, this function returns 1 and the response is saved in the response structure of
 * \p handle. The request structure is filled, too. In case of an error, 0 will be returned.
 *
 * \since
 * Version 0.5.4
 *
 */
{
	int ret=irislwz_Query2(&handle, payload, bytes);
	if (!ret) CopyError(&handle);
	return ret;
}


int CIrisLWZ::CheckResponse()
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
	int ret=irislwz_CheckResponse(&handle);
	if (!ret) CopyError(&handle);
	return ret;
}


IRISLWZ_PACKET *CIrisLWZ::GetRequestPacket()
/*!\brief Get Pointer of Request Packet
 *
 * This function returns a \ref IRISLWZ_PACKET pointer to the request packet. It's contents
 * are only valid, if a request was send before.
 *
 * \returns Pointer to an \ref IRISLWZ_PACKET
 */
{
	return &handle.request;
}


IRISLWZ_PACKET *CIrisLWZ::GetResponsePacket()
/*!\brief Get Pointer of Response Packet
 *
 * This function returns a \ref IRISLWZ_PACKET pointer to the response packet. It's contents
 * are only valid, if a request was send before.
 *
 * \returns Pointer to an \ref IRISLWZ_PACKET
 */
{
	return &handle.response;
}

IRISLWZ_HANDLE *CIrisLWZ::GetHandle()
/*!\brief Get Pointer to \ref IRISLWZ_HANDLE
 *
 * This function returns a pointer to the \ref IRISLWZ_HANDLE. You can use it to directly call the
 * low level \ref DCHK_C_API [C-API] functions or read informations from it.
 *
 * \returns Pointer to the \ref IRISLWZ_HANDLE used by this class.
 */
{
	return &handle;
}

bool CIrisLWZ::IsResponse(IRISLWZ_PACKET *p)
/*!\brief Check if packet is a response packet
 *
 * This function checks, if the given packet is a response packet.
 *
 * \param[in] p Optional pointer to a \ref IRISLWZ_PACKET structure. If no pointer is given,
 * the function will use the last response packet revived with this class.
 * \returns Returns true or false.
 */
{
	if (!p) p=&handle.response;
	if (!p->header) return false;
	if (p->header->header&32) return true;
	return false;
}

bool CIrisLWZ::IsDeflated(IRISLWZ_PACKET *p)
/*!\brief Check if packet was deflated
 *
 * This function checks, if the payload of the given packet was deflated (compressed).
 *
 * \param[in] p Optional pointer to a \ref IRISLWZ_PACKET structure. If no pointer is given,
 * the function will use the last response packet revived with this class.
 * \returns Returns true or false.
 */
{
	if (!p) p=&handle.response;
	if (!p->header) return false;
	if (p->header->header&16) return true;
	return false;
}

bool CIrisLWZ::IsDeflateSupported(IRISLWZ_PACKET *p)
/*!\brief Check if deflate is supported
 *
 * This function checks, if the given packet indicates that deflate (compression)
 * is supported.
 *
 * \param[in] p Optional pointer to a \ref IRISLWZ_PACKET structure. If no pointer is given,
 * the function will use the last response packet revived with this class.
 * \returns Returns true or false.
 */
{
	if (!p) p=&handle.response;
	if (!p->header) return false;
	if (p->header->header&8) return true;
	return false;

}

bool CIrisLWZ::PayloadIsXML(IRISLWZ_PACKET *p)
/*!\brief Check if payload is XML
 *
 * This function checks, if the payload of the given packet is XML.
 *
 * \param[in] p Optional pointer to a \ref IRISLWZ_PACKET structure. If no pointer is given,
 * the function will use the last response packet revived with this class.
 * \returns Returns true or false.
 */
{
	if (!p) p=&handle.response;
	if (!p->header) return false;
	if ((p->header->header&3)==0) return true;
	return false;

}

bool CIrisLWZ::PayloadIsVersion(IRISLWZ_PACKET *p)
/*!\brief Check if payload is version information
 *
 * This function checks, if the payload of the given packet contains version information.
 *
 * \param[in] p Optional pointer to a \ref IRISLWZ_PACKET structure. If no pointer is given,
 * the function will use the last response packet revived with this class.
 * \returns Returns true or false.
 */
{
	if (!p) p=&handle.response;
	if (!p->header) return false;
	if ((p->header->header&3)==1) return true;
	return false;
}

bool CIrisLWZ::PayloadIsSizeInfo(IRISLWZ_PACKET *p)
/*!\brief Check if payload is size information
 *
 * This function checks, if the payload of the given packet contains size information.
 *
 * \param[in] p Optional pointer to a \ref IRISLWZ_PACKET structure. If no pointer is given,
 * the function will use the last response packet revived with this class.
 * \returns Returns true or false.
 */
{
	if (!p) p=&handle.response;
	if (!p->header) return false;
	if ((p->header->header&3)==2) return true;
	return false;
}

bool CIrisLWZ::PayloadIsOther(IRISLWZ_PACKET *p)
/*!\brief Check if payload is other information
 *
 * This function checks, if the payload of the given packet contains other information.
 *
 * \param[in] p Optional pointer to a \ref IRISLWZ_PACKET structure. If no pointer is given,
 * the function will use the last response packet revived with this class.
 * \returns Returns true or false.
 */
{
	if (!p) p=&handle.response;
	if (!p->header) return false;
	if ((p->header->header&3)==3) return true;
	return false;
}

const char *CIrisLWZ::GetPayloadType(IRISLWZ_PACKET *p)
/*!\brief Returns the name of the payload type
 *
 * This returns the name of the payload type.
 *
 * \param[in] p Optional pointer to a \ref IRISLWZ_PACKET structure. If no pointer is given,
 * the function will use the last response packet revived with this class.
 * \returns Returns a pointer to the name or NULL, if payload type is unknown.
 */
{
	if (!p) p=&handle.response;
	if (!p->header) return NULL;
	switch (p->header->header&3) {
		case 0: return "XML";
		case 1: return "Version Info";
		case 2: return "Size Info";
		case 3: return "Other Info";
	}
	return NULL;
}


} // EOF namespace dchk
