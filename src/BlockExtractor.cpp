//#####################################################################
// Copyright 2010, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#include <algorithm>
#include <QDebug>
#include "BlockExtractor.h"
#include "BentoBlock.h"
using namespace bricolage;
//#####################################################################
// BlockExtractor
//#####################################################################
BlockExtractor::BlockExtractor(BentoBlock* rootBlock)
:mRootBlock(rootBlock)
{
	divideDOMTree(mRootBlock->mDOMNode);
}
//#####################################################################
// Function divideDOMTree
//#####################################################################
void BlockExtractor::divideDOMTree(DOMElement* domNode)
{
	qDebug() << domNode->mTagName << " " << domNode->mAttributes["id"] << " " << domNode->mAttributes["class"];
    
	int result = isDividable(domNode);
	if (result == DIVIDE)
        foreach(DOMElement* child, domNode->mElementChildren) divideDOMTree(child);
	else if (result == NOT_DIVIDE)
		mBlockPool.insert(new BentoBlock(domNode)); 
}
//#####################################################################
// Function isDividable
//#####################################################################
int BlockExtractor::isDividable(DOMElement* domNode)
{	
	if (!DOMUtils::isValidNode(*domNode)) return DONE;
	
	int result; QString tag = domNode->mTagName;
	
	result=ruleIsRootBlock(domNode); if (result!=MOVE_ON) return result;
	result=ruleIsHiddenOverflow(domNode); if (result!=MOVE_ON) return result;
	result=ruleBackgroundImage(domNode); if (result!=MOVE_ON) return result;
	result=ruleTextChildren(domNode); if (result!=MOVE_ON) return result;

	if(tag=="TABLE" || tag=="TR") {		
		result=ruleNoValidChildren(domNode); if (result!=MOVE_ON) return result;
		result=ruleOneValidChild(domNode); if (result!=MOVE_ON) return result;
		result=ruleNoChildrenInBBox(domNode); if (result!=MOVE_ON) return result;
		result=ruleDiffBackgroundColorChild(domNode); if (result!=MOVE_ON) return result;
		result=ruleSameSizeAsRoot(domNode); if (result!=MOVE_ON) return result;
	}
	else if(tag=="TD") {
		result=ruleNoValidChildren(domNode); if (result!=MOVE_ON) return result;
		result=ruleOneValidChild(domNode); if (result!=MOVE_ON) return result;
		result=ruleNoChildrenInBBox(domNode); if (result!=MOVE_ON) return result;
		result=ruleAllChildrenVTextNodes(domNode); if (result!=MOVE_ON) return result;
		result=ruleSameSizeAsRoot(domNode); if (result!=MOVE_ON) return result;
	}
	else if(tag!="IMG" && tag!="IFRAME" && tag!="AREA" && tag!="INPUT" && tag!="SELECT" && tag!="CANVAS") {
		result=ruleNoValidChildren(domNode); if (result!=MOVE_ON) return result;
		result=ruleOneValidChild(domNode); if (result!=MOVE_ON) return result;
		result=ruleNoChildrenInBBox(domNode); if (result!=MOVE_ON) return result;
		result=ruleAllChildrenVTextNodes(domNode); if (result!=MOVE_ON) return result;
		if (DOMUtils::isInlineNode(*domNode) || tag == "P") {
			result=ruleLineBreakChild(domNode); if (result!=MOVE_ON) return result; }
		result=ruleHRChild(domNode); if (result!=MOVE_ON) return result;
		result=ruleSameSizeAsRoot(domNode); if (result!=MOVE_ON) return result;
	}
	
	return ruleNotDivide();
}
//#####################################################################
// Function ruleIsRootBlock
//#####################################################################
int BlockExtractor::ruleIsRootBlock(DOMElement* domNode) const
{
	if(domNode == mRootBlock->mDOMNode) { 
		qDebug() << "DIVIDE (ruleIsRootBlock)"; 
		return DIVIDE; 
	}
	return MOVE_ON;
}
//#####################################################################
// Function ruleIsHiddenOverflow
//#####################################################################
int BlockExtractor::ruleIsHiddenOverflow(DOMElement* domNode) const
{	
	if(DOMUtils::isHiddenOverflow(*domNode)) { 
		qDebug() << "DIVIDE (ruleIsHiddenOverflow)"; 
		return DIVIDE; 
	}
	return MOVE_ON;
}
//#####################################################################
// Function ruleBackgroundImage
//#####################################################################
int BlockExtractor::ruleBackgroundImage(DOMElement* domNode) const
{
	if(domNode->mComputedStyles["background-image"]!="none") {
		QRect bBox = domNode->mGeometry;
		if (DOMUtils::hasValidDimensions(bBox)) { qDebug() << "NOT_DIVIDE (ruleBackgroundImage)"; return NOT_DIVIDE; }
		else { qDebug() << "DIVIDE (ruleBackgroundImage)"; return DIVIDE; }
	}
	return MOVE_ON;
}
//#####################################################################
// Function ruleHasTextChildren
//#####################################################################
int BlockExtractor::ruleTextChildren(DOMElement* domNode) const
{
	if(DOMUtils::numTextChildren(*domNode)>0) {
		if (DOMUtils::hasOnlyTextChildren(*domNode)) {
			QRect bBox = domNode->mGeometry;
			int left = DOMUtils::parsePixelFeature(domNode->mComputedStyles["text-indent"]) + bBox.left() + bBox.width();
			if (left<0) { qDebug() << "DONE (ruleTextChildren)"; return DONE; } 
		}
		qDebug() << "NOT_DIVIDE (ruleTextChildren)"; return NOT_DIVIDE; 
	}
 	return MOVE_ON;
}
//#####################################################################
// Function ruleNoValidChildren
//#####################################################################
int BlockExtractor::ruleNoValidChildren(DOMElement* domNode) const
{
	if(DOMUtils::numTextChildren(*domNode) > 0) return MOVE_ON;
    foreach(DOMElement* child, domNode->mElementChildren) if(DOMUtils::isValidNode(*child)) return MOVE_ON;
	qDebug() << "DONE (ruleNoValidChildren)"; 
	return DONE;
}
//#####################################################################
// Function ruleOneValidChild
//#####################################################################
int BlockExtractor::ruleOneValidChild(DOMElement* domNode) const
{
	if(DOMUtils::numTextChildren(*domNode) > 0) return MOVE_ON;
	uint validChildCount = 0;
    foreach(DOMElement* child, domNode->mElementChildren) if(DOMUtils::isValidNode(*child)) validChildCount++; 
	if(validChildCount==1) { 
		qDebug() << "DIVIDE (ruleOneValidChild)"; 
		return DIVIDE; 
	}
	return MOVE_ON;
}
//#####################################################################
// Function ruleNoChildrenInBBox
//#####################################################################
int BlockExtractor::ruleNoChildrenInBBox(DOMElement* domNode) const
{	
	QRect bBox = domNode->mGeometry;
	if(bBox.width()*bBox.height()>0) {
		if(DOMUtils::numTextChildren(*domNode)>0) return MOVE_ON;
		bBox.setRect(bBox.left()-1, bBox.top()-1, bBox.width()+2, bBox.height()+2);
		
		QVector<DOMElement*> descList;
        foreach(DOMElement* child, domNode->mElementChildren) if(DOMUtils::isValidNode(*child)) descList.append(child); 
				
		while (!descList.isEmpty()) {
			DOMElement* currChild = descList.first(); descList.pop_front();
			QRect childBBox = currChild->mGeometry;
			if (childBBox.width()*childBBox.height()==0) {
                foreach(DOMElement* child, currChild->mElementChildren) if (DOMUtils::isValidNode(*child)) descList.append(child);
			}
			else if(bBox.contains(childBBox)) return MOVE_ON;
		}
	}
	qDebug() << "DIVIDE (ruleNoChildrenInBBox)"; 
	return DIVIDE;
}
//#####################################################################
// Function ruleAllChildrenVTextNodes
//#####################################################################
int BlockExtractor::ruleAllChildrenVTextNodes(DOMElement* domNode) const
{	
	foreach(DOMElement* child, domNode->mElementChildren) if (!DOMUtils::isVirtualTextNode(*child)) return MOVE_ON;
	qDebug() << "NOT_DIVIDE (ruleAllChildrenVTextNodes)"; 
	return NOT_DIVIDE;
}
//#####################################################################
// Function ruleLineBreakChild
//#####################################################################
int BlockExtractor::ruleLineBreakChild(DOMElement* domNode) const
{
    foreach(DOMElement* child, domNode->mElementChildren) {
		if (DOMUtils::isLineBreakNode(*child)) { 
			qDebug() << "DIVIDE (ruleLineBreakChild)"; 
			return DIVIDE; 
		}}
	return MOVE_ON;
}
//#####################################################################
// Function ruleHRChild
//#####################################################################
int BlockExtractor::ruleHRChild(DOMElement* domNode) const
{
    foreach(DOMElement* child, domNode->mElementChildren) {
		if (child->mTagName=="HR") { 
			qDebug() << "DIVIDE (ruleHRChild)"; 
			return DIVIDE; 
		}}
	return MOVE_ON;
}
//#####################################################################
// Function ruleDiffBackgroundColorChild
//#####################################################################
int BlockExtractor::ruleDiffBackgroundColorChild(DOMElement* domNode)
{	QColor parentColor, childColor;
	parentColor = DOMUtils::parseColorFeature(domNode->mComputedStyles["background-color"]);
	QSet<DOMElement*> differingChildren;
	foreach(DOMElement* child, domNode->mElementChildren) {
		childColor = DOMUtils::parseColorFeature(child->mComputedStyles["background-color"]);
		if (parentColor!=childColor) differingChildren.insert(child);
	}
	if (differingChildren.size()==0) return MOVE_ON;
	
	foreach(DOMElement* child, domNode->mElementChildren) {
		if(differingChildren.contains(child)) mBlockPool.insert(new BentoBlock(child));
		else divideDOMTree(child);
	}
	qDebug() << "NOT_DIVIDED " << QString::number(differingChildren.size()) << " (ruleDiffBackgroundColorChild)";
	return DONE;
}
//#####################################################################
// Function ruleSameSizeAsRoot
//#####################################################################
int BlockExtractor::ruleSameSizeAsRoot(DOMElement* domNode) const
{
	QRect domRect = domNode->mGeometry;
	float domArea = domRect.width()*domRect.height();
	float rootArea = mRootBlock->mGeometry.width()*mRootBlock->mGeometry.height();
	if (domArea/(1.0*rootArea)>0.85) { 
		qDebug() << "DIVIDE (SameSizeAsRoot)"; 
		return DIVIDE; 
	}
	return MOVE_ON;
}
//#####################################################################
// Function ruleNotDivide
//#####################################################################
int BlockExtractor::ruleNotDivide() const
{
	qDebug() << "NOT_DIVIDE (ruleNotDivide)"; 
	return NOT_DIVIDE;
}