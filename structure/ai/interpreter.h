//
//  interpreter.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-29.
//

#ifndef interpreter_h
#define interpreter_h

#include "engine.h"
#include "script.h"

struct ScriptInterpreterOptions {
    
};

struct ScriptInterpreter {
    struct Script* script;
    struct ScriptNode* current;
    struct ScriptInterpreterOptions opt;
    uint16_t pc;
    
    /* Macro callstack */
    struct Script* macros[16];
    int current_macro;
    
    bool condition;
};

int script_interpreter_create(struct ScriptInterpreter** intr,
                              struct Script* scpt,
                              struct ScriptInterpreterOptions* opt,
                              struct Engine* engine);

void script_interpreter_step(struct ScriptInterpreter* intr);

#endif /* interpreter_h */
