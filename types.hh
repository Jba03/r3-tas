#ifndef types_hh
#define types_hh

#include "memory.hh"

#include <array>
#include <string>
#include <concepts>
#include <type_traits>

#include <cstdint>
#include <cmath>

namespace CPA {
  
  /// An address type on the target platform
  struct Address {
    Address();
    /// Initialize with target address
    Address(Memory::TargetAddressType address);
    /// Initialize with host address
    Address(Memory::HostAddressType hostAddress);
    /// Return the physical hardware address
    Memory::TargetAddressType physicalAddress();
    /// Return the effective (emulated) address
    Memory::TargetAddressType effectiveAddress();
    /// Return the host platform address
    Memory::HostAddressType hostAddress();
    /// Is the address non-zero?
    bool valid();
    /// valid
    operator bool();
    // equals
    bool operator ==(Address other);
    /// cast
    operator Memory::TargetAddressType();
    /// Any pointer cast
    template <typename T> operator T*() { return static_cast<T*>(hostAddress()); }
    
  private:
    Memory::TargetAddressType addr = 0;
  };
  
  template <typename T>
  concept IntegralType = std::is_integral_v<T>;
  template <typename T>
  concept ArithmeticType = std::is_arithmetic_v<T>;
  
  struct TypeIdentifier {};
  /// A type convertible to and from target platform memory
  template <IntegralType T, ArithmeticType OpT>
  struct Type : TypeIdentifier {
    Type() : data(0) {
      /* ... */
    }
    
    /// Initialize with any aritmetic type
    template <ArithmeticType S>
    Type(const S value) {
      if constexpr (std::is_same<S, float>::value) {
        // Copy float type to memory
        data = Memory::bswap(*(T*)&value);
      } else if constexpr (std::is_integral<S>::value) {
        // Copy integral type to memory
      }
    }
    
    /// Assign any 
    template <IntegralType S>
    Type& operator =(const S v) {
      if (memoryBound()) {
        return *this;
      }
      data = Memory::bswap(*(T*)&v);
      return *this;
    }

    /// Is the type bound within the address space of the target?
    bool memoryBound() {
      return Memory::HostAddressType(&data) >= static_cast<uint8_t*>(Memory::baseAddress) &&
             Memory::HostAddressType(&data) <=(static_cast<uint8_t*>(Memory::baseAddress) + Memory::size);
    }
    
    Address memoryOffset() {
      return &data;
    }
    
    operator OpT() const {
      T tmp = Memory::bswap(*(T*)(&data));
      return *(OpT*)&tmp;
    }

    T operator  +(std::integral auto other) { return OpT(data) + OpT(other); }
    T operator  -(std::integral auto other) { return OpT(data) - OpT(other); }
    T operator  *(std::integral auto other) { return OpT(data) * OpT(other); }
    T operator  /(std::integral auto other) { return OpT(data) / OpT(other); }
    OpT operator +=(OpT other) { return *this = *this + other;  }
    OpT operator -=(OpT other) { return *this = *this - other;  }
    OpT operator *=(OpT other) { return *this = *this * other;  }
    OpT operator /=(OpT other) { return *this = *this / other;  }
    Type& operator ++() { *this += 1; return *this; }
    Type& operator --() { *this -= 1; return *this; }
    T operator ++(int) { Type c = *this; ++(*this); return c; }
    T operator --(int) { Type c = *this; --(*this); return c; }
    OpT operator -() { Type c = *this; return 0 - c; }
    
    using U = T;
    using T1 = T;
    using T2 = OpT;
  private:
    T data = 0;
  };

  template <typename T>
  using is_game_type = std::is_base_of<T, TypeIdentifier>;
  
  using TargetAddressTypeLocal = Type<Memory::TargetAddressType, Memory::TargetAddressType>;
  
#pragma mark - Pointer
  
  /// A pointer exception
  struct BadPointer {
    std::string msg;
    std::string what() { return msg; }
    BadPointer(std::string s) : msg(s) {}
  };
  
  /// A pointer
  template <typename T = Address>
  struct Pointer {
    Pointer() {
      /* ... */
    }
    
    Pointer(Address addr) : ptr(addr) {
      /* ... */
    }
    
    Pointer(void* addr) : ptr(addr) {
      /* ... */
    }
    
    template <typename X>
    Pointer(Pointer<X>& other) : ptr(other.ptr) {
      /* ... */
    }
    
