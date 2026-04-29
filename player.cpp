#include "player.h"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <utility>

#include "auction_exceptions.h"

player::player(const std::string& n, int b) : name(n), balance(b), inv(new int[kInvSize]) {
    if (balance < 0) {
        throw PlayerError("Invalid player balance: must be non-negative");
    }
    for (int i = 0; i < kInvSize; i++) {
        inv[i] = 0;
    }
}

player::player(const player& other) : name(other.name), balance(other.balance), inv(new int[kInvSize]) {
    std::copy(other.inv, other.inv + kInvSize, inv);
}

player& player::operator=(player other) {
    swap(*this, other);
    return *this;
}

player::~player() {
    delete[] inv;
}

void player::bought(int id, int pret) {
    inv[id]++;
    balance -= pret;
}

void player::sold(int id, int pret) {
    if (inv[id] > 0) {
        inv[id]--;
        balance += pret;
    } else {
        std::cout << "You don't have this item\n";
    }
}

int player::check_balance() const {
    return balance;
}

const std::string& player::getName() const {
    return name;
}

AuctionParticipant* player::clone() const {
    return new player(*this);
}

void player::print(std::ostream& os) const {
    os << "Player: " << name << " | Balance: " << balance << " $";
}

void swap(player& a, player& b) noexcept {
    using std::swap;
    swap(a.name, b.name);
    swap(a.balance, b.balance);
    swap(a.inv, b.inv);
}

std::ostream& operator<<(std::ostream& os, const player& p) {
    os << "Player: " << p.name << " | Balance: " << p.balance << " $";
    return os;
}
