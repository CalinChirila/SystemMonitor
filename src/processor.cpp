#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  float totalIdle = LinuxParser::IdleJiffies();
  float totalActive = LinuxParser::ActiveJiffies();
  
  float total = totalActive + totalIdle;
  
  float deltaTotal = total - Processor::_prev_total; // Value read now - Value read the previous time
  Processor::_prev_total = total; // Previous becomes current
    
  float deltaIdle = totalIdle - Processor::_prev_idle;
  Processor::_prev_idle = totalIdle;
    
  return (deltaTotal - deltaIdle) / deltaTotal;
}