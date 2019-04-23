#include "progress_bar.h"

ProgressBar::ProgressBar(int width) {
  this->width = width;
  this->progress = 0;
  this->prev_progress = 0;
}

void ProgressBar::update_progress(int pct) {
  this->prev_progress = this->progress;
  this->progress = pct;
  if (this->progress - this->prev_progress < 5) {
    return;
  }

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
  cout << prog_bar << flush; 
}
