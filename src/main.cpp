#include <opencv2/opencv.hpp>
#include <iostream>
#include "yolo_detector.hpp"
#include "sort.hpp"
#include "counter.hpp"

int main(int argc, char** argv) {
  cv::VideoCapture cap;
  if (argc > 1) {
    std::string source = argv[1];
    if (source.size() == 1 && isdigit(source[0])) {
      int camIndex = std::stoi(source);
      cap.open(camIndex, cv::CAP_DSHOW);
      std::cout << "Usando camara: " << camIndex << std::endl;
    }
    else if (source.rfind("rtsp://", 0) == 0) {
      cap.open(source, cv::CAP_FFMPEG);
      std::cout << "Usando RTSP: " << source << std::endl;
    }
    else {
      cap.open(source);
      std::cout << "Usando video: " << source << std::endl;
    }
  }
  else {
    cap.open(0, cv::CAP_DSHOW);
    std::cout << "Usando camara por defecto (0)" << std::endl;
  }

  if (!cap.isOpened()) {
    std::cerr << "ERROR: No se pudo abrir la fuente de video\n";
    return -1;
  }

  YoloDetector yolo("../models/yolov8n.onnx");
  SortTracker tracker;
  Counter counter;

  while (true) {
    cv::Mat frame;
    cap >> frame;

    if (frame.empty()) {
      std::cout << "Fin del video o frame vacio\n";
      break;
    }

    cv::resize(frame, frame, cv::Size(640, 640));

    auto detections = yolo.detect(frame);

    for (auto &d : detections) {
      cv::rectangle(frame, d.box, {0, 255, 0}, 2);
      cv::putText(
        frame,
        "person " + std::to_string(int(d.confidence * 100)) + "%",
        {d.box.x, d.box.y - 5},
        cv::FONT_HERSHEY_SIMPLEX,
        0.5,
        {0, 255, 0},
        1
      );
    }

    std::vector<cv::Rect> boxes;
    for (auto &d : detections)
      boxes.push_back(d.box);

    auto tracks = tracker.update(boxes);

    for (auto &[id, t] : tracks) {
      cv::circle(frame, t.centroid, 4, {0, 255, 0}, -1);
      cv::putText(
        frame,
        std::to_string(id),
        t.centroid,
        cv::FONT_HERSHEY_SIMPLEX,
        0.6,
        {0, 255, 0},
        2
      );
    }

    int lineY = frame.rows / 2;
    cv::line(frame, {0, lineY}, {frame.cols, lineY}, {0, 0, 255}, 2);

    cv::imshow("People Counter", frame);

    if (cv::waitKey(1) == 27) // ESC
      break;
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}
