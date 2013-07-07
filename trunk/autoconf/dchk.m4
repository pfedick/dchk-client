#
# This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
# (http://dchk.sourceforge.net/). It can be used to check the availability
# of domains by using the DCHK protocol described in RFC 5144.
#
# $Author: pfedick $
# $Revision: 1.3 $
# $Date: 2009/08/07 08:10:23 $
# $Id: dchk.m4,v 1.3 2009/08/07 08:10:23 pfedick Exp $
#
#
# Copyright (C) 2008 Patrick Fedick <dchk@pfp.de>
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# SoftwareFoundation; either version 3 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
#

dnl AM_DCHK([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
AC_DEFUN([AM_DCHK],[
AC_ARG_WITH([dchk],
	[  --with-dchk[[=PATH]]     Prefix where dchk is installed (optional)],
	[dchk_prefix="$withval"],
	[dchk_prefix="no"])

am_save_CPPFLAGS="$CPPFLAGS"
am_save_LIBS="$LIBS"
am_save_LDFLAGS="$LDFLAGS"
DCHK_LIBS="-ldchk"
DCHK_CFLAGS=""
have_dchk="no"
	if test "$dchk_prefix" != "yes"
	then
		if test "$dchk_prefix" != "no"
		then
			LIBS="-L$dchk_prefix/lib -ldchk $LIBS"
			CPPFLAGS="$CPPFLAGS -I$dchk_prefix/include"
			DCHK_LIBS="-L$dchk_prefix/lib -ldchk"
			DCHK_LIBS_DEBUG="-L$dchk_prefix/lib -ldchk-debug"
			DCHK_CFLAGS="-I$dchk_prefix/include"
		else
			LIBS="-ldchk $LIBS"
			DCHK_LIBS="-ldchk"
			DCHK_LIBS_DEBUG="-ldchk-debug"
		fi
	else
		LIBS="-ldchk $LIBS"
		DCHK_LIBS="-ldchk"
		DCHK_LIBS_DEBUG="-ldchk-debug"
	fi
	AC_CHECK_HEADER([dchk.h], [
	
		AC_MSG_CHECKING([for dchk library and linking])
    	AC_LINK_IFELSE(
      	[[
        	 #include <dchk.h>
         	int main()
         	{
         		IRISLWZ_HANDLE header;
         		if (!irislwz_Init(&header)) {
         			return 1;
         		}
         		irislwz_Exit(&header);
         		return 0;
         	}
      	]],
      	[AC_MSG_RESULT(yes)
      	have_dchk="yes"
        ],
    	[
         AC_MSG_RESULT(no)
      	]
    	)
    ])
CPPFLAGS=$am_save_CPPFLAGS
LIBS=$am_save_LIBS
LDFLAGS=$am_save_LDFLAGS

if test "$have_dchk" = "yes"
then
	ifelse([$2], , :, [$2])
else
	ifelse([$3], , :, [$3])
fi


])
