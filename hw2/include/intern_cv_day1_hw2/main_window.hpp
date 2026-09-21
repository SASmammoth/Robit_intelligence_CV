/**
 * @file /include/intern_cv_day1_hw2/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date August 2024
 **/

#ifndef intern_cv_day1_hw2_MAIN_WINDOW_H
#define intern_cv_day1_hw2_MAIN_WINDOW_H

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
  void closeEvent(QCloseEvent* event);

  int currentTarget_ = 0;   // 지금 편집 중인 객체 (0=하양,1=파랑,2=오렌지,3=네온)
  void pushHsvToNode();     // 현재 슬라이더 값 수정 및 라벨 글자 수정
  void loadTargetToSliders(int target); // 노드 → 슬라이더 (+라벨)
};

#endif  // intern_cv_day1_hw2_MAIN_WINDOW_H
