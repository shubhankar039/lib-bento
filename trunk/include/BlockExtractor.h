//#####################################################################
// Copyright 2010, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _BLOCK_EXTRACTOR_H_
#define _BLOCK_EXTRACTOR_H_

#include <QSet>
#include <QVarLengthArray>
#include "DOMElement.h"

namespace bricolage{

class BentoBlock;

//#####################################################################
// Class BlockExtractor
//#####################################################################
class BlockExtractor
{
public:
	QSet<BentoBlock*> mBlockPool;
	
protected:
	enum {DIVIDE, NOT_DIVIDE, MOVE_ON, DONE};
	BentoBlock* mRootBlock;

public:	
	~BlockExtractor()
	{}

public:
	BlockExtractor(BentoBlock* rootBlock);

protected:
	void divideDOMTree(DOMElement* domNode);
	int isDividable(DOMElement* domNode);
	
	int ruleIsRootBlock(DOMElement* domNode) const;
	int ruleIsHiddenOverflow(DOMElement* domNode) const;
	int ruleBackgroundImage(DOMElement* domNode) const;
	int ruleTextChildren(DOMElement* domNode) const;
	int ruleNoValidChildren(DOMElement* domNode) const;
	int ruleOneValidChild(DOMElement* domNode) const;
	int ruleNoChildrenInBBox(DOMElement* domNode) const;
	int ruleAllChildrenVTextNodes(DOMElement* domNode) const;
	int ruleLineBreakChild(DOMElement* domNode) const;
	int ruleHRChild(DOMElement* domNode) const;
	int ruleDiffBackgroundColorChild(DOMElement* domNode);
	int ruleSameSizeAsRoot(DOMElement* domNode) const;
	int ruleNotDivide() const;

//#####################################################################
};
	
//#####################################################################
} // namespace bricolage

#endif //  _BLOCK_EXTRACTOR_H_
