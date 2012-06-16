//#####################################################################
// Copyright 2010, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _STRUCTURE_CONSTRUCTOR_H_
#define _STRUCTURE_CONSTRUCTOR_H_

#include <QSet>
#include <QVector>
#include "Separator.h"

namespace bricolage{

class BentoBlock;

//#####################################################################
// Class Bento
//#####################################################################
class StructureConstructor
{	
protected:
	QVector<Separator>& mSeparatorList;
	BentoBlock* mRootBlock;
	
public:
	StructureConstructor(QVector<Separator>& separatorList, QSet<BentoBlock*>& blockPool, BentoBlock* rootBlock)
	:mSeparatorList(separatorList), mRootBlock(rootBlock)
	{ if (mSeparatorList.size()==0) attachToParent(blockPool); else mergeBlocks(); }
	
protected:
	void mergeBlocks();
	void attachToParent(QSet<BentoBlock*>& children, bool rootCase=true, int currIndex=0);

//#####################################################################
};
	
//#####################################################################
} // namespace bricolage

#endif // _STRUCTURE_CONSTRUCTOR_H_
