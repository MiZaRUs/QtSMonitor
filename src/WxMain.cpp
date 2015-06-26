/**************************************************************************
 *  ReportMaker    WxMain.cpp                                             *
 *  Copyright (C)  2010-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 **************************************************************************/
#include "QtCore"
#include "QtGui"
#include "WxMain.h"
#include "BoxObject.h"
//#include "unistd.h"
// ----------------------------------------------------------------------
WxMain::WxMain( DBReader *db, QWidget *pwgt ) : QWidget( pwgt ){
    resize( 800, 400 );
    setCursor( Qt::PointingHandCursor );
    dbr = db;
    parent = pwgt;
    Err = 0;
    Fl = 0;
    strActName = "";
    strActID = "";

    ptbl = new QTableWidget( 30, 7 );
    QStringList       lst;
    lst << tr( "Дата / Время" ) << tr( "Сообщ" ) << tr( "Статус" ) << tr( "Польз" ) << tr( "ПК" ) << tr( "IP" ) << tr( "Сессия" );
    ptbl->setHorizontalHeaderLabels( lst );
    ptbl->setColumnWidth( 0,150 );
    ptbl->setColumnWidth( 1,77 );
//  --
    menu = dbr->MenuRead( this );
    connect( menu, SIGNAL( triggered( QAction* )),SLOT( slotMenuClicked( QAction* )));

    lout = new QVBoxLayout();
    lout->addWidget( ptbl );
    setLayout( lout );
//  --
    timer = new QTimer( this );
    timer->setInterval( 60000 );	 //=1min// 20 sek = 20000
    connect( timer, SIGNAL( timeout()), this, SLOT( slotTimerRefresh()) );
    timer->start();
}//End WxMain
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void WxMain::upDate( void ){
    if(( strActName ==  "" )||( strActID == "" )) return;
    setWindowTitle( strActName );

    QTableWidgetItem *ptwi;

    BoxObject obj[30];
    dbr->ObjRead( strActID, obj, 30 );
    for( int i = 29; i >=  0; i-- ){
        if( obj[i].message == "Session" ){
            if( obj[i].status == "Begin" ) obj[i].fl = 1;
            if( obj[i].status == "End" ) obj[i].fl = 2;
            if( obj[i].status == "Closed" ) obj[i].fl = 2;
        }
    }
//  --
int end = 0;
QColor cl = QColor( 127, 127, 127, 127 );
//  --
    for( int i = 0; i <  30; i++ ){
//        qDebug() << i+1 <<")\t"<< obj[i].uclock << " - " << obj[i].time << " - " << obj[i].message << " - " << obj[i].status << " - " << obj[i].user << " - " << obj[i].host;
        if( obj[i].message == "SyMon" ){
            if(( obj[i].status == "START" )||( obj[i].status == "STOP" )) end = 1;
        }

        if( end ){
            if( obj[i].fl == 0 ) cl = QColor( 127, 127, 127, 200);
        }else{
            if( obj[i].fl == 1 ) cl = QColor( 0, 127, 0, 255);
            if( obj[i].fl == 2 ) cl = QColor( 0, 0, 255, 155);
            if( obj[i].fl == 3 ) cl = QColor( 255, 0, 0, 255);
        }

        ptwi = new QTableWidgetItem( obj[i].time );
        ptwi->setTextColor ( cl );
        ptbl->setItem( i, 0, ptwi );

        ptwi = new QTableWidgetItem( obj[i].message );
        ptwi->setTextColor ( cl);
        ptbl->setItem( i, 1, ptwi );

        ptwi = new QTableWidgetItem( obj[i].status );
        ptwi->setTextColor ( cl);
        ptbl->setItem( i, 2, ptwi );

        ptwi = new QTableWidgetItem( obj[i].user );
        ptwi->setTextColor ( cl);
        ptbl->setItem( i, 3, ptwi );

        ptwi = new QTableWidgetItem( obj[i].host );
        ptwi->setTextColor ( cl);
        ptbl->setItem( i, 4, ptwi );

        ptwi = new QTableWidgetItem( obj[i].ip );
        ptwi->setTextColor ( cl);
        ptbl->setItem( i, 5, ptwi );

        ptwi = new QTableWidgetItem( obj[i].sessia );
        ptwi->setTextColor ( cl);
        ptbl->setItem( i, 6, ptwi );
    }
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void WxMain::keyPressEvent( QKeyEvent *event ){
//qDebug() << tr("Kl >>") << event->key();
    switch ( event->key()){
        case 16777264:	// F1
            slotActivHelp();
        break;

        case 16777268:	// F5
            upDate();
        break;

        case 16777272:	// F9
            menu->exec( QCursor::pos());
        break;

//        case 16777216:	// esc

        case 16777273:	// F10
            slotActivExit();
        break;
    default:
        QWidget::keyPressEvent( event );
    }
}
// ----------------------------------------------------------------------
void WxMain::slotMenuClicked( QAction* pAction ){
    QString stAName = pAction->text().remove( "&" );
    QString stAID = pAction->objectName();
//    qDebug() << tr("Делай ЭТО! >>") << strActName << " - " << strActID;
    if( stAName == "" ) return;
    if( stAName == tr( "Выход" )) slotActivExit();
    if( stAName == tr( "Обновить" )) upDate();
    if( stAID == "" ) return;
    strActName = stAName;
    strActID   = stAID;
    Fl = 1;
    upDate();
return;
}// End slot
//-----------------------------------------------------------------------
void WxMain::slotTimerRefresh(){
//    qDebug() << tr("WxMain::slotTimerRefresh()");
    if( Fl ) upDate();
}// End slot
// ----------------------------------------------------------------------
void WxMain::slotActivHelp(){
//    QString str = ((QPushButton*)sender())->text();
//    if (str == tr("Справка")){
        QTextEdit *txt = new QTextEdit;
        txt->setReadOnly( true );
        txt->setHtml( tr("<HTML>"
                "<BODY>"
                "<H2><CENTER> SMonitor </CENTER></H2>"
                "<P ALIGN=\"left\">"
                    "Наблюдение за сессиями"
                    "F5 обновление"
                    "F9 меню"
                    "<BR>"
                    "<BR>"
                    "<BR>"
                "</P>"
                "<H3><CENTER> Версия 0.4 </CENTER></H3>"
                "<H4><CENTER> Сентябрь 2015 </CENTER></H4>"
                "<H4><CENTER> Широков О.Ю. </CENTER></H4>"
                "<BR>"
                "</BODY>"
                "</HTML>"
               ));
        txt->resize( 250, 200 );
        txt->show();
return;
}// End slot
// ----------------------------------------------------------------------
void WxMain::slotActivExit(){
//    qDebug() << tr("WxMain::slotActivExit()");
    close();
return;
}// End slot
// ----------------------------------------------------------------------
