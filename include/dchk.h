/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.19 $
 * $Date: 2011/01/26 12:13:43 $
 * $Id: dchk.h,v 1.19 2011/01/26 12:13:43 pfedick Exp $
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

#ifndef DCHK_H_
#define DCHK_H_

#define DCHK_VERSION			"0.5.8-alpha1"
#define DCHK_RELEASEDATE		"26.01.2011"
#define DCHK_COPYRIGHT			"Copyright (c) 2008-2011 by Patrick Fedick <dchk@pfp.de>"
#define DCHK_VERSION_MAJOR 0
#define DCHK_VERSION_MINOR	5
#define DCHK_VERSION_BUILD	8


#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)			// Compiler should align the following variables
						// on byte boundaries
typedef struct {
	unsigned char	header;
	unsigned short	transactionId;
	unsigned short	max_response_length;
	unsigned char	authority_length;
} IRISLWZ_HEADER;
#pragma pack()

typedef struct {
	IRISLWZ_HEADER *header;
	char *packet;
	int size;
	char *payload;
	char *authority;
	int size_payload;
	char *uncompressed_payload;
	int size_uncompressed;
	char *beautified;
} IRISLWZ_PACKET;


typedef struct {
	char *errortext;
	char *authority;
	char *localcharset;
	char *server;
	int error;
	int port;
	unsigned char initialized;
	void *socket;
	char acceptDeflated;
	char sendDeflated;
	char flag_si;
	char flag_oi;
	char rfc1950_workaround;
	char ignoreInvalidXML;
	char dontValidateInput;
	int	maximumResponseLength;
	int	timeout_seconds, timeout_useconds;
	IRISLWZ_PACKET request;
	IRISLWZ_PACKET response;
	void (*DebugHandler) (int level, const char *text, void *privatedata);
	void *debugdata;
	int debuglevel;
	char verboseflag;
	char test_flags;
	char test_version;
	char test_header;
	int test_tid;
	int test_authlen;
	int test_truncate;

} IRISLWZ_HANDLE;


typedef struct __dchk_record {
	struct __dchk_record *next;
	char *domain;
	char *lastdatabaseupdate;
	char *status;
} DCHK_RECORD;

typedef struct {
	DCHK_RECORD *first, *last;
	int num;
} DCHK_DOMAINLIST;

// IRISLWZ

const char *irislwz_GetVersion();
const char *irislwz_GetCopyrightString();
const char *irislwz_GetReleaseDate();
int irislwz_CheckMinVersion (int major, int minor, int build);

int irislwz_InitWSA();
int irislwz_Init(IRISLWZ_HANDLE *handle);
int irislwz_Exit(IRISLWZ_HANDLE *handle);
int irislwz_UseRFC1950(IRISLWZ_HANDLE *handle, int flag);
int irislwz_SetTimeout(IRISLWZ_HANDLE *handle, int seconds, int useconds);
int irislwz_SetAuthority(IRISLWZ_HANDLE *handle, const char *authority);
int irislwz_SetAuthorityByDomain(IRISLWZ_HANDLE *handle, const char *domain);
int irislwz_SetAuthorityByDomainList(IRISLWZ_HANDLE *handle, DCHK_DOMAINLIST *list);
int irislwz_SetMaximumResponseLength(IRISLWZ_HANDLE *handle, int bytes);
int irislwz_SetHost(IRISLWZ_HANDLE *handle, const char *host, int port);
const char *irislwz_GetHostname(IRISLWZ_HANDLE *handle);
int irislwz_GetPort(IRISLWZ_HANDLE *handle);
int irislwz_AcceptDeflated(IRISLWZ_HANDLE *handle, int flag);
int irislwz_SendDeflated(IRISLWZ_HANDLE *handle, int flag);
int irislwz_IgnoreInvalidXML(IRISLWZ_HANDLE *handle, int flag);
int irislwz_Connect(IRISLWZ_HANDLE *handle);
int irislwz_Disconnect(IRISLWZ_HANDLE *handle);
void irislwz_SetDebugLevel(IRISLWZ_HANDLE *handle, int level);
void irislwz_SetDebugHandler(IRISLWZ_HANDLE *handle, void (*DebugHandler)(int, const char *, void *), void *privatedata);
void irislwz_Debug(IRISLWZ_HANDLE *handle, int level, const char *fmt, ...);
int irislwz_Talk(IRISLWZ_HANDLE *handle, IRISLWZ_PACKET *request, IRISLWZ_PACKET *response);
int irislwz_QueryVersion(IRISLWZ_HANDLE *handle);
int irislwz_Validate(IRISLWZ_HANDLE *handle, const char *xml, char **beautified);
void irislwz_FreePacket(IRISLWZ_PACKET *p);
int irislwz_PrepareQuery(IRISLWZ_HANDLE *handle);
int irislwz_GetCompressedSize(const char *buffer, int size, int rfc1950);
int irislwz_Query(IRISLWZ_HANDLE *handle, const char *payload);
int irislwz_Query2(IRISLWZ_HANDLE *handle, const char *payload, int bytes);
int irislwz_FlagSizeInfo(IRISLWZ_HANDLE *handle, int flag);
int irislwz_FlagOtherInfo(IRISLWZ_HANDLE *handle, int flag);
int irislwz_TestSetVersion(IRISLWZ_HANDLE *handle, int version);
int irislwz_TestSetTransactionId(IRISLWZ_HANDLE *handle, int tid);
int irislwz_TestSetHeader(IRISLWZ_HANDLE *handle, int header);
int irislwz_TestTruncatePacket(IRISLWZ_HANDLE *handle, int bytes);
int irislwz_TestSetAuthorityLength(IRISLWZ_HANDLE *handle, int bytes);
int irislwz_TestCorruptCompression(IRISLWZ_HANDLE *handle, int flag);
int irislwz_TestCleanup(IRISLWZ_HANDLE *handle);
int irislwz_CheckResponse(IRISLWZ_HANDLE *handle);

