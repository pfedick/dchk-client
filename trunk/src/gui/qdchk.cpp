/*
 * This file is part of the Domain CHecK Client (DCHK) from Patrick Fedick
 * (http://dchk.sourceforge.net/). It can be used to check the availability
 * of domains by using the DCHK protocol described in RFC 5144.
 *
 * $Author: pfedick $
 * $Revision: 1.5 $
 * $Date: 2009/07/16 11:37:07 $
 * $Id: qdchk.cpp,v 1.5 2009/07/16 11:37:07 pfedick Exp $
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

#include "qdchk.h"

void Error2String(QString &target, dchk::CError *error)
{
	target.sprintf("ERROR: %i, %s",error->GetErrorCode(), error->GetErrorText());
	const char *a=error->GetAdditionalErrorText();
	if (a) {
		target+=" [";
		target+=a;
		target+="]";
	}
}

void HexDump(QString &target, char *address, size_t bytes)
{
	target.clear();
    char buff[1024], tmp[10], cleartext[20];
    char *_adresse=(char*)address;
    int spalte=0;
    size_t i=0;
    sprintf (buff,"0x%08zX: ",i);
    memset(cleartext,0,20);
    for (i=0;i<bytes;i++) {
        if (spalte>15) {
            strcat(buff,"                                                               ");
            buff[60]=0;
            strcat (buff,": ");
            strcat (buff,cleartext);
            target+=buff;
            target+="\n";
            sprintf (buff,"0x%08zX: ",i);
            memset(cleartext,0,20);
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
        target+=buff;
        target+="\n";
    }

}

qdchk::qdchk(QWidget *parent)
    : QDialog(parent)
{
	QString Version="Version ";
	Version+=DCHK_VERSION;

	ui.setupUi(this);
	ui.about_name->setText("DomainCheck GUI");
	ui.about_version->setText(Version);
	ui.page->setCurrentIndex(0);
	QString Title=QApplication::translate("qdchkClass", "DomainCheck GUI", 0, QApplication::UnicodeUTF8);
	Title+=" ";
	Title+=Version;
	setWindowTitle(Title);
	/*
	int flags=0;
	flags |= Qt::WindowMinimizeButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	ui.setWindowFlags(flags);
	*/
}

qdchk::~qdchk()
{

}

