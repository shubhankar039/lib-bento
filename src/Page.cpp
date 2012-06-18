//#####################################################################
// Copyright 2012, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#include "Page.h"
#include "Bento.h"
#include "BentoBlock.h"
using namespace bricolage;
//#####################################################################
// Function Page
//#####################################################################
Page::Page(QWebPage& webPage, int pageID, QString url)
:mPageID(pageID),mURL(url),mBentoTree(new BentoTree())
{
    mDOMTree.init(webPage);
    Bento bento(mDOMTree);
    bento.computeBentoTree(*mBentoTree);
}
