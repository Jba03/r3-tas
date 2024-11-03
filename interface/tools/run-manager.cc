#include "run-manager.hh"
#include "interface.hh"

//#include <curl/curl.h>

RunManager runManager;

#pragma mark - Run

Run::Section::Section(Type t) {
  type = t;
  timer = g_stEngineStructure->timer;
}


Run::Run() {
}

void Run::RecordFrame() {
  if (game::engineModeChangedTo(engineModeChangeLevel, engineModeGameplay)) {
    // Enter transition
    Section section(Run::Section::Type::Load);
    char buf[128];
    sprintf(buf, "%s -> %s", (const char*)g_stEngineStructure->currentLevelName, (const char*)g_stEngineStructure->nextLevelName);
    
    section.name = buf;
    sections.push_back(section);
  } else if (game::engineModeChangedTo(engineModeGameplay, engineModeEnterLevel)) {
    // Enter level
    Section section(Run::Section::Type::Level);
    section.name = std::string(g_stEngineStructure->currentLevelName);
    sections.push_back(section);
  }
  
  if (sections.size() > 0)
    currentSection = &sections.back();
  
  if (currentSection) {
    //printf("updating section: %s, framecount: %lld\n", currentSection->name.c_str(), currentSection->framecount);
    
    Run::Frame frame;
    //frame.playerPosition = g_stEngineStructure->currentMainPlayers[0]->position();
    //memcpy(&frame.ipt.device, g_stInputStructure->device, sizeof frame.ipt.device);
    frame.Store();
    
    currentSection->frames.push_back(frame);
    
    currentSection->framecount++;
  }
}

void Run::PlaybackFrame() {
  
  
  if (game::engineModeChangedTo(engineModeChangeLevel, engineModeGameplay)) {
    //currentSection++;
  } else if (game::engineModeChangedTo(engineModeGameplay, engineModeEnterLevel)) {
    currentSection++;
    currentFrame = 0;
    g_stEngineStructure->timer = currentSection->timer;
  }
  
  
  if (currentSection) {
    if (currentFrame < currentSection->framecount && currentSection->type == Section::Level) {
      printf("playing back frame %d out of %d from section %s\n", currentFrame, currentSection->framecount, currentSection->name.c_str());
      //printf("frame count: %d\n", currentSection->framecount);
      if (currentSection->frames.size() > 0) {
        Frame& frame = currentSection->frames[currentFrame+1];
        frame.Load();
      }
    }
    currentFrame++;
  }
}

#pragma mark - Run Manager


static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
  data->append((char*) ptr, size * nmemb);
  return size * nmemb;
}

void DownloadRuns() {
  
}


void RunManager::FetchRuns() {
  
//  ordered_json runs = ordered_json::parse(response_string)["runs"];
//
////  std::vector<std::string> categories;
////  for (auto run : runs) {
////    if (std::find(categories.begin(), categories.end(), run["category"]) == categories.end()) {
////      categories.emplace_back(run["category"]);
////    }
////  }
//
//  listing.clear();
//  for (auto run : runs) {
//    listing[run["category"]].push_back(run);
//  }
  
}

void RunManager::StartRecording(StartOptions& opt) {
  mode = Recording;
  currentRun = new Run();  
  g_stEngineStructure->loadLevel(opt.levelName);
}

void RunManager::StartPlayback() {
  mode = Playback;
  if (currentRun) {
    currentRun->currentSection = &currentRun->sections[0];
    
    Run::Section* section = nullptr;
    for (auto &sct : currentRun->sections) {
      if (sct.type == Run::Section::Level) {
        section = &sct;
        break;
      }
    }
    
    if (section) {
      printf("loading %s", section->name.c_str());
      g_stEngineStructure->loadLevel(section->name);
    }
  }
}

void RunManager::Update() {
  if (currentRun) {
    if (mode == Playback) {
      currentRun->PlaybackFrame();
    } else if (mode == Recording) {
      currentRun->RecordFrame();
    }
  }
}
