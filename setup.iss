;/*
; * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
; * (http://dchk.sourceforge.net/). It can be used to check the availability
; * of domains by using the DCHK protocol described in RFC 5144.
; *
; * $Author: pfedick $
; * $Revision: 1.13 $
; * $Date: 2011/01/26 12:13:21 $
; * $Id: setup.iss,v 1.13 2011/01/26 12:13:21 pfedick Exp $
; *
; *
; * Copyright (C) 2008/2009 Patrick Fedick <dchk@pfp.de>
; *
; * This program is free software; you can redistribute it and/or modify it under
; * the terms of the GNU Lesser General Public License as published by the Free
; * SoftwareFoundation; either version 3 of the License, or (at your option) any
; * later version.
; *
; * This program is distributed in the hope that it will be useful, but WITHOUT
; * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
; * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
; * more details.
; *
; * You should have received a copy of the GNU Lesser General Public License
; * along with this program; if not, see <http://www.gnu.org/licenses/>.
; *
; */


[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{33333172-1F29-4235-82AD-3E07A324BB87}
AppName=DomainCheck
AppVerName=DomainCheck Version 0.5.8
AppCopyright=Copyright (C) 2008-2011 Patrick Fedick
AppVersion=0.5.8
AppPublisher=Patrick Fedick
AppPublisherURL=http://dchk.sourceforge.net/
AppSupportURL=http://dchk.sourceforge.net/
AppUpdatesURL=http://dchk.sourceforge.net/
DefaultDirName={pf}\DomainCheck
DefaultGroupName=DomainCheck
AllowNoIcons=yes
OutputDir=distfiles
OutputBaseFilename=DomainCheck_v0.5.8_Setup
SetupIconFile=src\gui\resources\dchk.ico
Compression=lzma
SolidCompression=yes
WizardImageFile=src\gui\resources\setup.bmp
WizardSmallImageFile=src\gui\resources\setup-icon.bmp
LicenseFile=LICENSE.TXT
PrivilegesRequired=none

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"

[CustomMessages]
german.dchk_client=Kommandozeilen Client
german.dchk_gui=Grafische Oberfl�che
german.dchk_docs=Dokumentation
german.dchk_dev=Vorkompilierte Library und Header
german.dchk_src=Quellcode
german.install_full=Komplett Installation
german.install_custom=Benutzerdefinierte Installation

english.dchk_client=Commandline client
english.dchk_gui=Graphical userinterface
english.dchk_docs=Documentation
english.dchk_dev=Precompiled library und header
english.dchk_src=Sourcecode
english.install_full=Full installation
english.install_custom=Custom installation

[Types]
Name: "full"; Description: "{cm:install_full}"
Name: "custom"; Description: "{cm:install_custom}"; Flags: iscustom


[Components]
Name: "client"; Description: "{cm:dchk_client}"; Types: full custom
Name: "gui"; Description: "{cm:dchk_gui}"; Types: full
Name: "docs"; Description: "{cm:dchk_docs}"; Types: full
Name: "dev"; Description: "{cm:dchk_dev}"; Types: full
Name: "src"; Description: "{cm:dchk_src}"; Types: full

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; Components: gui
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; Components: gui

[Dirs]
Name: "{app}\documentation"; Components: docs
Name: "{app}\include"; Components: dev
Name: "{app}\src"; Components: src
Name: "{app}\src\src"; Components: src
Name: "{app}\src\src\lib"; Components: dev
Name: "{app}\src\src\cli"; Components: src
Name: "{app}\src\src\docs"; Components: src
Name: "{app}\src\src\gui"; Components: src
Name: "{app}\src\src\gui\resources"; Components: src
Name: "{app}\src\autoconf"; Components: src
Name: "{app}\src\include"; Components: src
Name: "{app}\src\vs2008"; Components: src

[Files]
Source: "release\dchk.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: client
Source: "src\gui\release/qdchk.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: gui
Source: "C:\Qt\4.4.3\bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: gui
Source: "C:\Qt\4.4.3\bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: gui
Source: "C:\Qt\4.4.3\bin\mingwm10.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: gui
Source: "release\libdchk.a"; DestDir: "{app}\lib"; Flags: ignoreversion; Components: dev
Source: "debug\libdchk-debug.a"; DestDir: "{app}\lib"; Flags: ignoreversion; Components: dev
Source: "include\dchk.h"; DestDir: "{app}\include"; Flags: ignoreversion; Components: dev
Source: "documentation\html\*"; DestDir: "{app}\documentation"; Flags: ignoreversion; Components: docs
Source: "LICENSE.TXT"; DestDir: "{app}"; Flags: ignoreversion; Components: client
Source: "src\gui\resources\uninstall.ico"; DestDir: "{app}"; Flags: ignoreversion; Components: client
Source: "*.TXT"; DestDir: "{app}\src"; Flags: ignoreversion; Components: src
Source: "Doxyfile"; DestDir: "{app}\src"; Flags: ignoreversion; Components: src
Source: "configure"; DestDir: "{app}\src"; Flags: ignoreversion; Components: src
Source: "Makefile.in"; DestDir: "{app}\src"; Flags: ignoreversion; Components: src
Source: "autoconf\*"; DestDir: "{app}\src\autoconf"; Flags: ignoreversion; Components: src
Source: "src\lib\*"; DestDir: "{app}\src\src\lib"; Flags: ignoreversion; Components: src
Source: "src\cli\*"; DestDir: "{app}\src\src\cli"; Flags: ignoreversion; Components: src
Source: "src\gui\*"; DestDir: "{app}\src\src\gui"; Flags: ignoreversion; Components: src
Source: "src\docs\*"; DestDir: "{app}\src\src\docs"; Flags: ignoreversion; Components: src
Source: "vs2008\*.vcproj"; DestDir: "{app}\src\vs2008"; Flags: ignoreversion; Components: src
Source: "vs2008\*.sln"; DestDir: "{app}\src\vs2008"; Flags: ignoreversion; Components: src
Source: "include\config.h.in"; DestDir: "{app}\src\include"; Flags: ignoreversion; Components: src
Source: "include\dchk.h"; DestDir: "{app}\src\include"; Flags: ignoreversion; Components: src
Source: "include\ns_parse.h"; DestDir: "{app}\src\include"; Flags: ignoreversion; Components: src
Source: "include\compat.h"; DestDir: "{app}\src\include"; Flags: ignoreversion; Components: src
Source: "include\msvc-config.h"; DestDir: "{app}\src\include"; Flags: ignoreversion; Components: src
Source: "src\gui\resources\*"; DestDir: "{app}\src\src\gui\resources"; Flags: ignoreversion; Components: src
Source: "src\gui\main.cpp"; DestDir: "{app}\src\src\gui"; Flags: ignoreversion; Components: src
Source: "src\gui\qdchk.*"; DestDir: "{app}\src\src\gui"; Flags: ignoreversion; Components: src
Source: "src\gui\res.qrc"; DestDir: "{app}\src\src\gui"; Flags: ignoreversion; Components: src


; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\DomainCheck Client"; WorkingDir: "{app}"; Filename: "cmd.exe"; Parameters: "/K echo Please type 'dchk' for help"; Components: client
Name: "{group}\DomainCheck GUI"; WorkingDir: "{app}"; Filename: "{app}\qdchk.exe"; Components: gui
Name: "{group}\DomainCheck Documentation"; WorkingDir: "{app}\documentation"; Filename: "{app}\documentation\index.html"; Components: docs
Name: "{group}\{cm:UninstallProgram,DomainCheck}"; IconFilename: "{app}\uninstall.ico"; Filename: "{uninstallexe}"; Components: client
Name: "{commondesktop}\DomainCheck GUI"; Filename: "{app}\qdchk.exe"; Tasks: desktopicon; Components: gui
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\DomainCheck GUI"; Filename: "{app}\qdchk.exe"; Tasks: quicklaunchicon; Components: gui

[Run]
Filename: "{app}\qdchk.exe"; Description: "{cm:LaunchProgram,DomainCheck GUI}"; Flags: nowait postinstall skipifsilent; Components: gui

