//#####################################################################
// Copyright 2012, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _DOM_UTILS_H_
#define _DOM_UTILS_H_

#include <QColor>
#include <QDebug>
#include <QRect>
#include <QRegExp>
#include <QSet>
#include <QStringList>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>
#include "DOMElement.h"

namespace bricolage{

//#####################################################################
// Class DOMUtils
//#####################################################################
class DOMUtils
{
	
public:
    static bool isInlineNode(const DOMElement& domNode)
	{ return !(isLineBreakNode(domNode)); }
    
	static bool isLineBreakNode(const DOMElement& domNode)
	{ return (domNode.mComputedStyles["display"] == "block"); }
    
	static int numTextChildren(const DOMElement& domNode)
	{ 
		int numTextNodes = 0;
        foreach(DOMElement* child, domNode.mChildren)
        if (child->mNodeType==TYPE_TEXT && child->mNodeValue.trimmed()!="") numTextNodes++;
        return numTextNodes;
	}
	
	static bool hasOnlyTextChildren(const DOMElement& domNode)
	{ return (domNode.mChildren.size() == numTextChildren(domNode)); }
	
	static QString textInNode(const DOMElement& domNode)
	{
		QString nodeText = "";
		QVector<DOMElement> nodeStack; nodeStack.append(domNode);
		while(nodeStack.size()>0){
			DOMElement currNode = nodeStack.front(); nodeStack.pop_front();
            if(currNode.mNodeType==TYPE_TEXT) nodeText += currNode.mNodeValue.trimmed() + " ";
            foreach(DOMElement* child, currNode.mChildren) nodeStack.append(*child);
        }
		return nodeText;
	}
	
	static bool isVirtualTextNode(const DOMElement& domNode)
	{
		if (isInlineNode(domNode)) {
			if (hasOnlyTextChildren(domNode)) return true;
            foreach(DOMElement* child, domNode.mChildren) if (!isVirtualTextNode(*child)) return false;
            return true; 
        }
		return false;
	}
    
    static bool hasValidDimensions(const QRect& bBox) 
	{ return (bBox.width()>5 && bBox.height()>5 && bBox.width()*bBox.height()>100); }

	static bool isValidNode(const DOMElement& domNode)
	{		
		if ((domNode.mTagName == "SCRIPT") || (domNode.mTagName == "STYLE"))
			return false;
		
		if (domNode.mComputedStyles["display"] == "none" ||
			domNode.mComputedStyles["visibility"] == "hidden" ||
			domNode.mComputedStyles["opacity"].toFloat() == 0.0)
			return false;
		
		QRect bBox = domNode.mGeometry;
		if (bBox.left()+bBox.width()<0 || bBox.top()+bBox.height()<0 || !hasValidDimensions(bBox)) {
			foreach(DOMElement* child, domNode.mChildren) if(isValidNode(*child)) return true; 
			return false;
		}
		
		return true;
	}
	
	static bool isHiddenOverflow(const DOMElement& domNode)
	{	
		QRect bBox = domNode.mGeometry;
		DOMElement* parentNode = domNode.mParent;
        while (parentNode!=NULL) {
			while (parentNode!=NULL) {
				if(parentNode->mComputedStyles["overflow"] == "hidden") break;
				parentNode = parentNode->mParent;
			}
			
			if (parentNode!=NULL) {
				QRect parentBBox = parentNode->mGeometry;
				if(!parentBBox.intersects(bBox)) return true;
				parentNode = parentNode->mParent;
			}
		}
		return false;
	}
	
