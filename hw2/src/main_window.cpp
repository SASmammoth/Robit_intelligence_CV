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

#include "../include/intern_cv_day1_hw2/main_window.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindowDesign)
{
  ui->setupUi(this);

  QIcon icon("://ros-icon.png");
  this->setWindowIcon(icon);

  qnode = new QNode();

  // QNode가 넘긴 QImage를 라벨에 표시.
  connect(qnode, &QNode::usbImageReady, this, [this](const QImage &img)
        { ui->displayUsb->setPixmap(QPixmap::fromImage(img)); }, Qt::QueuedConnection);
connect(qnode, &QNode::findObjectImageReady, this, [this](const QImage &img)
        { ui->displayObject->setPixmap(QPixmap::fromImage(img)); }, Qt::QueuedConnection);
connect(qnode, &QNode::neonConeImageReady, this, [this](const QImage &img)
        { ui->displayNeonCone->setPixmap(QPixmap::fromImage(img)); }, Qt::QueuedConnection);
connect(qnode, &QNode::orange_coneImageReady, this, [this](const QImage &img)
        { ui->displayOrangeCone->setPixmap(QPixmap::fromImage(img)); }, Qt::QueuedConnection);  
connect(qnode, &QNode::whiteLineImageReady, this, [this](const QImage &img)
        { ui->displayWhiteLine->setPixmap(QPixmap::fromImage(img)); }, Qt::QueuedConnection);   
connect(qnode, &QNode::blueLineImageReady, this, [this](const QImage &img)
        { ui->displayBlueLine->setPixmap(QPixmap::fromImage(img)); }, Qt::QueuedConnection);    

  connect(qnode, &QNode::rosShutDown, this, &MainWindow::close);

  // --- 라디오버튼 4개:
  connect(ui->radioBtnWhiteLine, &QRadioButton::clicked, this, [this]
          { loadTargetToSliders(0); });
  connect(ui->radioBtnBlueLine, &QRadioButton::clicked, this, [this]
          { loadTargetToSliders(1); });
  connect(ui->radioBtnOrangeCone, &QRadioButton::clicked, this, [this]
          { loadTargetToSliders(2); });
  connect(ui->radioBtnNeonCone, &QRadioButton::clicked, this, [this]
          { loadTargetToSliders(3); });

  // --- 슬라이더 6개:
  for (QSlider *s : {ui->sliderHueLow, ui->sliderHueHigh,
                     ui->sliderSaturationLow, ui->sliderSaturationHigh,
                     ui->sliderValueLow, ui->sliderValueHigh})
  {
    connect(s, &QSlider::valueChanged, this, [this]
            { pushHsvToNode(); });
  }

  connect(qnode, &QNode::zoneReady, this, [this](const QString &text){
    ui->display_enther->setText(text);
  }, Qt::QueuedConnection);

  // --- 시작 시: 0번(하얀선) 라디오 체크 + 저장값 불러오기 ---
  ui->radioBtnWhiteLine->setChecked(true);
  loadTargetToSliders(0);
}

// 슬라이더 → 노드 (+라벨). 현재 선택된 객체의 값을 갱신
void MainWindow::pushHsvToNode()
{
  int hLow = ui->sliderHueLow->value();
  int hHigh = ui->sliderHueHigh->value();
  int sLow = ui->sliderSaturationLow->value();
  int sHigh = ui->sliderSaturationHigh->value();
  int vLow = ui->sliderValueLow->value();
  int vHigh = ui->sliderValueHigh->value();

  // 슬라이더 옆 라벨에 현재 값 표시
  ui->dispaly_Hue_Low->setNum(hLow);
  ui->dispaly_Hue_High->setNum(hHigh);
  ui->dispaly_Saturation_Low->setNum(sLow);
  ui->dispaly_Saturation_High->setNum(sHigh);
  ui->dispaly_Value_Low->setNum(vLow);
  ui->dispaly_Value_High->setNum(vHigh);

  qnode->updateHsvRange(currentTarget_,
                        hLow, hHigh, sLow, sHigh, vLow, vHigh);
}

// 노드 → 슬라이더 (+라벨). 라디오로 대상 바꿀 때 저장값을 불러옴
void MainWindow::loadTargetToSliders(int target)
{
  currentTarget_ = target;

  int hLow, hHigh, sLow, sHigh, vLow, vHigh;
  qnode->getHsvRange(target, hLow, hHigh, sLow, sHigh, vLow, vHigh);

  // 슬라이더 세팅 중 valueChanged가 튀어 pushHsvToNode가 도로 불리는 것 방지
  const QList<QSlider *> sliders = {
      ui->sliderHueLow, ui->sliderHueHigh,
      ui->sliderSaturationLow, ui->sliderSaturationHigh,
      ui->sliderValueLow, ui->sliderValueHigh};
  for (QSlider *s : sliders)
    s->blockSignals(true);

  ui->sliderHueLow->setValue(hLow);
  ui->sliderHueHigh->setValue(hHigh);
  ui->sliderSaturationLow->setValue(sLow);
  ui->sliderSaturationHigh->setValue(sHigh);
  ui->sliderValueLow->setValue(vLow);
  ui->sliderValueHigh->setValue(vHigh);

  for (QSlider *s : sliders)
    s->blockSignals(false);

  // 라벨 갱신
  ui->dispaly_Hue_Low->setNum(hLow);
  ui->dispaly_Hue_High->setNum(hHigh);
  ui->dispaly_Saturation_Low->setNum(sLow);
  ui->dispaly_Saturation_High->setNum(sHigh);
  ui->dispaly_Value_Low->setNum(vLow);
  ui->dispaly_Value_High->setNum(vHigh);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
  delete ui;
}