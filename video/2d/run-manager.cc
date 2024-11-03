#include "gui.hh"
#include "run-manager.hh"

#include "imgui_internal.h"

static int i = 0;
static int fetchInterval = 10 * 60;

static std::map<std::string, ImColor> CategoryColor {
  {"Any%", ImColor(0.2f, 0.6f, 1.0f, 1.0f)},
  {"Any% NMG", ImColor(0.6f, 0.1f, 1.0f, 1.0f)},
  {"100%", ImColor(0.2f, 0.8f, 0.2f, 1.0f)},
  {"All Teensies", ImColor(1.0f, 1.0f, 0.25f, 1.0f)},
  {"No Teensies", ImColor(1.0f, 1.0f, 0.25f, 1.0f)},
  {"Glitched", ImColor(0.25f, 0.25f, 0.25f, 1.0f)},
};

static const char* const WorldNames[] = {
  "The Fairy Council",
  "Clearleaf Forest",
  "The Bog of Murk",
  "The Land of the Livid Dead",
  "The Desert of the Knaaren",
  "The Longest Shortcut",
  "The Summit Beyond the Clouds",
  "Hoodlum Headquarters",
  "The Tower of the Leptys",
};

static const char* const WorldEntryLevelNames[] {
  "intro_15",
  "wood_11",
  "Swamp_60",
  "Moor_00",
  "Knaar_10",
  "Flash_20",
  "Sea_10",
  "Fact_40",
  "Tower_10",
};

static std::map<std::string, std::string> LevelNameMap {
  {"intro_15", "Fairy Council 2"},
  {"Intro_17", "Fairy Council 3"},
  {"intro_20", "Fairy Council 4"},
  {"menu_00" , "Fairy Council 5"},
  
  {"wood_11", "Clearleaf Forest 1"},
  {"Wood_10", "Clearleaf Forest 2"},
  {"Wood_19", "Clearleaf Forest 3"},
  {"Wood_50", "Clearleaf Forest 4"},
  {"menu_10", "Clearleaf Forest 5"},
  
  {"Swamp_60", "Bog of Murk 1"},
  {"Swamp_82", "Bog of Murk 2"},
  {"Swamp_81", "Bog of Murk 3"},
  {"Swamp_83", "Bog of Murk 4"},
  {"Swamp_50", "Bog of Murk 5"},
  {"Swamp_51", "Bog of Murk 6"},
  
  {"Moor_00", "Land of the Livid Dead 1"},
  {"Moor_30", "Land of the Livid Dead 2"},
  {"moor_60", "Land of the Livid Dead 3"},
  {"moor_19", "Land of the Livid Dead 4"},
  {"menu_20", "Land of the Livid Dead 5"},
  
  {"Knaar_10", "Desert of the Knaaren 1"},
  {"Knaar_20", "Desert of the Knaaren 2"},
  {"Knaar_30", "Desert of the Knaaren 3"},
  {"Knaar_45", "Desert of the Knaaren 4"},
  {"Knaar_60", "Desert of the Knaaren 5"},
  {"Knaar_69", "Desert of the Knaaren 6"},
  {"Knaar_70", "Desert of the Knaaren 7"},
  {"menu_30" , "Desert of the Knaaren 8"},
  
  {"Flash_20", "Longest Shortcut 1"},
  {"Flash_30", "Longest Shortcut 2"},
  {"flash_10", "Longest Shortcut 3"},
  
  {"Sea_10"  , "Summit beyond the Clouds 1"},
  {"mount_50", "Summit beyond the Clouds 2"},
  {"mount_4x", "Summit beyond the Clouds 3"},
  
  {"Fact_40", "Hoodlum Headquarters 1"},
  {"Fact_50", "Hoodlum Headquarters 2"},
  {"Fact_55", "Hoodlum Headquarters 3"},
  {"fact_34", "Hoodlum Headquarters 4"},
  {"fact_22", "Hoodlum Headquarters 5"},
  
  {"Tower_10", "Tower of the Leptys 1"},
  {"Tower_20", "Tower of the Leptys 2"},
  {"Tower_30", "Tower of the Leptys 3"},
  {"Tower_40", "Tower of the Leptys 4"},
  {"lept_15" , "Tower of the Leptys 5"},
};

