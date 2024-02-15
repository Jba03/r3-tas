#include "cpa.hh"
#include <regex>

namespace CPA {
  
  void FileStreamContext::Load() {
    for (auto& file : files) {
      FileFormat::LVL<> *lvl = file.second;
      Stream *lvlStream = lvl->stream;
      // Set up pointers
      for (auto pointer : lvl->pointerFile->pointers) {
        FileFormat::DataFileIndex file = std::get<0>(pointer);
        Pointer<> ptr = std::get<1>(pointer);
        
        lvlStream->seek(ptr + 4);
        Pointer<> ptr2 = lvlStream->read<Pointer<>>();
        
        //lvl->pointerFile->
      }
    }
  }
  
  FileStreamContext::FileStreamContext(std::filesystem::path level) {
    if (std::filesystem::exists(level)) {
      std::string levelFolder = level.parent_path();
      std::string gamedataFolder = level.parent_path().parent_path();
      std::string lvlname = level.filename().replace_extension("").string();
      
      std::vector<std::pair<FileFormat::DataFileIndex, std::pair<std::filesystem::path, std::filesystem::path>>> f {
        { FileFormat::DataFileIndex::Fix, { gamedataFolder + "/fix.lvl", gamedataFolder + "/fix.ptr" } },
        { FileFormat::DataFileIndex::KeyFramesFix, { gamedataFolder + "/fixkf.lvl", gamedataFolder + "/fixkf.ptr" } },
        { FileFormat::DataFileIndex::Lvl, { levelFolder + "/" + lvlname + ".lvl", levelFolder + "/" + lvlname + ".ptr" }},
        { FileFormat::DataFileIndex::KeyFramesLvl, { levelFolder + "/" + lvlname + "kf.lvl", levelFolder + "/" + lvlname + "kf.ptr" }},
        { FileFormat::DataFileIndex::VertexBuffer, { levelFolder + "/" + lvlname + "_vb.lvl", levelFolder + "/" + lvlname + "_vb.ptr" }},
        { FileFormat::DataFileIndex::Transit, { levelFolder + "/transit.lvl", levelFolder + "/transit.ptr" }}
      };
      
      for (auto entry : f) {
        FileFormat::DataFileIndex idx = entry.first;
        std::pair<std::filesystem::path, std::filesystem::path> pair = entry.second;
        files[idx] = new FileFormat::LVL<>(pair.first);
      }
      
      Load();
    }
  }
  
  FileStreamContext::FileStreamContext(std::vector<std::filesystem::path> files) {
    
  }
  
};
