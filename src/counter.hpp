#pragma once
#include<map>

class Counter {
public:
  void update(int id, int prevY, int currY);
  int in = 0;
  int out = 0;
private:
  std::map<int, int> lastY;
};