void drawRunManager() {
  
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.075f, 0.075f, 0.075f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
  
  ImGui::Begin("New run", nullptr);
  
  static int mode = 1;
  static int category = 0;
  static int uploadMode = 0;
  
  static int world = 7;
  static int level = 0;
  static std::string tmpLevelName = "";
  
  // Mode
  ImGui::SeparatorText("Mode");
  ImGui::RadioButton("Full game", &mode, 0);
  ImGui::RadioButton("Individual world", &mode, 1);
  ImGui::RadioButton("Individual level", &mode, 2);
  
  ImGui::Spacing();
  
  // Category
  ImGui::SeparatorText("Category");
  ImGui::BeginGroup();
  ImGui::RadioButton("Any%", &category, 0);
  ImGui::RadioButton("Any% NMG", &category, 1);
  ImGui::RadioButton("100%", &category, 2);
  ImGui::EndGroup();
  ImGui::SameLine();
  ImGui::BeginGroup();
  ImGui::RadioButton("All Teensies", &category, 3);
  ImGui::RadioButton("No Teensies", &category, 4);
  ImGui::EndGroup();
  ImGui::SameLine();
  
  ImGui::Spacing();
  
  // Mode options
  ImGui::SeparatorText("Options");
  if (mode == 1) {
    ImGui::Combo("World", &world, WorldNames, 9);
  } else if (mode == 2) {
    int i = 0;
    if (ImGui::BeginCombo("Level", tmpLevelName.c_str())) {
      for (auto& [fileName, levelName] : LevelNameMap) {
        if (ImGui::Selectable(levelName.c_str())) {
          tmpLevelName = levelName;
          level = i;
        }
        i++;
      }
      ImGui::EndCombo();
    }
   // ImGui::Combo("Level", &level, levelNameGetter, nullptr, g_stEngineStructure->levelCount);
  }
  
  ImGui::Spacing();
  
  // Upload options
  ImGui::SeparatorText("Upload run");
  ImGui::RadioButton("If it beats the current database record", &uploadMode, 0);
  ImGui::RadioButton("Always", &uploadMode, 1);
  ImGui::RadioButton("Do not upload", &uploadMode, 2);
  
  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();
  
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.6f, 1.0f, 1.0f));
  if (ImGui::Button("Go!")) {
    std::string levelName = WorldEntryLevelNames[0];
    if (mode == 1) {
      levelName = WorldEntryLevelNames[world];
    } else if (mode == 2) {
      levelName = (const char*)g_stEngineStructure->levelNames[level];
    }
    
    RunManager::StartOptions opt;
    opt.levelName = levelName;
    
    runManager.StartRecording(opt);
    
  }
  ImGui::PopStyleColor();
  
  if (ImGui::Button("Start playback")) {
    runManager.StartPlayback();
  }
  
  ImGui::End();
  
  ImGui::PopStyleVar();
  ImGui::PopStyleColor(2);
  
  
  
//  if ((i++ % fetchInterval) == 0) {
//    runManager.FetchRuns();
//  }
//
//
//  float Dx = ImGui::GetIO().DisplaySize.x;
//  float Dy = ImGui::GetIO().DisplaySize.y;
//
//  ImDrawList* drawlist = ImGui::GetBackgroundDrawList();
//  drawlist->AddRectFilled(ImVec2(0.0f, 0.0f), ImGui::GetIO().DisplaySize, ImColor(0.0f, 0.0f, 0.0f, 0.75f));
//
//  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
//  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.075f, 0.075f, 0.075f, 1.0f));
//  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
//  ImGui::Begin("Browse runs", nullptr);
//
//
//  for (auto& category : runManager.listing.items()) {
//    //category.type_name();
//   // printf("%s\n", category.object().);
//
//
//    ImGui::BeginTabBar("#aw");
//
//    if (ImGui::BeginTabItem(category.key().c_str())) {
//
//      if (ImGui::BeginTable("Dynamics advanced block", 6, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter)) {
//        ImGui::TableSetupColumn("Player", ImGuiTableColumnFlags_WidthStretch);
//        ImGui::TableSetupColumn("Platform", ImGuiTableColumnFlags_WidthStretch);
//        ImGui::TableSetupColumn("Mode", ImGuiTableColumnFlags_WidthStretch);
//        ImGui::TableSetupColumn("LRT", ImGuiTableColumnFlags_WidthFixed, 100.0f);
//        ImGui::TableSetupColumn("Verified", ImGuiTableColumnFlags_WidthFixed, 25.0f);
//        //ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
//        ImGui::TableHeadersRow();
//
//        for (auto run : category.value()) {
//          std::string author = run["author"];
//          std::string platform = run["platform"];
//          std::string mode = run["mode"];
//          std::string category = run["category"];
//          std::string loadRemovedTime = run["loadRemovedTime"];
//          bool verified = std::stoi(std::string(run["verified"]));
//
//
//          ImColor platformColor;
//          ImColor modeColor;
//
//
//
//
//          ImGui::TableNextColumn();
//          ImGui::TextColored(ImColor(0.2f, 0.6f, 1.0f, 1.0f), "%s", author.c_str());
//          ImGui::TableNextColumn();
//          ImGui::TextColored(ImVec4(0.1f, 0.5, 0.6f, 1.0f), "%s", platform.c_str());
//          ImGui::TableNextColumn();
//          ImGui::TextColored(ImVec4(0.5f, 0.1, 0.6f, 1.0f), "%s", mode.c_str());
//          ImGui::TableNextColumn();
//          ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.75), "%s", loadRemovedTime.c_str());
//          ImGui::TableNextColumn();
//          ImGui::RenderCheckMark( ImGui::GetForegroundDrawList(), ImGui::GetCursorScreenPos(), ImColor(0.0f, 1.0f, 0.25f, 1.0f), 10.0f);
//          ImGui::TableNextColumn();
//
//
//
//          //ImGui::TableNextColumn();
//        }
//        //ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.4f, 1.0f), "00:07:42");
//
//        ImGui::EndTable();
//      }
//
//
//      ImGui::EndTabItem();
//    }
//    ImGui::EndTabBar();
//  }
//
//
//  ImGui::End();
//  ImGui::PopStyleColor(2);
//  ImGui::PopStyleVar();
  
}
