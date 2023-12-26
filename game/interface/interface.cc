#include "interface.hh"
#include "hook.hh"

namespace interface
{
    bruteforcingTool *bruteforceTool = nullptr;
  
    auto initialize() -> bool
    {
      
        bruteforceTool = new bruteforcingTool;
      applyOptimizations();
      
        return true;
    }
};
