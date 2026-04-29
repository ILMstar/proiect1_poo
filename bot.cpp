#include "bot.h"

#include <iostream>
#include <ostream>
#include <random>

#include "player.h"
#include "rng.h"

bot::bot(const std::string& n) : bot_bid(0), max_price(0.0), name(n) {}

bot::bot(const bot& other) : bot_bid(other.bot_bid), max_price(other.max_price), name(other.name) {}

bot& bot::operator=(const bot& other) {
    if (this != &other) {
        name = other.name;
        bot_bid = other.bot_bid;
        max_price = other.max_price;
    }
    return *this;
}

double bot::gen_mul() {
    std::uniform_real_distribution<double> dis(0.5, 3.0);
    return dis(global_rng());
}

void bot::call_mpr(int price_of_item) {
    max_price = price_of_item * gen_mul();
}

int bot::new_bidding(player& current_player, int player_price, int item_id, int item_pret) {
    (void)item_pret;
    if (player_price < max_price) {
        std::uniform_real_distribution<double> dis(0.5, 2.1);
        bot_bid = player_price + static_cast<int>(500 * dis(global_rng()));
        std::cout << "The bot has bid " << bot_bid << "\n";
        return true;
    }
    std::cout << "You got the item!\n";
    current_player.bought(item_id, player_price);
    return false;
}

int bot::get_bid() const {
    return bot_bid;
}

const std::string& bot::getName() const {
    return name;
}

AuctionParticipant* bot::clone() const {
    return new bot(*this);
}

void bot::print(std::ostream& os) const {
    os << "Bot " << name;
}

std::ostream& operator<<(std::ostream& os, const bot& b) {
    os << "Bot " << b.name;
    return os;
}
