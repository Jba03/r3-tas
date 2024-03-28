#pragma once

#include <cpatools/common.hpp>
#include <cpatools/memory.hpp>

namespace cpa {

struct address_id { };
struct type_id { };
struct pointer_id {  };
struct string_id { };

/// An address type on the target platform
struct address : address_id {
  address() = default;
  /// Initialize with target address
  address(memory::TargetAddressType address) : addr(memory::bswap(address)) { /* ... */ }
  /// Initialize with host address
  address(memory::HostAddressType hostAddress) : addr(hostAddress ? memory::bswap(uint32_t(long(hostAddress) - long(memory::baseAddress))) : 0) { /* ... */ }
  /// Return the physical hardware address
  inline auto physicalAddress() -> memory::TargetAddressType { return memory::bswap(addr) | 0x80000000; };
  /// Return the effective (emulated) address
  inline auto effectiveAddress() -> memory::TargetAddressType { return memory::bswap(addr) & 0x7FFFFFFF; }
  /// Return the host platform address
  inline auto hostAddress() -> memory::HostAddressType { return valid() ? static_cast<uint8_t*>(memory::baseAddress) + effectiveAddress() : nullptr; }
  /// Is the address non-zero?
  inline auto valid() -> bool { return effectiveAddress() != 0; }
  /// valid
  operator bool() { return valid(); };
  // equals
  bool operator ==(address other) { return effectiveAddress() == other.effectiveAddress(); }
  /// cast
  operator memory::TargetAddressType() { return addr; };
  /// Any pointer cast
  template <typename T> operator T*() { return static_cast<T*>(hostAddress()); }
  
private:
  memory::TargetAddressType addr = 0;
};
  
template <typename T> concept IntegralType = std::is_integral_v<T>;
template <typename T> concept ArithmeticType = std::is_arithmetic_v<T>;
  
/// A type convertible to and from target platform memory
template <typename T, typename OpT> requires IntegralType<T> && ArithmeticType<OpT>
struct type : type_id {
  type() : data(0) { /* ... */ }
  /// Initialize with arithmetic type
  template<typename S> requires ArithmeticType<S> type(const S value) {
    if constexpr (std::is_same<S, float>::value) {
      // Copy float type to memory
      data = memory::bswap(*(T*)&value);
    } else if constexpr (std::is_integral<S>::value) {
      // Copy integral type to memory
    }
  }
  /// Assign any
  template<typename S> requires IntegralType<S> type& operator =(const S v) {
    if (memoryBound()) return *this;
    data = memory::bswap(*(T*)&v);
    return *this;
  }
  
  /// Return the offset of this type in target memory
  inline auto memoryOffset() -> address { return &data; }
  /// Is the type bound to the address space of the target?
  inline auto memoryBound() -> bool { return memory::memoryBound(&data); }
  
  operator OpT() const { T tmp = memory::bswap(*(T*)(&data)); return *(OpT*)&tmp; }
  
  T operator  +(std::integral auto other) { return OpT(data) + OpT(other); }
  T operator  -(std::integral auto other) { return OpT(data) - OpT(other); }
  T operator  *(std::integral auto other) { return OpT(data) * OpT(other); }
  T operator  /(std::integral auto other) { return OpT(data) / OpT(other); }
  OpT operator +=(OpT other) { return *this = *this + other;  }
  OpT operator -=(OpT other) { return *this = *this - other;  }
  OpT operator *=(OpT other) { return *this = *this * other;  }
  OpT operator /=(OpT other) { return *this = *this / other;  }
  type& operator ++() { *this += 1; return *this; }
  type& operator --() { *this -= 1; return *this; }
  T operator ++(int) { type c = *this; ++(*this); return c; }
  T operator --(int) { type c = *this; --(*this); return c; }
  OpT operator -() { type c = *this; return 0 - c; }
  
  using UnderlyingPrimaryType = T;
  using UnderlyingSecondaryType = OpT;
  
private:
  T data = 0;
};
  
  using TargetAddressTypeLocal = type<memory::TargetAddressType, memory::TargetAddressType>;
  
#pragma mark - Pointer
  
/// A pointer exception
class bad_pointer {
  std::string msg;
public:
  std::string what() { return msg; }
  bad_pointer(std::string s) : msg(s) { /* ... */ }
};

/// A pointer
template<typename T = address>
struct pointer : pointer_id {
  pointer() = default;
  pointer(address addr) : ptr(addr) { /* ... */ }
  pointer(void* addr) : ptr(addr) { /* ... */ }
  template<typename X> pointer(pointer<X> other) : ptr(other.ptr.physicalAddress()) { /* ... */ }
  
