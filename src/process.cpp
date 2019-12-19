#include <unistd.h>
#include <iostream>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { 
  return _pid; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  int pid = Process::Pid();
  std::vector<std::string> all_data = LinuxParser::CpuUtilization(pid);
  if(all_data.size() >= 22) {
    int utime = std::stoi(all_data[13]); // CPU time spent in user code, measured in clock ticks
    int stime = std::stoi(all_data[14]); // CPU time spent in kernel code, measured in clock ticks
    int cutime = std::stoi(all_data[15]); // Waited-for children's CPU time spent in user code (in clock ticks)
    int cstime = std::stoi(all_data[16]); // Waited-for children's CPU time spent in kernel code (in clock ticks)
    int starttime = std::stoi(all_data[21]); // Time when the process started, measured in clock ticks
    
    float total_time = (utime + stime + cutime + cstime);
    float seconds = Process::_upTime - starttime;
    
    Process::_cpu = total_time / seconds ;
    return std::abs(Process::_cpu);
  }
  
  return 0.0; 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Process::Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Process::Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  Process::_upTime = LinuxParser::UpTime(Process::Pid()); 
  return Process::_upTime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return Process::_cpu < a._cpu;
}

bool Process::operator==(Process const& a) const {
  return Process::_pid == a._pid; 
}