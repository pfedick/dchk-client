/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.2 $
 * $Date: 2009/03/12 11:56:19 $
 * $Id: example5.cpp,v 1.2 2009/03/12 11:56:19 pfedick Exp $
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
#ifdef _WIN32
	irislwz_InitWSA();
#endif
	// Instantiate CDchk-Class
	dchk::CDchk dchk;

	// Assume all parameters are domain names. Create a CDomainList instance
	dchk::CDomainList list;
	if (argc<2) {
		printf("Enter some domains.");
		return 1;
	}

	// Add the domain names
	for (int i=1;i<=argc;i++) list.AddDomain(argv[i]);

	// Use TLD of first domain to find DCHK server automatically
	const char *tld=strrchr(argv[1],'.');		// Find last occurrance of '.'
	if (tld) tld++;								// Point to first char after '.'
	else {
		printf ("No tld found!\n");
		return 2;
	}

	// Use compression
	dchk.SendDeflated(true);
	dchk.AcceptDeflated(true);

	// Set hostname automatically by using DNS
	if (!dchk.SetHostByDNS(tld)) {
		dchk.PrintError();
		return dchk.GetErrorCode();
	}

	// Check if connection is possible
	if (!dchk.Connect()) {
		dchk.PrintError();
		return dchk.GetErrorCode();
	}
	// bundle 20 domains to one query
	dchk.SetMaxDomainsPerQuery(20);
	if (!dchk.QueryDomainList(&list)) {
		dchk.PrintError();
		return dchk.GetErrorCode();
	}
	// Print answer
	dchk.PrintResults(&list);
	return 0;
}

// end
