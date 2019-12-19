#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : _pid(pid), _prev_tt(0.0), _prev_s(0.0) {}
  
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  bool operator==(Process const& a) const;

  // TODO: Declare any necessary private members
 private:
  int _pid;
  std::string _user;
  float _cpu;
  std::string _ram;
  long int _upTime;
  
  float _prev_tt;
  float _prev_s;
};

#endif