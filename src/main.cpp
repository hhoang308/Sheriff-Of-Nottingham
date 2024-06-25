#include "Server.h"
#include "Card.h"
#include "spdlog/spdlog.h"
#include "Player.h"
#include <iostream> 
#include <algorithm>  // for std::shuffle
#include <random>     // for std::default_random_engine
#include <chrono>     // for std::chrono::system_clock
#include <mutex>
#include <thread>
#include <string>
#include <enet/enet.h>

std::mutex playersMutex;
std::vector<Player> players;

// Assuming Card and Player classes are already defined

// Function to create and shuffle the deck
std::vector<Card> createAndShuffleDeck() {
    std::vector<Card> deck;

    // Add cards to the deck
    for (int i = 0; i < 48; ++i) {
        deck.emplace_back("Apple", CardType::LEGAL, 2, 2);
    }
    for (int i = 0; i < 36; ++i) {
        deck.emplace_back("Cheese", CardType::LEGAL, 3, 2);
    }
    for (int i = 0; i < 24; ++i) {
        deck.emplace_back("Chicken", CardType::LEGAL, 4, 2);
    }
    for (int i = 0; i < 18; ++i) {
        deck.emplace_back("Pepper", CardType::CONTRABAND, 8, 4);
    }
    for (int i = 0; i < 16; ++i) {
        deck.emplace_back("Mead", CardType::CONTRABAND, 7, 4);
    }
    for (int i = 0; i < 9; ++i) {
        deck.emplace_back("Silk", CardType::CONTRABAND, 9, 4);
    }
    for (int i = 0; i < 5; ++i) {
        deck.emplace_back("Crossbow", CardType::CONTRABAND, 10, 4);
    }

    // Shuffle the deck
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));

    return deck;
}

void broadcastMessage(ENetHost* server, const std::string& message) {
    ENetPacket* packet = enet_packet_create(message.c_str(), message.size() + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server, 0, packet);
}

void handleClient(ENetEvent& event){
    spdlog::info("A packet of length %u containing %s was received from %s on channel %u.", event.packet->dataLength, event.packet->data, event.peer->data, event.channelID);

    std::string receivedData(reinterpret_cast<char*>(event.packet->data));
    std::lock_guard<std::mutex> lock(playersMutex);

    // Parse the received data (format: "name:card")
    size_t delimiterPos = receivedData.find(':');
    if (delimiterPos != std::string::npos) {
        std::string playerName = receivedData.substr(0, delimiterPos);
        std::string playerRole = receivedData.substr(delimiterPos + 1);
        Player playerInfo(playerName, 50, playerRole);

        // Save the player information
        players.push_back(playerInfo);

        // Broadcast the message to all clients
        broadcastMessage(event.peer->host, receivedData);
    }

    enet_packet_destroy(event.packet);
}

int main(int argc, char* argv[]) {
    spdlog::info("Welcome to Sheriff of Nottingham!");

    // Create and shuffle the deck
    std::vector<Card> deck = createAndShuffleDeck();

    // Print the shuffled deck to verify
    for (auto& card : deck) {
        spdlog::info("Card Name: %s, Type: %s", card.getName(), (card.getType() == CardType::LEGAL ? "Legal" : "Contraband"));
    }

    if (enet_initialize() != 0) {
        spdlog::error("An error occurred while initializing ENet.");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    ENetAddress address;
    ENetHost* server;

    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */

    address.host = ENET_HOST_ANY;
    address.port = 80;

    server = enet_host_create(&address, 6, 2, 0, 0);
    if (server == nullptr) {
        spdlog::error("An error occurred while trying to create an ENet server host.");
        return EXIT_FAILURE;
    }

    spdlog::info("Server started on port 1234");

    ENetEvent event;
    while (true) {
        while (enet_host_service(server, &event, 10000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                spdlog::info("A new client connected from %d : %d", event.peer->address.host, event.peer->address.port);
                event.peer->data = (void*)"Client information";
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                handleClient(event);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                spdlog::info("Client disconnected.");
                event.peer->data = nullptr;
                break;

            default:
                break;
            }
        }
    }

    enet_host_destroy(server);

    return EXIT_SUCCESS;
}
