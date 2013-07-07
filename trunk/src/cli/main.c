/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.11 $
 * $Date: 2011/01/26 12:15:54 $
 * $Id: main.c,v 1.11 2011/01/26 12:15:54 pfedick Exp $
 *
 *
 * Copyright (C) 2008 Patrick Fedick <dchk@pfp.de>
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

#ifdef HAVE_STDIO_H
	#include <stdio.h>
#endif

#ifdef HAVE_STRING_H
	#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

#ifdef HAVE_SYS_TYPES_H
     #include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
     #include <sys/stat.h>
#endif

#ifdef HAVE_ERRNO_H
	#include <errno.h>
#endif

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#include "dchk.h"
#ifdef HAVE_XML2
#ifndef LIBXML_STATIC
	#define LIBXML_STATIC
	#define LIBXSLT_STATIC
	#define XMLSEC_STATIC
#endif
	#include <libxml/parser.h>
	#include <libxml/tree.h>
	#include <libxml/xpath.h>
	#include <libxml/xpathInternals.h>
#endif

static const char * tlds[] = {
"AC","AD","AE","AERO","AF","AG","AI","AL","AM","AN","AO","AQ","AR","ARPA","AS","ASIA","AT","AU",
"AW","AX","AZ","BA","BB","BD","BE","BF","BG","BH","BI","BIZ","BJ","BL","BM","BN","BO","BR","BS",
"BT","BV","BW","BY","BZ","CA","CAT","CC","CD","CF","CG","CH","CI","CK","CL","CM","CN","CO","COM",
"COOP","CR","CU","CV","CX","CY","CZ","DE","DJ","DK","DM","DO","DZ","EC","EDU","EE","EG","EH","ER",
"ES","ET","EU","FI","FJ","FK","FM","FO","FR","GA","GB","GD","GE","GF","GG","GH","GI","GL","GM",
"GN","GOV","GP","GQ","GR","GS","GT","GU","GW","GY","HK","HM","HN","HR","HT","HU","ID","IE","IL",
"IM","IN","INFO","INT","IO","IQ","IR","IS","IT","JE","JM","JO","JOBS","JP","KE","KG","KH","KI",
"KM","KN","KP","KR","KW","KY","KZ","LA","LB","LC","LI","LK","LR","LS","LT","LU","LV","LY","MA",
"MC","MD","ME","MF","MG","MH","MIL","MK","ML","MM","MN","MO","MOBI","MP","MQ","MR","MS","MT","MU",
"MUSEUM","MV","MW","MX","MY","MZ","NA","NAME","NC","NE","NET","NF","NG","NI","NL","NO","NP","NR",
"NU","NZ","OM","ORG","PA","PE","PF","PG","PH","PK","PL","PM","PN","PR","PRO","PS","PT","PW","PY",
"QA","RE","RO","RS","RU","RW","SA","SB","SC","SD","SE","SG","SH","SI","SJ","SK","SL","SM","SN",
"SO","SR","ST","SU","SV","SY","SZ","TC","TD","TEL","TF","TG","TH","TJ","TK","TL","TM","TN","TO",
"TP","TR","TRAVEL","TT","TV","TW","TZ","UA","UG","UK","UM","US","UY","UZ","VA","VC","VE","VG",
"VI","VN","VU","WF","WS","YE","YT","YU","ZA","ZM","ZW",
NULL
};

int Execute(int argc, char **argv, IRISLWZ_HANDLE *dchk);

