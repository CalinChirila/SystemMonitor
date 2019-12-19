#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <iostream>

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  // TODO: Declare any necessary private members
 private:
  float _prev_total = 0, _prev_idle = 0;
};

#endif