	static QRect isPartiallyHidden(const DOMElement& domNode, bool& result)
	{	
		QRect bBox = domNode.mGeometry;
		DOMElement* parentNode = domNode.mParent;
		while (parentNode!=NULL) {
			while (parentNode!=NULL) {
				if(parentNode->mComputedStyles["overflow"] == "hidden") break;
				parentNode = parentNode->mParent;
			}
			
			if (parentNode!=NULL) {
				QRect parentBBox = parentNode->mGeometry;
				if(parentBBox.intersects(bBox)) {
					parentBBox.setRect(parentBBox.left()-1, parentBBox.top()-1, parentBBox.width()+2, parentBBox.height()+2);
					if (!parentBBox.contains(bBox)) { result=true; return parentBBox.intersected(bBox); }
				}
				parentNode = parentNode->mParent;
			}
		}
		
		result = false; return QRect(0,0,0,0);
	}
    
    static void addNonElementNodes(QWebElement& webNode, DOMElement* domNode)
    {
        int numChildren = webNode.evaluateJavaScript("this.childNodes.length").toInt();
		for (int i=0; i<numChildren; i++) {
			QString jScript = "this.childNodes["+QString::number(i)+"].nodeType";
			if (webNode.evaluateJavaScript(jScript).toInt()!=TYPE_ELEMENT) {
                DOMElement* d = new DOMElement();
                d->mNodeType = webNode.evaluateJavaScript(jScript).toInt();
				jScript = "this.childNodes["+QString::number(i)+"].nodeValue";
                d->mNodeValue = webNode.evaluateJavaScript(jScript).toString();
                d->mParent = domNode;
                domNode->mChildren.append(d);
			}
		}
        
    }
    
	static int parsePixelFeature (const QString& pixelString)
	{
		int pixelFeature; bool okay;
		QString conversionString = pixelString;
		conversionString.remove(QRegExp("px"));

		pixelFeature = conversionString.toInt(&okay);
		if(!okay) return 0;
		return pixelFeature;
	}
	
	static QColor parseColorFeature(const QString& colorString)
	{
		bool okay; QColor colorFeature;
		QString conversionString = colorString;
		conversionString.replace("transparent", "rgba(0,0,0,0)");
		conversionString.remove(QRegExp("[rgba]")).chop(1); 
		conversionString=conversionString.mid(1);
		QStringList rgba = conversionString.split(",");
		
		Q_ASSERT(rgba.size()==3 || rgba.size()==4);
		int r = rgba[0].toInt(&okay); Q_ASSERT(okay);
		int g = rgba[1].toInt(&okay); Q_ASSERT(okay);
		int b = rgba[2].toInt(&okay); Q_ASSERT(okay);
		int a = 255;
		if(rgba.size()==4) { a = rgba[3].toInt(&okay); Q_ASSERT(okay); }
		colorFeature.setRgb(r,g,b,a);
		return colorFeature;
	}
	
	static QRect getGeometry(QWebElement& domNode)
	{	
		QRect r = QRect(0,0,0,0);
		r.setX(domNode.evaluateJavaScript("jQuery(this).offset().left;").toInt());
		r.setY(domNode.evaluateJavaScript("jQuery(this).offset().top;").toInt());
		r.setWidth(domNode.evaluateJavaScript("parseInt(this.offsetWidth);").toInt());
		r.setHeight(domNode.evaluateJavaScript("parseInt(this.offsetHeight);").toInt());
		//r.setWidth(domNode.evaluateJavaScript("jQuery(this).outerWidth();").toInt());
		//r.setHeight(domNode.evaluateJavaScript("jQuery(this).outerHeight();").toInt());	
		if (domNode.tagName()=="BODY") {
			r.setWidth(std::max(r.width(),domNode.webFrame()->contentsSize().width()));	
			r.setHeight(std::max(r.height(),domNode.webFrame()->contentsSize().height()));	
		}
		return r;
	}
	
	static void getPostOrderList(QList<QWebElement>& postOrderList, const QWebElement& domNode)
	{
	    QWebElement domChild = domNode.firstChild();
        while (!domChild.isNull()) {
            getPostOrderList(postOrderList, domChild);
            domChild = domChild.nextSibling();
        }

        postOrderList.append(domNode);
	}
	
