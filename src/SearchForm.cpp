#include "SearchForm.h"
#include <QMessageBox>

SearchForm::SearchForm(QWidget* parent, SearchEngine* engine) : QWidget(parent), se(engine) {
    // Hide results window?
    reswin = new ResultWin(NULL,se);
    reswin->hide();

    // Allocate memory for all widgets
    searchBtn = new QPushButton("&Search");
    aboutBtn = new QPushButton("&About");
    quitBtn = new QPushButton("&Quit");
    searchTerms = new QLineEdit;
    resultBox = new QListWidget;
    pageRank = new QRadioButton("&PageRank");
    alphabet = new QRadioButton("&Alphabetical");
    searchSection = new QHBoxLayout;
    buttons = new QHBoxLayout;
    radioButtons = new QHBoxLayout;
    mainLayout = new QVBoxLayout;
    buttonBox = new QGroupBox("Sort results by...");

    // Lay out buttons
    buttons->addWidget(aboutBtn);
    buttons->addWidget(quitBtn);

    // Lay out search sections
    searchSection->addWidget(searchTerms);
    searchSection->addWidget(searchBtn);

    // Lay out radio buttons, with pagerank as default
    pageRank->setChecked(true);
    radioButtons->addWidget(pageRank);
    radioButtons->addWidget(alphabet);
    buttonBox->setLayout(radioButtons);

    // Lay out everything
    mainLayout->addLayout(searchSection);
    mainLayout->addWidget(buttonBox);
    mainLayout->addWidget(resultBox);
    mainLayout->addLayout(buttons);
    
    // Signals and slots
    connect(aboutBtn,SIGNAL(clicked()),this,SLOT(aboutClicked()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(quitClicked()));
    connect(searchBtn,SIGNAL(clicked()),this,SLOT(searchClicked()));
    connect(resultBox,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(resultClicked(QListWidgetItem*)));

    // Set layout
    setLayout(mainLayout);
}

SearchForm::~SearchForm()   {
    delete searchBtn;
    delete aboutBtn;
    delete quitBtn;
    delete searchTerms;
    delete resultBox;
    delete searchSection;
    delete buttons;
    delete mainLayout;
}

void SearchForm::aboutClicked() {
    QMessageBox::information(this, tr("About"), tr("Search Engine Application"));
}

void SearchForm::quitClicked()  {
    close();
    reswin->close();
}

void SearchForm::searchClicked()    {
    resultBox->clear();
    results = se->processSearch(searchTerms->text().toStdString(),pageRank->isChecked());
    for(std::deque<WebPage*>::iterator it = results.begin(); it != results.end(); ++it)    {
        new QListWidgetItem(QString((*it)->filename().c_str()), resultBox);
    }
}

void SearchForm::resultClicked(QListWidgetItem* item)   {
    // If hidden, show it
    if(reswin->isHidden()) reswin->show();
    reswin->populate(item->text().toStdString());
}
