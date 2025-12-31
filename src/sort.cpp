#include "sort.hpp"

std::map<int, Track> SortTracker::update(const std::vector<cv::Rect>& detections) {
  std::map<int, Track> updated;
  for (const auto& box: detections) {
    cv::Point c(box.x + box.width / 2, box.y + box.height / 2);
    updated[nextId] = {nextId, c, 0};
    nextId++;
  }
  tracks = updated;
  return tracks;
}
