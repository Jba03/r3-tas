#ifndef cpa_hh
#define cpa_hh

#include "structure.hh"
#include "fileformat.hh"

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
