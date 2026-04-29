#pragma once

#include <ostream>
#include <string>

class AuctionParticipant {
public:
    virtual ~AuctionParticipant() = default;
    virtual AuctionParticipant* clone() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual const std::string& getName() const = 0;
};

inline std::ostream& operator<<(std::ostream& os, const AuctionParticipant& p) {
    p.print(os);
    return os;
}
