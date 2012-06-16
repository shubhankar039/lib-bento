//#####################################################################
// Copyright 2012, Ranjitha Kumar.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <QMainWindow>
#include <QtWebKit>
#include "ui_window.h"
#include "Page.h"
#include "BentoTree.h"
#include "Bento.h"

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
QT_END_NAMESPACE

namespace bricolage {
	
class Window : public QMainWindow, private Ui::Window
{
    Q_OBJECT

private:
	QWebElement mDocument;
	Page* mPage;
	BentoTree* bentoTree;
    QHash<QTreeWidgetItem*, const BentoBlock*> bentoBlockHash;
    QWebElement selectedElement;
	
public:
    Window(QWidget *parent = 0)
	:QMainWindow(parent){ 
		setupUi(this); 
		QObject::connect(webView->page()->currentFrame(), SIGNAL(initialLayoutCompleted()),
                         this, SLOT(on_webView_initialLayoutCompleted()));

	}
	
	~Window()
	{ delete mPage; }
	
    void setUrl(const QUrl &url)
	{ webView->setUrl(url); }

public slots:
	void on_timer_finished();
    void on_webView_loadFinished();
	void on_webView_initialLayoutCompleted();
    void on_treeWidget_itemSelectionChanged();
	void on_pushButton_clicked();
	
private:
    void examineChildElements(const BentoBlock* bentoBlock, QTreeWidgetItem* parentItem);
	void drawSeparators(Bento& bento);

//#####################################################################
};

//#####################################################################
} // namespace bricolage

#endif // _WINDOW_H_