// DCHK
int dchk_SetHostByDNS(IRISLWZ_HANDLE *handle, const char *tld);
int dchk_QueryDomainList(IRISLWZ_HANDLE *handle, DCHK_DOMAINLIST *list, int maxdomainsperquery);
int dchk_QueryDomain(IRISLWZ_HANDLE *handle, const char *domain, char **status, char **dbupdate);
int dchk_XML_ParseResult(IRISLWZ_HANDLE *handle,DCHK_DOMAINLIST *domainset);
int dchk_XML_ParseDomain(IRISLWZ_HANDLE *handle, void *node, DCHK_RECORD *r);
int dchk_XML_ParseSize(IRISLWZ_HANDLE *handle);
int dchk_XML_ParseOther(IRISLWZ_HANDLE *handle);
DCHK_DOMAINLIST *dchk_NewDomainList();
void dchk_FreeDomainList(DCHK_DOMAINLIST *list);
int dchk_AddDomain(DCHK_DOMAINLIST *list, const char *domain);
int dchk_AddnDomain(DCHK_DOMAINLIST *list, const char *domain, int n);
int dchk_ExplodeDomain(DCHK_DOMAINLIST *list, const char *string,const char *delimiter);
int dchk_PrintResults(DCHK_DOMAINLIST *list);
int dchk_EscapeSpecialChars(const char *string, char *buffer, int buffersize);

// Error handling
const char *irislwz_GetError(int code);
int irislwz_GetErrorCode(IRISLWZ_HANDLE *handle);
const char *irislwz_GetErrorText(IRISLWZ_HANDLE *handle);
const char *irislwz_GetAdditionalErrorText(IRISLWZ_HANDLE *handle);
void irislwz_SetError(IRISLWZ_HANDLE *handle, int code, const char *fmt, ...);
void irislwz_PrintError(IRISLWZ_HANDLE *handle);

// Helper and compatibility functions
void irislwz_srand(unsigned int x);
char * irislwz_rand_initstate(unsigned long seed, char *arg_state, long n);
char * irislwz_rand_setstate(char *arg_state);
unsigned int irislwz_rand(unsigned int min, unsigned int max);
void irislwz_HexDump(void *address, int bytes);

#ifdef __cplusplus
}
#endif



#ifdef __cplusplus
namespace dchk {


class CError
{
	private:
		int code;
		char *errortext;
	public:
		CError();
		~CError();
		//!\name Error functions
		//@{
		void SetError(int code, const char *fmt=NULL, ...);
		void PrintError() const;
		int GetErrorCode() const;
		const char *GetErrorText() const;
		const char *GetAdditionalErrorText() const;
		void CopyError(IRISLWZ_HANDLE *handle);
		//@}
};

class CDomainList : public CError
{
	friend class CDchk;
	private:
		DCHK_DOMAINLIST list;
		DCHK_RECORD *ptr;

