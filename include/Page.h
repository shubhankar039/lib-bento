//#####################################################################
// Copyright 2012, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _PAGE_H_
#define _PAGE_H_

#include <QWebPage>
#include "DOMTree.h"
#include "BentoTree.h"

namespace bricolage {

class BentoBlock;
    
//#####################################################################
// Class Page
//#####################################################################
class Page
{
    
public:
    uint mPageID;
    QString mURL;
    DOMTree mDOMTree;
    BentoTree* mBentoTree;
	
public:
    Page(QWebPage& webPage, int pageID=0, QString url="");

	~Page() 
	{ delete mBentoTree; }

//#####################################################################
};

//#####################################################################
} // namespace bricolage.database

#endif // _PAGE_H_



