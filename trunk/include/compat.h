/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.2 $
 * $Date: 2009/03/12 11:56:19 $
 * $Id: compat.h,v 1.2 2009/03/12 11:56:19 pfedick Exp $
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

#ifndef DCHK_COMPAT_H_
#define DCHK_COMPAT_H_

#ifdef __cplusplus
extern "C" {
#endif

const char *irislwz_GetSocketError(int code);
int irislwz_TranslateSocketError();
int irislwz_TranslateGaiError(int e);


#ifndef HAVE_STRNDUP
char *strndup(const char *str, size_t len);
#endif

#ifndef HAVE_BZERO
void bzero(void *s, size_t n);
#endif

#ifndef HAVE_VASPRINTF
int vasprintf(char **buff, const char *fmt, va_list args);
#endif

#ifdef __cplusplus
}
#endif

#endif /*COMPAT_H_*/
