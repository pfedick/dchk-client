/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.2 $
 * $Date: 2009/03/12 11:56:19 $
 * $Id: qdchk.h,v 1.2 2009/03/12 11:56:19 pfedick Exp $
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

#ifndef QDCHK_H
#define QDCHK_H
#define WITH_QT
#ifdef _WIN32
#include <Winsock2.h>
#else
#include <netinet/in.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include <dchk.h>
#include <QtGui/QDialog>
#include <QMessageBox>
#include <QString>
#include "ui_qdchk.h"

void Error2String(QString &target, dchk::CError *error);
void HexDump(QString &target, char *address, size_t bytes);

class qdchk : public QDialog
{
    Q_OBJECT

public:
    qdchk(QWidget *parent = 0);
    ~qdchk();

private:
    Ui::qdchkClass ui;

private slots:
	void on_start_clicked();
	void on_useDNS_toggled();
	void on_authority_automatic_toggled(bool v);
	void on_authority_manual_toggled(bool v);
};

#endif // QDCHK_H
