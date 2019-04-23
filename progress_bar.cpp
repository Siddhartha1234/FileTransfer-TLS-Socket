#include "progress_bar.h"

//Init width and progress
ProgressBar::ProgressBar(int width) {
  this->width = width;
  this->progress = 0;
}

//Update and render progress 
void ProgressBar::update_progress(int pct) {
  this->progress = pct;
  string prog_bar = "[";
  for (int i = 0; i < this->width; i++) {
    if (i < (pct * this->width / 100))
      prog_bar += '=';
    else if (i == (pct * this->width / 100))
      prog_bar += '>';
    else
      prog_bar += ' ';
  }
  prog_bar += ']';
  cout << "\r"; // go back to start
  cout.width(3);
  cout << pct << "%";
  cout << prog_bar;
  if (pct == 100)
    cout << "\n";
  cout << flush;
}