    Address memoryOffset() {
      return Address(&ptr);
    }
    
    Address pointeeAddress() {
      return ptr.effectiveAddress();
    }
    
    template <typename X = T>
    X* pointee() {
      return ptr ? (X*)ptr.hostAddress() : nullptr;
    }
    
    template <typename X = T>
    operator X*() {
      return pointee<X>();
    }
    
    template <typename X = T>
    X* operator ->() {
      if (pointee() == nullptr)
        throw BadPointer("bad pointer");
      return pointee();
    }
    
    template <typename X = T>
    Pointer<X> operator [](auto idx) {
      X *obj = pointee<X>();
      if (obj == nullptr)
        throw BadPointer("array access into bad pointer");
      else return Pointer<X>(obj + idx);
    }
    
    template <typename X>
    void operator =(X *other) {
      ptr = other;
    }
    
    Pointer<T> operator +(std::integral auto c) { return Pointer<T>(ptr.physicalAddress() + sizeof(T) * c); }
    Pointer<T> operator -(std::integral auto c) { return ptr.effectiveAddress() - sizeof(T) * c; }
    Pointer<T> operator ++() { return (*this = Pointer<T>(ptr.physicalAddress() + sizeof(T))); }
    Pointer<T> operator ++(std::integral auto) { Pointer<T> o = *this; (*this = Pointer<T>(ptr.physicalAddress() + sizeof(T))); return o; }
    template <typename X = T> bool operator ==(Pointer<X>& other) { return ptr == other.ptr; }
    operator bool() { return ptr.valid(); }
    
    Address ptr;
  };
  
  /// A pointer to a pointer
  template <typename T = Address>
  struct DoublePointer : Pointer<Pointer<T>> {
    DoublePointer(Address addr) : ptr(addr) {
      /* ... */
    }
    
    template <typename X>
    DoublePointer(DoublePointer<X>& other) : ptr(other.ptr) {
      /* ... */
    }
    
    template <typename X = T>
    X* pointee() {
      Address *primary = Pointer<Address>(ptr.physicalAddress());
      return primary ? Pointer<X>(primary->physicalAddress()) : nullptr;
    }
    
    template <typename X = T>
    operator X*() {
      return pointee<X>();
    }
    
    operator bool() {
      return ptr.valid();
    }
    
    Address ptr;
  };
  
  /// A string, zero-terminated unless size specified
  template <size_t size = std::string::npos>
  struct String {
    /// The length of the string
    size_t length() {
      return size;
    }
    
    /// Return the last path component, if such exists
    std::string lastPathComponent() {
      std::string str = *this;
      size_t idx = str.rfind(':');
      if (idx == std::string::npos) return "";
      return str.substr(idx + 1, std::string::npos);
    }
    
    operator void*() { return static_cast<void*>(string); }
    operator const char*() { return reinterpret_cast<const char*>(string); }
    operator std::string() { return std::string(reinterpret_cast<char*>(string), size); }
    auto operator ==(const char *str) { return std::string(str) == std::string(reinterpret_cast<char*>(string), size); }
    auto operator ==(std::string str) { return std::string(str) == std::string(reinterpret_cast<char*>(string), size); }
    auto operator ==(String&     str) { return std::string(str) == std::string(this); }
    auto operator =(std::string& str) { std::memset(string, 0, size); std::memcpy(string, str.data(), size); }
    
  private:
    int8_t string[size];
  };
  
  using char8   = Type<int8_t, int8_t>;
  using uchar8  = Type<uint8_t, uint8_t>;
  using int8    = Type<int8_t, int8_t>;
  using uint8   = Type<uint8_t, uint8_t>;
  using int16   = Type<int16_t, int16_t>;
  using uint16  = Type<uint16_t, uint16_t>;
  using int32   = Type<int32_t, int32_t>;
  using uint32  = Type<uint32_t, uint32_t>;
  using int64   = Type<int64_t, int64_t>;
  using uint64  = Type<uint64_t, uint64_t>;
  using float32 = Type<uint32_t, float>;
  
  template <typename T = Memory::TargetAddressType> using pointer = Pointer<T>;
  template <typename T = Memory::TargetAddressType> using doublepointer = DoublePointer<T>;
  template <size_t size = std::string::npos> using string = String<size>;
  
};
  
#endif /* types_hh */
