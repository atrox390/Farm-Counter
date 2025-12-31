# Farm Counter

Lightweight C++ pipeline that counts people crossing a configurable line using a YOLOv8 ONNX detector, a minimal SORT-style tracker, and OpenCV for video ingest and visualization.

## Features
- Loads YOLOv8n ONNX and runs person detection entirely on CPU via OpenCV DNN
- Tracks detections frame-to-frame with a simple centroid-based SORT implementation
- Counts objects moving in and out across a horizontal line overlay
- Supports webcams, RTSP streams, and video files as input sources

## Requirements
- CMake 3.16+
- A C++17-compatible compiler (MSVC, Clang, or GCC)
- OpenCV 4.x built with the DNN module
- YOLOv8n ONNX weights placed under `models/yolov8n.onnx`

## Build
```bash
cmake -S . -B build
cmake --build build --config Release
```
On Windows with the Visual Studio generator, you can open the resulting solution or run the last command with `--config Debug`.

## Usage
```bash
./build/farm-counter [source]
```
- Omit `source` to use the default camera (index 0)
- Pass an integer to pick another camera index
- Pass `rtsp://...` for RTSP streams
- Pass a path to a video file for offline processing

During execution a window titled "People Counter" will display detections, tracked IDs, and the counting line. Press `ESC` to exit.

## Project Layout
```
src/
  main.cpp           # entry point, handles video IO and visualization
  yolo_detector.*    # wraps ONNX model loading and inference
  sort.*             # minimal centroid tracker
  counter.*          # in/out counting logic
models/
  yolov8n.onnx       # default object detector weights
```

## Notes
- Counting thresholds are tuned for 640×640 frames and a horizontal mid-line; tweak `Counter::update` if your geometry differs.
- The tracker is intentionally simple and may miscount with heavy occlusion; integrate a full SORT/DeepSORT pipeline for production scenarios.
- Ensure your OpenCV build includes FFMPEG support to use RTSP or compressed video sources.
