#pragma once

#include <iosfwd>
#include <string>

class item {
private:
    std::string name;
    int price;
    int stock;
    static int total_items_created;

public:
    item(const std::string& n = "", int p = 0, int s = 0);
    item(const item& other);
    item& operator=(const item& other);
    ~item();

    void stk_dcl();

    static int getTotalItems();

    std::string getName() const;
    int getPrice() const;
    int getStock() const;

    friend std::ostream& operator<<(std::ostream& os, const item& i);
};
