/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pafe $
 * $Revision: 1.5 $
 * $Date: 2009/03/20 18:57:24 $
 * $Id: errors.c,v 1.5 2009/03/20 18:57:24 pafe Exp $
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



#include "dchk.h"
#include "compat.h"


static const char * errorslwz[] = {
	"DNS: initialization of resolver failed",							// 74000
	"DNS: query for DCHK NAPTR-record failed",							// 74001
	"DNS: Unknown or invalid TLD",										// 74002
	"DNS: parsing error",												// 74003
	"DNS: No NAPTR records found for this TLD",							// 74004
	"DCHK: No DCHK service registered for this TLD",					// 74005
	"DNS: No DCHK service found for this TLD (Unknown or invalid SRV-record)",		// 74006
	"DNS: No DCHK service found for this TLD (Query for SRV-record failed)",		// 74007
	"DNS: No DCHK service found for this TLD (No SRV records found for this name)",	// 74008
	"UDP: Sending request packet to DCHK server failed",				// 74009
	"DCHK: Hostname missing",											// 74010
	"DCHK: No domain names given",										// 74011
	"DCHK: Could not resolve hostname",									// 74012
	"DCHK: Could not open file with debug input (-i)",					// 74013
	"DCHK: Parsing of XML response failed",								// 74014
	"XML: unable to create new XPath context",							// 74015
	"XML: libxml2 not available",										// 74016
	"DCHK: Unknown XML response",										// 74017
	"XML: unable to register namespace",								// 74018
	"XML: Empty resultSet",												// 74019
	"DCHK: Timeout - server did not respond in time",					// 74020
	"DCHK: TLD is missing",												// 74021
	"Sorry, automatic lookup of DCHK server by DNS is not supported on this platform",	// 74022
	"DCHK object points to NULL",										// 74023
	"Out of memory",													// 74024
	"Parameter missing or NULL",										// 74025
	"No hostname or port specified yet",								// 74026
	"DCHK: Authority name is to long",									// 74027
	"Could not initialize DCHK handle",									// 74028
	"Could not add domain to query list",								// 74029
	"Could not create socket",											// 74030
	"Already connected to DCHK server",									// 74031
	"Not connected to DCHK server",										// 74032
	"Setting of socket timeout failed",									// 74033
	"Socket error",														// 74034
	"Operation timeout",												// 74035
	"DCHK: Answer is too short",										// 74036
	"Decompression of payload failed",									// 74037
	"Nothing to do, domain list is empty",								// 74038
	"Compression of payload failed",									// 74039
	"DCHK: Answer is bigger than maximum allowed size",					// 74040
	"DCHK: Server has set size information bit, but no size found",		// 74041
	"Domain list is empty",												// 74042
	"No more domains in list",											// 74043
	"Payload contains not a valid XML document",						// 74044
	"XML in request packet is invalid",									// 74045
	"XML in response packet is invalid",								// 74046
	"Server response with \"other information\"",						// 74047
	"Server raised descriptor error",									// 74048
	"Server raised payload error",										// 74049
	"Server raised system error",										// 74050
	"Server raised authority-error",									// 74051
	"Server raised no-inflation-support-error",							// 74052
	"Unknown server response",											// 74053
	"Domain name too long",												// 74054
	"String does not contain any domains",								// 74055
	"Delimiter is empty, cannot explode string",						// 74056
	"DNS: Not a NAPTR record",											// 74057
	"invalid domain name",												// 74058
	"could not open inputfile",											// 74059
	"could not dtermine size of inputfile",								// 74060
	"error while reading inputfile",									// 74061

	NULL
};

static const char * errorsdchk[] = {
	"libxml2 is not installed",										// 74100
	"DCHK: not a valid XML document",								// 74101
	"DCHK: unexpected payload type in DCHK answer",					// 74102
	"DCHK: transaction ID in answer does not match request",		// 74103
	"DCHK: no or incomplete answer",								// 74104
	"DCHK: connect to server failed",								// 74105
	"DCHK: could not escape special chars in domainname",			// 74106
	NULL
};


