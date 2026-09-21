/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date August 2024
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/intern_cv_day1_hw1/main_window.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindowDesign)
{
  ui->setupUi(this);

  qnode = new QNode();

  my_ball_cv = std::make_shared<My_ball_cv>();
  my_ball_cv->set_label1(ui->display_label1);
  my_ball_cv->set_label2(ui->display_label2);
  my_ball_cv->image_callback(); 
  qnode->set_remote(my_ball_cv);
  qnode->start();

  QObject::connect(qnode, SIGNAL(rosShutDown()), this, SLOT(close()));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
  delete ui;
}
