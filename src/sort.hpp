#pragma once
#include<opencv2/opencv.hpp>
#include<map>

struct Track {
  int id;
  cv::Point centroid;
  int missing;
};

class SortTracker {
public:
  std::map<int, Track> update(const std::vector<cv::Rect>& detections);
private:
  int nextId = 0;
  std::map<int, Track> tracks;
};
