#ifndef cpa_hh
#define cpa_hh

#include "structure.hh"
#include "fileformat.hh"

//
// Ways of making this library dynamic (target platform changeable at runtime):
//  1. Make CPA a class and template it with a version parameter.
//     Enable struct members according to version specializations (may be difficult).
//     Pros: It's dynamic. Cons: Header-only, single file, very slow compile time.
//
//  2. Create a version data library and read all structures into std:map.
//     Pros: Dynamic, fast compile time. Cons: Ugly map access syntax + types not pre-specified, slow runtime with constant rwops.
//
//
//
//

namespace CPA {
  
  /// Statically mapped context using live memory stream.
  struct MemoryStreamContext {
    MemoryStreamContext(void * memory, size_t size);
  };
  
  /// Statically mapped context using file streams.
  struct FileStreamContext {
    /// Initialize using level file in gamedata
    FileStreamContext(std::filesystem::path level);
    /// Initialize using a list of gamedata files
    FileStreamContext(std::vector<std::filesystem::path> files);
    
  private:
    void Load();
    std::map<FileFormat::DataFileIndex, FileFormat::LVL<>*> files;
    
    friend struct FileFormat::LVL<>;
    friend struct FileFormat::PTR<>;
  };
  
};

#endif /* cpa_hh */
