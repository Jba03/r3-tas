#pragma once

#include <cpatools/types.hpp>

#include <iterator>
#include <vector>
#include <sstream>
#include <unordered_map>

#pragma mark - Markup

namespace cpa::markup {

struct node {
  node() = default;
  node(const std::string& name) : _name(name) { /* ... */ }
  node(const std::string& name, const std::string& value) : _name(name), _value(value) { /* ... */ }
  
  auto name() const -> std::string { return _name; }
  auto value() const -> std::string { return _value; }
  auto begin() { return _children.begin(); }
  auto end() { return _children.end(); }
  auto add_child(const node& nd) -> node& { return _children.emplace_back(nd); }
  
  auto operator [](std::string name) -> node& {
    for (auto& node : _children) {
      if (node.name() == name) return node;
    }
    throw "No such node '" + name + "' found";
  }
  
  inline static auto serialize(node& nd, unsigned depth = 0) -> std::string {
    if (nd.name().length() == 0) {
      std::string text;
      for (auto child : nd) {
        text.append(serialize(child, depth));
      }
      return text;
    }
    
    std::string pad;
    for (int i = 0; i < depth * 2; i++)
      pad.append(" ");
    
    std::vector<std::string> lines;
    std::stringstream value(nd.value());
    for (std::string line; std::getline(value, line, '\n');) {
      lines.emplace_back(line);
    }
    
    std::string result;
    result.append(pad + nd.name());
    if (lines.size() == 1) {
      result.append(std::string(":") + " " + lines[0]);
    }
    result.append("\n");
    if (lines.size() > 1) {
      result.append("  ");
      for (std::string line : lines) {
        result.append(pad + std::string(":") + " " + line + "\n");
      }
    }
    for (auto child : nd) {
      result.append(serialize(child, depth + 1));
    }
    return result;
  }
  
  inline auto unserialize(std::string text) -> void {
    std::vector<std::string> lines;
    std::string line;
    std::stringstream s(text);
    while (std::getline(s, line, '\n')) {
      if (depth(line) != std::string::npos) {
        if (!line.substr(depth(line), std::string::npos).starts_with("//")) {
          lines.push_back(line);
        }
      }
    }
    
    int i = 0;
    while (i < lines.size()) {
      node node;
      node.parse(lines, i, " ");
      _children.emplace_back(node);
    }
  }
  
  inline auto depth(std::string text) -> size_t {
    return text.find_first_not_of(" \t\n\v\f\r");
  }
  
  auto valid(char c) const -> bool {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '.' || c == '-' || c == '_';
  }
  
  void parse(const std::vector<std::string>& text, int& i, std::string_view spacing) {
    const char *p = text[i++].c_str();
    _depth = depth(p);
    p += _depth;
    
    //name
    int length = 0;
    while (valid(p[length])) length++;
    if (length == 0) throw "Invalid node name";
    _name = std::string(p).substr(0, length);
    p += length;
    
    //value
    if (*p == ':') {
      int length = 1;
      while(p[length] && p[length] != '\n') length++;
      _value = std::string(p).substr(1, length - 1) + "\n";
      _value = _value.substr(spacing.length(), std::string::npos);
      p += length;
    }
    
    while (i < text.size()) {
      size_t subdepth = depth(text[i]);
      if (subdepth <= _depth) break;
      node node;
      node.parse(text, i, spacing);
      _children.push_back(node);
    }
  }
  
  auto children() {
    return _children;
  }
  
protected:
  size_t _depth;
  std::string _name;
  std::string _value;
  std::vector<node> _children;
};
  
}

#pragma mark - Serializer interface

namespace cpa {

static std::unordered_map<memory::TargetAddressType, bool> pointers;

struct serializer {
  serializer() = default;
  
  struct node : markup::node {
    node() = default;
    
    node(markup::node& nd) {
      _name = nd.name();
      _value = nd.value();
    }
    
    node(serializer* s, std::string name) {
      _serializer = s;
      _name = name;
    }
    
    template<typename T>
    node(serializer* s, std::string name, T value) {
      _serializer = s;
      _name = name;
      _value = std::to_string(value);
      //markup_node = markup::node(_name, _value);
    }
    
    node(serializer* s, std::string name, std::string value) {
      _serializer = s;
      _name = name;
      _value = value;
    }
    
    
    auto _add(node nd) -> void {
      _children.emplace_back(nd);
    }
    
    // typeless pointer
    auto add(const std::string& name, pointer<> v) -> void {
      node ptr(_serializer, "pointer", v.pointeeAddress().physicalAddress());
      add_child(ptr);
    }
    
    // typeless pointer
//    auto _add(const std::string& name, pointer<>& v) -> void {
//      node nd(name);
//      add(nd);
//    }
    
    template<typename T>
    auto add(const std::string& name, T& v, bool pointerResolve = true) -> void {
      node nd(_serializer, name);
      if constexpr (is_type<T>::value) {
        std::string type;
        if constexpr (std::is_same<T, char8>::value)  type = "char8";
        if constexpr (std::is_same<T, uchar8>::value) type = "uchar8";
        if constexpr (std::is_same<T, int8>::value)   type = "int8";
        if constexpr (std::is_same<T, uint8>::value)  type = "uint8";
        if constexpr (std::is_same<T, int16>::value)  type = "int16";
        if constexpr (std::is_same<T, uint16>::value) type = "uint16";
        if constexpr (std::is_same<T, int32>::value)  type = "int32";
        if constexpr (std::is_same<T, uint32>::value) type = "uint32";
        if constexpr (std::is_same<T, int64>::value)  type = "int64";
        if constexpr (std::is_same<T, uint64>::value) type = "uint64";
        if constexpr (std::is_same<T, float32>::value) type = "float32";
        //if constexpr (std::is_same<T, float32>::value) _children.push_back({ "float32", float(v) });
        //this->add(nd);
        //markup_node.add(<#const node &nd#>)
        
        std::string g = std::to_string(v);
        if constexpr (std::is_same<T, float32>::value) g = std::to_string(float(v));
        
        std::stringstream ss;
        ss << type + "(" << g  << ")";// << std::hex << int(v.memoryOffset());
        std::string value = ss.str();
        nd._value = value;
        
      } else if constexpr (is_pointer<T>::value) {
//        std::stringstream ss;
//        ss << type + "(" << std::to_string(v) << ") @ " << std::hex << int(v.memoryOffset());
//        std::string value = ss.str();
//        nd._value = value;
        
        try {
          memory::TargetAddressType addr = v.pointeeAddress();
          std::stringstream ss;
          ss << "0x" << std::uppercase << std::setfill('0') << std::setw(sizeof(memory::TargetAddressType)) << std::hex << addr;
          
          node ptr(_serializer, "pointer", ss.str());
          if (pointerResolve) {
            if (_serializer->pointers.find(addr) == _serializer->pointers.end()) {
              v->serialize(ptr);
              _serializer->pointers[addr] = v;
            }
          }
          nd.add_child(ptr);
        } catch (bad_pointer& e) {
          
        }
        //this->add(nd);
      } else if constexpr (std::is_array<T>::value) {
        
      } else {
        nd._value = typeid(decltype(v)).name();
        v.serialize(nd);
      }
      
      add_child(nd);
    }
    
    void type(std::string type) {
      _value = type;
    }
    
  private:
    serializer* _serializer;
  };
  
  operator node&() {
    return _root;
  }
  
  std::unordered_map<memory::TargetAddressType, pointer<>> pointers;
  
private:
  node _root { this, "root" };
};

}