  inline auto valid() -> bool { return ptr.valid(); }
  inline auto memoryOffset() -> address { return address(&ptr); }
  inline auto pointeeAddress() -> address { return ptr.effectiveAddress(); }
  
  template<typename X = T> X* pointee() { return ptr ? (X*)ptr.hostAddress() : nullptr; }
  template<typename X = T> operator X*() { return pointee<X>(); }
  template<typename X> void operator =(X* other) { ptr = other; }
  auto operator =(std::nullptr_t) -> void { ptr = address(); }
  
  template<typename X = T> X* operator ->() {
    if (!pointee()) throw bad_pointer("bad pointer");
    return pointee();
  }
  
  template<typename X = T> pointer<X> operator[](auto idx) {
    X *obj = pointee<X>();
    if (!obj) throw bad_pointer("array access into bad pointer");
    return pointer<X>(obj + idx);
  }
  
  pointer<T> operator +(std::integral auto c) { return pointer<T>(ptr.physicalAddress() + sizeof(T) * c); }
  pointer<T> operator -(std::integral auto c) { return ptr.effectiveAddress() - sizeof(T) * c; }
  pointer<T> operator ++() { return (*this = pointer<T>(ptr.physicalAddress() + sizeof(T))); }
  pointer<T> operator ++(std::integral auto) { pointer<T> o = *this; (*this = pointer<T>(ptr.physicalAddress() + sizeof(T))); return o; }
  template <typename X = T> bool operator ==(pointer<X>& other) { return ptr == other.ptr; }
  operator bool() { return valid(); }
  
  using UnderlyingType = T;
  
  address ptr;
};
  
/// A pointer to a pointer
template <typename T = address>
struct doublepointer : pointer_id {
  doublepointer() = default;
  doublepointer(address addr) : ptr(addr) {}
  template<typename X> doublepointer(doublepointer<X>& other) : ptr(other.ptr) {}
  
  template<typename X = T> X* pointee() {
    address* primary = pointer<address> { ptr.physicalAddress() };
    return primary ? pointer<X> { primary->physicalAddress() } : nullptr;
  }
  
  template<typename X = T> pointer<X> operator [](auto idx) {
    address *primary = pointer<address>(ptr.physicalAddress()) + idx;
    return primary ? pointer<X>(primary->physicalAddress()) : nullptr;
  }
  
  template<typename X = T> X* operator ->() {
    if (!pointee()) throw bad_pointer("bad pointer");
    return pointee();
  }
  
  template<typename X = T> operator X*() { return pointee<X>(); }
  template<typename X = T> operator pointer<X>() { return pointee<X>(); }
  operator bool() { return ptr.valid(); }
  
  using UnderlyingType = T;
  
private:
  address ptr;
};

/// A string, zero-terminated unless size specified
template <size_t Size = 0>
struct string : string_id {
  string() = default;
  
  static constexpr bool FixedSize = Size != 0;
  /// The true length of the string
  inline constexpr auto length() -> size_t { return FixedSize ? Size : strlen(_string); }
  /// Return the last path component, if such exists
  auto lastPathComponent() -> std::string {
    std::string str = *this;
    size_t idx = str.rfind(':');
    if (idx == std::string::npos) return "";
    return str.substr(idx + 1, std::string::npos);
  }
  
  operator void*() { return static_cast<void*>(_string); }
  operator const char*() { return reinterpret_cast<const char*>(_string); }
  operator std::string() { return std::string(reinterpret_cast<char*>(_string), Size); }
  
  auto operator ==(const char *str) -> bool { return std::string(str) == std::string(reinterpret_cast<char*>(_string)); }
  auto operator ==(std::string str) -> bool { return std::string(str) == std::string(reinterpret_cast<char*>(_string)); }
  auto operator ==(string&     str) -> bool { return std::string(str) == std::string(this); }
  auto operator =(std::string& str) -> void { std::memset(_string, 0, Size); std::memcpy(_string, str.data(), Size); }
  
private:
  std::conditional_t<FixedSize, int8_t[Size], char*> _string;
};

using char8   = type<int8_t, int8_t>;
using uchar8  = type<uint8_t, uint8_t>;
using int8    = type<int8_t, int8_t>;
using uint8   = type<uint8_t, uint8_t>;
using int16   = type<int16_t, int16_t>;
using uint16  = type<uint16_t, uint16_t>;
using int32   = type<int32_t, int32_t>;
using uint32  = type<uint32_t, uint32_t>;
using int64   = type<int64_t, int64_t>;
using uint64  = type<uint64_t, uint64_t>;
using float32 = type<uint32_t, float>;

template<typename T> using is_type = std::is_base_of<type_id, T>;
template<typename T> using is_pointer = std::is_base_of<pointer_id, T>;
template<typename T> using is_string = std::is_base_of<string_id, T>;

};