	static QVariantMap getAttributes(QWebElement& domNode)
	{
        QVariantMap attributes;
        QStringList attributeNames = domNode.attributeNames();
        foreach(QString attr, attributeNames)
            attributes[attr] = domNode.attribute(attr);
            
        return attributes;
	}
	
	static QVariantMap getStyles(QWebElement& domNode)
	{
        QVariantMap styles;
        QStringList styleNames;
        styleNames << "color" << "direction" << "display" << "font" << "font-family" << "font-size" << "font-style" << "font-variant" << "font-weight" << "text-rendering" << "background" << "background-attachment" << "background-clip" << "background-color" << "background-image" << "background-origin" << "background-position" << "background-position-x" << "background-position-y" << "background-repeat" << "background-repeat-x" << "background-repeat-y" << "background-size" << "border" << "border-bottom" << "border-bottom-color" << "border-bottom-left-radius" << "border-bottom-right-radius" << "border-bottom-style" << "border-bottom-width" << "border-collapse" << "border-color" << "border-image" << "border-image-outset" << "border-image-repeat" << "border-image-slice" << "border-image-source" << "border-image-width" << "border-left" << "border-left-color" << "border-left-style" << "border-left-width" << "border-radius" << "border-right" << "border-right-color" << "border-right-style" << "border-right-width" << "border-spacing" << "border-style" << "border-top" << "border-top-color" << "border-top-left-radius" << "border-top-right-radius" << "border-top-style" << "border-top-width" << "border-width" << "bottom" << "box-shadow" << "box-sizing" << "clear" << "clip" << "content" << "counter-increment" << "counter-reset" << "cursor" << "empty-cells" << "float" << "font-stretch" << "image-rendering" << "letter-spacing" << "list-style" << "list-style-image" << "list-style-position" << "list-style-type" << "margin" << "margin-bottom" << "margin-left" << "margin-right" << "margin-top" << "max-height" << "max-width" << "min-height" << "min-width" << "opacity" << "orphans" << "outline" << "outline-color" << "outline-offset" << "outline-style" << "outline-width" << "overflow" << "overflow-x" << "overflow-y" << "padding" << "padding-bottom" << "padding-left" << "padding-right" << "padding-top" << "page" << "page-break-after" << "page-break-before" << "page-break-inside" << "pointer-events" << "position" << "quotes" << "resize" << "size" << "src" << "speak" << "table-layout" << "tab-size" << "text-align" << "text-decoration" << "text-indent" << "text-line-through" << "text-line-through-color" << "text-line-through-mode" << "text-line-through-style" << "text-line-through-width" << "text-overflow" << "text-overline" << "text-overline-color" << "text-overline-mode" << "text-overline-style" << "text-overline-width" << "text-shadow" << "text-transform" << "text-underline" << "text-underline-color" << "text-underline-mode" << "text-underline-style" << "text-underline-width" << "unicode-bidi" << "unicode-range" << "vertical-align" << "visibility" << "white-space" << "widows" << "word-break" << "word-spacing" << "word-wrap" << "z-index";
        foreach(QString style, styleNames)
        {
            QString s = domNode.styleProperty(style, QWebElement::ComputedStyle);
            if(s != "")
                styles[style] = s;
        }    
        
        return styles;
	}

	static int fontWeightToInt(QString conversionString)
	{
		int fontWeight; bool okay; 
		if (conversionString=="normal" || conversionString=="") conversionString = "400";
		else if (conversionString=="bold") conversionString = "700";

		fontWeight = conversionString.toInt(&okay);
        qDebug() << "fontWeight: " + QString::number(fontWeight);
        qDebug() << "conversionString: " + conversionString;
        qDebug() << "ok: " + QString::number(okay);
		Q_ASSERT(okay);
		return fontWeight;
	}
				
//#####################################################################
};
	
//#####################################################################
} // namespace bricolage

#endif // _DOM_UTILS_H_
