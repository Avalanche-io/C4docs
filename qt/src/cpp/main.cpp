#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>

#include "c4asset.h"

using namespace c4;

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  QTextStream cout(stdout);
  if(argc != 4)
  {
    cout << "Usage: C4 Asset id [filename]";
    cout << "(no other commands supported yet.)";
  }
  Asset input;
  input.id(QUrl::fromLocalFile(argv[3]));
  cout << "address: " << input.address() << endl;
  cout << "c4id: " << input.c4id() << endl;
  cout << "type: " << input.type() << endl;
  // return a.exec();
}
