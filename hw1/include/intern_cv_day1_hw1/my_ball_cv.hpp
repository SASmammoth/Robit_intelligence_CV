#pragma once
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include <QLabel>

class My_ball_cv : public rclcpp::Node
{
public:
    My_ball_cv();
    void set_label1(QLabel *label) { display_label1_ = label; }
    void set_label2(QLabel *label) { display_label2_ = label; }
    
    // 이미지 콜백이긴 한데, 일단 타이머로 불러옴
    void image_callback();


private:
    
    //rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
    
    // 이미지 불러오는 타이머, subscription 대신 사용
    rclcpp::TimerBase::SharedPtr timer_; 

    QLabel *display_label1_ = nullptr; // 화면1 (블러X)
    QLabel *display_label2_ = nullptr; // 화면2 (블러O)

};