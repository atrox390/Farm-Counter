#include "counter.hpp"

void Counter::update(int id, int prevY, int currY) {
  if (prevY < 0) {
    return;
  }
  if (prevY < 300 && currY >= 300) {
    in++;
  } else if (prevY > 300 && currY <= 300) {
    out++;
  }
  lastY[id] = currY;
}