void help()
{
	int i;
	char *title=(char*)malloc(256);
	sprintf(title,"DomainCheck (dchk) Version %s", DCHK_VERSION);
	printf("%s\n",title);
	for (i=0;title[i]!=0;i++) {
		printf("=");
	}
	free (title);
	printf ("\n");
	printf (
		"Copyright 2008-2011 by Patrick Fedick <http://dchk.sourceforge.net/>\n"
		"Release date: %s\n\n"
		"Syntax: dchk [-hpastv] Domain Domain Domain ...\n"
		"\n"
		"     -h HOST      Hostname or IP of DCHK-Server (default=DNS lookup)\n"
		"     -p PORT      Port of DCHK-Server (Default=DNS lookup or port 715)\n"
		"     -a AUTHORITY Authority (Default=TLD)\n"
		"     -t TLD       TLD to query, only used when no hostname is given (default=de)\n"
		"     -v           Verbose: Request and Answer will be printed as XML\n"
		"     -V           same as -v plus request and answer header will be explained\n"
		"     -f FILE      Read complete XML request from FILE and write response to\n"
		"                  STDOUT\n"
		"     --ex         When using verbose mode (-v), XML-output will be enhanced\n"
		"                  for better reading\n"
		"     --ps #       Set maximum packet size (default is 1500 bytes)\n"
		"     --md #       Set maximum number of domains per DCHK query (default=20)\n"
		"     --dac        Dont't accept compressed answers (default=accept)\n"
		"     --sc         Send compressed requests (default=uncompressed)\n"
		"     --timeout #  Timeout for DCHK requests in seconds (default=5, 0=none)\n"
		"     Domain       Domain names which should be checked. If you use the\n"
		"                  word \"version\" instead of a domain name, a version\n"
		"                  query will be send to the server. You must use the -t or\n"
		"                  the -h and -p flags in conjunction with this\n"
		"     --fs         Check all TLDs for DCHK server\n"
		"     --rfc1950    Use RFC 1950 for compression instead of RFC 1951\n"

		"", DCHK_RELEASEDATE);
}

const char * getargv (int argc, char * argv[], const char * argument)
{
	int i;
	size_t argl,l;
	char *ret;
	if (argc>1) {
		argl=strlen(argument);
		for (i=1;i<argc;i++) {
			if (strncmp(argv[i],argument,argl) == 0) {
				l=strlen(argv[i]);
				if (l>argl || argv[i+1]==NULL) {
					ret=(argv[i]+argl);
					//if (ret[0]=='-') return (char*)"";
					//if (ret[0]=='\\' && ret[1]=='-') return ret+1;
					return ret;
				} else {
					ret=(argv[i+1]);
					if (ret[0]=='-') return (char*)"";
					if (ret[0]=='\\' && ret[1]=='-') return ret+1;
					return ret;
				}
			}
		}
	}
	return (NULL);
}

static int myatoi(const char *str)
{
	if (!str) return 0;
	return atoi(str);
}

int mystrncasecmp(const char *s1, const char *s2, size_t n)
{
#ifdef HAVE_STRNCASECMP
	return strncasecmp(s1,s2,n);
#else
    if (n != 0) {
        const unsigned char
                *us1 = (const unsigned char *)s1,
                *us2 = (const unsigned char *)s2;

        do {
            if (tolower(*us1) != tolower(*us2++))
                return (tolower(*us1) - tolower(*--us2));
            if (*us1++ == '\0')
                break;
        } while (--n != 0);
    }
    return (0);
#endif
}


int main(int argc, char **argv)
{
	/*
	char buffer[1024];
	if (!dchk_EscapeSpecialChars("<Ha&\"llo>'", buffer,1024)) {
		printf ("Error\n");
	} else {
		printf ("Result: %s\n",buffer);
	}
	return 0;
	 */

	int ret=0;
	IRISLWZ_HANDLE handle;

	// check if user requests help
	if (getargv(argc,argv,"-?")!=NULL || getargv(argc,argv,"--help")!=NULL || argc==1) {
		help();
		return 0;
	}

#ifdef HAVE_XML2
	/*
	 * this initialize the library and check potential ABI mismatches
	 * between the version it was compiled for and the actual shared
	 * library used.
	 */
	LIBXML_TEST_VERSION
#endif
#ifdef HAVE_SETLOCALE
	setlocale(LC_ALL,  "" );
#endif
#ifdef _WIN32
	irislwz_InitWSA();
#define strcasecmp _stricmp
#endif

	if (!irislwz_Init(&handle)) {
		printf ("Error: could not initialize DCHK client\n");
		return 74028;
	}
	xmlInitParser();
	/*
	const char *bfff[100];
	int lfff=100;
	xmlGetFeaturesList(&lfff,&bfff);
	int i;
	for (i=0;i<lfff;i++) {
		printf ("%s",bfff[i]);
	}
	*/

	if (!Execute(argc,argv,&handle)) {
		ret=irislwz_GetErrorCode(&handle);
		irislwz_PrintError(&handle);
	}
	xmlCleanupParser();
	irislwz_Exit(&handle);
	return ret;
}

