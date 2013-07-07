/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.3 $
 * $Date: 2009/07/14 10:11:31 $
 * $Id: ns_parse.c,v 1.3 2009/07/14 10:11:31 pfedick Exp $
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

/*
 * Copyright (c) 2004 by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1996,1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#ifndef _WIN32

#ifdef _MSC_VER
#include "msvc-config.h"
#else
#include "config.h"
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_NETINET_IN_H
	#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_NAMESER_H
#include <arpa/nameser.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
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

#ifdef HAVE_XML2
	#define LIBXML_STATIC
	#define LIBXSLT_STATIC
	#define XMLSEC_STATIC
	#include <libxml/parser.h>
	#include <libxml/tree.h>
	#include <libxml/xpath.h>
	#include <libxml/xpathInternals.h>
#endif
#include "dchk.h"
#include "compat.h"
#include "ns_parse.h"

/* Macros. */

#if !defined(SOLARIS2) || defined(__COVERITY__)
#define RETERR(err) do { errno = (err); return (-1); } while (0)
#else
#define RETERR(err) \
	do { errno = (err); if (errno == errno) return (-1); } while (0)
#endif

/* Public. */


#ifndef HAVE_NS_MSG_GETFLAG
/* These need to be in the same order as the nres.h:ns_flag enum. */
struct _ns_flagdata _ns_flagdata[16] = {
	{ 0x8000, 15 },		/*%< qr. */
	{ 0x7800, 11 },		/*%< opcode. */
	{ 0x0400, 10 },		/*%< aa. */
	{ 0x0200, 9 },		/*%< tc. */
	{ 0x0100, 8 },		/*%< rd. */
	{ 0x0080, 7 },		/*%< ra. */
	{ 0x0040, 6 },		/*%< z. */
	{ 0x0020, 5 },		/*%< ad. */
	{ 0x0010, 4 },		/*%< cd. */
	{ 0x000f, 0 },		/*%< rcode. */
	{ 0x0000, 0 },		/*%< expansion (1/6). */
	{ 0x0000, 0 },		/*%< expansion (2/6). */
	{ 0x0000, 0 },		/*%< expansion (3/6). */
	{ 0x0000, 0 },		/*%< expansion (4/6). */
	{ 0x0000, 0 },		/*%< expansion (5/6). */
	{ 0x0000, 0 },		/*%< expansion (6/6). */
};
#endif

int dchk_bind_ns_msg_getflag(ns_msg handle, int flag) {
#ifdef HAVE_NS_MSG_GETFLAG
	return ns_msg_getflag(handle,flag);
#else
	return(((handle)._flags & _ns_flagdata[flag].mask) >> _ns_flagdata[flag].shift);
#endif
}

int dchk_bind_ns_skiprr(const u_char *ptr, const u_char *eom, ns_sect section, int count) {
#ifdef HAVE_NS_SKIPRR
	return ns_skiprr(ptr,eom,section,count);
#else
	const u_char *optr = ptr;

	for ((void)NULL; count > 0; count--) {
		int b, rdlength;

		b = dn_skipname(ptr, eom);
		if (b < 0)
			RETERR(EMSGSIZE);
		ptr += b/*Name*/ + NS_INT16SZ/*Type*/ + NS_INT16SZ/*Class*/;
		if (section != ns_s_qd) {
			if (ptr + NS_INT32SZ + NS_INT16SZ > eom)
				RETERR(EMSGSIZE);
			ptr += NS_INT32SZ/*TTL*/;
			NS_GET16(rdlength, ptr);
			ptr += rdlength/*RData*/;
		}
	}
	if (ptr > eom)
		RETERR(EMSGSIZE);
	return (ptr - optr);
#endif
}


#if (! defined HAVE_NS_INITPARSE ) || (! defined HAVE_NS_PARSERR)
static void dchk_setsection(dchk_ns_msg *msg, ns_sect sect) {
	msg->_sect = sect;
	if (sect == ns_s_max) {
		msg->_rrnum = -1;
		msg->_msg_ptr = NULL;
	} else {
		msg->_rrnum = 0;
		msg->_msg_ptr = msg->_sections[(int)sect];
	}
}

