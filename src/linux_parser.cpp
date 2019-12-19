#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <algorithm>

#include "linux_parser.h"
#include "system.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  // Variables for the information contained in the file + a string to read into
  std::string os, version, kernel, line;
  
  // Stream to read from file
  std::ifstream kernel_stream(LinuxParser::kProcDirectory + LinuxParser::kVersionFilename);
  if(kernel_stream.is_open()) {
    std::getline(kernel_stream, line);
    std::istringstream kernel_parser(line);
    // Look for the 3rd element. That is our kernel
    try {
      kernel_parser >> os >> version >> kernel;
      return kernel; 
    } catch(...) {
      return std::string();
    }
  }
  
  return std::string();
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        if(std::find(pids.begin(), pids.end(), pid) == pids.end()){
          pids.push_back(pid);
        }
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string line, key;
  std::ifstream fileStream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  float memTotal, memFree;
  
  if(fileStream.is_open()){
    for(int i = 0; i < 2; i++){
      std::getline(fileStream, line);
      switch(i) {
        case 0:
          std::istringstream(line) >> key >> memTotal;
          break;
        case 1:
          std::istringstream(line) >> key >> memFree;
          break;
      }
    }
    
    return ((memTotal - memFree) / memTotal);
  }
  
  return 0.0; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long int value;
  std::string line;
  std::ifstream uptime_stream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
  if(uptime_stream.is_open()) {
    std::getline(uptime_stream, line);
    std::istringstream iss_uptime(line);
    
    while(iss_uptime) {
      iss_uptime >> value;
      return value;
    }
  }
  
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::GetJiffiesByState(LinuxParser::JiffyState::total);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return LinuxParser::GetJiffiesByState(LinuxParser::JiffyState::active);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  return LinuxParser::GetJiffiesByState(LinuxParser::JiffyState::idle);
}

// TODO: Read and return CPU utilization
std::vector<std::string> LinuxParser::CpuUtilization(int& pid) {
  std::vector<std::string> result = LinuxParser::ReadDataFromProcStat(pid);
  return result;
}

long LinuxParser::GetJiffiesByState(LinuxParser::JiffyState state){
 // Read CPU data from /proc/stat
  std::string line, cpu_key;
  long _user, _nice, _system, _idle, _iowait, _irq, _softirq, _steal; 
  std::ifstream cpustat_stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  
  if(cpustat_stream.is_open()){
    std::getline(cpustat_stream, line);
    if(std::istringstream(line) >> cpu_key >> _user >> _nice >> _system >> _idle >> _iowait >> _irq >> _softirq >> _steal){
      if(state == LinuxParser::JiffyState::active){
        return _user + _nice + _system + _irq + _softirq + _steal;
      } else if (state == LinuxParser::JiffyState::idle){
        return _idle + _iowait;
      } else {
        return _user + _nice + _system + _irq + _softirq + _steal + _idle + _iowait; // total jiffies
      }
    }
  }
  return 0;  
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line, key;
  int value;
  
  std::ifstream total_proc_stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if(total_proc_stream.is_open()) {
    while(std::getline(total_proc_stream, line)) {
      std::istringstream issProc(line);
      while(issProc){
        issProc >> key >> value;
        if(key == "processes") {
          return value;
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
std::string line, key;
  int value;
  
  std::ifstream total_proc_stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if(total_proc_stream.is_open()) {
    while(std::getline(total_proc_stream, line)) {
      std::istringstream issProc(line);
      while(issProc){
        issProc >> key >> value;
        if(key == "procs_running") {
          return value;
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream commandStream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kCmdlineFilename);
  std::string line;
  
  if(commandStream.is_open()) {
    std::getline(commandStream, line);
    return line;
  }
  
  return std::string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::string valueString = LinuxParser::ReadDataFromProcStatus(pid, "VmSize");
  unsigned int index_of_kb = valueString.find("kB");
  if(index_of_kb <= valueString.length()){
    valueString.erase(valueString.find("kB"), 2);
  }
  try {
    return std::to_string(std::stol(valueString) / 1024);
  } catch (...) {
    return string();
  }
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  int uid;
  std::istringstream userIdStream(LinuxParser::ReadDataFromProcStatus(pid, "Uid"));
  if(userIdStream >> uid){
    return std::to_string(uid);
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string line;
  std::ifstream userStream(LinuxParser::kPasswordPath);
  std::string uid = LinuxParser::Uid(pid);
  
  if(userStream.is_open()){
    std::vector<string> result;

    while(std::getline(userStream, line, ':')) {
      result.push_back(line);
      if(result.size() >= 3 && result[2] == uid){
        // If we find the information we are looking for, return the USER NAME.
        return result[0];
      }
    }
  }
  // Return an empty string if we couldn't find the user name.
  return std::string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::ifstream uptimeStream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);
  std::string value;
  std::vector<string> values;
  
  if(uptimeStream.is_open()) {
    std::string line;
    while(std::getline(uptimeStream, line)){
      std::istringstream iss(line);
      while(iss >> value) {
        values.push_back(value);
      }
    }
  }
  
  if(values.size() > 21) {
    long clockTicks = std::stol(values.at(21));
    int seconds = clockTicks / sysconf(_SC_CLK_TCK);
    return seconds;
  }
  
  return 0; 
}

// This function returns the data associated with the given key from the /proc/PID/status file
std::string LinuxParser::ReadDataFromProcStatus(int& pid, std::string key) {
  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename);
  std::string keyvalue;
  
  if(stream.is_open()) {
    while(std::getline(stream, keyvalue)){
      // Separate each line into key-value pairs at the : symbol
      keyvalue.erase(std::remove(keyvalue.begin(), keyvalue.end(), ' '), keyvalue.end());
      int index_of_separator = keyvalue.find(':');
      std::string keystring = keyvalue.substr(0, index_of_separator);
      
      // Get the value only if it's the key we're looking for
      if(keystring == key) {
        return keyvalue.substr(index_of_separator + 1);
      }
    }
  }
  return std::string();
}

// This function returns the data associated with the /proc/PID/stat file
std::vector<std::string> LinuxParser::ReadDataFromProcStat(int& pid) {
  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);
  std::string value;
  std::vector<std::string> values;
  
  if(stream.is_open()) {
    while(std::getline(stream, value, ' ')) {
      values.push_back(value);
    }
  }
  
  return values;
}