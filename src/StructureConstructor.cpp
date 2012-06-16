//#####################################################################
// Copyright 2010, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#include "StructureConstructor.h"
#include "BentoBlock.h"
using namespace bricolage;
//#####################################################################
// Function mergeBlocks
//#####################################################################
void StructureConstructor::mergeBlocks()
{	
	qStableSort(mSeparatorList.begin(), mSeparatorList.end(), Separator::lessThanWeight);
	int separatorIndex = 0;	
    // go thru all the separators
	while(separatorIndex < mSeparatorList.size()) {
        // order separators by weight
		int minWeight = mSeparatorList[separatorIndex].mWeight;
		QVector<QSet<BentoBlock*> > mergeBlockList;
        // tempblock pool has all the blocks by min weight 
		QSet<BentoBlock*> tempBlockPool = mSeparatorList[separatorIndex].blocks();
        // add blocks to mergeBlocklist (has only the first )
		mergeBlockList.append(tempBlockPool);
		separatorIndex++;
		
		while(separatorIndex < mSeparatorList.size()) {
			if(mSeparatorList[separatorIndex].mWeight > minWeight) break;
			
			tempBlockPool = mSeparatorList[separatorIndex].blocks(); // has two
			QVector<int> mergeIndexArray;
			
			for (int i=mergeBlockList.size()-1; i>=0; i--) {
				QSet<BentoBlock*> intersectBlockPool = mergeBlockList[i]; // has only one
				if (!intersectBlockPool.intersect(tempBlockPool).isEmpty()) mergeIndexArray.append(i);
			}
			 
			foreach (int index, mergeIndexArray) tempBlockPool = tempBlockPool.unite(mergeBlockList[index]);
			mergeBlockList.append(tempBlockPool); 
            foreach (int index, mergeIndexArray) mergeBlockList.remove(index);
					
			separatorIndex++;
		}
		
		if (separatorIndex < mSeparatorList.size()) foreach(QSet<BentoBlock*> blockPool, mergeBlockList) attachToParent(blockPool, false, separatorIndex);
		else attachToParent(mergeBlockList[0]);
	}
}
//#####################################################################
// Function attachToParent
//#####################################################################
void StructureConstructor::attachToParent(QSet<BentoBlock*>& children, bool rootCase, int currIndex)
{
	BentoBlock* parent = mRootBlock;
	if (!rootCase) {
		parent = new BentoBlock(children);
        
		for(int i=currIndex; i<mSeparatorList.size(); i++) {
			bool blocksRemoved = false;
			foreach(BentoBlock* bentoBlock, children) 
                if(mSeparatorList[i].mBlocks.remove(bentoBlock)) blocksRemoved=true;
			if (blocksRemoved) mSeparatorList[i].mBlocks.insert(parent);
	
        }
	}
	
	foreach(BentoBlock* bentoBlock, children) { bentoBlock->mParent = parent; parent->mChildren.append(bentoBlock); }
	qStableSort(parent->mChildren.begin(), parent->mChildren.end(), BentoBlock::topBottomLeftRight);

}