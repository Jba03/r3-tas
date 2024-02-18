//
//  main.c
//  r3lib
//
//  Created by Jba03 on 2022-11-12.
//

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#include <string>
#include <string_view>
#include <functional>

#include "game.hh"
#include "log.hh"
#include "gui.hh"
#include "graphics.hh"
#include "emulator.hh"
#include "interface.hh"

#pragma mark - Main

static std::function<uint8_t*()> mram = nullptr;
static std::function<std::string()> get_config_path = nullptr;

static void on_load(emulator::message *msg) {
  log::info(log::bold, log::green, "r3-tas loaded successfully\n");
  game::initialize();
  //graphics::initialize();
  gui::initialize();
}

static void receive_mram_function(emulator::message *msg) {
  mram = std::function<uint8_t*()>((uint8_t*(*)(void))msg->data);
}

static void on_update(emulator::message *msg) {
  if (!Memory::baseAddress)
    Memory::baseAddress = mram();
  
  game::update();
}

static void on_video(emulator::message *msg) {
//  gui::video_payload* payload = (gui::video_payload*)msg->data;
//  gui::draw(payload->context, payload->texture, &payload->windowed);
}

static void receive_create_texture_function(emulator::message *msg) {
  graphics::createTexture = (void (*)(const char*, unsigned, unsigned, unsigned))(msg->data);
}

static void receive_update_texture_function(emulator::message *msg) {
  graphics::updateTexture = (void (*)(const char*, unsigned, unsigned, unsigned, uint8_t*, size_t))(msg->data);
}

static void receive_get_texture_function(emulator::message *msg) {
    graphics::getTexture = (void* (*)(const char*))(msg->data);
}

static void receive_upload_texture_function(emulator::message *msg) {
    graphics::uploadTexture = (void (*)(const char*, void*, unsigned, unsigned))(msg->data);
}

static void receive_get_projection_matrix_function(emulator::message *msg) {
    graphics::getProjectionMatrix = (float *(*)(void))(msg->data);
}// 805d8518

static void graphics_initialize(emulator::message *msg) {
//  graphics::initialize();
//  gui::initialize();
}

static void create_hle_hooks(emulator::message *msg) {
  emulator::createHook = (void (*)(uint32_t, const char*, int, int, void (*)()))(msg->data);
  //interface::initialize();
}

static void receive_ppc_state(emulator::message *msg) {
  emulator::processorState *s = static_cast<emulator::processorState*>(msg->data);
  emulator::ppcState.pc = s->pc;
  emulator::ppcState.npc = s->npc;
  emulator::ppcState.gpr = s->gpr;
  emulator::ppcState.lr = s->lr;
}

static void on_unload(emulator::message *msg) {
  log::info(log::bold, log::red, "r3-tas successfully unloaded\n");
}

#if platform == GCN
  static Interface *interface = new GCNInterface();
#elif platform == NATIVE
  static Interface *interface = new NativeInterface();
#endif
  
//  interface->Update(&msg);
  
//    switch (msg.type) {
//        case m("load"):
//            on_load(&msg);
//            break;
//
//        case m("mram-function"):
//            receive_mram_function(&msg);
//            break;
//
//        case m("update"):
//            update(&msg);
//            break;
//
//        case m("hle-hook"):
//            create_hle_hooks(&msg);
//            break;
////
//        case m("ppcstate"):
//            receive_ppc_state(&msg);
//            break;
////
////        case m("input-request"):
////            break;
////
////        case m("graphics-initialize"):
////            graphics_initialize(&msg);
////            break;
//
//        case m("video"):
//            video(&msg);
//            break;
//
////        case m("create-texture"):
////            receive_create_texture_function(&msg);
////            break;
////
////        case m("update-texture"):
////            receive_update_texture_function(&msg);
////            break;
////
////        case m("upload-texture"):
////            receive_upload_texture_function(&msg);
////            break;
////
////        case m("get-texture"):
////            receive_get_texture_function(&msg);
////            break;
////
////        case m("get-projection-matrix"):
////            receive_get_projection_matrix_function(&msg);
////            break;
////
////        case m("unload"):
////            unload(&msg);
////            break;
//    }
//    
//    if (message.type == EXTERN_MESSAGE_ON_UPDATE) update("");
//    if (message.type == EXTERN_MESSAGE_ON_VIDEO) video(message.data);
//    if (message.type == EXTERN_MESSAGE_ON_LOADSTATE) loadstate_handler(*(int*)message.data);
//    if (message.type == EXTERN_MESSAGE_ON_SAVESTATE) savestate_handler(*(int*)message.data);
//    if (message.type == EXTERN_MESSAGE_LOADSTATE_FUNCTION) emulatorLoadState = message.data;
//    if (message.type == EXTERN_MESSAGE_SAVESTATE_FUNCTION) emulatorSaveState = message.data;
//    if (message.type == EXTERN_MESSAGE_MRAM_POINTER) get_mRAM = message.data;
//    if (message.type == EXTERN_MESSAGE_CONFIG_POINTER) get_config_path = message.data;
//    if (message.type == EXTERN_MESSAGE_SET_INPUT_STATUS) {
//        BruteforceStateHandler(message.data);
//        //BruteforceInput();
//        //emulatorPadStatus = message.data;
//        //BruteforceMain();
//        //printf("request pad status: %p\n", message.data);
//    }
//   // if (message.type == EXTERN_MESSAGE_FRAMEADVANCE_FUNCTION) emulatorFrameAdvance = message.data;
//
//    if (message.type == 11)
//    {
//        emulatorRunFunction = message.data;
//    }
//
//    if (message.type == 12)
//    {
//        AfterEngineExecuted();
//        //BruteforceMain(message.data);
//    }
//
//    if (message.type == 23)
//    {
//        //printf("hi!\n");
//        BeforeEngineExecuted(message.data);
//        //BruteforceInput();
//    }
//}