#endif

int dchk_bind_ns_initparse(const u_char *msg, int msglen, dchk_ns_msg *handle) {
#ifdef HAVE_NS_INITPARSE
	return ns_initparse(msg,msglen,(ns_msg*)handle);
#else
	const u_char *eom = msg + msglen;
	int i;

	memset(handle, 0x5e, sizeof *handle);
	handle->_msg = msg;
	handle->_eom = eom;
	if (msg + NS_INT16SZ > eom)
		RETERR(EMSGSIZE);
	NS_GET16(handle->_id, msg);
	if (msg + NS_INT16SZ > eom)
		RETERR(EMSGSIZE);
	NS_GET16(handle->_flags, msg);
	for (i = 0; i < ns_s_max; i++) {
		if (msg + NS_INT16SZ > eom)
			RETERR(EMSGSIZE);
		NS_GET16(handle->_counts[i], msg);
	}
	for (i = 0; i < ns_s_max; i++)
		if (handle->_counts[i] == 0)
			handle->_sections[i] = NULL;
		else {
			int b = dchk_bind_ns_skiprr(msg, eom, (ns_sect)i,
					  handle->_counts[i]);

			if (b < 0)
				return (-1);
			handle->_sections[i] = msg;
			msg += b;
		}
	if (msg != eom)
		RETERR(EMSGSIZE);
	dchk_setsection(handle, ns_s_max);
	return (0);
#endif
}

int dchk_bind_ns_parserr(dchk_ns_msg *handle, ns_sect section, int rrnum, ns_rr *rr) {
#ifdef HAVE_NS_PARSERR
	return ns_parserr((ns_msg*)handle,section,rrnum,rr);
#else
	int b;
	int tmp;

	/* Make section right. */
	tmp = section;
	if (tmp < 0 || section >= ns_s_max)
		RETERR(ENODEV);
	if (section != handle->_sect)
		dchk_setsection(handle, section);

	/* Make rrnum right. */
	if (rrnum == -1)
		rrnum = handle->_rrnum;
	if (rrnum < 0 || rrnum >= handle->_counts[(int)section])
		RETERR(ENODEV);
	if (rrnum < handle->_rrnum)
		dchk_setsection(handle, section);
	if (rrnum > handle->_rrnum) {
		b = dchk_bind_ns_skiprr(handle->_msg_ptr, handle->_eom, section,
			      rrnum - handle->_rrnum);

		if (b < 0)
			return (-1);
		handle->_msg_ptr += b;
		handle->_rrnum = rrnum;
	}

	/* Do the parse. */
	b = dn_expand(handle->_msg, handle->_eom,
		      handle->_msg_ptr, rr->name, NS_MAXDNAME);
	if (b < 0)
		return (-1);
	handle->_msg_ptr += b;
	if (handle->_msg_ptr + NS_INT16SZ + NS_INT16SZ > handle->_eom)
		RETERR(EMSGSIZE);
	NS_GET16(rr->type, handle->_msg_ptr);
	NS_GET16(rr->rr_class, handle->_msg_ptr);
	if (section == ns_s_qd) {
		rr->ttl = 0;
		rr->rdlength = 0;
		rr->rdata = NULL;
	} else {
		if (handle->_msg_ptr + NS_INT32SZ + NS_INT16SZ > handle->_eom)
			RETERR(EMSGSIZE);
		NS_GET32(rr->ttl, handle->_msg_ptr);
		NS_GET16(rr->rdlength, handle->_msg_ptr);
		if (handle->_msg_ptr + rr->rdlength > handle->_eom)
			RETERR(EMSGSIZE);
		rr->rdata = handle->_msg_ptr;
		handle->_msg_ptr += rr->rdlength;
	}
	if (++handle->_rrnum > handle->_counts[(int)section])
		dchk_setsection(handle, (ns_sect)((int)section + 1));

	/* All done. */
	return (0);
#endif
}


#endif

