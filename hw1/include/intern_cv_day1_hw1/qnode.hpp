/**
 * @file /include/intern_cv_day1/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef intern_cv_day1_QNODE_HPP_
#define intern_cv_day1_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/
#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif
#include <QThread>
#include "my_ball_cv.hpp" 

/*****************************************************************************
** Class
*****************************************************************************/
class QNode : public QThread
{
  Q_OBJECT
public:
  QNode();
  ~QNode();

  void set_remote(std::shared_ptr<My_ball_cv> remote) { remote_ = remote; } // cv_ball 노드를 스레드로 생성하기 위해

protected:
  void run();

private:
  std::shared_ptr<rclcpp::Node> node;
  std::shared_ptr<My_ball_cv> remote_;

Q_SIGNALS:
  void rosShutDown();
};

#endif /* intern_cv_day1_QNODE_HPP_ */
