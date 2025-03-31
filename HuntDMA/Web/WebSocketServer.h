#pragma once
#include <atomic>

void StartWebSocketServer();

extern std::shared_ptr<CheatFunction> SendData;
extern std::atomic<bool> server_running;