const char *irislwz_GetError(int code)
/*!\brief Return text of a specific error code
 *
 * \param[in] code Error code
 * \returns pointer to the corresponding text of the error code or NULL, if the error
 * code is unknown.
 *
 * \ingroup DCHK_errors
 */
{
	if (code>=74000 && code <= 74000+(sizeof(errorslwz) / sizeof(char *)-2)) {
		return ((const char*)errorslwz[code-74000]);
	} else if (code>=74100 && code <= 74100+(sizeof(errorsdchk) / sizeof(char *)-2)) {
		return ((const char*)errorsdchk[code-74100]);
	} else if (code>=1000 && code <=1999) {
		return irislwz_GetSocketError(code);
	}
	return NULL;
}

int irislwz_GetErrorCode(IRISLWZ_HANDLE *dchk)
/*!\brief Return last error code
 *
 * Return last error code
 * \param[in] dchk Pointer to an DCHK object
 * \returns last error code
 * \remarks
 * Please keep in mind that a non zero value does not necessarily mean that the
 * previous function was not successful. You should always check the return value
 * of a dchk_*-Function to check if an error occurred. Only when the return value
 * indicates an error, the error code of this function is valid.
 * \ingroup DCHK_errors
 */
{
	if (!dchk) return 74023;
	return dchk->error;
}

const char *irislwz_GetErrorText(IRISLWZ_HANDLE *dchk)
/*!\brief Return text of last error code
 *
 * Return text of last error code
 *
 * \param[in] dchk Pointer to an DCHK object
 * \returns Pointer to the error text.
 *
 * \ingroup DCHK_errors
 */
{
	const char *ret;
	if (!dchk) ret=irislwz_GetError(74023);
	else ret=irislwz_GetError(dchk->error);
	if (!ret) ret="unknown";
	return ret;
}

const char *irislwz_GetAdditionalErrorText(IRISLWZ_HANDLE *dchk)
/*!\brief Return additional information about last error
 *
 * Return additional information about last error.
 *
 * \param[in] dchk Pointer to an DCHK object
 * \returns Pointer to the additional error text, or NULL if there is no text.
 *
 * \ingroup DCHK_errors
 */
{
	if (!dchk) return irislwz_GetError(74023);
	if (dchk->errortext) return (const char*) dchk->errortext;
	return NULL;
}

void irislwz_SetError(IRISLWZ_HANDLE *dchk, int code, const char *fmt, ...)
/*!\brief Set an error on an DCHK object
 *
 * This function is used internally to set an error on an DCHK object.
 *
 * \param[in] dchk Pointer to the DCHK-Object
 * \param[in] code Error code
 * \param[in] fmt Format string for an additional error text. The syntax of this
 * string is identical to sprintf/printf.
 * \param[in] ... Optional additional parameters for the format string
 *
 * \ingroup DCHK_errors
 */
{
	if (!dchk) return;
	if (!dchk->initialized) return;
	dchk->error=code;
	if (dchk->errortext) free(dchk->errortext);
	dchk->errortext=NULL;
	if (fmt==NULL) return;
	va_list args;
	va_start(args, fmt);
	if (vasprintf(&dchk->errortext,(char*)fmt,args)<0) {
		dchk->errortext=NULL;
	}
	va_end(args);
}

void irislwz_PrintError(IRISLWZ_HANDLE *dchk)
/*!\brief Print last error to STDOUT
 *
 * This function prints the last error on the DCHK object to STDOUT (Screen).
 *
 * \param[in] dchk Pointer to the DCHK-Object
 *
 * \ingroup DCHK_errors
 */
{
	if (!dchk) {
		printf("ERROR 74023: %s\n",irislwz_GetError(74023));
	} else {
		printf("ERROR %i: %s",dchk->error, irislwz_GetError(dchk->error));
		if (dchk->errortext) printf (" [%s]",dchk->errortext);
		printf ("\n");
	}
}

