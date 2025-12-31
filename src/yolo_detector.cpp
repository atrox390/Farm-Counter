#include "yolo_detector.hpp"
#include <iostream>
#include <algorithm>

YoloDetector::YoloDetector(const std::string &modelPath) {
  net = cv::dnn::readNetFromONNX(modelPath);
  if (net.empty()) {
    std::cerr << "ERROR: No se pudo cargar el modelo YOLO\n";
    std::exit(1);
  }
  net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
  net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

std::vector<Detection> YoloDetector::detect(const cv::Mat &frame) {
  std::vector<Detection> detections;
  std::vector<cv::Rect> boxes;
  std::vector<float> confidences;
  std::vector<int> classIds;

  cv::Mat blob = cv::dnn::blobFromImage(
      frame, 1.0 / 255.0, cv::Size(640, 640),
      cv::Scalar(), true, false);

  net.setInput(blob);

  std::vector<cv::Mat> outputs;
  net.forward(outputs, net.getUnconnectedOutLayersNames());

  // Salida YOLOv8: [1, 84, 8400]
  cv::Mat output = outputs[0];

  // [1,84,8400] -> [84,8400]
  output = output.reshape(1, output.size[1]);

  // [84,8400] -> [8400,84]
  cv::transpose(output, output);

  for (int i = 0; i < output.rows; ++i) {
    float* data = output.ptr<float>(i);

    float cx = data[0];
    float cy = data[1];
    float w  = data[2];
    float h  = data[3];

    float xScale = frame.cols / 640.0f;
    float yScale = frame.rows / 640.0f;

    float* classes = data + 4;
    int classId = std::max_element(classes, classes + 80) - classes;
    float confidence = classes[classId];
    // SOLO personas
    if (classId == 0 && confidence >= 0.1f) {
      int left   = int((cx - w / 2) * xScale);
      int top    = int((cy - h / 2) * yScale);
      int width  = int(w * xScale);
      int height = int(h * yScale);

      cv::Rect box(left, top, width, height);
      box &= cv::Rect(0, 0, frame.cols, frame.rows);

      boxes.push_back(box);
      confidences.push_back(confidence);
      classIds.push_back(classId);
    }
  }

  std::vector<int> indices;
  cv::dnn::NMSBoxes(boxes, confidences, 0.1f, 0.45f, indices);

  for (int idx : indices) {
    Detection det;
    det.class_id = classIds[idx];
    det.confidence = confidences[idx];
    det.box = boxes[idx];
    detections.push_back(det);
  }
  
  return detections;
}
