#include "Server.h"
#include "Logging.h"
#include "Card.h"
#include <algorithm>  // for std::shuffle
#include <random>     // for std::default_random_engine
#include <chrono>     // for std::chrono::system_clock

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

int main(int argc, char* argv[]) {
    // Server initialize
    // Server server;
    // if (server.init()) {
    //     server.run();
    // }

    // Create and shuffle the deck
    std::vector<Card> deck = createAndShuffleDeck();

    // Print the shuffled deck to verify
    for (auto& card : deck) {
        //std::cout << "Card Name: " << card.getName() << ", Type: " << (card.getType() == CardType::LEGAL ? "Legal" : "Contraband") << "\n";
        LOG_INFO("Card Name: %s, Type: %s\n", card.getName(), (card.getType() == CardType::LEGAL ? "Legal" : "Contraband"));
    }
    return 0;
}
