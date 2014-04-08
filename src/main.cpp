#include <QApplication>
#include "SearchForm.h"
#include "ResultWin.h"
#include "SearchEngine.h"
#include <stdexcept>

int main(int argc, char* argv[])    {
    if(argc < 2) throw std::invalid_argument("Invalid number of command line args");
    // Intialize search engine
    SearchEngine se(argv[1]);
    // Make application
    QApplication app(argc, argv);
    // Make the first form and give it pointer to search engine
    SearchForm form(NULL,&se);
    form.show();
    return app.exec();
}
