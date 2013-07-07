/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.2 $
 * $Date: 2009/03/12 11:56:19 $
 * $Id: compat.c,v 1.2 2009/03/12 11:56:19 pfedick Exp $
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
#ifdef HAVE_STRING_H
	#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

#ifdef HAVE_STDIO_H
	#include <stdio.h>
#endif

#ifdef HAVE_STDARG_H
	#include <stdarg.h>
#endif


#ifdef HAVE_ERRNO_H
	#include <errno.h>
#endif


#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif


#include "dchk.h"
#include "compat.h"

int irislwz_InitWSA()
/*!\brief Windows only: initialize Windows Sockets
 *
 * Before you can use any Socket functions on Microsoft Windows it is necessary to
 * initialize the windows socket API first (see \ref DCHK_usage_win32). This is done by calling the WSAStartup-function.
 * This function will do this for you.
 *
 * \returns On non-windows platforms this function always returns 1, on windows systems it
 * will return 1 when the winsock API was initialized successfully, otherwise it will return
 * 0.
 *
 * \remarks
 * This function is Microsoft Windows specific.
 *
 * \ingroup DCHK_API_IRISLWZ
 */
{
#ifdef _WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );	// desired winsock version 2.2
	err = WSAStartup( wVersionRequested, &wsaData );
	if (err==0)	return 1;
	return 0;
#else
	return 1;
#endif
}

#ifndef HAVE_BZERO
void bzero(void *s, size_t n)
{
	memset(s,0,n);
}
#endif

#ifndef HAVE_STRNDUP
char *strndup(const char *str, size_t len)
{
	if (len<0) return NULL;
	if (!str) return NULL;
	size_t l=strlen(str);
	if (l<len) len=l;
	char *buff=(char*)malloc(len+1);
	if (!buff) return NULL;
	strncpy(buff,str,len);
	buff[len]=0;
	return buff;
}
#endif


#ifndef HAVE_VASPRINTF
int vasprintf(char **buff, const char *fmt, va_list args)
{
	#if defined HAVE_WORKING_VSNPRINTF
		char tb[4];
		int size=vsnprintf(tb,1,fmt,args);
		char *b=(char*)malloc(size+2);
		if (b) {
			vsnprintf(b,size+1,fmt,args);
			*buff=b;
			return size;
		} else {
			*buff=NULL;
			return 0;
		}
	#elif defined _WIN32
		// check how long the string would get
		int size=_vscprintf(fmt,args);
		// allocate buffer
		char *b=(char*)malloc(size+2);
		if (b) {
			_vsnprintf(b,size+1,fmt,args);
			*buff=b;
			return size;
		} else {
			*buff=NULL;
			return 0;
		}

	#else
		#pragma error No working vasprintf!!!
		*buff=NULL;
		return 0;
    #endif
}
#endif

/*
int irislwz_TranslateSocketError()
{
#ifdef _WIN32
	int e=WSAGetLastError();

#else
	int e=errno;
	//printf ("errno=%i, %s\n",e,strerror(e));
	switch(e) {
		case ETIMEDOUT: return 74035;
		case EINTR:		return 74035;
		case EAGAIN: 	return 74035;
	}

#endif
	return 74034;
}
*/

void irislwz_HexDump(void *address, int bytes)
{
    char buff[1024], tmp[10], cleartext[20];
    char *_adresse=(char*)address;
    int spalte=0;
    int i=0;
    sprintf (buff,"0x%08X: ",i);
    bzero(cleartext,20);
    for (i=0;i<bytes;i++) {
        if (spalte>15) {
            strcat(buff,"                                                               ");
            buff[60]=0;
            strcat (buff,": ");
            strcat (buff,cleartext);
            printf("%s\n",buff);
            sprintf (buff,"0x%08X: ",i);
            bzero(cleartext,20);
            spalte=0;
        }
        sprintf (tmp,"%02X ",(unsigned char)(_adresse[i]));
        strcat (buff,tmp);
        if ((unsigned char)_adresse[i]>31 && (unsigned char)_adresse[i]<128)  cleartext[spalte]=(unsigned char)_adresse[i];
        else cleartext[spalte]='.';
        spalte++;
    }

    if (spalte>0) {
        strcat(buff,"                                                               ");
        buff[60]=0;
        strcat (buff,": ");
        strcat (buff,cleartext);
        printf("%s\n",buff);
    }
}




