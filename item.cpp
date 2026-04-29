#include "item.h"

#include <ostream>

#include "auction_exceptions.h"

int item::total_items_created = 0;

item::item(const std::string& n, int p, int s) : name(n), price(p), stock(s) {
    if (price < 0 || stock < 0) {
        throw ItemError("Invalid item values: price/stock must be non-negative");
    }
    total_items_created++;
}

item::item(const item& other) : name(other.name), price(other.price), stock(other.stock) {
    total_items_created++;
}

item& item::operator=(const item& other) {
    if (this != &other) {
        name = other.name;
        price = other.price;
        stock = other.stock;
    }
    return *this;
}

item::~item() = default;

void item::stk_dcl() {
    stock--;
}

int item::getTotalItems() {
    return total_items_created;
}

std::string item::getName() const {
    return name;
}

int item::getPrice() const {
    return price;
}

int item::getStock() const {
    return stock;
}

std::ostream& operator<<(std::ostream& os, const item& i) {
    os << "Item: " << i.name << " -> Price: " << i.price << " $ (Stock: " << i.stock << ")";
    return os;
}
