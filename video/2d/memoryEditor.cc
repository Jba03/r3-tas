#include "gui.hh"

static auto highlightActor(stEngineObject *T, size_t offset, bool *highlight, ImColor *color) -> void {
  size_t addr = pointer<stEngineObject>(T).pointeeAddress().effectiveAddress();
  if (offset >= addr && offset < addr + sizeof(stEngineObject)) {
    *highlight = true;
    *color = ImColor(1.0f, 0.1f, 0.25f, 0.25f);
  }
}

static auto highlightTransform(stTransform *T, size_t offset, bool *highlight, ImColor *color) -> void {
  size_t addr = pointer<stTransform>(T).pointeeAddress().effectiveAddress();
  if (offset >= addr && offset < addr + sizeof(stTransform)) {
    *highlight = true;
    *color = ImColor(1.0f, 0.0f, 0.5f, 0.25f);
  }
}

template <typename T> static auto highlight(pointer<T> object, size_t offset) -> bool {
  size_t addr = object.pointeeAddress().effectiveAddress();
  return offset >= addr && offset <= addr + sizeof(T);
}

static auto highlightSuperObject(size_t offset, bool *b, ImColor *color) -> void {
  game::p_stDynamicWorld->forEachChild([&](stSuperObject *spo, void*) {
    size_t effective = pointer<stSuperObject>(spo).pointeeAddress().effectiveAddress();
    if (offset >= effective && offset <= effective + sizeof(stSuperObject)) {
      *b = true;
      *color = ImColor(1.0f, 0.75f, 0.0f, 0.25f);
    }
    
    if (spo->type == superObjectTypeActor) {
      
      if (highlight(spo->actor, offset)) {
        *color = ImColor(1.0f, 0.0f, 0.1f, 0.25f);
        *b = true;
      }
      
      if (spo->actor->dynam) {
        if (spo->actor->dynam->dynamics->base.report) {
          if (highlight(spo->actor->dynam->dynamics->base.report, offset)) {
            *color = ImColor(1.0f, 0.0f, 1.0f, 0.35f);
            *b = true;
          }
        }
        
        if (highlight(spo->actor->dynam->dynamics, offset)) {
          *color = ImColor(0.0f, 0.45f, 1.0f, 0.35f);
          *b = true;
        }
      }
    }
    
    if (highlight(spo->globalTransform, offset)) {
      *color = ImColor(0.0f, 1.0f, 0.5f, 0.25f);
      *b = true;
    }
  });
}

namespace gui {
  auto memoryEditorHighlight(const ImU8 *address, size_t offset) -> bool {
    if (!game::isValidGameState()) return false;
    
    bool highlight = false;
    ImColor color;
//    
//    try {
//      highlightSuperObject(offset, &highlight, &color);
//    } catch (bad_ptr& e) {
//      
//    }
    
    memoryEditor.HighlightColor = color;
    
    return highlight;
  }
}
