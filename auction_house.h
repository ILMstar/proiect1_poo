#pragma once

#include <iosfwd>
#include <memory>

#include "auction_participant.h"

class AuctionHouse {
private:
    std::unique_ptr<AuctionParticipant> opponent;

public:
    explicit AuctionHouse(const AuctionParticipant& opp);
    AuctionHouse(const AuctionHouse& other);
    AuctionHouse& operator=(AuctionHouse other);

    void describeOpponent(std::ostream& os) const;
    bool opponentIsBot() const;

    friend void swap(AuctionHouse& a, AuctionHouse& b) noexcept;
};
