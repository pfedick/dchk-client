/********************************************************************************
** Form generated from reading ui file 'qdchk.ui'
**
** Created: Thu Mar 19 19:40:22 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QDCHK_H
#define UI_QDCHK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qdchkClass
{
public:
    QGridLayout *gridLayout;
    QTabWidget *page;
    QWidget *tab;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout1;
    QHBoxLayout *hboxLayout1;
    QLabel *label;
    QSpacerItem *spacerItem;
    QPushButton *start;
    QTextEdit *domainlist;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hboxLayout2;
    QLabel *label_5;
    QCheckBox *useDNS;
    QSpacerItem *spacerItem1;
    QFrame *serverFrame;
    QGridLayout *gridLayout1;
    QLabel *label_2;
    QLineEdit *server;
    QLabel *label_3;
    QLineEdit *port;
    QGridLayout *gridLayout2;
    QLabel *label_4;
    QLineEdit *packetsize;
    QLabel *label_14;
    QSpacerItem *spacerItem2;
    QLabel *label_12;
    QLineEdit *timeout;
    QLabel *label_13;
    QSpacerItem *spacerItem3;
    QGridLayout *gridLayout3;
    QCheckBox *sendCompressed;
    QCheckBox *acceptCompressed;
    QGroupBox *groupBox_4;
    QVBoxLayout *vboxLayout2;
    QGridLayout *gridLayout4;
    QRadioButton *authority_automatic;
    QRadioButton *authority_manual;
    QLineEdit *authority;
    QSpacerItem *spacerItem4;
    QTreeWidget *result;
    QWidget *tab_2;
    QVBoxLayout *vboxLayout3;
    QGroupBox *groupBox_2;
    QHBoxLayout *hboxLayout3;
    QGridLayout *gridLayout5;
    QLabel *label_15;
    QLabel *req_payload_header;
    QLabel *label_22;
    QLabel *req_packet_size;
    QLabel *label_16;
    QLabel *req_transactionId;
    QLabel *label_17;
    QLabel *req_maxRespLength;
    QLabel *label_18;
    QLabel *req_authorityLength;
    QLabel *label_19;
    QLabel *req_authority;
    QSpacerItem *spacerItem5;
    QVBoxLayout *vboxLayout4;
    QGridLayout *gridLayout6;
    QLabel *label_26;
    QLabel *req_version;
    QLabel *label_30;
    QLabel *req_compressed;
    QLabel *label_31;
    QLabel *req_accept_compression;
    QLabel *label_32;
    QLabel *req_payloadtype;
    QSpacerItem *spacerItem6;
    QTextBrowser *req_packet;
    QLabel *label_23;
    QTextBrowser *request;
    QWidget *tab_4;
    QVBoxLayout *vboxLayout5;
    QGroupBox *groupBox_3;
    QHBoxLayout *hboxLayout4;
    QVBoxLayout *vboxLayout6;
    QGridLayout *gridLayout7;
    QLabel *label_20;
    QLabel *resp_payload_header;
    QLabel *label_24;
    QLabel *resp_packet_size;
    QLabel *label_21;
    QLabel *resp_transactionId;
    QSpacerItem *spacerItem7;
    QVBoxLayout *vboxLayout7;
    QGridLayout *gridLayout8;
    QLabel *label_25;
    QLabel *resp_version;
    QLabel *label_27;
    QLabel *resp_compressed;
    QLabel *label_28;
    QLabel *resp_accept_compression;
    QLabel *label_29;
    QLabel *resp_payloadtype;
    QSpacerItem *spacerItem8;
    QTextBrowser *resp_packet;
    QTextBrowser *answer;
    QWidget *tab_3;
    QVBoxLayout *vboxLayout8;
    QGridLayout *gridLayout9;
    QSpacerItem *spacerItem9;
    QTextBrowser *textBrowser;
    QSpacerItem *spacerItem10;
    QHBoxLayout *hboxLayout5;
    QSpacerItem *spacerItem11;
    QVBoxLayout *vboxLayout9;
    QLabel *about_name;
    QLabel *about_version;
    QLabel *label_10;
    QLabel *label_11;
    QSpacerItem *spacerItem12;
    QLabel *label_6;

    void setupUi(QDialog *qdchkClass)
    {
    if (qdchkClass->objectName().isEmpty())
        qdchkClass->setObjectName(QString::fromUtf8("qdchkClass"));
    qdchkClass->resize(819, 564);
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/images/resources/icon32.png")), QIcon::Normal, QIcon::Off);
    qdchkClass->setWindowIcon(icon);
    qdchkClass->setAutoFillBackground(true);
    qdchkClass->setSizeGripEnabled(true);
    qdchkClass->setModal(false);
    gridLayout = new QGridLayout(qdchkClass);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    page = new QTabWidget(qdchkClass);
    page->setObjectName(QString::fromUtf8("page"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    tab->setGeometry(QRect(0, 0, 797, 520));
    vboxLayout = new QVBoxLayout(tab);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    label = new QLabel(tab);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout1->addWidget(label);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem);

    start = new QPushButton(tab);
    start->setObjectName(QString::fromUtf8("start"));
    QIcon icon1;
    icon1.addPixmap(QPixmap(QString::fromUtf8(":/icons/resources/button_ok.png")), QIcon::Normal, QIcon::Off);
    start->setIcon(icon1);
    start->setDefault(true);

    hboxLayout1->addWidget(start);


    vboxLayout1->addLayout(hboxLayout1);

    domainlist = new QTextEdit(tab);
    domainlist->setObjectName(QString::fromUtf8("domainlist"));

    vboxLayout1->addWidget(domainlist);


    hboxLayout->addLayout(vboxLayout1);

    groupBox = new QGroupBox(tab);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setMinimumSize(QSize(350, 280));
    groupBox->setMaximumSize(QSize(350, 16777215));
    verticalLayout = new QVBoxLayout(groupBox);
    verticalLayout->setSpacing(6);
    verticalLayout->setMargin(11);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setSpacing(4);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    label_5 = new QLabel(groupBox);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    hboxLayout2->addWidget(label_5);

    useDNS = new QCheckBox(groupBox);
    useDNS->setObjectName(QString::fromUtf8("useDNS"));
    QIcon icon2;
    icon2.addPixmap(QPixmap(QString::fromUtf8(":/icons/resources/database.png")), QIcon::Normal, QIcon::Off);
    useDNS->setIcon(icon2);
    useDNS->setChecked(true);

    hboxLayout2->addWidget(useDNS);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout2->addItem(spacerItem1);


    verticalLayout->addLayout(hboxLayout2);

    serverFrame = new QFrame(groupBox);
    serverFrame->setObjectName(QString::fromUtf8("serverFrame"));
    serverFrame->setEnabled(false);
    serverFrame->setMinimumSize(QSize(0, 0));
    serverFrame->setFrameShape(QFrame::NoFrame);
    serverFrame->setFrameShadow(QFrame::Raised);
    gridLayout1 = new QGridLayout(serverFrame);
    gridLayout1->setSpacing(6);
    gridLayout1->setMargin(11);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setHorizontalSpacing(6);
    gridLayout1->setVerticalSpacing(0);
    gridLayout1->setContentsMargins(30, 2, -1, 2);
    label_2 = new QLabel(serverFrame);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout1->addWidget(label_2, 0, 0, 1, 1);

    server = new QLineEdit(serverFrame);
    server->setObjectName(QString::fromUtf8("server"));

    gridLayout1->addWidget(server, 0, 1, 1, 1);

    label_3 = new QLabel(serverFrame);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout1->addWidget(label_3, 1, 0, 1, 1);

    port = new QLineEdit(serverFrame);
    port->setObjectName(QString::fromUtf8("port"));
    port->setMaximumSize(QSize(80, 16777215));

    gridLayout1->addWidget(port, 1, 1, 1, 1);


    verticalLayout->addWidget(serverFrame);

    gridLayout2 = new QGridLayout();
    gridLayout2->setSpacing(6);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    gridLayout2->setHorizontalSpacing(6);
    gridLayout2->setVerticalSpacing(0);
    label_4 = new QLabel(groupBox);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout2->addWidget(label_4, 0, 0, 1, 1);

    packetsize = new QLineEdit(groupBox);
    packetsize->setObjectName(QString::fromUtf8("packetsize"));
    packetsize->setMaximumSize(QSize(100, 16777215));

    gridLayout2->addWidget(packetsize, 0, 1, 1, 1);

    label_14 = new QLabel(groupBox);
    label_14->setObjectName(QString::fromUtf8("label_14"));

    gridLayout2->addWidget(label_14, 0, 2, 1, 1);

    spacerItem2 = new QSpacerItem(101, 23, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout2->addItem(spacerItem2, 0, 3, 1, 1);

    label_12 = new QLabel(groupBox);
    label_12->setObjectName(QString::fromUtf8("label_12"));

    gridLayout2->addWidget(label_12, 1, 0, 1, 1);

    timeout = new QLineEdit(groupBox);
    timeout->setObjectName(QString::fromUtf8("timeout"));

    gridLayout2->addWidget(timeout, 1, 1, 1, 1);

    label_13 = new QLabel(groupBox);
    label_13->setObjectName(QString::fromUtf8("label_13"));

    gridLayout2->addWidget(label_13, 1, 2, 1, 1);

    spacerItem3 = new QSpacerItem(126, 23, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout2->addItem(spacerItem3, 1, 3, 1, 1);


    verticalLayout->addLayout(gridLayout2);

    gridLayout3 = new QGridLayout();
    gridLayout3->setSpacing(6);
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    sendCompressed = new QCheckBox(groupBox);
    sendCompressed->setObjectName(QString::fromUtf8("sendCompressed"));
    sendCompressed->setEnabled(true);
    sendCompressed->setChecked(false);

    gridLayout3->addWidget(sendCompressed, 0, 0, 1, 1);

    acceptCompressed = new QCheckBox(groupBox);
    acceptCompressed->setObjectName(QString::fromUtf8("acceptCompressed"));
    acceptCompressed->setChecked(true);

    gridLayout3->addWidget(acceptCompressed, 1, 0, 1, 1);


    verticalLayout->addLayout(gridLayout3);

    groupBox_4 = new QGroupBox(groupBox);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    groupBox_4->setMinimumSize(QSize(0, 60));
    vboxLayout2 = new QVBoxLayout(groupBox_4);
    vboxLayout2->setSpacing(0);
    vboxLayout2->setMargin(11);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    vboxLayout2->setContentsMargins(30, 0, -1, 0);
    gridLayout4 = new QGridLayout();
    gridLayout4->setSpacing(6);
    gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
    gridLayout4->setVerticalSpacing(0);
    authority_automatic = new QRadioButton(groupBox_4);
    authority_automatic->setObjectName(QString::fromUtf8("authority_automatic"));
    authority_automatic->setChecked(true);

    gridLayout4->addWidget(authority_automatic, 0, 0, 1, 1);

    authority_manual = new QRadioButton(groupBox_4);
    authority_manual->setObjectName(QString::fromUtf8("authority_manual"));

    gridLayout4->addWidget(authority_manual, 1, 0, 1, 1);

    authority = new QLineEdit(groupBox_4);
    authority->setObjectName(QString::fromUtf8("authority"));
    authority->setEnabled(false);

    gridLayout4->addWidget(authority, 1, 1, 1, 1);


    vboxLayout2->addLayout(gridLayout4);


    verticalLayout->addWidget(groupBox_4);

    spacerItem4 = new QSpacerItem(328, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout->addItem(spacerItem4);


    hboxLayout->addWidget(groupBox);


    vboxLayout->addLayout(hboxLayout);

    result = new QTreeWidget(tab);
    result->setObjectName(QString::fromUtf8("result"));
    result->setMidLineWidth(1);
    result->setAlternatingRowColors(true);
    result->setRootIsDecorated(false);

    vboxLayout->addWidget(result);

    QIcon icon3;
    icon3.addPixmap(QPixmap(QString::fromUtf8(":/icons/resources/find.png")), QIcon::Normal, QIcon::Off);
    page->addTab(tab, icon3, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    tab_2->setGeometry(QRect(180, 199, 1192, 833));
    vboxLayout3 = new QVBoxLayout(tab_2);
    vboxLayout3->setSpacing(6);
    vboxLayout3->setMargin(11);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    groupBox_2 = new QGroupBox(tab_2);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setMinimumSize(QSize(0, 200));
    groupBox_2->setMaximumSize(QSize(16777215, 200));
    hboxLayout3 = new QHBoxLayout(groupBox_2);
    hboxLayout3->setSpacing(6);
    hboxLayout3->setMargin(11);
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    gridLayout5 = new QGridLayout();
    gridLayout5->setSpacing(6);
    gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
    label_15 = new QLabel(groupBox_2);
    label_15->setObjectName(QString::fromUtf8("label_15"));

    gridLayout5->addWidget(label_15, 0, 0, 1, 1);

    req_payload_header = new QLabel(groupBox_2);
    req_payload_header->setObjectName(QString::fromUtf8("req_payload_header"));
    req_payload_header->setMinimumSize(QSize(100, 0));
    req_payload_header->setFrameShape(QFrame::StyledPanel);
    req_payload_header->setFrameShadow(QFrame::Sunken);
    req_payload_header->setMargin(0);

    gridLayout5->addWidget(req_payload_header, 0, 1, 1, 1);

    label_22 = new QLabel(groupBox_2);
    label_22->setObjectName(QString::fromUtf8("label_22"));

    gridLayout5->addWidget(label_22, 1, 0, 1, 1);

    req_packet_size = new QLabel(groupBox_2);
    req_packet_size->setObjectName(QString::fromUtf8("req_packet_size"));
    req_packet_size->setFrameShape(QFrame::StyledPanel);
    req_packet_size->setFrameShadow(QFrame::Sunken);
    req_packet_size->setMargin(0);

    gridLayout5->addWidget(req_packet_size, 1, 1, 1, 1);

    label_16 = new QLabel(groupBox_2);
    label_16->setObjectName(QString::fromUtf8("label_16"));

    gridLayout5->addWidget(label_16, 2, 0, 1, 1);

    req_transactionId = new QLabel(groupBox_2);
    req_transactionId->setObjectName(QString::fromUtf8("req_transactionId"));
    req_transactionId->setFrameShape(QFrame::StyledPanel);
    req_transactionId->setFrameShadow(QFrame::Sunken);
    req_transactionId->setMargin(0);

    gridLayout5->addWidget(req_transactionId, 2, 1, 1, 1);

    label_17 = new QLabel(groupBox_2);
    label_17->setObjectName(QString::fromUtf8("label_17"));

    gridLayout5->addWidget(label_17, 3, 0, 1, 1);

    req_maxRespLength = new QLabel(groupBox_2);
    req_maxRespLength->setObjectName(QString::fromUtf8("req_maxRespLength"));
    req_maxRespLength->setFrameShape(QFrame::StyledPanel);
    req_maxRespLength->setFrameShadow(QFrame::Sunken);
    req_maxRespLength->setMargin(0);

    gridLayout5->addWidget(req_maxRespLength, 3, 1, 1, 1);

    label_18 = new QLabel(groupBox_2);
    label_18->setObjectName(QString::fromUtf8("label_18"));

    gridLayout5->addWidget(label_18, 4, 0, 1, 1);

    req_authorityLength = new QLabel(groupBox_2);
    req_authorityLength->setObjectName(QString::fromUtf8("req_authorityLength"));
    req_authorityLength->setFrameShape(QFrame::StyledPanel);
    req_authorityLength->setFrameShadow(QFrame::Sunken);
    req_authorityLength->setMargin(0);

    gridLayout5->addWidget(req_authorityLength, 4, 1, 1, 1);

    label_19 = new QLabel(groupBox_2);
    label_19->setObjectName(QString::fromUtf8("label_19"));

    gridLayout5->addWidget(label_19, 5, 0, 1, 1);

    req_authority = new QLabel(groupBox_2);
    req_authority->setObjectName(QString::fromUtf8("req_authority"));
    req_authority->setFrameShape(QFrame::StyledPanel);
    req_authority->setFrameShadow(QFrame::Sunken);
    req_authority->setMargin(0);

    gridLayout5->addWidget(req_authority, 5, 1, 1, 1);

    spacerItem5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout5->addItem(spacerItem5, 6, 0, 1, 1);


    hboxLayout3->addLayout(gridLayout5);

    vboxLayout4 = new QVBoxLayout();
    vboxLayout4->setSpacing(6);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    gridLayout6 = new QGridLayout();
    gridLayout6->setSpacing(6);
    gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
    label_26 = new QLabel(groupBox_2);
    label_26->setObjectName(QString::fromUtf8("label_26"));

    gridLayout6->addWidget(label_26, 0, 0, 1, 1);

    req_version = new QLabel(groupBox_2);
    req_version->setObjectName(QString::fromUtf8("req_version"));
    req_version->setMinimumSize(QSize(80, 0));
    req_version->setFrameShape(QFrame::StyledPanel);
    req_version->setFrameShadow(QFrame::Sunken);
    req_version->setMargin(0);

    gridLayout6->addWidget(req_version, 0, 1, 1, 1);

    label_30 = new QLabel(groupBox_2);
    label_30->setObjectName(QString::fromUtf8("label_30"));

    gridLayout6->addWidget(label_30, 1, 0, 1, 1);

    req_compressed = new QLabel(groupBox_2);
    req_compressed->setObjectName(QString::fromUtf8("req_compressed"));
    req_compressed->setFrameShape(QFrame::StyledPanel);
    req_compressed->setFrameShadow(QFrame::Sunken);
    req_compressed->setMargin(0);

    gridLayout6->addWidget(req_compressed, 1, 1, 1, 1);

    label_31 = new QLabel(groupBox_2);
    label_31->setObjectName(QString::fromUtf8("label_31"));

    gridLayout6->addWidget(label_31, 2, 0, 1, 1);

    req_accept_compression = new QLabel(groupBox_2);
    req_accept_compression->setObjectName(QString::fromUtf8("req_accept_compression"));
    req_accept_compression->setFrameShape(QFrame::StyledPanel);
    req_accept_compression->setFrameShadow(QFrame::Sunken);
    req_accept_compression->setMargin(0);

    gridLayout6->addWidget(req_accept_compression, 2, 1, 2, 1);

    label_32 = new QLabel(groupBox_2);
    label_32->setObjectName(QString::fromUtf8("label_32"));

    gridLayout6->addWidget(label_32, 3, 0, 2, 1);

    req_payloadtype = new QLabel(groupBox_2);
    req_payloadtype->setObjectName(QString::fromUtf8("req_payloadtype"));
    req_payloadtype->setFrameShape(QFrame::StyledPanel);
    req_payloadtype->setFrameShadow(QFrame::Sunken);
    req_payloadtype->setMargin(0);

    gridLayout6->addWidget(req_payloadtype, 4, 1, 1, 1);


    vboxLayout4->addLayout(gridLayout6);

    spacerItem6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout4->addItem(spacerItem6);


    hboxLayout3->addLayout(vboxLayout4);

    req_packet = new QTextBrowser(groupBox_2);
    req_packet->setObjectName(QString::fromUtf8("req_packet"));
    QFont font;
    font.setFamily(QString::fromUtf8("Courier"));
    font.setStyleStrategy(QFont::PreferDefault);
    req_packet->setFont(font);
    req_packet->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    req_packet->setLineWrapMode(QTextEdit::NoWrap);

    hboxLayout3->addWidget(req_packet);


    vboxLayout3->addWidget(groupBox_2);

    label_23 = new QLabel(tab_2);
    label_23->setObjectName(QString::fromUtf8("label_23"));

    vboxLayout3->addWidget(label_23);

    request = new QTextBrowser(tab_2);
    request->setObjectName(QString::fromUtf8("request"));

    vboxLayout3->addWidget(request);

    QIcon icon4;
    icon4.addPixmap(QPixmap(QString::fromUtf8(":/icons/resources/out.png")), QIcon::Normal, QIcon::Off);
    page->addTab(tab_2, icon4, QString());
    tab_4 = new QWidget();
    tab_4->setObjectName(QString::fromUtf8("tab_4"));
    tab_4->setGeometry(QRect(202, 221, 1192, 833));
    vboxLayout5 = new QVBoxLayout(tab_4);
    vboxLayout5->setSpacing(6);
    vboxLayout5->setMargin(11);
    vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
    groupBox_3 = new QGroupBox(tab_4);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    groupBox_3->setMinimumSize(QSize(0, 200));
    groupBox_3->setMaximumSize(QSize(16777215, 200));
    hboxLayout4 = new QHBoxLayout(groupBox_3);
    hboxLayout4->setSpacing(6);
    hboxLayout4->setMargin(11);
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    vboxLayout6 = new QVBoxLayout();
    vboxLayout6->setSpacing(6);
    vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
    gridLayout7 = new QGridLayout();
    gridLayout7->setSpacing(6);
    gridLayout7->setObjectName(QString::fromUtf8("gridLayout7"));
    label_20 = new QLabel(groupBox_3);
    label_20->setObjectName(QString::fromUtf8("label_20"));

    gridLayout7->addWidget(label_20, 0, 0, 1, 1);

    resp_payload_header = new QLabel(groupBox_3);
    resp_payload_header->setObjectName(QString::fromUtf8("resp_payload_header"));
    resp_payload_header->setMinimumSize(QSize(100, 0));
    resp_payload_header->setFrameShape(QFrame::StyledPanel);
    resp_payload_header->setFrameShadow(QFrame::Sunken);
    resp_payload_header->setMargin(0);

    gridLayout7->addWidget(resp_payload_header, 0, 1, 1, 1);

    label_24 = new QLabel(groupBox_3);
    label_24->setObjectName(QString::fromUtf8("label_24"));

    gridLayout7->addWidget(label_24, 1, 0, 1, 1);

    resp_packet_size = new QLabel(groupBox_3);
    resp_packet_size->setObjectName(QString::fromUtf8("resp_packet_size"));
    resp_packet_size->setFrameShape(QFrame::StyledPanel);
    resp_packet_size->setFrameShadow(QFrame::Sunken);
    resp_packet_size->setMargin(0);

    gridLayout7->addWidget(resp_packet_size, 1, 1, 1, 1);

    label_21 = new QLabel(groupBox_3);
    label_21->setObjectName(QString::fromUtf8("label_21"));

    gridLayout7->addWidget(label_21, 2, 0, 1, 1);

    resp_transactionId = new QLabel(groupBox_3);
    resp_transactionId->setObjectName(QString::fromUtf8("resp_transactionId"));
    resp_transactionId->setFrameShape(QFrame::StyledPanel);
    resp_transactionId->setFrameShadow(QFrame::Sunken);
    resp_transactionId->setMargin(0);

    gridLayout7->addWidget(resp_transactionId, 2, 1, 1, 1);


    vboxLayout6->addLayout(gridLayout7);

    spacerItem7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout6->addItem(spacerItem7);


    hboxLayout4->addLayout(vboxLayout6);

    vboxLayout7 = new QVBoxLayout();
    vboxLayout7->setSpacing(6);
    vboxLayout7->setObjectName(QString::fromUtf8("vboxLayout7"));
    gridLayout8 = new QGridLayout();
    gridLayout8->setSpacing(6);
    gridLayout8->setObjectName(QString::fromUtf8("gridLayout8"));
    label_25 = new QLabel(groupBox_3);
    label_25->setObjectName(QString::fromUtf8("label_25"));

    gridLayout8->addWidget(label_25, 0, 0, 1, 1);

    resp_version = new QLabel(groupBox_3);
    resp_version->setObjectName(QString::fromUtf8("resp_version"));
    resp_version->setMinimumSize(QSize(80, 0));
    resp_version->setFrameShape(QFrame::StyledPanel);
    resp_version->setFrameShadow(QFrame::Sunken);
    resp_version->setMargin(0);

    gridLayout8->addWidget(resp_version, 0, 1, 1, 1);

    label_27 = new QLabel(groupBox_3);
    label_27->setObjectName(QString::fromUtf8("label_27"));

    gridLayout8->addWidget(label_27, 1, 0, 1, 1);

    resp_compressed = new QLabel(groupBox_3);
    resp_compressed->setObjectName(QString::fromUtf8("resp_compressed"));
    resp_compressed->setFrameShape(QFrame::StyledPanel);
    resp_compressed->setFrameShadow(QFrame::Sunken);
    resp_compressed->setMargin(0);

    gridLayout8->addWidget(resp_compressed, 1, 1, 1, 1);

    label_28 = new QLabel(groupBox_3);
    label_28->setObjectName(QString::fromUtf8("label_28"));

    gridLayout8->addWidget(label_28, 2, 0, 1, 1);

    resp_accept_compression = new QLabel(groupBox_3);
    resp_accept_compression->setObjectName(QString::fromUtf8("resp_accept_compression"));
    resp_accept_compression->setFrameShape(QFrame::StyledPanel);
    resp_accept_compression->setFrameShadow(QFrame::Sunken);
    resp_accept_compression->setMargin(0);

    gridLayout8->addWidget(resp_accept_compression, 2, 1, 2, 1);

    label_29 = new QLabel(groupBox_3);
    label_29->setObjectName(QString::fromUtf8("label_29"));

    gridLayout8->addWidget(label_29, 3, 0, 2, 1);

    resp_payloadtype = new QLabel(groupBox_3);
    resp_payloadtype->setObjectName(QString::fromUtf8("resp_payloadtype"));
    resp_payloadtype->setFrameShape(QFrame::StyledPanel);
    resp_payloadtype->setFrameShadow(QFrame::Sunken);
    resp_payloadtype->setMargin(0);

    gridLayout8->addWidget(resp_payloadtype, 4, 1, 1, 1);


    vboxLayout7->addLayout(gridLayout8);

    spacerItem8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout7->addItem(spacerItem8);


    hboxLayout4->addLayout(vboxLayout7);

    resp_packet = new QTextBrowser(groupBox_3);
    resp_packet->setObjectName(QString::fromUtf8("resp_packet"));
    QFont font1;
    font1.setFamily(QString::fromUtf8("Courier"));
    font1.setPointSize(8);
    resp_packet->setFont(font1);
    resp_packet->setLineWrapMode(QTextEdit::NoWrap);

    hboxLayout4->addWidget(resp_packet);


    vboxLayout5->addWidget(groupBox_3);

    answer = new QTextBrowser(tab_4);
    answer->setObjectName(QString::fromUtf8("answer"));

    vboxLayout5->addWidget(answer);

    QIcon icon5;
    icon5.addPixmap(QPixmap(QString::fromUtf8(":/icons/resources/in.png")), QIcon::Normal, QIcon::Off);
    page->addTab(tab_4, icon5, QString());
    tab_3 = new QWidget();
    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    tab_3->setGeometry(QRect(0, 0, 797, 520));
    vboxLayout8 = new QVBoxLayout(tab_3);
    vboxLayout8->setSpacing(6);
    vboxLayout8->setMargin(11);
    vboxLayout8->setObjectName(QString::fromUtf8("vboxLayout8"));
    gridLayout9 = new QGridLayout();
    gridLayout9->setSpacing(6);
    gridLayout9->setObjectName(QString::fromUtf8("gridLayout9"));
    spacerItem9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout9->addItem(spacerItem9, 1, 0, 1, 1);

    textBrowser = new QTextBrowser(tab_3);
    textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
    textBrowser->setMinimumSize(QSize(500, 0));

    gridLayout9->addWidget(textBrowser, 1, 1, 1, 1);

    spacerItem10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout9->addItem(spacerItem10, 1, 3, 1, 1);

    hboxLayout5 = new QHBoxLayout();
    hboxLayout5->setSpacing(6);
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    spacerItem11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout5->addItem(spacerItem11);

    vboxLayout9 = new QVBoxLayout();
    vboxLayout9->setSpacing(6);
    vboxLayout9->setObjectName(QString::fromUtf8("vboxLayout9"));
    about_name = new QLabel(tab_3);
    about_name->setObjectName(QString::fromUtf8("about_name"));
    QFont font2;
    font2.setPointSize(20);
    font2.setBold(true);
    font2.setWeight(75);
    about_name->setFont(font2);
    about_name->setAlignment(Qt::AlignCenter);

    vboxLayout9->addWidget(about_name);

    about_version = new QLabel(tab_3);
    about_version->setObjectName(QString::fromUtf8("about_version"));
    about_version->setScaledContents(false);
    about_version->setAlignment(Qt::AlignCenter);

    vboxLayout9->addWidget(about_version);

    label_10 = new QLabel(tab_3);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setAlignment(Qt::AlignCenter);

    vboxLayout9->addWidget(label_10);

    label_11 = new QLabel(tab_3);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    QFont font3;
    font3.setBold(true);
    font3.setWeight(75);
    label_11->setFont(font3);
    label_11->setAlignment(Qt::AlignCenter);

    vboxLayout9->addWidget(label_11);


    hboxLayout5->addLayout(vboxLayout9);

    spacerItem12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout5->addItem(spacerItem12);


    gridLayout9->addLayout(hboxLayout5, 0, 1, 1, 1);

    label_6 = new QLabel(tab_3);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setPixmap(QPixmap(QString::fromUtf8(":/images/resources/logo.png")));

    gridLayout9->addWidget(label_6, 0, 3, 1, 1);


    vboxLayout8->addLayout(gridLayout9);

    QIcon icon6;
    icon6.addPixmap(QPixmap(QString::fromUtf8(":/icons/resources/documentinfo.png")), QIcon::Normal, QIcon::Off);
    page->addTab(tab_3, icon6, QString());

    gridLayout->addWidget(page, 0, 0, 1, 1);


    retranslateUi(qdchkClass);

    page->setCurrentIndex(3);


    QMetaObject::connectSlotsByName(qdchkClass);
    } // setupUi

    void retranslateUi(QDialog *qdchkClass)
    {
    qdchkClass->setWindowTitle(QApplication::translate("qdchkClass", "DomainCheck GUI", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("qdchkClass", "Domainlist:", 0, QApplication::UnicodeUTF8));
    start->setText(QApplication::translate("qdchkClass", "Start", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    domainlist->setToolTip(QApplication::translate("qdchkClass", "Enter the domain names you want to query here, one domain per line", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    groupBox->setTitle(QApplication::translate("qdchkClass", "Options", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("qdchkClass", "Server:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    useDNS->setToolTip(QApplication::translate("qdchkClass", "DCHK will make a DNS lookup to find out if a DCHK-Server is available for the TLDs in your Domainlist", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_WHATSTHIS
    useDNS->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS

    useDNS->setText(QApplication::translate("qdchkClass", "Use DNS", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("qdchkClass", "DCHK-Server:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    server->setToolTip(QApplication::translate("qdchkClass", "Enter the name or IP of the DCHK-Server to which the queries should be send", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    server->setText(QApplication::translate("qdchkClass", "dchk.denic.de", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("qdchkClass", "Port:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    port->setToolTip(QApplication::translate("qdchkClass", "Enter the port of the DCHK-Server to which the queries should be send (default=715)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    port->setText(QApplication::translate("qdchkClass", "715", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("qdchkClass", "Maximum Packetsize:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    packetsize->setToolTip(QApplication::translate("qdchkClass", "Maximum size for answer packets from the DCHK-Server (maximum=4000)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    packetsize->setText(QApplication::translate("qdchkClass", "1500", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("qdchkClass", "Bytes", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("qdchkClass", "Timeout:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    timeout->setToolTip(QApplication::translate("qdchkClass", "Time to wait for answers from DCHK-server", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    timeout->setText(QApplication::translate("qdchkClass", "3", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("qdchkClass", "Seconds", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    sendCompressed->setToolTip(QApplication::translate("qdchkClass", "Use this to send compressed requests to DCHK-server.\n"
"You can query more domains at once if you use this", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    sendCompressed->setText(QApplication::translate("qdchkClass", "Send compressed packets", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    acceptCompressed->setToolTip(QApplication::translate("qdchkClass", "Enable this, if the client should accept compressed answers.\n"
"Use this, if you want to query many domains at one time.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    acceptCompressed->setText(QApplication::translate("qdchkClass", "Accept compressed packets", 0, QApplication::UnicodeUTF8));
    groupBox_4->setTitle(QApplication::translate("qdchkClass", "Authority", 0, QApplication::UnicodeUTF8));
    authority_automatic->setText(QApplication::translate("qdchkClass", "automatic", 0, QApplication::UnicodeUTF8));
    authority_manual->setText(QApplication::translate("qdchkClass", "manual:", 0, QApplication::UnicodeUTF8));
    authority->setText(QApplication::translate("qdchkClass", "unknown", 0, QApplication::UnicodeUTF8));
    result->headerItem()->setText(0, QApplication::translate("qdchkClass", "Domain", 0, QApplication::UnicodeUTF8));
    result->headerItem()->setText(1, QApplication::translate("qdchkClass", "Status", 0, QApplication::UnicodeUTF8));
    result->headerItem()->setText(2, QApplication::translate("qdchkClass", "Timestamp", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    result->setToolTip(QApplication::translate("qdchkClass", "This table will show the answers from the DCHK-server", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    page->setTabText(page->indexOf(tab), QApplication::translate("qdchkClass", "Query", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("qdchkClass", "LWZ Payload Request Descriptor", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("qdchkClass", "Payload Header:", 0, QApplication::UnicodeUTF8));
    req_payload_header->setText(QString());
    label_22->setText(QApplication::translate("qdchkClass", "Packet Size:", 0, QApplication::UnicodeUTF8));
    req_packet_size->setText(QString());
    label_16->setText(QApplication::translate("qdchkClass", "Transaction ID:", 0, QApplication::UnicodeUTF8));
    req_transactionId->setText(QString());
    label_17->setText(QApplication::translate("qdchkClass", "Maximum Response Length:", 0, QApplication::UnicodeUTF8));
    req_maxRespLength->setText(QString());
    label_18->setText(QApplication::translate("qdchkClass", "Authority Length:", 0, QApplication::UnicodeUTF8));
    req_authorityLength->setText(QString());
    label_19->setText(QApplication::translate("qdchkClass", "Authority:", 0, QApplication::UnicodeUTF8));
    req_authority->setText(QString());
    label_26->setText(QApplication::translate("qdchkClass", "Version:", 0, QApplication::UnicodeUTF8));
    req_version->setText(QString());
    label_30->setText(QApplication::translate("qdchkClass", "Compressed:", 0, QApplication::UnicodeUTF8));
    req_compressed->setText(QString());
    label_31->setText(QApplication::translate("qdchkClass", "Accept Compression:", 0, QApplication::UnicodeUTF8));
    req_accept_compression->setText(QString());
    label_32->setText(QApplication::translate("qdchkClass", "Payload-Type:", 0, QApplication::UnicodeUTF8));
    req_payloadtype->setText(QString());
    label_23->setText(QApplication::translate("qdchkClass", "Payload:", 0, QApplication::UnicodeUTF8));
    page->setTabText(page->indexOf(tab_2), QApplication::translate("qdchkClass", "Request-Packet", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QApplication::translate("qdchkClass", "LWZ Payload Request Descriptor", 0, QApplication::UnicodeUTF8));
    label_20->setText(QApplication::translate("qdchkClass", "Payload Header:", 0, QApplication::UnicodeUTF8));
    resp_payload_header->setText(QString());
    label_24->setText(QApplication::translate("qdchkClass", "Packet Size:", 0, QApplication::UnicodeUTF8));
    resp_packet_size->setText(QString());
    label_21->setText(QApplication::translate("qdchkClass", "Transaction ID:", 0, QApplication::UnicodeUTF8));
    resp_transactionId->setText(QString());
    label_25->setText(QApplication::translate("qdchkClass", "Version:", 0, QApplication::UnicodeUTF8));
    resp_version->setText(QString());
    label_27->setText(QApplication::translate("qdchkClass", "Compressed:", 0, QApplication::UnicodeUTF8));
    resp_compressed->setText(QString());
    label_28->setText(QApplication::translate("qdchkClass", "Accept Compression:", 0, QApplication::UnicodeUTF8));
    resp_accept_compression->setText(QString());
    label_29->setText(QApplication::translate("qdchkClass", "Payload-Type:", 0, QApplication::UnicodeUTF8));
    resp_payloadtype->setText(QString());
    page->setTabText(page->indexOf(tab_4), QApplication::translate("qdchkClass", "Answer Packet", 0, QApplication::UnicodeUTF8));
    textBrowser->setHtml(QApplication::translate("qdchkClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:12pt; font-weight:600;\">Copyright (c) 2008/2009 Patrick Fedick</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\"></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span "
        "style=\" font-family:'Sans Serif'; font-size:9pt;\">This program is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free SoftwareFoundation; either version 3 of the License, or (at your option) any later version.</span></p>\n"
"<p align=\"justify\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\"></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.</span></p>\n"
"<p align=\"justif"
        "y\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\"></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">You should have received a copy of the GNU Lesser General Public License along with this program; if not, see &lt;http://www.gnu.org/licenses/&gt;.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\"></p>\n"
"<hr />\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:"
        "12pt; font-weight:600;\">CREDITS</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt; font-weight:600;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">This software uses the following 3rd party libraries and routines:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">  - dchk Library (http://dchk.sourceforge.net)</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans "
        "Serif'; font-size:9pt;\">    Copyright (c) 2008 Patrick Fedick</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">  - Zlib (http://www.zlib.net/)</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">    Copyright (C) 1995-2005 Jean-loup Gailly and Mark Adler</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\"></p>\n"
"<p "
        "style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">  - libxml2 (http://xmlsoft.org/)</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">    Copyright (C) 1998-2003 Daniel Veillard.  All Rights Reserved.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">  - random number generator</span></p>\n"
"<p style=\" m"
        "argin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">    Copyright (c) 1983, 1993 The Regents of the University of California</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">  - resolver functions from bind9</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">    Copyright (c) 2004 by Internet Systems Consortium, Inc. "
        "(\"ISC\")</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans'; font-size:10pt;\"><span style=\" font-family:'Sans Serif'; font-size:9pt;\">    Copyright (c) 1996,1999 by Internet Software Consortium.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
    about_name->setText(QString());
    about_version->setText(QApplication::translate("qdchkClass", "Version: 1.0.0", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("qdchkClass", "Author", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("qdchkClass", "Patrick Fedick <dchk@pfp.de>", 0, QApplication::UnicodeUTF8));
    label_6->setText(QString());
    page->setTabText(page->indexOf(tab_3), QApplication::translate("qdchkClass", "About", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(qdchkClass);
    } // retranslateUi

};

namespace Ui {
    class qdchkClass: public Ui_qdchkClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDCHK_H
