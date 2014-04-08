#ifndef RESULTWIN_H
#define RESULTWIN_H
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include "WebPage.h"
#include "SearchEngine.h"
#include "Set.h"

class ResultWin : public QWidget    {
    Q_OBJECT

    public:
        ResultWin(QWidget* parent = NULL, SearchEngine* eng = NULL);
        ~ResultWin();

        //Help define initial size
        QSize sizeHint() const  {return QSize(800,600);}
        QSize minimumSizeHint() const   {return QSize(300,200);}

        // Populate the page
        void populate(const std::string fname); // Clears and repopulates the window

    private slots:
        void resultClicked(QListWidgetItem* item);  // If something in a list is clicked

    private:
        QListWidget*    inLinkBox;
        QListWidget*    outLinkBox;
        QHBoxLayout*    mainLayout;
        QVBoxLayout*    linkBar;
        QPlainTextEdit* resultText;

        // Links functionality
        WebPage* wp;
        SearchEngine* se;
        Set<WebPage*> inLinks;
        Set<WebPage*> outLinks;

};

#endif
