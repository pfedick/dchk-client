/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.3 $
 * $Date: 2009/03/12 11:56:19 $
 * $Id: socketerrors.c,v 1.3 2009/03/12 11:56:19 pfedick Exp $
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

#ifdef HAVE_ERRNO_H
	#include <errno.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif


#include <string.h>

#include "dchk.h"
#include "compat.h"

static const char * socketerrors[] = {
		"Socket: Unknown socket error",										// 1000
		"Socket: The file handle supplied is not valid (EBADF)",			// 1001
		"Socket: Permission denied (EACCES, EPERM)",						// 1002
		"Socket: Bad address (EFAULT)",										// 1003
		"Socket: Invalid argument (EINVAL)",								// 1004
		"Socket: Too many open sockets (EMFILE)",							// 1005
		"Socket: Resource temporarily unavailable (EWOULDBLOCK)",			// 1006
		"Socket: Operation now in progress (EINPROGRESS)",					// 1007
		"Socket: Operation already in progress (EALREADY)",					// 1008
		"Socket: Socket operation on nonsocket (ENOTSOCK)",					// 1009
		"Socket: Destination address required (EDESTADDRREQ)",				// 1010
		"Socket: Message too long (EMSGSIZE)",								// 1011
		"Socket: Protocol wrong type for socket (EPROTOTYPE)",				// 1012
		"Socket: Bad protocol option (ENOPROTOOPT)",						// 1013
		"Socket: Protocol not supported (EPROTONOSUPPORT)",					// 1014
		"Socket: Socket type not supported (ESOCKTNOSUPPORT)",				// 1015
		"Socket: Operation not supported (EOPNOTSUPP)",						// 1016
		"Socket: Protocol family not supported (EPFNOSUPPORT)",				// 1017
		"Socket: Address family not supported by protocol family (EAFNOSUPPORT)",	// 1018
		"Socket: Address already in use (EADDRINUSE)",						// 1019
		"Socket: Cannot assign requested address (EADDRNOTAVAIL)",			// 1020
		"Socket: Network is down (ENETDOWN)",								// 1021
		"Socket: Network is unreachable (ENETUNREACH)",						// 1022
		"Socket: Network dropped connection on reset (ENETRESET)",			// 1023
		"Socket: Software caused connection abort (ECONNABORTED)",			// 1024
		"Socket: Connection reset by peer (ECONNRESET)",					// 1025
		"Socket: No buffer space available (ENOBUFS)",						// 1026
		"Socket: Socket is already connected (EISCONN)",					// 1027
		"Socket: Socket is not connected (ENOTCONN)",						// 1028
		"Socket: Cannot send after socket shutdown (ESHUTDOWN)",			// 1029
		"Socket: Too many references (ETOOMANYREFS)",						// 1030
		"Socket: Connection timed out (ETIMEDOUT)",							// 1031
		"Socket: Connection refused (ECONNREFUSED)",						// 1032
		"Socket: Cannot translate name (ELOOP)",							// 1033
		"Socket: Name too long (ENAMETOOLONG)",								// 1034
		"Socket: Host is down (EHOSTDOWN)",									// 1035
		"Socket: No route to host (EHOSTUNREACH)",							// 1036
		"Socket: Directory not empty (ENOTEMPTY)",							// 1037
		"Socket: Too many processes (EPROCLIM)",							// 1038
		"Socket: User quota exceeded (EUSERS)",								// 1039
		"Socket: Disk quota exceeded (EDQUOT)",								// 1040
		"Socket: The file handle reference is no longer available (ESTALE)",	// 1041
		"Socket: Item is remote (EREMOTE)",									// 1042
		"Socket: Network subsystem is unavailable (WSASYSNOTREADY)",		// 1043
		"Socket: The Windows Sockets version requested is not supported. (WSAVERNOTSUPPORTED)",	// 1044
		"Socket: Successful WSAStartup not yet performed (WSANOTINITIALISED)",	// 1045
		"Socket: Graceful shutdown in progress (WSAEDISCON)",				// 1046
		"Socket: No more results (WSAENOMORE)",								// 1047
		"Socket: Call has been canceled (WSAECANCELLED)",					// 1048
		"Socket: Procedure call table is invalid (WSAEINVALIDPROCTABLE)",	// 1049
		"Socket: Service provider is invalid (WSAEINVALIDPROVIDER)",		// 1050
		"Socket: Service provider failed to initialize (WSAEPROVIDERFAILEDINIT)",	// 1051
		"Socket: System call failure (WSASYSCALLFAILURE)",					// 1052
		"Socket: Service not found (WSASERVICE_NOT_FOUND)",					// 1053
		"Socket: Class type not found (WSATYPE_NOT_FOUND)",					// 1054
		"Socket: No more results (WSA_E_NO_MORE)",							// 1055
		"Socket: Call was canceled (WSA_E_CANCELLED)",						// 1056
		"Socket: Database query was refused (WSAEREFUSED)",					// 1057
		"Socket: Host not found (WSAHOST_NOT_FOUND)",						// 1058
		"Socket: Nonauthoritative host not found (WSATRY_AGAIN)",			// 1059
		"Socket: This is a nonrecoverable error (WSANO_RECOVERY)",			// 1060
		"Socket: Valid name, no data record of requested type (WSANO_DATA)",	// 1061
		"Socket: A blocking operation was interrupted (EINTR)",				// 1062
		"Socket: Resource temporarily unavailable (EAI_AGAIN)",				// 1063
		"Socket: Invalid value for ai_flags (EAI_BADFLAGS)",				// 1064
		"Socket: Invalid value for hints (EAI_BADHINTS)",					// 1065
		"Socket: Non-recoverable failure in name resolution (EAI_FAIL)",	// 1066
		"Socket: ai_family not supported (EAI_FAMILY)",						// 1067
		"Socket: memory allocation failure (EAI_MEMORY)",					// 1068
		"Socket: hostname not provided or not known (EAI_NONAME)",			// 1069
		"Socket: argument buffer overflow (EAI_OVERFLOW)",					// 1070
		"Socket: resolved protocol is unknown (EAI_PROTOCOL)",				// 1071
		"Socket: servname not supported for ai_socktype (EAI_SERVICE)",		// 1072
		"Socket: ai_socktype not supported (EAI_SOCKTYPE)",					// 1073
		"Socket: Host exists, but does not have any network addresses defined (EAI_NODATA)",	// 1074
		NULL
};

