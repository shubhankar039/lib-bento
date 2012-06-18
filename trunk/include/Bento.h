//#####################################################################
// Copyright 2010, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _BENTO_H_
#define _BENTO_H_

#include <QVarLengthArray>
#include <QVector>
#include "DOMUtils.h"
#include "DOMTree.h"
#include "Separator.h"

namespace bricolage{

class BentoTree;

//#####################################################################
// Class Bento
//#####################################################################
class Bento
{
public:
	QVector<Separator> mSeparatorList;
	QVector<QString> mNoChildrenTags, mNoSameTagChildren;

protected:
	const DOMTree& mDOMTree;

public:
	Bento(const DOMTree& domTree) : mDOMTree(domTree), mNumMoves(0), mNumRedundant(0)
	{ 
        mNoChildrenTags << "INPUT" << "OPTION" << "OPTGROUP" << "IMG" << "AREA" << "PARAM" << "BR" << "HR" << "PRE" << "CODE" << "TEXTAREA" << "SELECT" << "IFRAME" << "NOFRAME" << "STYLE" << "SCRIPT" << "LINK";
		mNoSameTagChildren << "P" << "FORM" << "H1" << "H2" << "H3" << "H4" << "H5" << "H6" << "STRONG" << "B" << "I" << "SMALL" << "DEL" << "INS" << "OBJECT" << "FIELDSET" << "LABEL";		
    }
	
public:
	void computeBentoTree(BentoTree& bentoTree);

protected:	
	void blockExtractionPass(BentoBlock* bentoBlock);
	void reStructure(BentoBlock* bentoBlock, const BentoTree& bentoTree);
	void atLeastTwoChildren(BentoBlock* bentoBlock) const;
	void removeSameSizeChild(BentoBlock* bentoBlock) const;
	void canvasTagPass(BentoBlock* bentoBlock) const;
	bool divideFurther(BentoBlock* bentoBlock) const;
	bool parentTaboo(BentoBlock* bentoBlock, BentoBlock* potParent) const;
	bool contains(BentoBlock* bentoBlock, BentoBlock* potParent) const;
	void allDOMNodes(BentoBlock* bentoBlock);

public:
	int mNumMoves;
	int mNumRedundant;
	
//#####################################################################
};

//#####################################################################
} // namespace bricolage

#endif // _BENTO_H_
