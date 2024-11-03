#include "interface.hh"
#include "hook.hh"
#include "log.hh"
#include "video.hh"
#include "tools.hh"

#include <locale>

#include <cpatools/cpatools.hpp>

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
inline std::filesystem::path mainConfigurationPath;
ordered_json config;
static int autoSaveTimer = 0;

Interface::Interface() {
  std::locale::global(std::locale::classic()); //for stringstream
  videoInterface = new VideoInterface(this);
  //applyOptimizations();
}

#pragma mark - Resources

static void loadConfig() {
  const std::ifstream input_stream(mainConfigurationPath / "config.json", std::ios_base::binary);
  if (input_stream.fail()) return;
  std::stringstream ss;
  ss << input_stream.rdbuf();
  config = ordered_json::parse(ss.str());
}

#if defined(__APPLE__)
#include <dlfcn.h>
#include <stdio.h>
__attribute__((constructor)) void install(void) {
  Dl_info info;
  dladdr((void*)install, &info);
  std::filesystem::path path = info.dli_fname;
  mainConfigurationPath = path.remove_filename();
  log::info(log::bold, log::blue, "Initializing...\n");
  log::info(log::bold, log::blue, "Using resource path " + std::string(mainConfigurationPath) + "\n");
}
#endif

std::filesystem::path Interface::configPath() {
  return mainConfigurationPath;
}

#pragma mark - GCN

static GCNInterface *GCN_Interface = nullptr;
static std::function<uint8_t*()> GCN_MemoryFunction = nullptr;

static void GCN_OnLoad(emulator::message *msg) {
  loadConfig();
  game::initialize();
  cpa::memory::readonly = false;
  
  log::info(log::bold, log::green, "r3-tas loaded successfully\n");
}

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

static void GCN_OnMemoryPointer(emulator::message *msg) {
  GCN_MemoryFunction = std::function<uint8_t*()>((uint8_t*(*)(void))msg->data);
  
  printf("got memory!\n");
}

static bool inited = false;
static void *mapped_mem = NULL;

static void GCN_OnUpdate(emulator::message *msg) {
  cpa::memory::baseAddress = GCN_MemoryFunction();
  cpa::memory::size = 24 * 1000 * 1000;
  
//  if (!inited) {
//    shm_unlink("/tmp/cpa-mem");
//    //uint8_t *base = GCN_MemoryFunction();
//    //int fd = open("/Users/jba03/dev/cpa-e/build/cpa-mem", O_RDWR);
//    int fd = shm_open("/tmp/cpa-mem", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
//    if (fd < 0) {
//      perror("open");
//    }
//    
//    if (ftruncate(fd, cpa::memory::size) < 0) {
//      perror("ftruncate");
//    }
//    
////    if (mlock(cpa::memory::baseAddress, cpa::memory::size) < 0) {
////      perror("mlock");
////    }
//
////    if (munmap(cpa::memory::baseAddress, cpa::memory::size) < 0) {
////      perror("munmap");
////    }
//    
//    mapped_mem = mmap(cpa::memory::baseAddress, cpa::memory::size, PROT_WRITE, MAP_SHARED, fd, 0);
//    if (mapped_mem == MAP_FAILED) {
//      perror("mmap");
//    }
//    
////    if (munlock(cpa::memory::baseAddress, cpa::memory::size)< 0) {
////      perror("munlock");
////    }
//    
//    inited = true;
//  }
//  
//  printf("%llX, %llX\n", mapped_mem, cpa::memory::baseAddress);
  
  game::update();
  
  if (mapped_mem) memcpy(mapped_mem, cpa::memory::baseAddress, cpa::memory::size);
  
  if ((autoSaveTimer++ % 120) == 0) {
    //if (mapped_mem) memcpy(mapped_mem, cpa::memory::baseAddress, cpa::memory::size);
    
    
    config["memory"]["readonly"] = memory::readonly;
    FILE* fp = fopen(std::string(mainConfigurationPath / "config.json").c_str(), "wb");
    std::string contents = config.dump(2);
    fwrite(contents.c_str(), contents.length(), 1, fp);
    fclose(fp);
  }
}

static void GCN_OnVideo(emulator::message *msg) {
  videoInterface->Render(msg->data);
}

static void GCN_OnUnload(emulator::message *msg) {
  log::info(log::bold, log::red, "r3-tas unloaded successfully\n");
}

static void GCN_CreateHLEHooks(emulator::message *msg) {
  emulator::createHook = (void (*)(uint32_t, const char*, int, int, void (*)()))(msg->data);
  GCN_Interface->applyOptimizations();
  GCN_Interface->r3solveInit();
}

static void GCN_ReceivePPCState(emulator::message *msg) {
  emulator::processorState *s = static_cast<emulator::processorState*>(msg->data);
  emulator::ppcState.pc = s->pc;
  emulator::ppcState.npc = s->npc;
  emulator::ppcState.gpr = s->gpr;
  emulator::ppcState.lr = s->lr;
}

static const std::map<int, std::function<void(emulator::message*)>> msgmap {
  { HASH("load"),          &GCN_OnLoad },
  { HASH("mram-function"), &GCN_OnMemoryPointer },
  { HASH("update"),        &GCN_OnUpdate },
  { HASH("video"),         &GCN_OnVideo },
  { HASH("unload"),        &GCN_OnUnload },
  /* debug */
  { HASH("hle-hook"),      &GCN_CreateHLEHooks },
  { HASH("ppcstate"),      &GCN_ReceivePPCState },
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


#if platform == GCN
  Interface *interface = new GCNInterface();
#elif platform == NATIVE
  Interface *interface = new NativeInterface();
#endif