const char *irislwz_GetSocketError(int code)
/*!\brief Return text of a specific socket error code
 *
 * this is an internal function, which returns the text of a specific socket error
 * code. It is automatically called by \ref irislwz_GetError, if an error code
 * between 1000 and 1999 occurs.
 *
 * \param[in] code Socket error code
 * \returns pointer to the corresponding text of the error code or NULL, if the error
 * code is unknown.
 *
 * \ingroup DCHK_errors
 */
{
	if (code>=1000 && code <= 1000+(sizeof(socketerrors) / sizeof(char *)-2)) {
		return ((const char*)socketerrors[code-1000]);
	}
	return NULL;
}



int irislwz_TranslateSocketError()
{
#ifdef _WIN32
	int e=WSAGetLastError();
	switch (e) {
		case WSAEBADF: return 1001;
		case WSAEACCES: return 1002;
		case WSAEFAULT: return 1003;
		case WSAEINVAL: return 1004;
		case WSAEMFILE: return 1005;
		case WSAEWOULDBLOCK: return 1006;
		case WSAEINPROGRESS: return 1007;
		case WSAEALREADY: return 1008;
		case WSAENOTSOCK: return 1009;
		case WSAEDESTADDRREQ: return 1010;
		case WSAEMSGSIZE: return 1011;
		case WSAEPROTOTYPE: return 1012;
		case WSAENOPROTOOPT: return 1013;
		case WSAEPROTONOSUPPORT: return 1014;
		case WSAESOCKTNOSUPPORT: return 1015;
		case WSAEOPNOTSUPP: return 1016;
		case WSAEPFNOSUPPORT: return 1017;
		case WSAEAFNOSUPPORT: return 1018;
		case WSAEADDRINUSE: return 1019;
		case WSAEADDRNOTAVAIL: return 1020;
		case WSAENETDOWN: return 1021;
		case WSAENETUNREACH: return 1022;
		case WSAENETRESET: return 1023;
		case WSAECONNABORTED: return 1024;
		case WSAECONNRESET: return 1025;
		case WSAENOBUFS: return 1026;
		case WSAEISCONN: return 1027;
		case WSAENOTCONN: return 1028;
		case WSAESHUTDOWN: return 1029;
		case WSAETOOMANYREFS: return 1030;
		case WSAETIMEDOUT: return 1031;
		case WSAECONNREFUSED: return 1032;
		case WSAELOOP: return 1033;
		case WSAENAMETOOLONG: return 1034;
		case WSAEHOSTDOWN: return 1035;
		case WSAEHOSTUNREACH: return 1036;
		case WSAENOTEMPTY: return 1037;
		case WSAEPROCLIM: return 1038;
		case WSAEUSERS: return 1039;
		case WSAEDQUOT: return 1040;
		case WSAESTALE: return 1041;
		case WSAEREMOTE: return 1042;
		case WSASYSNOTREADY: return 1043;
		case WSAVERNOTSUPPORTED: return 1044;
		case WSANOTINITIALISED: return 1045;
		case WSAEDISCON: return 1046;
		case WSAENOMORE: return 1047;
		case WSAECANCELLED: return 1048;
		case WSAEINVALIDPROCTABLE: return 1049;
		case WSAEINVALIDPROVIDER: return 1050;
		case WSAEPROVIDERFAILEDINIT: return 1051;
		case WSASYSCALLFAILURE: return 1052;
		case WSASERVICE_NOT_FOUND: return 1053;
		case WSATYPE_NOT_FOUND: return 1054;
		case WSA_E_NO_MORE: return 1055;
		case WSA_E_CANCELLED: return 1056;
		case WSAEREFUSED: return 1057;
		case WSAHOST_NOT_FOUND: return 1058;
		case WSATRY_AGAIN: return 1059;
		case WSANO_RECOVERY: return 1060;
		case WSANO_DATA: return 1061;
		case WSAEINTR: return 1062;
	}
#else
	switch (errno) {
#ifdef EBADF
		case EBADF: return 1001;
#endif
#ifdef EACCES
		case EACCES: return 1002;
#endif
#ifdef EPERM
		case EPERM: return 1002;
#endif
#ifdef EFAULT
		case EFAULT: return 1003;
#endif
#ifdef EINVAL
		case EINVAL: return 1004;
#endif
#ifdef EMFILE
		case EMFILE: return 1005;
#endif
#ifdef ENFILE
		case ENFILE: return 1005;
#endif
#ifdef EWOULDBLOCK
		case EWOULDBLOCK: return 1006;
#endif
#ifdef EINPROGRESS
		case EINPROGRESS: return 1007;
#endif
#ifdef EALREADY
		case EALREADY: return 1008;
#endif
#ifdef ENOTSOCK
		case ENOTSOCK: return 1009;
#endif
#ifdef EDESTADDRREQ
		case EDESTADDRREQ: return 1010;
#endif
#ifdef EMSGSIZE
		case EMSGSIZE: return 1011;
#endif
#ifdef EPROTOTYPE
		case EPROTOTYPE: return 1012;
#endif
#ifdef ENOPROTOOPT
		case ENOPROTOOPT: return 1013;
#endif
#ifdef EPROTONOSUPPORT
		case EPROTONOSUPPORT: return 1014;
#endif
#ifdef ESOCKTNOSUPPORT
		case ESOCKTNOSUPPORT: return 1015;
#endif
#ifdef EOPNOTSUPP
		case EOPNOTSUPP: return 1016;
#endif
#ifdef EPFNOSUPPORT
		case EPFNOSUPPORT: return 1017;
#endif
#ifdef EAFNOSUPPORT
		case EAFNOSUPPORT: return 1018;
#endif
#ifdef EADDRINUSE
		case EADDRINUSE: return 1019;
#endif
#ifdef EADDRNOTAVAIL
		case EADDRNOTAVAIL: return 1020;
#endif
#ifdef ENETDOWN
		case ENETDOWN: return 1021;
#endif
#ifdef ENETUNREACH
		case ENETUNREACH: return 1022;
#endif
#ifdef ENETRESET
		case ENETRESET: return 1023;
#endif
#ifdef ECONNABORTED
		case ECONNABORTED: return 1024;
#endif
#ifdef ECONNRESET
		case ECONNRESET: return 1025;
#endif
#ifdef ENOBUFS
		case ENOBUFS: return 1026;
#endif
#ifdef EISCONN
		case EISCONN: return 1027;
#endif
#ifdef ENOTCONN
		case ENOTCONN: return 1028;
#endif
#ifdef ESHUTDOWN
		case ESHUTDOWN: return 1029;
#endif
#ifdef ETOOMANYREFS
		case ETOOMANYREFS: return 1030;
#endif
#ifdef ETIMEDOUT
		case ETIMEDOUT: return 1031;
#endif
#ifdef ECONNREFUSED
		case ECONNREFUSED: return 1032;
#endif
#ifdef ELOOP
		case ELOOP: return 1033;
#endif
#ifdef ENAMETOOLONG
		case ENAMETOOLONG: return 1034;
#endif
#ifdef EHOSTDOWN
		case EHOSTDOWN: return 1035;
#endif
#ifdef EHOSTUNREACH
		case EHOSTUNREACH: return 1036;
#endif
#ifdef ENOTEMPTY
		case ENOTEMPTY: return 1037;
#endif
#ifdef EPROCLIM
		case EPROCLIM: return 1038;
#endif
#ifdef EUSERS
		case EUSERS: return 1039;
#endif
#ifdef EDQUOT
		case EDQUOT: return 1040;
#endif
#ifdef ESTALE
		case ESTALE: return 1041;
#endif
#ifdef EREMOTE
		case EREMOTE: return 1042;
#endif
#ifdef EINTR
		case EINTR: return 1062;
#endif
	}

#endif
	//printf ("errno: %i, Text: %s\n",errno,strerror(errno));
	return 1000;
}

