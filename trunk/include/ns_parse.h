/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.2 $
 * $Date: 2009/03/12 11:56:19 $
 * $Id: ns_parse.h,v 1.2 2009/03/12 11:56:19 pfedick Exp $
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

#ifdef __cplusplus
extern "C" {
#endif


#define dchk_bind_ns_msg_count(handle, section) ((handle)._counts[section] + 0)

/*%
 * This is a message handle.  It is caller allocated and has no dynamic data.
 * This structure is intended to be opaque to all but ns_parse.c, thus the
 * leading _'s on the member names.  Use the accessor functions, not the _'s.
 */
typedef struct __dchk_ns_msg {
    const u_char    *_msg, *_eom;
    unsigned short   _id, _flags, _counts[ns_s_max];
    const u_char    *_sections[ns_s_max];
    ns_sect     _sect;
    int     _rrnum;
    const u_char    *_msg_ptr;
} dchk_ns_msg;


int dchk_bind_ns_initparse(unsigned const char *msg, int msglen, dchk_ns_msg *handle);
int dchk_bind_ns_parserr(dchk_ns_msg *handle, ns_sect section, int rrnum, ns_rr *rr);


#ifdef __cplusplus
}
#endif
