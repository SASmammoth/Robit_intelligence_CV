// my_cone_cv.cpp
#include "intern_cv_day1_hw2/my_cone_cv.hpp"
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>

using std::placeholders::_1;

My_cone_cv::My_cone_cv() : Node("my_cone_cv")
{
  set_hsv_range(Target::WhiteLine, cv::Scalar(0, 0, 225), cv::Scalar(179, 30, 255));    // 하얀선
  set_hsv_range(Target::BlueLine, cv::Scalar(90, 50, 120), cv::Scalar(120, 255, 255));  // 파란선
  set_hsv_range(Target::OrangeCone, cv::Scalar(10, 50, 120), cv::Scalar(35, 255, 255)); // 오렌지콘
  set_hsv_range(Target::NeonCone, cv::Scalar(35, 60, 120), cv::Scalar(85, 255, 255));   // 네온콘

  // usb_cam 노드가 발행하는 원본 이미지 토픽을 구독
  sub_ = this->create_subscription<sensor_msgs::msg::Image>(
      "/image_raw", 10,
      std::bind(&My_cone_cv::image_callback, this, _1));

  RCLCPP_INFO(this->get_logger(), "my_cone_cv 시작, /image_raw 구독 중");
}

void My_cone_cv::set_hsv_range(Target t, const cv::Scalar &lower, const cv::Scalar &upper)
{
  lower_[static_cast<size_t>(t)] = lower;
  upper_[static_cast<size_t>(t)] = upper;
}

cv::Scalar My_cone_cv::get_lower(Target t) const { return lower_[static_cast<size_t>(t)]; }
cv::Scalar My_cone_cv::get_upper(Target t) const { return upper_[static_cast<size_t>(t)]; }

void My_cone_cv::image_callback(const sensor_msgs::msg::Image::SharedPtr msg)
{
  cv::Mat frame; // 원본 입력

  try
  {
    frame = cv_bridge::toCvCopy(msg, "bgr8")->image;
  }
  catch (const cv_bridge::Exception &e)
  {
    RCLCPP_ERROR(this->get_logger(), "cv_bridge 변환 실패: %s", e.what());
    return;
  }
  if (frame.empty())
    return;

  neonMask_func(frame);
  orangeMask_func(frame);
  whiteLine_func(frame);
  blueLine_func(frame);
  find_object_func(frame);

  if (usb_cb_ && !frame.empty())
    usb_cb_(frame);
  if (find_object_cb && !find_object_frame.empty())
    find_object_cb(find_object_frame);
  if (neon_cone_cb_ && !neonMask_frame.empty())
    neon_cone_cb_(neonMask_frame);
  if (orange_cone_ && !orangeMask_frame.empty())
    orange_cone_(orangeMask_frame);
  if (white_line_ && !whiteLine_frame.empty())
    white_line_(whiteLine_frame);
  if (blue_line_cb_ && !blueLine_frame.empty())
    blue_line_cb_(blueLine_frame);
}

void My_cone_cv::neonMask_func(const cv::Mat &frame)
{
  cv::Mat blur, hsv;
  cv::GaussianBlur(frame, blur, cv::Size(9, 9), 0); // 가우시안 블러 필터 이용해서 약간의 노이즈 줄이기
  cv::cvtColor(blur, hsv, cv::COLOR_BGR2HSV);
  cv::inRange(hsv, get_lower(Target::NeonCone), get_upper(Target::NeonCone), neonMask_frame);

  // 모폴로지를 위한 원형 커널
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));

  cv::morphologyEx(neonMask_frame, neonMask_frame, cv::MORPH_OPEN, kernel);  // 모폴로지:열림, 노이즈제거
  cv::morphologyEx(neonMask_frame, neonMask_frame, cv::MORPH_CLOSE, kernel); // 모폴로지:닫힘, 구멍 매우기
}

