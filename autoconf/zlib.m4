dnl AM_ZLIB([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
AC_DEFUN([AM_ZLIB],[dnl
AC_MSG_CHECKING([for compatible zlib library and headers])
AC_ARG_WITH([zlib],
	[  --with-zlib[[=PATH]]     Prefix where zlib is installed (optional)],
	[zlib_prefix="$withval"],
	[zlib_prefix="no"])

am_save_CPPFLAGS="$CPPFLAGS"
am_save_LIBS="$LIBS"
am_save_LDFLAGS="$LDFLAGS"
ZLIB_LIBS="-lz"
ZLIB_CFLAGS=""
report_have_zlib="no"
	if test "$zlib_prefix" != "yes"
	then
		if test "$zlib_prefix" != "no"
		then
			LIBS="-L$zlib_prefix/lib -lz"
			CPPFLAGS="-I$zlib_prefix/include"
			ZLIB_LIBS="-L$zlib_prefix/lib -lz"
			ZLIB_CFLAGS="-I$zlib_prefix/include"
		else
			LIBS="$LIBS -lz"
			ZLIB_LIBS="-lz"
		fi
	else
		LIBS="$LIBS -lz"
		ZLIB_LIBS="-lz"
	fi
    AC_LINK_IFELSE(
      [[
         #include <zlib.h>
         int main()
         {
            char* source = "Hello World";
            char dest[1000];
            uLongf destLen = 1000;
            uLongf sourceLen = 11;
            int ret;
            ret=compress2((Bytef*)dest, (uLongf*)&destLen,(const Bytef*)source,sourceLen,5);
         }
      ]],
      [AC_MSG_RESULT(yes)
      AC_DEFINE(HAVE_LIBZ, 1, [ Define if you have zlib. ])
      AC_DEFINE(HAVE_ZLIB, 1, [ Define if you have zlib. ])
      AC_SUBST(ZLIB_CFLAGS)
	  AC_SUBST(ZLIB_LIBS)
      report_have_zlib="yes"
    
      ],
      [
         AC_MSG_RESULT(no)
         AC_MSG_ERROR([compatible zlib library (libz) and/or headers not found--see http://www.gzip.org/zlib/])
      ]
    )

CPPFLAGS=$am_save_CPPFLAGS
LIBS=$am_save_LIBS
LDFLAGS=$am_save_LDFLAGS


])
