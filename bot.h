#pragma once

#include <iosfwd>
#include <string>

#include "auction_participant.h"

class player;

class bot : public AuctionParticipant {
private:
    int bot_bid;
    double max_price;
    std::string name;

    double gen_mul();

public:
    bot(const std::string& n = "SimpleBOT");
    bot(const bot& other);
    bot& operator=(const bot& other);

    void call_mpr(int price_of_item);
    int new_bidding(player& current_player, int player_price, int item_id, int item_pret);
    int get_bid() const;

    const std::string& getName() const override;
    AuctionParticipant* clone() const override;
    void print(std::ostream& os) const override;

    friend std::ostream& operator<<(std::ostream& os, const bot& b);
};
