#include <string>
#include <chrono>
#include <iostream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  std::chrono::seconds s(seconds); // convert seconds to duration
  std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours> (s); // convert to hours
  s -= std::chrono::duration_cast<std::chrono::seconds>(h); // substract the hours
  std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(s); // convert to minutes
  s -= std::chrono::duration_cast<std::chrono::seconds>(m); // subtract the minutes
  
  string hours_string = std::to_string(h.count());
  string minutes_string = std::to_string(m.count());
  string seconds_string = std::to_string(s.count());
  
  if(s.count() < 10) {
    seconds_string = '0' + std::to_string(s.count()); 
  }
  
  if(m.count() < 10) {
    minutes_string = '0' + std::to_string(m.count()); 
  } 
  
  return hours_string + ':' + minutes_string + ':' + seconds_string;
}