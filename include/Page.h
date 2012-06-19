//#####################################################################
// Copyright 2012, Ranjitha Kumar. All rights reserved.
// This software is governed by the BSD 2-Clause License.
//#####################################################################
#ifndef _PAGE_H_
#define _PAGE_H_

#include <QList>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>
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
    QList<QWebElement> mDOMNodes;
    BentoTree* mBentoTree;
	
public:
    Page(QWebPage& webPage, int pageID=0, QString url="");

	~Page() 
	{ delete mBentoTree; }

private:
    void setDOMNodes(const QWebElement& domNode);

//#####################################################################
};

//#####################################################################
} // namespace bricolage.database

#endif // _PAGE_H_



