//
//  gui.hh
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#ifndef gui_hpp
#define gui_hpp


namespace gui
{
    void initialize();
    void draw(void *c, void *texture, bool *windowed);
}
    
#include "imgui.h"

struct Window
{
    void (*Draw)(void);
    void (*SaveSettings)(void);
    bool Open = false;
};

extern Window MenuBar;
extern Window ConfigurationWindow;
extern Window RNGWindow;
extern Window HierarchyWindow;
extern Window RunCreateWindow;
extern Window CinematicWindow;
//extern Window MovieInputWindow;
//extern Window ScriptWindow;
//extern Window SuperObjectWindow;
extern Window bruteforceWindow;

#endif /* gui_hh */
