#pragma once

#include <iosfwd>
#include <string>

#include "auction_participant.h"

class player : public AuctionParticipant {
private:
    static const int kInvSize = 99;

    std::string name;
    int balance;
    int* inv;

public:
    player(const std::string& n = "Unknown", int b = 0);
    player(const player& other);
    player& operator=(player other);
    ~player();

    void bought(int id, int pret);
    void sold(int id, int pret);
    int check_balance() const;

    const std::string& getName() const override;
    AuctionParticipant* clone() const override;
    void print(std::ostream& os) const override;

    friend void swap(player& a, player& b) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const player& p);
};
