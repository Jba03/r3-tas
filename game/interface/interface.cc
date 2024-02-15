#include "interface.hh"
#include "hook.hh"
#include "debugger.hh"
#include "log.hh"
#include "video.hh"

#undef GCN
#undef PS2
#undef PC
#include "cpa.hh"

#if defined(WIN32)
# define LIBR3TAS_EXPORT __declspec(dllexport)
#else
# define LIBR3TAS_EXPORT
#endif

constexpr std::size_t constexpr_strlen(std::string_view s) { return s.size(); }
/* http://lolengine.net/blog/2011/12/20/cpp-constant-string-hash */
#define H1(s,i,x)   (x*65599u+(uint8_t)s[(i)<constexpr_strlen(s)?constexpr_strlen(s)-1-(i):constexpr_strlen(s)])
#define H4(s,i,x)   H1(s,i,H1(s,i+1,H1(s,i+2,H1(s,i+3,x))))
#define H16(s,i,x)  H4(s,i,H4(s,i+4,H4(s,i+8,H4(s,i+12,x))))
#define H64(s,i,x)  H16(s,i,H16(s,i+16,H16(s,i+32,H16(s,i+48,x))))
#define H256(s,i,x) H64(s,i,H64(s,i+64,H64(s,i+128,H64(s,i+192,x))))
#define HASH(s) ((uint32_t)(H256(s,0,0)^(H256(s,0,0)>>16)))

static VideoInterface *videoInterface;

Interface::Interface() {
  
  videoInterface = new VideoInterface(this);
  
//  debugger = new Debugger("/Users/jba03/Library/Application Support/Dolphin/Maps/GRHE41.map");
//  collisionTracker = new CollisionTracker();
  
  //applyOptimizations();
}

//void Interface::Update(emulator::message *msg) {
//  
//}

#pragma mark - GCN

static GCNInterface *GCN_Interface = nullptr;
static std::function<uint8_t*()> GCN_MemoryFunction = nullptr;

static void GCN_OnLoad(emulator::message *msg) {
  log::info(log::bold, log::green, "r3-tas loaded successfully\n");
  game::initialize();
  //graphics::initialize();
}

static void GCN_OnMemoryPointer(emulator::message *msg) {
  GCN_MemoryFunction = std::function<uint8_t*()>((uint8_t*(*)(void))msg->data);
}

static void GCN_OnUpdate(emulator::message *msg) {
  memory::baseAddress = GCN_MemoryFunction();
  
  using ACP = CPA<R3_GCN>;
  
  ACP::stEngineStructure *st = ACP::Pointer<>(0x003e7c0c);
  printf("%p\n", &st->mode);
  printf("base: %p / %p\n", memory::baseAddress, st->drawSem.ptr.hostAddress());
  
  game::update();
}

static void GCN_OnVideo(emulator::message *msg) {
  videoInterface->Render(msg->data);
}

static void GCN_OnUnload(emulator::message *msg) {
  log::info(log::bold, log::red, "r3-tas unloaded successfully\n");
}

static const std::map<int, std::function<void(emulator::message*)>> msgmap {
  { HASH("load"),          &GCN_OnLoad },
  { HASH("mram-function"), &GCN_OnMemoryPointer },
  { HASH("update"),        &GCN_OnUpdate },
  { HASH("video"),         &GCN_OnVideo },
  { HASH("unload"),        &GCN_OnUnload },
//  /* debug */
//  { HASH("hle-hook"),      &GCNInterface::create_hle_hooks },
//  { HASH("ppcstate"),      &GCNInterface::receive_ppc_state },
};

LIBR3TAS_EXPORT extern "C" void on_message(emulator::message msg) {
  auto entry = msgmap.find(msg.type);
  if (entry != msgmap.end())
    entry->second(&msg);
}

GCNInterface::GCNInterface() {
  /* ... */
}


#pragma mark - Native

NativeInterface::NativeInterface() {
  /* ... */
}
