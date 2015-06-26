/**************************************************************************
 *  Copyright (C)  2013-2015   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 **************************************************************************/
#ifndef _WxMain_h_
#define _WxMain_h_

#include <QtGui>
#include "dbreader.h"
// ======================================================================
class WxMain : public QWidget {

    Q_OBJECT

public:
    WxMain( DBReader *db, QWidget *pwgt = 0 );
    int Error( void ){return Err;};

private:
    int Err;
    int Fl;
    QString strActID;
    QString strActName;
    QWidget *parent;
    DBReader *dbr;
//  --
    QTimer *timer;
    QTableWidget *ptbl;

    QMenu *menu;
    QVBoxLayout *lout;
//  --
    void upDate( void );
//  --
protected:
    virtual void contextMenuEvent( QContextMenuEvent *pe ){
        menu->exec( pe->globalPos());
    }
    virtual void keyPressEvent( QKeyEvent *event );

public slots:
    void slotMenuClicked( QAction *pAction );
    void slotActivHelp();
    void slotActivExit();
    void slotTimerRefresh();

//signals:
};
#endif  //_WxMain_h_
//----------------------------------------------------------