void FindServer(IRISLWZ_HANDLE *handle)
{
	printf("Try to find DCHK-server...\n");
	int i=0;
	int found=0;
	while (tlds[i]!=NULL) {
		printf ("checking \".%s\"... ",tlds[i]);
		if (dchk_SetHostByDNS(handle,tlds[i])) {
			printf ("Server found: %s:%i\n",irislwz_GetHostname(handle),irislwz_GetPort(handle));
			found ++;
		} else {
			printf ("failed\n");
		}
		i++;
	}
	printf ("\n%i DCHK server found\n",found);

}

int Execute(int argc, char **argv, IRISLWZ_HANDLE *handle)
{
	int ret;
	// check for parameters
	const char *tld=getargv(argc, argv, "-t");
	const char *host=getargv(argc, argv, "-h");
	const char *port=getargv(argc, argv, "-p");
	const char *inputfile=getargv(argc, argv, "-f");
	if (!port) port="715";
	const char *timeout=getargv(argc, argv, "--timeout");
	if (timeout) irislwz_SetTimeout(handle,myatoi(timeout),0);
	else irislwz_SetTimeout(handle,5,0);

	if (getargv(argc, argv, "--ex")) {
		if (getargv(argc, argv, "-V")) handle->verboseflag=54;
		else if (getargv(argc, argv, "-v")) handle->verboseflag=18;
	} else {
		if (getargv(argc, argv, "-V")) handle->verboseflag=45;
		else if (getargv(argc, argv, "-v")) handle->verboseflag=9;
	}


	int packetsize=myatoi(getargv(argc, argv, "--ps"));
	if (packetsize) irislwz_SetMaximumResponseLength(handle,packetsize);
	else irislwz_SetMaximumResponseLength(handle,1500);

	int maxdomains=myatoi(getargv(argc, argv, "--md"));

	if (getargv(argc, argv, "--rfc1950")) irislwz_UseRFC1950(handle,1);

	if (getargv(argc, argv, "--sc")) irislwz_SendDeflated(handle,1);
	else irislwz_SendDeflated(handle,0);
	if (getargv(argc, argv, "--dac")) irislwz_AcceptDeflated(handle,0);
	else irislwz_AcceptDeflated(handle,1);


	const char *authority=getargv(argc, argv, "-a");

	// Search-Request for Server?
	if (getargv(argc,argv,"--fs")) {
		FindServer(handle);
		return 1;
	}
	// find first parameter, which is not an option
	int first=0;
	int i;
	DCHK_DOMAINLIST *query=NULL;

	if (!inputfile) {
		for (i=1; i<argc; i++) {
			if (argv[i][0]=='-') {
				if (mystrncasecmp(argv[i], "-a",2)==0
						||mystrncasecmp(argv[i], "-i",2)==0
						||mystrncasecmp(argv[i], "-t",2)==0
						||mystrncasecmp(argv[i], "-h",2)==0
						||mystrncasecmp(argv[i], "-f",2)==0
						||mystrncasecmp(argv[i], "-p",2)==0) {
					if (strlen(argv[i])==2)	i++;
				} else if (mystrncasecmp(argv[i], "--ps",4)==0
						|| mystrncasecmp(argv[i], "--md",4)==0) {
					if (strlen(argv[i])==4)	i++;
				} else if (mystrncasecmp(argv[i], "--timeout",9)==0) {
					if (strlen(argv[i])==9)	i++;
				}
			}
			else {
				first=i;
				break;
			}
		}
		if (!first) {	// it seems we have no parameters
			help();
			return 0;
		}

		// read all domain parameters into DCHK_DOMAINLIST
		query=dchk_NewDomainList();
		if (!query) return 0;

		for (i=first;i<argc;i++) {
			if (!dchk_AddDomain(query,argv[i])) {
				dchk_FreeDomainList(query);
				irislwz_SetError(handle,74029,argv[i]);
				return 0;

			}
		}

		// check if we have something to do
		if (query->num==0) {
			irislwz_SetError(handle,74011,NULL);
			irislwz_PrintError(handle);
			help();
			dchk_FreeDomainList(query);
			return 0;
		}


		// If no TLD is given, use TLD of first domain in list
		if ((!tld) && query->num>0) {
			tld=strrchr(query->first->domain,'.');		// Find last occurrance of '.'
			if (tld) tld++;								// Point to first char after '.'

		}
	}
	if (!tld) tld="de";								// or fallback to TLD 'de'

	// If no hostname is given, we try to find one in the DNS zone of the TLD
	if (!host) {
		if (!dchk_SetHostByDNS(handle,tld)) {
			dchk_FreeDomainList(query);
			return 0;
		}
	} else {
		irislwz_SetHost(handle,host,myatoi(port));
	}

	// If no authority is given, use the tld of the first domain in list
	if (authority) irislwz_SetAuthority(handle,authority);
	else if (inputfile) irislwz_SetAuthority(handle,tld);
	else {
		if (!irislwz_SetAuthorityByDomainList(handle,query)) {
			dchk_FreeDomainList(query);
			return 0;
		}
	}


	//printf ("Using DCHK server: %s:%i\n",dchk_GetHostname(dchk), dchk_GetPort(dchk));
	if (!irislwz_Connect(handle)) {
		dchk_FreeDomainList(query);
		return 0;
	}

	// Inputfile?
	if (inputfile) {
		FILE *ff;
		ff=fopen(inputfile,"rb");
		if (!ff) {
			irislwz_SetError(handle,74059,"%s: %s",inputfile,strerror(errno));
			return 0;
		}
		struct stat buf;
		if ((fstat (fileno((FILE*)ff),&buf))!=0) {
			irislwz_SetError(handle,74060,"%s: %s",inputfile,strerror(errno));
			fclose(ff);
			return 0;
		}
		char *payload=(char*)malloc(buf.st_size+1);
		if (!payload) {
			irislwz_SetError(handle,74024,"reading file: %s",inputfile);
			fclose(ff);
			return 0;
		}
		if (1!=fread(payload,buf.st_size,1,ff)) {
			irislwz_SetError(handle,74061,"%s: %s",inputfile,strerror(errno));
			free(payload);
			fclose(ff);
			return 0;
		}
		payload[buf.st_size]=0;
		handle->dontValidateInput=1;
		irislwz_IgnoreInvalidXML(handle,1);
		ret=irislwz_Query2(handle,payload,buf.st_size);
		if (!ret) return 0;
		if (!irislwz_CheckResponse(handle)) return 0;
		if (handle->response.uncompressed_payload) printf ("%s\n",handle->response.uncompressed_payload);
		free(payload);

	} else if (strcasecmp(query->first->domain,"version")==0) { // Version request?
		if (!irislwz_QueryVersion(handle)) {
			dchk_FreeDomainList(query);
			return 0;
		}
		printf("%s",handle->response.beautified);
	} else {
		if (!dchk_QueryDomainList(handle,query,maxdomains)) {
			dchk_FreeDomainList(query);
			return 0;
		}
		if (handle->verboseflag&63) printf("[Result]\n");
		dchk_PrintResults(query);
	}
	if (query) dchk_FreeDomainList(query);
	irislwz_Disconnect(handle);
	return 1;
}

