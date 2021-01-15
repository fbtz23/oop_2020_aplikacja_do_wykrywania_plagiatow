#include "mainwindow.h"
#include <QApplication>
#include "prepare.h"
#include "levenshteindistance.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    std::ifstream ifs("../project/test");
    std::string str;
    getline(ifs,str,'\0');
    std::cout << "Przed: \n" << str << std::endl;
    Prepare p{"../project/cppkeywords.txt"};
    LevenshteinDistance ld{};
    std::string test = p.DeleteComments(str);
    std::cout<< "Po:\n" << test << std::endl;
    std::cout<< "podobienstwo " << ld.compare(str,"abba");
    //return a.exec();
    return 0;

}
