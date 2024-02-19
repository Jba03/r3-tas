//
//  logging.h
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#ifndef logging_h
#define logging_h

#include <iostream>

namespace log {
  
  inline static const std::string bold = "\033[1m";
  inline static const std::string faded = "\033[2m";
  inline static const std::string italic = "\033[3m";
  inline static const std::string underline = "\033[4m";
  
  inline static const std::string normal = "\x1B[0m";
  inline static const std::string red    = "\x1B[31m";
  inline static const std::string green  = "\x1B[32m";
  inline static const std::string yellow = "\x1B[33m";
  inline static const std::string blue   = "\x1B[34m";
  inline static const std::string pink   = "\x1B[35m";
  inline static const std::string cyan   = "\x1B[36m";
  inline static const std::string white  = "\x1B[37m";
  inline static const std::string grey   = "\x1B[38m";
  
  inline static void info(std::string mode, std::string color, std::string message) {
    std::cout << mode << color << message << normal;
  }
  
};

#endif /* logging_h */
