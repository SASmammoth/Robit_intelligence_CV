/**
 * @file /include/intern_cv_day1_hw2/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date August 2024
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef intern_cv_day1_hw2_QNODE_HPP_
#define intern_cv_day1_hw2_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/
#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#include "intern_cv_day1_hw2/my_cone_cv.hpp"
#endif
#include <QThread>
#include <QImage>

/*****************************************************************************
** Class
*****************************************************************************/
class QNode : public QThread
{
  Q_OBJECT
public:
  QNode();
  ~QNode();

  void updateHsvRange(int target,
                      int hLow, int hHigh,
                      int sLow, int sHigh,
                      int vLow, int vHigh);

  // 특정 객체의 저장된 HSV 범위를 UI가 읽어올 때 (out 파라미터로 반환)
  void getHsvRange(int target,
                   int &hLow, int &hHigh,
                   int &sLow, int &sHigh,
                   int &vLow, int &vHigh);

protected:
  void run();

private:
  std::shared_ptr<My_cone_cv> node;

Q_SIGNALS:
  void zoneReady(const QString &text);

  void usbImageReady(const QImage &img);
  void findObjectImageReady(const QImage &img);
  void neonConeImageReady(const QImage &img);
  void orange_coneImageReady(const QImage &img);
  void whiteLineImageReady(const QImage &img);
  void blueLineImageReady(const QImage &img);

  void rosShutDown();
};

#endif /* intern_cv_day1_hw2_QNODE_HPP_ */