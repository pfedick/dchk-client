/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.2 $
 * $Date: 2009/03/12 11:56:19 $
 * $Id: example4.c,v 1.2 2009/03/12 11:56:19 pfedick Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dchk.h>


int main(int argc, char **argv)
{
	int ret;
	// Define IRISLWZ_HANDLE
	IRISLWZ_HANDLE iris;

	// Assume all parameters are domain names
	int i;

	// Get a DomainList object
	DCHK_DOMAINLIST *list=dchk_NewDomainList();

	// Add the domains
	for (i=1;i<=argc;i++) dchk_AddDomain(list,argv[i]);

	// Use TLD of first domain to find DCHK server automatically
	const char *tld=strrchr(argv[1],'.');		// Find last occurance of '.'
	if (tld) tld++;								// Point to first char after '.'
	else {
		// No TLD found, free the DomainList
		dchk_FreeDomainList(list);
		printf ("No tld found!\n");
		return 1;
	}

	// Initialize IRISLWZ_HANDLE
	irislwz_Init(&iris);

	// Use compression
	irislwz_SendDeflated(&iris,1);
	irislwz_AcceptDeflated(&iris,1);

	// Set hostname automatically by using DNS
	dchk_SetHostByDNS(&iris,tld);

	// Check if connection is possible
	if (!irislwz_Connect(&iris)) {
		ret=irislwz_GetErrorCode(&iris);
		irislwz_PrintError(&iris);
		irislwz_Exit(&iris);
		return ret;
	}
	// bundle 20 domains to one query
	if (!dchk_QueryDomainList(&iris,list,20)) {
		ret=irislwz_GetErrorCode(&iris);
		irislwz_PrintError(&iris);
		irislwz_Exit(&iris);
		dchk_FreeDomainList(list);
		return ret;
	}
	// Print answer
	dchk_PrintResults(list);
	irislwz_Exit(&iris);
	dchk_FreeDomainList(list);
	return 0;
}

// end
