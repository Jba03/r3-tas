#pragma once

#include <any>
#include <fstream>
#include <filesystem>

#include "serialize.hpp"
#include "types.hpp"

struct settings {

  struct convertible {
    enum type { Int, Boolean, String, Pointer };
    convertible() = default;
    
    template<typename T> convertible(T v) {
//      if constexpr (cpa::is_pointer<T>::value) {
//        type = Pointer;
//        std::stringstream s;
//        s << "0x" << std::hex << v.pointeeAddress().effectiveAddress();
//        value = s.str();
//      } else if constexpr (std::is_same<T, bool>::value) {
//        type = Boolean;
//        value = v;
//      } else if constexpr (std::is_integral<T>::value) {
//        type = Int;
//        value = int64_t(v);
//      } else if constexpr (std::is_literal_type<T>::value) {
//        type = String;
//        value = std::string(v);
//      }
    }
    
    operator std::string() const {
      if (type == Int) return std::to_string(std::any_cast<int64_t>(value));
      if (type == String) return std::any_cast<std::string>(value);
      if (type == Boolean) return std::any_cast<bool>(value) ? "true" : "false";
      if (type == Pointer) return std::any_cast<std::string>(value);
      return "NULL";
    }
    
    std::any value;
    type type;
  };

  
 //template<typename T>
  convertible& operator [](std::string idx) {
    return map[idx];
  }
  
  void save(std::string filename) {
    std::string base = LIBR3TAS_DIR;
    std::string folder = "settings";
    if (!std::filesystem::exists(base + "/settings")) {
      std::filesystem::create_directory(base + "/settings");
    }
    
//    cpa::serializer s;
//    cpa::serializer::node root(&s, "root");
//    for (auto& [key, value] : map) {
//      cpa::serializer::node nd(&s, key, std::string(value));
//      root.add_child(nd);
//    }
//    
//    std::string data = cpa::serializer::node::serialize(root);
//    std::ofstream f(base + "/settings/" + filename);
//    f.write(data.c_str(), data.length());
//    f.close();
  }
  
  static std::string folder() {
    std::string base = LIBR3TAS_DIR;
    std::string folder = "settings";
    if (!std::filesystem::exists(base + "/settings")) {
      std::filesystem::create_directory(base + "/settings");
    }
    return base + "/settings/";
  }
  
private:
  std::unordered_map<std::string, convertible> map;
};
