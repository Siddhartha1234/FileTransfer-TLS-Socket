#include <iostream>
#include <string>

using namespace std;

class ProgressBar {
private:
  int width;
  int progress;
  int prev_progress;

public:
  ProgressBar(int width);
  void update_progress(int cprog);
};
