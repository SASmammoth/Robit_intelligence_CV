/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date August 2024
 **/

#include "../include/intern_cv_day1_hw2/qnode.hpp"
#include <opencv2/opencv.hpp>

QNode::QNode()
{
  int argc = 0;
  char **argv = NULL;
  rclcpp::init(argc, argv);
  node = std::make_shared<My_cone_cv>();

  // Node가 넘겨준 cv::Mat(BGR) → QImage 변환 → 시그널로 emit
  node->set_usb_cb([this](const cv::Mat &bgr)
                   {
                     cv::Mat rgb;
                     cv::cvtColor(bgr, rgb, cv::COLOR_BGR2RGB);
                     QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
                     Q_EMIT usbImageReady(img.copy()); });
  node->set_find_object_cb([this](const cv::Mat &bgr)
                           {
                     cv::Mat rgb;
                     cv::cvtColor(bgr, rgb, cv::COLOR_BGR2RGB);
                     QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
                     Q_EMIT findObjectImageReady(img.copy()); });
  node->set_neon_cone_cb([this](const cv::Mat &bgr)
                         {
                     cv::Mat rgb;
                     cv::cvtColor(bgr, rgb, cv::COLOR_GRAY2BGR);
                     QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
                     Q_EMIT neonConeImageReady(img.copy()); });
  node->set_orange_cone_cb([this](const cv::Mat &bgr)
                           {
                     cv::Mat rgb;
                     cv::cvtColor(bgr, rgb, cv::COLOR_GRAY2BGR);
                     QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
                     Q_EMIT orange_coneImageReady(img.copy()); });
  node->set_white_line_cb([this](const cv::Mat &bgr)
                          {
                     cv::Mat rgb;
                     cv::cvtColor(bgr, rgb, cv::COLOR_GRAY2BGR);
                     QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
                     Q_EMIT whiteLineImageReady(img.copy()); });
  node->set_blue_line_cb([this](const cv::Mat &bgr)
                         {
                     cv::Mat rgb;
                     cv::cvtColor(bgr, rgb, cv::COLOR_GRAY2BGR);
                     QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
                     Q_EMIT blueLineImageReady(img.copy()); });

  node->set_zone_cb([this](const std::string &s)
                    { Q_EMIT zoneReady(QString::fromStdString(s)); });

  this->start();
}

QNode::~QNode()
{
  if (rclcpp::ok())
    rclcpp::shutdown();
}

void QNode::run()
{
  rclcpp::WallRate loop_rate(20);
  while (rclcpp::ok())
  {
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  Q_EMIT rosShutDown();
}

void QNode::updateHsvRange(int target,
                           int hLow, int hHigh,
                           int sLow, int sHigh,
                           int vLow, int vHigh)
{
  if (!node)
    return;
  node->set_hsv_range(
      static_cast<My_cone_cv::Target>(target),
      cv::Scalar(hLow, sLow, vLow),     // lower: H,S,V 순서
      cv::Scalar(hHigh, sHigh, vHigh)); // upper
}

void QNode::getHsvRange(int target,
                        int &hLow, int &hHigh,
                        int &sLow, int &sHigh,
                        int &vLow, int &vHigh)
{
  if (!node)
    return;
  auto t = static_cast<My_cone_cv::Target>(target);
  cv::Scalar lo = node->get_lower(t); // (H,S,V)
  cv::Scalar hi = node->get_upper(t);
  hLow = lo[0];
  sLow = lo[1];
  vLow = lo[2];
  hHigh = hi[0];
  sHigh = hi[1];
  vHigh = hi[2];
}
