//#####################################################################
// Copyright 2012, Ranjitha Kumar. All rights reserved.
// This software is governed by the BSD 2-Clause License.
//#####################################################################
#ifndef _BENTO_BLOCK_H_
#define _BENTO_BLOCK_H_

#include <QRect>
#include <QSet>
#include <QVector>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>
#include "DOMUtils.h"

namespace bricolage {
    
//#####################################################################
// Class BentoBlock
//#####################################################################
class BentoBlock
{
    
public: 
    uint mBentoID;
	QWebElement mDOMNode;
    BentoBlock* mParent;  QVector<BentoBlock*> mChildren;
    QRect mGeometry;
    uint mLevel;
    bool mSameSizeContent; // TODO: keep this?

	BentoBlock()
	:mParent(NULL)
	{}
	
	BentoBlock(QWebElement domNode)
	:mDOMNode(domNode),mParent(NULL),mGeometry(DOMUtils::getGeometry(domNode)),mSameSizeContent(false)
	{
		bool result;
		QRect clipRect = DOMUtils::isPartiallyHidden(domNode, result);
		if (result) mGeometry = clipRect;
	}
	
	BentoBlock(QSet<BentoBlock*>& blockPool)
	:mParent(NULL),mGeometry(boundingRectangle(blockPool)),mSameSizeContent(false)
	{}
	
	~BentoBlock()
	{}
	
	static QRect boundingRectangle(const QSet<BentoBlock*>& unionBlocks)
	{
		QRect bounding;
		foreach (BentoBlock* BentoBlock, unionBlocks) bounding = bounding.united(BentoBlock->mGeometry);
		return bounding;
	}
	
	static bool topBottomLeftRight(const BentoBlock* lhs, const BentoBlock* rhs) 
	{ if(lhs->mGeometry.top() == rhs->mGeometry.top()) return lhs->mGeometry.left() < rhs->mGeometry.left();
	else return lhs->mGeometry.top() < rhs->mGeometry.top();
	} 
	
	static bool leftRightTopBottom(const BentoBlock* lhs, const BentoBlock* rhs) 
	{ if(lhs->mGeometry.left() == rhs->mGeometry.left()) return lhs->mGeometry.top() < rhs->mGeometry.top();
	else return lhs->mGeometry.left() < rhs->mGeometry.left();
	} 
	
	static bool increasingArea(const BentoBlock* lhs, const BentoBlock* rhs) 
	{  
		return (lhs->mGeometry.width()*lhs->mGeometry.height()) < (rhs->mGeometry.width()*rhs->mGeometry.height());
	} 
	
//#####################################################################
};


//#####################################################################
} // namespace bricolage

#endif // _BENTO_BLOCK_H_
