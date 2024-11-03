#ifndef run_manager_hpp
#define run_manager_hpp

#include "game.hh"
#include "constants.hh"

using nlohmann::ordered_json;

struct Run {
  
  Run();
  
  const std::vector<std::string> PlatformNames {
    "GCN Emu",
    "PC"
  };

  const std::vector<std::string> ModeNames {
    "Full game",
    "Individual world",
    "Individual level",
  };
  
  const std::vector<std::string> CategoryNames {
    "Any%",
    "All Teensies",
    "No Teensies",
    "Any% No Major Glitches",
    "100%",
    "Glitched"
  };

  const std::map<std::string, std::string> WorldNames {
    
  };
  
  struct Frame {
    stInputStructure ipt;
    float mAngleX = 0.0f, mAngleY = 0.0f;
    float cAngleX = 0.0f, cAngleY = 0.0f;
    uint32_t levelTimer = 0;
    
    stInputEntryElement entries[1000];
    
    stVector3D playerPosition;
    stTransform cameraTransformGlobal;
    stTransform cameraTransformLocal;
    stTransform cameraTransformViewport;
    //vector<3, float> cameraPosition;
    
    stVector3D dsg36, dsg38, dsg65, dsg67;
    
    void Store() {
//      playerPosition = g_stEngineStructure->currentMainPlayers[0]->position();
//      cameraTransformViewport = g_stEngineStructure->viewportCamera[0]->transform;
//
//      dsg36 = *(stVector3D*)g_stEngineStructure->standardCamera->actor->dsgVar(36);
//      dsg38 = *(stVector3D*)g_stEngineStructure->standardCamera->actor->dsgVar(38);
//      dsg65 = *(stVector3D*)g_stEngineStructure->standardCamera->actor->dsgVar(65);
//      dsg67 = *(stVector3D*)g_stEngineStructure->standardCamera->actor->dsgVar(67);
//
//      cameraTransformGlobal = *g_stEngineStructure->standardCamera->globalTransform;
//      cameraTransformLocal = *g_stEngineStructure->standardCamera->localTransform;
//
      
      memcpy(entries, g_stInputStructure->entries, sizeof(stInputEntryElement) * (int)g_stInputStructure->numEntries);
      
//      mAngleX = g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeX].analogValue;
//      mAngleY = g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeY].analogValue;
//      cAngleX = g_stInputStructure->entries[IPT_E_Entry_Action_Pad1_AxeX].analogValue;
//      cAngleY = g_stInputStructure->entries[IPT_E_Entry_Action_Pad2_AxeY].analogValue;
      levelTimer = g_stEngineStructure->timer.currentFrame;
    }
    
    void Load() {
      try {
        //g_stEngineStructure->currentMainPlayers[0]->actor->brain->mind->intelligence = nullptr;
        //g_stEngineStructure->standardCamera->actor->brain->mind->intelligence = nullptr;
        
//        *(uint8_t*)g_stEngineStructure->currentMainPlayers[0]->actor->dsgVar(0) = 4;
//        *(uint8_t*)g_stEngineStructure->currentMainPlayers[0]->actor->dsgVar(1) = 1;
//        *(uint8_t*)g_stEngineStructure->currentMainPlayers[0]->actor->dsgVar(4) = 1;
//
//        *(uint8_t*)g_stEngineStructure->standardCamera->actor->dsgVar(27) = 1;
//        *(uint8_t*)g_stEngineStructure->standardCamera->actor->dsgVar(25) = 1;
//
//        *(stVector3D*)g_stEngineStructure->standardCamera->actor->dsgVar(36) = dsg36;
//        *(stVector3D*)g_stEngineStructure->standardCamera->actor->dsgVar(38) = dsg38;
//        *(stVector3D*)g_stEngineStructure->standardCamera->actor->dsgVar(65) = dsg65;
//        *(stVector3D*)g_stEngineStructure->standardCamera->actor->dsgVar(67) = dsg67;
//
//       // *(uint8*)g_stEngineStructure->currentMainPlayers[0]->actor->dsgVar(4) = 1;
//        g_stEngineStructure->viewportCamera[0]->transform = cameraTransformViewport;
//        *g_stEngineStructure->standardCamera->globalTransform = cameraTransformGlobal;
//        *g_stEngineStructure->standardCamera->localTransform = cameraTransformLocal;
//        g_stEngineStructure->currentMainPlayers[0]->position() = playerPosition;
//        printf("skfaofjpfaokfpsfokp2e2e12421\n");
        //g_stEngineStructure->inputMode = engineInputModeCommands;
//        g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeX].analogValue = mAngleX;
//        g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeY].analogValue = mAngleY;
//        g_stInputStructure->entries[IPT_E_Entry_Action_Pad1_AxeX].analogValue = cAngleX;
//        g_stInputStructure->entries[IPT_E_Entry_Action_Pad2_AxeY].analogValue = cAngleY;
//        g_stInputStructure->entries[IPT_E_Entry_Action_Tirer].state = mAngleX;
        
        
        memcpy(g_stInputStructure->entries, entries, sizeof(stInputEntryElement) * (int)g_stInputStructure->numEntries);
        
        printf("expected level frame %d, current = %d\n", levelTimer, int(g_stEngineStructure->timer.currentFrame));
        
      } catch (...) {
       // printf("skfaofjpfaokfpsfokp\n");
      }
//      g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeX].analogValue = float((drand48() - 0.5f) * 200.0f);
//      g_stInputStructure->entries[IPT_E_Entry_Action_Pad0_AxeY].analogValue = float((drand48() - 0.5f) * 200.0f);
    }
  };
  
  struct Section {
    enum Type { Level, Load } type;
    std::string levelName;
    std::string name;
    uint32_t framecount = 0;
    uint64_t score = 0;
    std::vector<Frame> frames;
    
    stEngineTimer timer;
    
    Section(Type t);
  };
  
  struct Info {
    /// Format version
    std::string version = "1.0v";
    /// The player (author) of the run
    std::string author;
    /// The platform
    std::string platform;
    /// Run mode
    std::string mode;
    /// Run category
    std::string category;
    /// Hash
    std::string hash;
  };
  
  void RecordFrame();
  void PlaybackFrame();
  
  ordered_json info;
  /// Run sections (loads & levels)
  std::vector<Section> sections;
  
  Section* currentSection = nullptr;
  
  int currentFrame = 0;
};

class RunManager {
public:
  enum UploadMode {
    BeatsCurrentRecord,
    Always,
    Never,
  };
  
  enum Mode {
    Recording,
    Playback
  };
  
  struct StartOptions {
    std::string mode;
    std::string category;
    std::string levelName;
    UploadMode uploadMode;
  };
  
  
  void StartRecording(StartOptions& opt);
  void StartPlayback();
  
  void Update();
  
  void LoadRun(std::string name);
  void UploadRun(std::string name);
  
  void DownloadRun();
  void FetchRuns();
  
  ordered_json listing;
  
  bool timerTick = false;
  
  Mode mode = Recording;
  Run* currentRun = nullptr;
};

extern RunManager runManager;

#endif /* run_manager_hpp */