int irislwz_TranslateGaiError(int e)
{
#ifdef _WIN32
	return irislwz_TranslateSocketError();
#else
	switch (e) {
#ifdef EAI_AGAIN
		case EAI_AGAIN: return 1063;	// temporary failure in name resolution
#endif
#ifdef EAI_ADDRFAMILY
		case EAI_ADDRFAMILY: return 1067;
#endif
#ifdef EAI_BADFLAGS
		case EAI_BADFLAGS: return 1064;	// invalid value for ai_flags
#endif
#ifdef EAI_BADHINTS
		case EAI_BADHINTS: return 1065;	// invalid value for hints
#endif
#ifdef EAI_FAIL
		case EAI_FAIL: return 1066;		// non-recoverable failure in name resolution
#endif
#ifdef EAI_FAMILY
		case EAI_FAMILY: return 1067;	// ai_family not supported
#endif
#ifdef EAI_MEMORY
		case EAI_MEMORY: return 1068;	// memory allocation failure
#endif
#ifdef EAI_NONAME
		case EAI_NONAME: return 1069;	// hostname or servname not provided, or not known
#endif
#ifdef EAI_OVERFLOW
		case EAI_OVERFLOW: return 1070;	// argument buffer overflow
#endif
#ifdef EAI_PROTOCOL
		case EAI_PROTOCOL: return 1071;	// resolved protocol is unknown
#endif
#ifdef EAI_SERVICE
		case EAI_SERVICE: return 1072;	// servname not supported for ai_socktype
#endif
#ifdef EAI_SOCKTYPE
		case EAI_SOCKTYPE: return 1073;	// ai_socktype not supported
#endif
#ifdef EAI_SYSTEM
		case EAI_SYSTEM: return irislwz_TranslateSocketError();	//  system error returned in errno
#endif
#ifdef EAI_NODATA
		case EAI_NODATA: return 1074;
#endif

	}
#endif
	return 1000;
}


