#include "auction_house.h"

#include <ostream>
#include <utility>

#include "bot.h"

AuctionHouse::AuctionHouse(const AuctionParticipant& opp)
    : opponent(opp.clone()) {}

AuctionHouse::AuctionHouse(const AuctionHouse& other)
    : opponent(other.opponent ? other.opponent->clone() : nullptr) {}

AuctionHouse& AuctionHouse::operator=(AuctionHouse other) {
    swap(*this, other);
    return *this;
}

void AuctionHouse::describeOpponent(std::ostream& os) const {
    if (opponent) {
        os << *opponent;
    }
}

bool AuctionHouse::opponentIsBot() const {
    return dynamic_cast<const bot*>(opponent.get()) != nullptr;
}

void swap(AuctionHouse& a, AuctionHouse& b) noexcept {
    using std::swap;
    swap(a.opponent, b.opponent);
}
