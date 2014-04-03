#ifndef SEARCHFORM_H
#define SEARCHFORM_H
#include <QPushButton>
#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include "SearchEngine.h"
#include "ResultWin.h"
#include "Set.h"

class SearchForm : public QWidget   {
    Q_OBJECT

    public:
        SearchForm(QWidget* parent = NULL, SearchEngine* engine = NULL);
        ~SearchForm();

        //Help define initial size
        QSize sizeHint() const  {return QSize(800,600);}
        QSize minimumSizeHint() const   {return QSize(300,200);}

    private slots:
        void aboutClicked();
        void quitClicked();
        void searchClicked();
        void resultClicked(QListWidgetItem* item);

    private:
        QPushButton*    searchBtn;
        QPushButton*    aboutBtn;
        QPushButton*    quitBtn;
        QLineEdit*      searchTerms;
        QListWidget*    resultBox;
        QHBoxLayout*    searchSection;
        QHBoxLayout*    buttons;
        QVBoxLayout*    mainLayout;

        // Pointer to search engine
        SearchEngine* se;

        // Pointer to Result Window
        ResultWin* reswin;

        // List of dynamic result windows
        Set<WebPage*> results;
        //std::list<QListWidgetItem*> results;
};

#endif
