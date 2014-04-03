#include <QApplication>
#include "SearchForm.h"
#include "ResultWin.h"
#include "SearchEngine.h"
#include <stdexcept>

int main(int argc, char* argv[])    {
    if(argc < 2) throw std::invalid_argument("Invalid number of command line args");
    SearchEngine se(argv[1]);
    QApplication app(argc, argv);
    SearchForm form(NULL,&se);
    form.show();
    return app.exec();
}
