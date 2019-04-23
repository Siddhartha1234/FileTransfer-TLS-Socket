#pragma once
#include <iostream>
#include <string>

#include <chrono>

using namespace std;

class ProgressBar {
private:
  int width;
  int progress;

public:
  ProgressBar(int width);
  void update_progress(int cprog);
};
