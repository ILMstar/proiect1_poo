#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "item.h"

class lista {
private:
    int display[10];
    int shopItms[10];

public:
    std::vector<item> lista_licitatie;

    lista();
    lista(const lista& other);
    lista& operator=(const lista& other);

    void citire_lista(const std::string& fisier);
    void item_select(int nr);
    void show_selected_itms(int more) const;
    void show_bidding_item(int n) const;
    void copy_item(item& current_item, int i);
    int get_id(int i) const;
    int get_display(int i) const;

    friend std::ostream& operator<<(std::ostream& os, const lista& l);
};