void My_cone_cv::orangeMask_func(const cv::Mat &frame)
{
  cv::Mat blur, hsv;
  cv::GaussianBlur(frame, blur, cv::Size(9, 9), 0); // 가우시안 블러 필터 이용해서 약간의 노이즈 줄이기
  cv::cvtColor(blur, hsv, cv::COLOR_BGR2HSV);
  cv::inRange(hsv, get_lower(Target::OrangeCone), get_upper(Target::OrangeCone), orangeMask_frame);

  // 모폴로지를 위한 원형 커널
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));

  cv::morphologyEx(orangeMask_frame, orangeMask_frame, cv::MORPH_OPEN, kernel);  // 모폴로지:열림, 노이즈제거
  cv::morphologyEx(orangeMask_frame, orangeMask_frame, cv::MORPH_CLOSE, kernel); // 모폴로지:닫힘, 구멍 매우기
}

void My_cone_cv::whiteLine_func(const cv::Mat &frame)
{
  cv::Mat blur, hsv;
  cv::GaussianBlur(frame, blur, cv::Size(9, 9), 0); // 가우시안 블러 필터 이용해서 약간의 노이즈 줄이기
  cv::cvtColor(blur, hsv, cv::COLOR_BGR2HSV);
  cv::inRange(hsv, get_lower(Target::WhiteLine), get_upper(Target::WhiteLine), whiteLine_frame);

  // 모폴로지를 위한 원형 커널
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));

  cv::morphologyEx(whiteLine_frame, whiteLine_frame, cv::MORPH_OPEN, kernel);  // 모폴로지:열림, 노이즈제거
  cv::morphologyEx(whiteLine_frame, whiteLine_frame, cv::MORPH_CLOSE, kernel); // 모폴로지:닫힘, 구멍 매우기
}

void My_cone_cv::blueLine_func(const cv::Mat &frame)
{
  cv::Mat blur, hsv;
  cv::GaussianBlur(frame, blur, cv::Size(9, 9), 0); // 가우시안 블러 필터 이용해서 약간의 노이즈 줄이기
  cv::cvtColor(blur, hsv, cv::COLOR_BGR2HSV);
  cv::inRange(hsv, get_lower(Target::BlueLine), get_upper(Target::BlueLine), blueLine_frame);

  // 모폴로지를 위한 원형 커널
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));

  cv::morphologyEx(blueLine_frame, blueLine_frame, cv::MORPH_OPEN, kernel);  // 모폴로지:열림, 노이즈제거
  cv::morphologyEx(blueLine_frame, blueLine_frame, cv::MORPH_CLOSE, kernel); // 모폴로지:닫힘, 구멍 매우기
}

// 직선 그리기
static void drawFitLine(cv::Mat &img, const cv::Vec4f &line, const cv::Scalar &color, double thikness)
{
  float vx = line[0], vy = line[1], x0 = line[2], y0 = line[3];
  float len = 1000.0f;
  cv::Point p1(x0 - vx * len, y0 - vy * len);
  cv::Point p2(x0 + vx * len, y0 + vy * len);
  cv::line(img, p1, p2, color, (int)thikness);
}

// 직선 계산
static bool measureLine(const cv::Mat &mask, cv::Vec4f &line, float &thickness)
{
  // 흰 픽셀을 전부 모아서 직선 하나 찾기
  std::vector<cv::Point> pts;
  cv::findNonZero(mask, pts);
  if (pts.size() < 2)
    return false;
  cv::fitLine(pts, line, cv::DIST_L2, 0, 0.01, 0.01); // 가장 알맞은 직선을 찾는 함수

  // 두께: 전체 면적 / 길이 = 평균 두께
  double totalArea = cv::countNonZero(mask);                 // 면적 = 흰 픽셀 수
  cv::RotatedRect rr = cv::minAreaRect(pts);                 // 전체를 감싸는 회전사각형
  double totalLen = std::max(rr.size.width, rr.size.height); // 긴 변 = 선 길이

  thickness = (totalLen > 0) ? (float)(totalArea / totalLen) : 0.0f;

  return true;
}

