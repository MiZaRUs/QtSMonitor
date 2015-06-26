/**************************************************************************
 *  Copyright (C)  2013-2015   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 **************************************************************************/
#include <QtGui>
#include <stdio.h>
#include "dbreader.h"

//  --
QSqlError sqlErr;
// ----------------------------------------------------------------------
static bool createConnection( QString drvName, QString dbName, QString hstName, QString usrName, QString passw, int prt ){
    QSqlDatabase db = QSqlDatabase::addDatabase( drvName );
    db.setDatabaseName( dbName );
    db.setHostName( hstName );
    db.setPort( prt );
    db.setUserName( usrName );
    db.setPassword( passw );
    if( !db.open()){
        sqlErr = db.lastError();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase( "qt_sql_default_connection" );
        return false;
    }
return true;
}
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
DBReader::DBReader( QString driv, QString host, QString name, QString user, QString pass, int port ){
//qDebug() << "DBReader::DBReader()";
    msg  = "DBReader. Поиск драйвера.";
    Err = true;
//    query = NULL;
//  --
    if( !QSqlDatabase::drivers().isEmpty()){
        QStringList drs = QSqlDatabase::drivers();
        if( drs.contains( driv )){
            Err = false;
        }
    }
//  --
    if( !Err ){
        dbdriv = driv;
        dbhost = host;
        dbname = name;
        dbuser = user;
        dbpass = pass;
        dbport = port;
        msg  = "DBReader. Ok.";
    }
//  --
}// End DBReader::DBReader
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
QMenu* DBReader::MenuRead( QWidget *parent ){
//qDebug() << "DBReader::MenuRead();";
    if( Err )throw QString( msg );
    msg  = "DBReader::MenuRead() - ";
    QMenu *pmnu = NULL;
    pmnu = new QMenu( parent );
//  --
    if (!createConnection( dbdriv, dbname, dbhost, dbuser, dbpass, -1)){
        msg += sqlErr.text();
        Err = true;
        throw QString( msg );
    }
    QSqlQuery *query = new QSqlQuery();
    QString sql;
//  --
// получить все имеющиеся хосты из БД zabbix имеющие соответствующие записи от SessionMonitor
    sql = QString( "SELECT DISTINCT split_part(value,';',1), hosts.name FROM history_text, hosts  WHERE split_part(value,';',1) = hosts.host AND split_part(value,';',3) like 'SM' ORDER BY hosts.name limit 300" );
//qDebug() << "sql: " << sql;
    if( query->exec( sql )){
        while( query->next()){
            QString area = query->value(0).toString();
            QString name = query->value(1).toString();
            pmnu->addAction( name )->setObjectName( area );
//qDebug() << "- " << area;
        }// while
        msg += "Ok!";
    }else{
        msg += "QSqlQuery(" + sql + ");";
        throw QString(msg);
    }
    pmnu->addSeparator();
    pmnu->addAction( QWidget::tr( "&Обновить" ));
    pmnu->addSeparator();
    pmnu->addAction( QWidget::tr( "&Выход" ));
//  --
    msg += " Ok.";
    QSqlDatabase::removeDatabase( "qt_sql_default_connection" );
    delete query;
    query = NULL;
return pmnu;
}//End MenuRead
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
int  DBReader::ObjRead( QString obj, BoxObject *ob, int max_obj ){ 
//qDebug() << "DBReader::ObjRead();";
    if( Err )throw QString( msg );
    msg  = "DBReader::ObjRead() - ";
//  --
    if ( !createConnection( dbdriv, dbname, dbhost, dbuser, dbpass, -1 )){
        msg += sqlErr.text();
        Err = true;
        throw QString( msg );
    }
    QSqlQuery *query = new QSqlQuery();
    QString sql;
//  --
// Читаем последние несколько(max_obj) записей лога SessionMonitor
    int i = 0;		//0			time  1			message 2		status  3		session  4		user   5		host  6				ip  7
    sql = QString( "SELECT clock, split_part(value,';',2), split_part(value,';',4), split_part(value,';',5), split_part(value,';',6), split_part(value,';',7), split_part(value,';',8), split_part(value,';',9) FROM history_text WHERE split_part(value,';',3) like 'SM' AND split_part(value,';',1) like '%1' ORDER BY clock desc limit %2").arg( obj ).arg( max_obj );
//qDebug() << "sql2: " << sql;
    if( query->exec( sql )){
        while( query->next()){
            ob[i].fl		= 0;
            ob[i].uclock	= query->value(0).toInt();	//clock
            ob[i].time		= query->value(1).toString();	//time
            ob[i].message	= query->value(2).toString();	//message
            ob[i].status	= query->value(3).toString();	//status
            ob[i].sessia	= query->value(4).toString();	//session
            ob[i].user		= query->value(5).toString();	//user
            ob[i].host		= query->value(6).toString();	//host
            ob[i].ip		= query->value(7).toString();	//ip
//qDebug() << ob[i].time << " - " << ob[i].message << " - " << ob[i].status << " - " << ob[i].user << " - " << ob[i].host;
            i++;
            if( i >= max_obj ) break;
        }// while
    }else{
        msg += "QSqlQuery(" + sql + ");";
        throw QString( msg );
    }

//  --
//qDebug() << "End.";
//  --
    msg += " Ok.";
    QSqlDatabase::removeDatabase( "qt_sql_default_connection" );
    delete query;
    query = NULL;

return i - 1;
}//End ReadMenu
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
DBReader::~DBReader(){
    QSqlDatabase::removeDatabase( "qt_sql_default_connection" );
}// End
//  -------------------------------------------------------------------------
