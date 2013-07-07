/* include/config.h.  Generated from config.h.in by configure.  */
/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.3 $
 * $Date: 2009/07/14 10:11:30 $
 * $Id: config.h.in,v 1.3 2009/07/14 10:11:30 pfedick Exp $
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
 
#ifndef _DCHK_CONFIG
#define _DCHK_CONFIG

#define PACKAGE_VERSION "0.5.6"
#define PACKAGE_BUGREPORT "dchk@pfp.de"
#define PACKAGE_NAME "dchk"
#define PACKAGE_STRING "dchk 0.5.6"


/*
 * Endianess
 */
/* #undef __BIG_ENDIAN__ */
#define __LITTLE_ENDIAN__ 1



/* Define as 1 if you have unistd.h. */
#define HAVE_UNISTD_H 1

/* #undef size_t */

#define HAVE_INTTYPES_H 1
#define HAVE_MEMORY_H 1

#define HAVE_STDARG_H 1
#define HAVE_STDBOOL_H 1
#define HAVE_STDIO_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 1
#define HAVE_ERRNO_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_POLL_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_LIMITS_H 1
#define HAVE_SIGNAL_H 1
#define HAVE_RESOLV_H 1
#define HAVE_ARPA_NAMESER_H 1
#define HAVE_ARPA_INET_H 1
#define HAVE_NETDB_H 1
#define HAVE_LOCALE_H 1

#define HAVE_STRCASECMP 1
#define HAVE_STRDUP 1
#define HAVE_STRNDUP 1
#define HAVE_STRERROR 1
#define HAVE_STRNCASECMP 1
#define HAVE__BOOL 1
#define HAVE_MALLOC 1
#define HAVE_MEMMOVE 1
#define HAVE_BZERO 1
#define HAVE_GETPID 1
#define HAVE_GETPPID 1
#define HAVE_GETUID 1
#define HAVE_GETEUID 1
#define HAVE_MEMSET 1
#define HAVE_REALLOC 1
#define HAVE_SETENV 1
#define HAVE_PUTENV 1
#define HAVE_GETENV 1
#define HAVE_SIGNAL 1
#define HAVE_SETLOCALE 1
#define HAVE_VASPRINTF 1
#define HAVE_WORKING_VSNPRINTF 1
/* #undef HAVE_NS_MSG_GETFLAG */
/* #undef HAVE_NS_SKIPRR */
/* #undef HAVE_NS_INITPARSE */
/* #undef HAVE_NS_PARSERR */


/*
 * Compression
 */

#define HAVE_LIBZ 1
#define HAVE_XML2 1


#endif
