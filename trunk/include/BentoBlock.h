//#####################################################################
// Copyright 2012, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _BENTO_BLOCK_H_
#define _BENTO_BLOCK_H_

#include <QRect>
#include <QSet>
#include <QVector>
#include "DOMElement.h"
#include "DOMUtils.h"

namespace bricolage {
    
//#####################################################################
// Class BentoBlock
//#####################################################################
class BentoBlock
{
    
public: 
    uint mBentoID;
	DOMElement* mDOMNode;
    BentoBlock* mParent;  QVector<BentoBlock*> mChildren;
    QRect mGeometry;
    uint mLevel;
    bool mSameSizeContent; // TODO: keep this?

	BentoBlock()
	:mDOMNode(NULL),mParent(NULL)
	{}
	
	BentoBlock(DOMElement* domNode)
	:mDOMNode(domNode),mParent(NULL),mGeometry(domNode->mGeometry),mSameSizeContent(false)
	{
		bool result;
		QRect clipRect = DOMUtils::isPartiallyHidden(*domNode, result);
		if (result) mGeometry = clipRect;
	}
	
	BentoBlock(QSet<BentoBlock*>& blockPool)
	:mDOMNode(NULL),mParent(NULL),mGeometry(boundingRectangle(blockPool)),mSameSizeContent(false)
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
