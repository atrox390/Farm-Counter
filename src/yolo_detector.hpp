#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/dnn.hpp>

struct Detection {
  int class_id;
  float confidence;
  cv::Rect box;
};

class YoloDetector {
public:
  YoloDetector(const std::string& modelPath);
  std::vector<Detection> detect(const cv::Mat& frame);
private:
  cv::dnn::Net net;
};
