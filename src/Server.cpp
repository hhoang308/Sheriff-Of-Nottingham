#include "Server.h"
#include <iostream>
#include <enet/enet.h>

bool Server::init() {
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet.\n";
        return false;
    }
    atexit(enet_deinitialize);

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 1234;

    server = enet_host_create(&address, 32, 2, 0, 0);
    if (!server) {
        std::cerr << "An error occurred while trying to create an ENet server host.\n";
        return false;
    }

    return true;
}

void Server::run() {
    ENetEvent event;
    while (true) {
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "A new client connected from "
                              << event.peer->address.host
                              << ":" << event.peer->address.port << ".\n";
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "A packet of length "
                              << event.packet->dataLength
                              << " containing "
                              << (char*)event.packet->data
                              << " was received from client on channel "
                              << (int)event.channelID << ".\n";
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Client disconnected.\n";
                    break;

                default:
                    break;
            }
        }
    }
}

Server::~Server() {
    if (server) {
        enet_host_destroy(server);
    }
}
