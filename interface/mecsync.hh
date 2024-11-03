#pragma once

#include "game.hh"

void mecsync_start();
void mecsync_send(std::string marker, pointer<stDynamics> dynamics, pointer<stSuperObject> object, double dt = 0.0f);
