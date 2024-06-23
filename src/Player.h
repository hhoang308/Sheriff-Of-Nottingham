#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <algorithm>
#include "Card.h"

enum class Role {
    SHERIFF,
    MERCHANT
};

class Player {
public:
    // Constructor
    Player(const std::string& name, Role role);

    // Getters and setters
    std::string getName() const;
    Role getRole() const;
    void setRole(Role newRole);
    int getGold() const;
    void addGold(int amount);
    void subtractGold(int amount);

    // Card management
    void addCardToHand(const Card& card);
    const std::vector<Card>& getHand() const;
    bool removeCardFromHand(const Card& card);
    
    void addCardToGoods(const Card& card);
    const std::vector<Card>& getGoods() const;

private:
    std::string name;
    Role role;
    int gold;
    std::vector<Card> hand;
    std::vector<Card> goods;

};

#endif // PLAYER_H
