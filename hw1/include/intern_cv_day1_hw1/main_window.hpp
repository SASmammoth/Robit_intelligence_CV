/**
 * @file /include/intern_cv_day1/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date August 2024
 **/

#ifndef intern_cv_day1_hw1_MAIN_WINDOW_H
#define intern_cv_day1_hw1_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QMainWindow>
#include "QIcon"
#include "qnode.hpp"
#include "ui_mainwindow.h"

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  QNode* qnode;

private:
  Ui::MainWindowDesign* ui;

  std::shared_ptr<My_ball_cv> my_ball_cv;

  void closeEvent(QCloseEvent* event);
};

#endif  // intern_cv_day1_MAIN_WINDOW_H