// 선으로 부터 위치 구하기
static int classifyBoxByLine(const cv::Vec4f &line, const cv::Rect &box)
{
  float vx = line[0], vy = line[1], x0 = line[2], y0 = line[3];

  // 박스 네 꼭짓점의 부호거리
  cv::Point2f corners[4] = {
      {(float)box.x, (float)box.y},
      {(float)(box.x + box.width), (float)box.y},
      {(float)box.x, (float)(box.y + box.height)},
      {(float)(box.x + box.width), (float)(box.y + box.height)}};

  bool hasPos = false, hasNeg = false;
  for (auto &p : corners)
  {
    float d = (p.x - x0) * vy - (p.y - y0) * vx; // 부호거리
    if (d > 0)
      hasPos = true;
    if (d < 0)
      hasNeg = true;
  }

  if (hasPos && hasNeg)
    return 0;             // 선이 박스 관통
  return hasPos ? 1 : -1; // 박스 관통안함 = 그쪽에 물체 잇음
}

static std::string zoneName(int hx, int vy)
{
  if (hx == 0 && vy == 0)
    return "중앙";

  if (hx > 0) // 오른쪽
  {
    if (vy > 0)
      return "1사분면";
    else if (vy == 0)
      return "오른쪽 파란선 위";
    else
      return "4사분면";
  }
  else if (hx < 0) // 왼쪽
  {
    if (vy > 0)
      return "2사분면";
    else if (vy == 0)
      return "왼쪽 파란선 위";
    else
      return "3사분면";
  }
  else // hx == 0, 하얀선 위
  {
    if (vy > 0)
      return "위쪽 하얀선 위";
    else
      return "아래쪽 하얀선 위";
  }
}

void My_cone_cv::find_object_func(const cv::Mat &frame)
{
  find_object_frame = frame.clone();

  // 직선 2개
  cv::Vec4f blueLine, whiteLine;
  float blueThick, whiteThick;
  bool hasBlue = measureLine(blueLine_frame, blueLine, blueThick);
  bool hasWhite = measureLine(whiteLine_frame, whiteLine, whiteThick);
  if (hasBlue)
  {
    drawFitLine(find_object_frame, blueLine, cv::Scalar(255, 0, 0), blueThick);
  }
  if (hasWhite)
  {
    drawFitLine(find_object_frame, whiteLine, cv::Scalar(255, 255, 255), whiteThick);
  }

  // 콘 리턴 텍스트
  std::string neonZone = "없음";
  std::string orangeZone = "없음";

  // 네온콘 박스
  { // 같음 이름을 중복으로 사용하기 위해 중괄호로 묶음
    std::vector<std::vector<cv::Point>> contours;
    // 외각선을 따는 함수
    cv::findContours(neonMask_frame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    double best = 0;
    cv::Rect bestBox;
    for (const auto &c : contours)
    {
      double a = cv::contourArea(c);
      if (a < 500)
        continue;
      if (a > best)
      {
        best = a;
        bestBox = cv::boundingRect(c);
      }
    }
    if (best > 0)
    {
      cv::rectangle(find_object_frame, bestBox, cv::Scalar(0, 255, 0), 2);
      if (hasBlue && hasWhite)
      {
        neonZone = zoneName(classifyBoxByLine(whiteLine, bestBox),
                            classifyBoxByLine(blueLine, bestBox));
      }
    }
  }

  // 오렌지콘 박스
  {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(orangeMask_frame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    double best = 0;
    cv::Rect bestBox;
    for (const auto &c : contours)
    {
      double a = cv::contourArea(c);
      if (a < 500)
        continue;
      if (a > best)
      {
        best = a;
        bestBox = cv::boundingRect(c);
      }
    }
    if (best > 0)
    {
      cv::rectangle(find_object_frame, bestBox, cv::Scalar(0, 128, 255), 2);
      if (hasBlue && hasWhite)
      {
        orangeZone = zoneName(classifyBoxByLine(whiteLine, bestBox),
                              classifyBoxByLine(blueLine, bestBox));
      }
    }
  }

  if (zone_cb_)
    zone_cb_("NeonCone: " + neonZone + "\nOrangeCone: " + orangeZone);
}