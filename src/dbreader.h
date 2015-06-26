/**************************************************************************
 *  Copyright (C)  2013-2015   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 **************************************************************************/
#ifndef _DBReader_h_
#define _DBReader_h_
//  --
#include <QtGui>
#include <QtSql>
#include <QString>
#include "BoxObject.h"
//  --
QT_FORWARD_DECLARE_CLASS( QSqlError )
//----------------------------------------------------------------
class DBReader{
public:
    DBReader( QString driv, QString host, QString name, QString user, QString pass, int port );
    virtual ~DBReader();

    QMenu* MenuRead( QWidget *parent );
    int ObjRead( QString str, BoxObject* o, int i );
//  --
    QString getMsg( void ){return msg;};

private:
    bool Err;
    QString msg;
//  --
    QString dbdriv;
    QString dbhost;
    QString dbname;
    QString dbuser;
    QString dbpass;
    int     dbport;
//  --
    bool connection( void );
};
//----------------------------------------------------------------
#endif
