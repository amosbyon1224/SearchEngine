#include "ResultWin.h"
#include <QObject>

ResultWin::ResultWin(QWidget* parent, SearchEngine* eng) : QWidget(parent), se(eng) {
    // Allocate memory for all widget
    inLinkBox = new QListWidget;
    outLinkBox = new QListWidget;
    mainLayout = new QHBoxLayout;
    linkBar = new QVBoxLayout;
    resultText = new QPlainTextEdit;

    // Make textbox read only
    resultText->setReadOnly(true);

    // Lay out Links
    linkBar->addWidget(inLinkBox);
    linkBar->addWidget(outLinkBox);

    // Lay out main layout
    mainLayout->addWidget(resultText);
    mainLayout->addLayout(linkBar);

    // Set layout
    setLayout(mainLayout);

    // Signals and slots
    connect(inLinkBox,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(resultClicked(QListWidgetItem*)));
    connect(outLinkBox,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(resultClicked(QListWidgetItem*)));

}

ResultWin::~ResultWin() {
    delete inLinkBox;
    delete outLinkBox;
    delete linkBar;
    delete resultText;
    delete mainLayout;
}

void ResultWin::populate(const std::string fname) {
    // Clear all items
    inLinkBox->clear();
    outLinkBox->clear();
    wp = se->lookUp(fname);

    // Set Text
    resultText->setPlainText(QString(wp->rawfile.c_str()));
    // Populate List Widgets
    inLinks = wp->allIncomingLinks();
    outLinks = wp->allOutgoingLinks();

    for(std::set<WebPage*>::iterator it = inLinks.begin(); it != inLinks.end(); ++it)   {
        new QListWidgetItem(QString((*it)->filename().c_str()), inLinkBox);
    }
    for(std::set<WebPage*>::iterator it = outLinks.begin(); it != outLinks.end(); ++it) {
        new QListWidgetItem(QString((*it)->filename().c_str()), outLinkBox);
    }
}

void ResultWin::resultClicked(QListWidgetItem* item)    {
    populate(item->text().toStdString());
}
