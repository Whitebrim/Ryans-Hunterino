#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <algorithm> 
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <fstream>
#include <ppltasks.h>
#include <windowsx.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <TlHelp32.h>
#include <thread>
#include <filesystem>
#include <cctype>    
#include <iomanip>
#include <random>
#include <sstream>
#include <locale>
#include <cstdint>
#include <d2d1_1.h>
#include <dwrite.h>
#include <d3d11.h>
#include <dxgi.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "hv.lib")

#ifdef DrawText
#undef DrawText
#endif
#ifdef GetCurrentTime
#undef GetCurrentTime
#endif
#ifdef GetObject
#undef GetObject
#endif
#ifdef SendMessage
#undef SendMessage
#endif
#ifdef GetUserName
#undef GetUserName
#endif
#ifdef CreateFont
#undef CreateFont
#endif
#ifdef Button
#undef Button
#endif
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#include <vmmdll.h>
#define DEBUG_INFO
#ifdef DEBUG_INFO
#include "Logger.h"
#define LOG_DEBUG(...) Logger::GetInstance().LogDebug(__VA_ARGS__)
#define LOG_INFO(...) Logger::GetInstance().LogInfo(__VA_ARGS__)
#define LOG_WARNING(...) Logger::GetInstance().LogWarning(__VA_ARGS__)
#define LOG_ERROR(...) Logger::GetInstance().LogError(__VA_ARGS__)
#define LOG_CRITICAL(...) Logger::GetInstance().LogCritical(__VA_ARGS__)
#else
#define LOG
#define LOGW
#endif
#include "json.hpp"
using json = nlohmann::json;
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "vmm.lib")
#pragma comment(lib, "leechcore.lib")
#pragma comment(lib, "FTD3XX.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "windowscodecs.lib")
#include "Vector.h"
#include "Input.h"
#include "XorStr.h"
#include "Memory.h"
#include "CheatFunction.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"