/**************************************************************************
 *  Copyright (C)  2014-2015   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 **************************************************************************/
#include <QApplication>
#include <QMessageBox>
//  --
#include "dbreader.h"
#include "WxMain.h"
//---------------------------------
int main( int argc, char *argv[] ){
    QApplication app( argc, argv );
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
//  ----------------
    QString id = ( argc > 1 ) ? argv[1] : QWidget::tr("-");
    try{
        DBReader *dbr = new DBReader( "QPSQL", "192.168.1.1", "zabbix", "user", "", -1);
//  ----------------
        WxMain gwx( dbr );
        if( gwx.Error()) exit( EXIT_SUCCESS );
        gwx.setWindowTitle( QApplication::translate( "SMonitor", "SMonitor", 0, QApplication::UnicodeUTF8 ));
        gwx.show();
//  ----------------
        return app.exec();
//  ----------------
    }catch( QString e ){
        qDebug() << "FATAL_ERROR." << e;
        QMessageBox::warning(0, QWidget::tr( "ERROR!" ), e);
        exit( EXIT_FAILURE );
    }catch(...){
        qDebug() << "FATAL_ERROR." << "!!!";
        QMessageBox::warning( 0,"!!!", QWidget::tr( "!!!" ));
        exit( EXIT_FAILURE );
    }
}
// ----------------------------------------------------------------------

