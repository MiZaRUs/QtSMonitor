/**************************************************************************
 *  ReportMaker    BoxObject.h                                            *
 *  Copyright (C)  2012-2013   by  Oleg Shirokov      olgshir@gmail.com   *
 *                                                                        *
 **************************************************************************/
#ifndef _bObject_H_
#define _bObject_H_

#include <QString>
//  ----------------------------------------------------------------
class BoxObject{
public:
    int		uclock;
    int		fl;
    QString 	time;
    QString 	message;
    QString 	status;
    QString 	user;	//
    QString 	host;	//
    QString 	ip;	//
    QString 	sessia;	//
};
//  ----------------------------------------------------------------
#endif
