// my_image_view.cpp
#include "intern_cv_day1_hw1/my_ball_cv.hpp"
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>
#include <QMetaObject>

My_ball_cv::My_ball_cv() : Node("my_ball_cv")
{
  /* 타이머도 굳이..?
  timer_ = create_wall_timer(
      std::chrono::milliseconds(500),
      std::bind(&My_ball_cv::image_callback, this));
  */
}

// 가우시안 블러 없는 버전의 필터
static cv::Mat no_GaussianBlur_func(const cv::Mat& frame)
{
  // 불러온 이미지를 HSV로 변경 필터로 사용하기 위해
  cv::Mat hsv_for_filter;
  cv::cvtColor(frame, hsv_for_filter, cv::COLOR_BGR2HSV);

  cv::Mat binery_filter_red1_; // 이진 필터 빨간색1
  cv::Mat binery_filter_red2_; // 이진 필터 빨간색2
  cv::Mat binery_filter_green_; // 이진 필터 초록색
  cv::Mat binery_filter_blue_; // 이진 필터 파란색

  cv::Mat binery_filler_total_; // 필터 토탈

  cv::Mat result;

  // 빨간색 추출 필터
  cv::Scalar lower_red1(0, 100, 100);
  cv::Scalar upper_red1(10, 255, 255);
  cv::Scalar lower_red2(170, 100, 100);
  cv::Scalar upper_red2(179, 255, 255);

  // 초록
  cv::Scalar lower_green(35, 100, 100);
  cv::Scalar upper_green(85, 255, 255);

  // 파랑
  cv::Scalar lower_blue(100, 100, 100);
  cv::Scalar upper_blue(130, 255, 255);


  // 초록 색 필터 적용
  cv::inRange(hsv_for_filter, lower_green, upper_green, binery_filter_green_);
  
  // 파란 색 필터 적용
  cv::inRange(hsv_for_filter, lower_blue, upper_blue, binery_filter_blue_);
  
  // 빨간 색 필터1 적용
  cv::inRange(hsv_for_filter, lower_red1, upper_red1, binery_filter_red1_);
  cv::inRange(hsv_for_filter, lower_red2, upper_red2, binery_filter_red2_);

  // 이진 필터 토탈
  binery_filler_total_ = binery_filter_blue_ | binery_filter_green_ | binery_filter_red1_ | binery_filter_red2_;

  cv::bitwise_and(frame, frame, result, binery_filler_total_);

  // BGR 값 반환
  return result;
}

static cv::Mat yes_GaussianBlur_func(const cv::Mat& frame)
{
  cv::Mat temp_;

  cv::GaussianBlur(frame, temp_, cv::Size(35, 35), 0); // 커널 크기가 35x35인 가우시안 블러

  return no_GaussianBlur_func(temp_);
}

void My_ball_cv::image_callback()
{
  // 코드를 실행할 때 경로 교체 요망
  cv::Mat frame = cv::imread("./src/intern_cv_day1_hw1/image/image_01.jpg");
  
  if (frame.empty())
  {
    RCLCPP_WARN(this->get_logger(), "이미지 로드 실패");
    return;
  }

  

  // 가우시어 블러 있는 버전, 없는버전
  cv::Mat noGaus_frame;
  cv::Mat yesGaus_frame;

  // 가우시안 블러 없는 필터 사용
  noGaus_frame = no_GaussianBlur_func(frame);

  // 가우시안 블러 있는 필터 사용
  yesGaus_frame = yes_GaussianBlur_func(frame);

  cv::Mat rgb1;
  cv::Mat rgb2;

  // BGR -> RGB 형식으로 변경, 출력형식으로 변경
  cv::cvtColor(noGaus_frame, rgb1, cv::COLOR_BGR2RGB);
  QImage img1(rgb1.data, rgb1.cols, rgb1.rows, rgb1.step, QImage::Format_RGB888);
  
  cv::cvtColor(yesGaus_frame, rgb2, cv::COLOR_BGR2RGB);
  QImage img2(rgb2.data, rgb2.cols, rgb2.rows, rgb2.step, QImage::Format_RGB888);
  

  if (!display_label1_) return;
  if (!display_label2_) return;
  display_label1_->setPixmap(QPixmap::fromImage(img1.copy()));
  display_label2_->setPixmap(QPixmap::fromImage(img2.copy()));

}