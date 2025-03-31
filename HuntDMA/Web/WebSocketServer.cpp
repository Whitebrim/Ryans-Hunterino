#include <Pch.h>
#include "WebSocketServer.h"
#include "hv/WebSocketServer.h"
#include <iostream>
#include <thread>
#include "Globals.h"
#include "json.hpp"
using namespace hv;

std::atomic<bool> server_running = true;
std::vector<WebSocketChannelPtr> clients;
std::mutex clients_mutex;

void WebSocketServerThread() {
    WebSocketService ws;

    ws.onopen = [](const WebSocketChannelPtr& channel, const HttpRequestPtr& req) {
        if (req.get()->GetParam("passcode") != "HuntDMA") {
            LOG_INFO("Invalid passcode");
            channel.get()->close();
            return;
        }
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.push_back(channel);
        LOG_INFO("Client connected [%d]", clients.size());
    };

    ws.onmessage = [](const WebSocketChannelPtr& channel, const std::string& msg) {
        LOG_INFO("Received: %s", msg);
        channel->send("Echo: " + msg);
    };

    ws.onclose = [](const WebSocketChannelPtr& channel) {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), channel), clients.end());
        LOG_INFO("Client disconnected [%d]", clients.size());
    };

    WebSocketServer server(&ws);
    server.setPort(1896);
    server.setThreadNum(4);
    LOG_INFO("WebSocket is started on port 1896");

    server.run();
}

void StartWebSocketServer() {
    std::thread ws_thread(WebSocketServerThread);
    ws_thread.detach();
}

std::shared_ptr<CheatFunction> SendData = std::make_shared<CheatFunction>(10, [] {
    if (EnvironmentInstance == nullptr)
        return;

    if (EnvironmentInstance->GetObjectCount() < 10)
        return;

    json jsonData;
    jsonData["players"] = json::array();
    jsonData["map"] = EnvironmentInstance->mapType;

    std::vector<std::shared_ptr<WorldEntity>> tempPlayerList = EnvironmentInstance->GetPlayerList();
    for (const auto& ent : tempPlayerList) {
        auto type = ent->GetType();
        if (ent == nullptr || !ent->GetValid() || ent->IsHidden()) // Has extracted
            continue;

        Vector3 pos = ent->GetPosition();

        json entity = {
            {"x", pos.x},
            {"y", pos.y},
            {"type", [type] {
                switch (type) {
                    case EntityType::EnemyPlayer:   return "Enemy";
                    case EntityType::LocalPlayer:   return "Host";
                    case EntityType::FriendlyPlayer:return "Teammate";
                    default:                        return "Dead";
                }
            }()}
        };

        jsonData["players"].push_back(entity);
    }

    std::vector<std::shared_ptr<WorldEntity>> tempBossesList = EnvironmentInstance->GetBossesList();
    for (const auto& ent : tempBossesList) {
        Vector3 pos = ent->GetPosition();

        json entity = {
            {"x", pos.x},
            {"y", pos.y}
        };

        jsonData["bosses"].push_back(entity);
    }

    std::vector<std::shared_ptr<WorldEntity>> tempPOIList = EnvironmentInstance->GetPOIList();
    for (const auto& ent : tempPOIList) {
        if (ent->GetType() == EntityType::ExtractionPoint) {
            Vector3 pos = ent->GetPosition();

            json entity = {
                {"x", pos.x},
                {"y", pos.y},
                {"type", "Extraction"}
            };

            jsonData["poi"].push_back(entity);
        }
    }

    std::vector<std::shared_ptr<WorldEntity>> tempBloodBondsList = EnvironmentInstance->GetBloodBondsList();
    for (const auto& ent : tempBloodBondsList) {
        if (ent->GetType() == EntityType::GoldCashRegister) {
            Vector3 pos = ent->GetPosition();

            json entity = {
                {"x", pos.x},
                {"y", pos.y},
                {"type", "GoldCashRegister"}
            };

            jsonData["bb"].push_back(entity);
        }
    }

    if (!jsonData.empty()) {
        std::string jsonStr = jsonData.dump();
        //LOG_INFO("%s", jsonStr.c_str());
        std::lock_guard<std::mutex> lock(clients_mutex);
        for (const auto& client : clients) {
            if (client->isConnected()) {
                client->send(jsonStr);
            }
        }
    }
});
