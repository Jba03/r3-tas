#include "mecsync.hh"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <thread>

struct Message {
  char type[256];
  size_t length;
  char data[4096*8];
};

static int connection = 0;

static void send(Message& msg) {
  send(connection, &msg, sizeof(Message), 0);
}

void mecsync_start() {
  connection = socket(PF_INET, SOCK_STREAM, 0);
  
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = PF_INET;
  serverAddress.sin_port = htons(3001);
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  
  connect(connection, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
  
  // sending data
//  const char* message = "Hello, server!";
//  send(connection, message, strlen(message), 0);
}

void mecsync_send(std::string marker, pointer<stDynamics> dynamics, pointer<stSuperObject> object, double dt) {
  
 // send(connection, cpa::memory::baseAddress, cpa::memory::size * 0.001, 0);
  //printf("send: %s\n", marker.c_str());
  Message msg;
  memset(msg.type, 0, sizeof msg.type);
  strcpy(msg.type, marker.c_str());
  msg.length = sizeof(stDynamics) + sizeof(stTransform) + sizeof(double);
  memcpy(msg.data, dynamics, sizeof(stDynamics));
  memcpy(msg.data + sizeof(stDynamics), object->globalTransform, sizeof(stTransform));
  memcpy(msg.data + sizeof(stDynamics) + sizeof(stTransform), &dt, sizeof(double));
  send(msg);
  
  char buf[64];
  while (recv(connection, buf, 64, 0) == 0) {
    ;
  }
  
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  
//
//  memset(msg.type, 0, sizeof msg.type);
//  strcpy(msg.type, "rayman-transform");
//  msg.length = sizeof object->globalTransform->matrix;
//  memcpy(msg.data, &object->globalTransform->matrix, msg.length);
//  send(msg);
//
//  memset(msg.type, 0, sizeof msg.type);
//  strcpy(msg.type, "camera-transform");
//  msg.length = sizeof g_stEngineStructure->standardCamera->globalTransform->matrix;
//  memcpy(msg.data, &g_stEngineStructure->standardCamera->globalTransform->matrix, msg.length);
//  send(msg);
  
}
