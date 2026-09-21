#pragma once
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include <opencv2/core.hpp>
#include <functional>
#include <string>

class My_cone_cv : public rclcpp::Node
{
public:
    My_cone_cv();

    enum class Target
    {
        WhiteLine = 0,
        BlueLine,
        OrangeCone,
        NeonCone,
        COUNT
    };

    // 이미지 구독 콜백
    void image_callback(const sensor_msgs::msg::Image::SharedPtr msg);

    // 가공 결과(cv::Mat, BGR)를 밖으로 넘길 콜백
    using MatCallback = std::function<void(const cv::Mat &)>;
    void set_usb_cb(MatCallback cb) { usb_cb_ = std::move(cb); }
    void set_find_object_cb(MatCallback cb) { find_object_cb = std::move(cb); }
    void set_neon_cone_cb(MatCallback cb) { neon_cone_cb_ = std::move(cb); }
    void set_orange_cone_cb(MatCallback cb) { orange_cone_ = std::move(cb); }
    void set_white_line_cb(MatCallback cb) { white_line_ = std::move(cb); }
    void set_blue_line_cb(MatCallback cb) { blue_line_cb_ = std::move(cb); }

    // UI에서 특정 객체의 HSV 범위를 갱신할 때 호출
    void set_hsv_range(Target t, const cv::Scalar &lower, const cv::Scalar &upper);

    // 현재 저장된 범위를 UI가 읽어올 때 (슬라이더 초기값 세팅용)
    cv::Scalar get_lower(Target t) const;
    cv::Scalar get_upper(Target t) const;

    using StrCallback = std::function<void(const std::string &)>;
    void set_zone_cb(StrCallback cb) { zone_cb_ = std::move(cb); }

private:
    // 이미지 불러오는 구독자
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;

    // 가공된 화면 입력을 밖으로 넘기는 콜백
    MatCallback usb_cb_;        // 기본 화면
    MatCallback find_object_cb; // 객체 탐지한 외각 박스 그림 output
    MatCallback neon_cone_cb_;  // 네온색 콘 탐지 화면
    MatCallback orange_cone_;   // 오랜지 콘 탐지 화면
    MatCallback white_line_;    // 하얀 선 탐지 화면
    MatCallback blue_line_cb_;  // 파란 선 선 탐지 화면

    StrCallback zone_cb_;

    // 객체별 HSV 하한/상한 (인덱스 = Target)
    std::array<cv::Scalar, static_cast<size_t>(Target::COUNT)> lower_;
    std::array<cv::Scalar, static_cast<size_t>(Target::COUNT)> upper_;

    // 각각의 출력을 만드는 함수
    void neonMask_func(const cv::Mat &frame);
    void orangeMask_func(const cv::Mat &frame);
    void whiteLine_func(const cv::Mat &frame);
    void blueLine_func(const cv::Mat &frame);
    void find_object_func(const cv::Mat &frame);

    // 각각의 처리된 프레임
    cv::Mat find_object_frame, neonMask_frame, orangeMask_frame, whiteLine_frame, blueLine_frame;

    void judgeZone(const cv::Rect &box, float blueY, float whiteX, const std::string &name);
};