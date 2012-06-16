//#####################################################################
// Copyright 2012, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#include <QDebug>
#include <QDir>
#include <QTimer>
#include "window.h"
#include "BentoBlock.h"
#include "Bento.h"
using namespace bricolage;
//#####################################################################
// Function on_webView_initialLayoutCompleted
//#####################################################################
void Window::on_webView_initialLayoutCompleted() {
	qDebug() << "Initial Layout Completed";
}
//#####################################################################
// Function :on_webView_loadFinished
//#####################################################################
void Window::on_webView_loadFinished()
{
	QTimer::singleShot(3000, this, SLOT(on_timer_finished()));
}
//#####################################################################
// Function on_pushButton_clicked()
//#####################################################################
void Window::on_pushButton_clicked()
{
	treeWidget->clear();
	webView->setUrl(lineEdit->text());
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	setEnabled(false);
}
//#####################################################################
// Function on_timer_finished
//#####################################################################
void Window::on_timer_finished() 
{
	bentoBlockHash.clear();
    treeWidget->clear();
	
	mDocument = webView->page()->currentFrame()->documentElement();
	mPage = new Page(*webView->page());
    
    //drawSeparators(mPage->mBento);
    examineChildElements(mPage->mBentoTree->mRootBlock, treeWidget->invisibleRootItem());
	
	qApp->restoreOverrideCursor();
	setEnabled(true);

}
//#####################################################################
// Function on_treeWidget_itemSelectionChanged
//#####################################################################
void Window::on_treeWidget_itemSelectionChanged()
{
	if(treeWidget->selectedItems().size()==0) return;
    const BentoBlock* bentoBlock = bentoBlockHash[treeWidget->selectedItems().first()];
	uint thickness = 4;

	mDocument.findFirst("div#BentoTest").removeFromDocument();
	QString tagText = "<div id=\"BentoTest\" style=\"left:" + QString::number(bentoBlock->mGeometry.left()-thickness) + "px; top:" + QString::number(bentoBlock->mGeometry.top()-thickness) + "px; width:" + 
	QString::number(bentoBlock->mGeometry.width()) + "px; height:" + QString::number(bentoBlock->mGeometry.height()) + "px; background-color:transparent; border-width:" + QString::number(thickness) + 
	"px; border-style:dashed; border-color:gold; z-index:400000; position:absolute; float:center; !important\" />";
	mDocument.findFirst("body").prependInside(tagText); 
    
	qDebug() << "left: " << bentoBlock->mGeometry.left()
			 << " top: " << bentoBlock->mGeometry.top()
			 << " width: " << bentoBlock->mGeometry.width()
			 << " height: " << bentoBlock->mGeometry.height()
             << endl;

}
//#####################################################################
// Function examineChildElements
//#####################################################################
void Window::examineChildElements(const BentoBlock* bentoBlock, QTreeWidgetItem* parentItem)
{
	QTreeWidgetItem *item = new QTreeWidgetItem();
	bentoBlockHash[item] = bentoBlock;
	if (!bentoBlock->mDOMNode.isNull())
		item->setText(0, QString::number(bentoBlock->mBentoID) + " " + bentoBlock->mDOMNode.tagName() + " " + bentoBlock->mDOMNode.attribute("id") + " " + bentoBlock->mDOMNode.attribute("class"));
	else
		item->setText(0, QString::number(bentoBlock->mBentoID) + " MERGE BLOCK");

	parentItem->addChild(item);
	
	for(uint i=0; i<(uint)bentoBlock->mChildren.size(); i++) examineChildElements(bentoBlock->mChildren[i], item);
}
//#####################################################################
// Function drawSeparators
//#####################################################################
void Window::drawSeparators(Bento& bento)
{
	for (int i=0; i<bento.mSeparatorList.size(); i++) {
		QString tagText = "<div id=\"BentoTest\" style=\"left:" + QString::number(bento.mSeparatorList[i].left()) + "px; top:" + QString::number(bento.mSeparatorList[i].top()) + "px; width:" + 
							QString::number(bento.mSeparatorList[i].width()) + "px; height:" + QString::number(bento.mSeparatorList[i].height()) + "px; background-color:red; opacity:0.4; z-index:400000; position:absolute; float:center; !important\" />";
		mDocument.findFirst("body").prependInside(tagText); 
	}
}