void qdchk::on_start_clicked()
{
	dchk::CDchk dchk;
	QString Tmp;
	QString domains=ui.domainlist->toPlainText();
	QStringList dl=domains.split("\n",QString::SkipEmptyParts);

	ui.result->clear();
	if (dl.size()==0) {
		QMessageBox::information(this, tr("DomainCheck"),
		                   tr("Domainlist is empty, nothing to do."),
		                   QMessageBox::Ok);
		return;
	}

	dchk::CDomainList list;

	for (int i = 0; i < dl.size(); ++i)
		list.AddDomain(dl.at(i).toUtf8().constData());


	if (ui.useDNS->isChecked()) {
		// We use the first domain in list to get the TLD
		DCHK_RECORD *d=list.GetFirst();
		const char *tld=strrchr(d->domain,'.');		// Find last occurrance of '.'
		if (tld) tld++;
		else {
			QMessageBox::critical(this, tr("DomainCheck"),
			                   tr("No TLD found in domainlist"),
			                   QMessageBox::Ok);

			return;
		}
		if (!dchk.SetHostByDNS(tld)) {
			QString e;
			Error2String(e,&dchk);
			QMessageBox::critical(this, tr("DomainCheck"),
			                   e,
			                   QMessageBox::Ok);
			return;
		}
	} else {
		Tmp=ui.server->text();
		QString Host=Tmp.trimmed();
		Tmp=ui.port->text();
		QString Port=Tmp.trimmed();
		if(Port.length()==0) Port="715";
		dchk.SetHost(Host.toUtf8(),Port.toInt());
	}
	QString Timeout=ui.timeout->text();
	QString PacketSize=ui.packetsize->text();

	dchk.SetTimeout(Timeout.toInt(),0);
	dchk.SetMaximumResponseLength(PacketSize.toInt());
	bool ac=false;
	bool sc=false;
	if (ui.acceptCompressed->isChecked()) ac=true;
	if (ui.sendCompressed->isChecked()) sc=true;
	//if (ui.useRFC1950->isChecked()) dchk.UseRFC1950(true);
	//else dchk.UseRFC1950(false);
	dchk.AcceptDeflated(ac);
	dchk.SendDeflated(sc);

	// Set Authority
	if (ui.authority_automatic->isChecked()) {
		if (!dchk.SetAuthorityByDomainList(list)) {
			QString e;
			Error2String(e,&dchk);
			QMessageBox::critical(this, tr("DomainCheck"),
			                   e,
			                   QMessageBox::Ok);
			return;
		}
	} else {
		Tmp=ui.authority->text();
		dchk.SetAuthority(Tmp.toUtf8());
	}

	// Connect to Server
	if (!dchk.Connect()) {
		QString e;
		Error2String(e,&dchk);
		QMessageBox::critical(this, tr("DomainCheck"),
		                   e,
		                   QMessageBox::Ok);
		return;
	}

	if (!dchk.QueryDomainList(list)) {
		QString e;
		Error2String(e,&dchk);
		QMessageBox::critical(this, tr("DomainCheck"),
		                   e,
		                   QMessageBox::Ok);
	}
	dchk.Disconnect();
	IRISLWZ_PACKET *request=dchk.GetRequestPacket();
	IRISLWZ_PACKET *response=dchk.GetResponsePacket();

	// Request
	ui.request->setText(request->beautified);
	HexDump(Tmp,request->packet,request->size);
	ui.req_packet->setText(Tmp);
	Tmp.sprintf("%i",request->size);
	ui.req_packet_size->setText(Tmp);

	Tmp.sprintf("%i",(request->header?request->header->header:0));
	ui.req_payload_header->setText(Tmp);
	Tmp.sprintf("%i",ntohs((request->header?request->header->transactionId:0)));
	ui.req_transactionId->setText(Tmp);
	Tmp.sprintf("%i",ntohs((request->header?request->header->max_response_length:0)));
	ui.req_maxRespLength->setText(Tmp);
	Tmp.sprintf("%i",(request->header?request->header->authority_length:0));
	ui.req_authorityLength->setText(Tmp);
	Tmp.clear();
	if (request->header!=NULL && request->header->authority_length>0) {
		Tmp=QString::fromUtf8(request->packet+6,request->header->authority_length);
	}
	ui.req_authority->setText(Tmp);
	Tmp.sprintf("%i",((request->header?request->header->header&192:0))>>6);
	ui.req_version->setText(Tmp);

	ui.req_compressed->setText((dchk.IsDeflated(request)?"yes":"no"));
	ui.req_accept_compression->setText((dchk.IsDeflateSupported(request)?"yes":"no"));
	ui.req_payloadtype->setText(dchk.GetPayloadType(request));

	// Response
	ui.answer->setText((response->beautified?response->beautified:""));
	if (response->packet) {
		HexDump(Tmp,response->packet,response->size);
		ui.resp_packet->setText(Tmp);
	} else {
		ui.resp_packet->setText("");
	}
	Tmp.sprintf("%i",response->size);
	ui.resp_packet_size->setText(Tmp);
	Tmp.sprintf("%i",(response->header?response->header->header:0));
	ui.resp_payload_header->setText(Tmp);
	Tmp.sprintf("%i",ntohs((response->header?response->header->transactionId:0)));
	ui.resp_transactionId->setText(Tmp);
	Tmp.sprintf("%i",(response->header?((response->header->header&192)>>6):0));
	ui.resp_version->setText(Tmp);
	ui.resp_compressed->setText((dchk.IsDeflated(response)?"yes":"no"));
	ui.resp_accept_compression->setText((dchk.IsDeflateSupported(response)?"yes":"no"));
	ui.resp_payloadtype->setText(dchk.GetPayloadType(response));

	list.Reset();
	DCHK_RECORD *row;
	while ((row=(DCHK_RECORD*)list.GetNext())) {
		QTreeWidgetItem *item;
		item=new QTreeWidgetItem(ui.result);
		item->setText(0,row->domain);
		if (row->status) item->setText(1,row->status);
		else item->setText(1,"");
		if (row->lastdatabaseupdate) item->setText(2,row->lastdatabaseupdate);
		else item->setText(2,"");
	}
	ui.result->resizeColumnToContents(0);
	ui.result->resizeColumnToContents(1);
	ui.result->resizeColumnToContents(2);
}

void qdchk::on_useDNS_toggled()
{

	if (ui.useDNS->isChecked()==true) {
		ui.serverFrame->setEnabled(false);
	} else {
		ui.serverFrame->setEnabled(true);
	}
}


void qdchk::on_authority_automatic_toggled(bool v)
{
	if (v) ui.authority->setEnabled(false);
}

void qdchk::on_authority_manual_toggled(bool v)
{
	if (v) ui.authority->setEnabled(true);
}

