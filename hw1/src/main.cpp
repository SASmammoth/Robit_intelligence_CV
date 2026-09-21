#include <QApplication>
#include <iostream>

#include "../include/intern_cv_day1_hw1/main_window.hpp"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
