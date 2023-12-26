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

//#include "structure.hh"

#pragma mark - Main

#if defined(WIN32)
#   define LIBR3TAS_EXPORT __declspec(dllexport)
#else
#   define LIBR3TAS_EXPORT
#endif

constexpr std::size_t constexpr_strlen(std::string_view s) { return s.size(); }
/* http://lolengine.net/blog/2011/12/20/cpp-constant-string-hash */
#define H1(s,i,x)   (x*65599u+(uint8_t)s[(i)<constexpr_strlen(s)?constexpr_strlen(s)-1-(i):constexpr_strlen(s)])
#define H4(s,i,x)   H1(s,i,H1(s,i+1,H1(s,i+2,H1(s,i+3,x))))
#define H16(s,i,x)  H4(s,i,H4(s,i+4,H4(s,i+8,H4(s,i+12,x))))
#define H64(s,i,x)  H16(s,i,H16(s,i+16,H16(s,i+32,H16(s,i+48,x))))
#define H256(s,i,x) H64(s,i,H64(s,i+64,H64(s,i+128,H64(s,i+192,x))))
#define m(s) ((uint32_t)(H256(s,0,0)^(H256(s,0,0)>>16)))

static std::function<uint8_t*()> mram = nullptr;
static std::function<std::string()> get_config_path = nullptr;

static void on_load(emulator::message *msg) {
  info(BOLD "r3-tas loaded successfully\n");
  game::initialize();
  //graphics::initialize();
  gui::initialize();
}

static void receive_mram_function(emulator::message *msg) {
  mram = std::function<uint8_t*()>((uint8_t*(*)(void))msg->data);
}

static void on_update(emulator::message *msg) {
  if (!memory::baseAddress)
    memory::baseAddress = mram();
  game::update();
}

static void on_video(emulator::message *msg) {
  gui::video_payload* payload = (gui::video_payload*)msg->data;
  gui::draw(payload->context, payload->texture, &payload->windowed);
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
    interface::initialize();
}

static void receive_ppc_state(emulator::message *msg) {
  emulator::processorState *s = static_cast<emulator::processorState*>(msg->data);
  emulator::ppcState.pc = s->pc;
  emulator::ppcState.npc = s->npc;
  emulator::ppcState.gpr = s->gpr;
  emulator::ppcState.lr = s->lr;
}

static void unload(struct message *msg) {
    info(BOLD COLOR_RED "r3-tas successfully unloaded\n");
}

static const std::map<int, std::function<void(emulator::message*)>> msgmap {
  { m("load"), &on_load},
  { m("mram-function"), &receive_mram_function},
  { m("update"), &on_update},
  { m("video"), &on_video},
};

LIBR3TAS_EXPORT extern "C" void on_message(emulator::message msg) {
  auto entry = msgmap.find(msg.type);
  if (entry != msgmap.end()) entry->second(&msg);
  
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
}
