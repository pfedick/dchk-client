#!/usr/bin/perl
# $Id: makedist.pl,v 1.2 2008/11/23 13:43:52 pafe Exp $

#  
# CONTACT     : patrick@pfp.de 
# AUTHOR      : Patrick Fedick <patrick@pfp.de> 
# 
# CREATED     : 2006/01/03 11:30:00 
# REVISION    : $Revision: 1.2 $  
# DATE        : $Date: 2008/11/23 13:43:52 $ 
# 

my $CVSTREE="client";
my $TAG=$ARGV[0];

if (!$TAG) {
	print "Aufruf: makedist.pl CVSTAG\n";
	exit;
}

my $VERSION;

if (($TAG =~ /^SNAP_(.*)$/)) {
	my $DATE=$1;
	$VERSION=$ARGV[1];
	if (!$VERSION) {
		print "Aufruf: makedist.pl SNAP_yyyymmdd VERSION\n";
		print ("Bei einem Snapshot muss als zweiter Parameter die Version angegegen werden.");
		exit;		
	}
	$VERSION.="-SNAP";
	$VERSION.=$DATE;
} elsif ($TAG eq "HEAD") {
	$VERSION="HEAD";
	$TAG="trunk";
} else {
	$TAG =~ /^REL_(.*)$/;
	$VERSION=lc($1);
	$VERSION =~ s/_/\./g;
	$TAG="tags/$TAG";
}

my $DISTNAME="dchk-$VERSION";

print "Packe DCHK mit dem Tag \"$TAG\" aus dem CVS aus...\n";
print "Version:  $VERSION\n";
print "Distname: $DISTNAME\n";

my $PWD=`pwd`;
chomp($PWD);
my $err=`mkdir -p tmp
cd tmp
rm -rf $CVSTREE
svn co http://svn.code.sf.net/p/dchk/client/$TAG $CVSTREE 2>&1`;

if ($? != 0 ) {
	print "ERROR: Version konnte nicht aus dem CVS geholt werden!\n";
	print $err;
	print "\n";
	exit;
}

print "Aktualisiere Dokumentation...\n";
`make docs`;
print " ok\n";
print "Kopiere Doku...\n";
`mkdir -p tmp/$CVSTREE/documentation; cp documentation/*.pdf tmp/$CVSTREE/documentation; cd documentation; find html | cpio -pdmv ../tmp/$CVSTREE/documentation`;
print " done\n";

print "Erstelle Distribution fuer Version: $VERSION\n";
print "PWD=$PWD\n";
print `mkdir -p distfiles
rm -f distfiles/$DISTNAME*
mkdir -p distfiles/$DISTNAME
cd tmp/$CVSTREE
tar -cf $PWD/distfiles/tmp.tar --exclude *.core --exclude CVS --exclude .cvsignore configure Makefile.in Doxyfile *.TXT src  vs2008 include documentation autoconf
cd $PWD
cd distfiles/$DISTNAME
tar -xf ../tmp.tar
cd ..
tar -czf $DISTNAME.tar.gz $DISTNAME
zip -qr $DISTNAME.zip $DISTNAME
rm -rf tmp.tar $DISTNAME
cd $PWD
rm -rf tmp
`;
print "\n";