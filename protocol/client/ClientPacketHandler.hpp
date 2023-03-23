#pragma once

#include "../PacketHandler.hpp"
#include "../packets/Packet.hpp"

#include <iostream>

namespace babel {

    class ClientManager;

    class ClientPacketHandler : public PacketHandler {
        public:
            virtual void handle(Packet &packet, std::shared_ptr<ClientManager> clientManager) = 0;
    };
    
}
