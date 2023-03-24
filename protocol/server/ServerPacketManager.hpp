#pragma once

#include "ServerPacketHandler.hpp"
#include "../PacketManager.hpp"

#include <map>
#include <vector>

namespace babel {

    class ServerPacketManager: public PacketManager {
        public:
            ServerPacketManager(std::shared_ptr<ServerManager> serverManager);

            void handlePacket(Packet &packet) override;
            void handleServerPacket(Packet &packet, IoClient *origin);
            void registerHandler(PacketType packetType, std::shared_ptr<ServerPacketHandler> packetHandler);

        private:
            std::shared_ptr<ServerManager> _serverManager = nullptr;
            std::map<PacketType, std::shared_ptr<ServerPacketHandler>> _packetHandlers = {};
    };
    
}