	public:
		CDomainList();
		~CDomainList();
		void Clear();
		void Reset();
		int AddDomain(const char *domain, int n=0);
		DCHK_RECORD *GetFirst();
		DCHK_RECORD *GetNext();
		int Count() const;
		int Explode(const char *string, const char *delimiter);
};

class CIrisLWZ : public CError
{
	friend class CDchk;
	private:
		IRISLWZ_HANDLE handle;

		int		PrepareQuery();
		int		Talk(IRISLWZ_PACKET *request, IRISLWZ_PACKET *response);
		void	FreePacket(IRISLWZ_PACKET *p);

	public:
		//!\name Constructures and Destructures
		//@{
		CIrisLWZ();
		~CIrisLWZ();
		//@}

		//!\name CIrisLWZ Options
		//@{
		int		SetTimeout(int seconds, int useconds);
		int		SetAuthority(const char *authority);
		int		SetAuthorityByDomain(const char *domain);
		int		SetAuthorityByDomainList(CDomainList &list);
		int		SetMaximumResponseLength(int bytes);
		int		AcceptDeflated(int flag);
		int		SendDeflated(int flag);
		int		IgnoreInvalidXML(int flag);
		int		UseRFC1950(int flag);
		void	SetDebugLevel(int level);
		void	SetDebugHandler(void (*DebugHandler)(int, const char *, void *), void *privatedata);
		//@}
		//!\name CIrisLWZ Connection
		//@{
		int		SetHost(const char *host, int port=715);
		int		Connect();
		int		Disconnect();
		const char *GetHostname();
		int		GetPort();
		//@}

		//!\name CIrisLWZ Queries
		//@{
		int		QueryVersion();
		int		Query(const char *payload);
		int		Query2(const char *payload, int bytes);
		IRISLWZ_PACKET *GetRequestPacket();
		IRISLWZ_PACKET *GetResponsePacket();
		IRISLWZ_HANDLE *GetHandle();
		//@}

		//!\name CIrisLWZ Tools
		//@{
		int		Validate(const char *xml, char **beautified);
		int		GetCompressedSize(const char *buffer, int size);
		//@}

		//!\name CIrisLWZ Checks
		//@{
		bool	IsResponse(IRISLWZ_PACKET *p=NULL);
		bool	IsDeflated(IRISLWZ_PACKET *p=NULL);
		bool	IsDeflateSupported(IRISLWZ_PACKET *p=NULL);
		bool	PayloadIsXML(IRISLWZ_PACKET *p=NULL);
		bool	PayloadIsVersion(IRISLWZ_PACKET *p=NULL);
		bool	PayloadIsSizeInfo(IRISLWZ_PACKET *p=NULL);
		bool	PayloadIsOther(IRISLWZ_PACKET *p=NULL);
		const char *GetPayloadType(IRISLWZ_PACKET *p=NULL);
		int		CheckResponse();
		//@}

		//!\name CIrisLWZ Testing Options
		//@{
		int		FlagSizeInfo(int flag);
		int		FlagOtherInfo(int flag);
		int		TestSetVersion(int version);
		int		TestSetTransactionId(int tid);
		int		TestSetHeader(int header);
		int		TestTruncatePacket(int bytes);
		int		TestSetAuthorityLength(int length);
		int		TestCorruptCompression(int flag);
		int		TestCleanup();


		//@}

};

class CDchk : public CIrisLWZ
{
	private:
		int maxdomainsperquery;

	public:
		//!\name Constructures and Destructures
		//@{
		CDchk();
		~CDchk();
		//@}
		//!\name CDchk Options
		//@{
		int		SetHostByDNS(const char *tld);
		void	SetMaxDomainsPerQuery(int count);
		//@}
		//!\name CDchk Queries
		//@{
		int		QueryDomainList(CDomainList &list);
		int		QueryDomain(const char *domain, char **status, char **dbupdate);
		int		PrintResults(CDomainList &list);
		//@}
};


} // EOF namespace dchk

#endif	// #ifdef __cplusplus

#endif	// #ifdef DCHK_H_
