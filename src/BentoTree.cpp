//#####################################################################
// Copyright 2010, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#include "BentoBlock.h"
#include "BentoTree.h"
using namespace bricolage;
//#####################################################################
// Function ~BentoTree
//#####################################################################
BentoTree::~BentoTree()
{
	for(uint i=0;i<(uint)mPostOrderList.size();i++) 
		delete mPostOrderList[i];
}
//#####################################################################
// Function preprocess
//#####################################################################
void BentoTree::setPostOrderList(BentoBlock* bentoBlock)
{
    for(int i=0;i<bentoBlock->mChildren.size();i++) setPostOrderList(bentoBlock->mChildren[i]);
    bentoBlock->mBentoID=mNumNodes++; mPostOrderList.append(bentoBlock);
}
//#####################################################################
// Function computeHeight
//#####################################################################
void BentoTree::computeHeight(BentoBlock* bentoBlock,uint level)
{
    bentoBlock->mLevel=level;
    if(mHeight<level) mHeight=level;
    for(int i=0;i<bentoBlock->mChildren.size();i++) computeHeight(bentoBlock->mChildren[i],level+1);
}
