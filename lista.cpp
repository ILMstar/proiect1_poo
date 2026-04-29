#include "lista.h"

#include <fstream>
#include <iostream>
#include <random>

#include "auction_exceptions.h"
#include "rng.h"

lista::lista() {
    for (int i = 0; i < 10; i++) {
        display[i] = 0;
    }
}

lista::lista(const lista& other) {
    for (int i = 0; i < 10; i++) {
        display[i] = other.display[i];
    }
    lista_licitatie = other.lista_licitatie;
}

lista& lista::operator=(const lista& other) {
    if (this != &other) {
        for (int i = 0; i < 10; i++) {
            display[i] = other.display[i];
        }
        lista_licitatie = other.lista_licitatie;
    }
    return *this;
}

void lista::citire_lista(const std::string& fisier) {
    std::ifstream file(fisier);
    if (!file.is_open()) {
        throw FileError("File not found: " + fisier);
    }

    std::string line;
    while (getline(file, line)) {
        int pret = 0, stoc = 0, cee = 0;
        std::string nume_produs = "";

        for (std::size_t i = 0; i < line.length(); i++) {
            if (line[i] == '@') {
                cee++;
                continue;
            }
            if (cee == 0) {
                nume_produs += line[i];
            }
            if (cee == 1) {
                pret = pret * 10 + (line[i] - '0');
            }
            if (cee == 2) {
                stoc = stoc * 10 + (line[i] - '0');
            }
        }
        lista_licitatie.push_back(item(nume_produs, pret, stoc));
    }
    file.close();
}

void lista::item_select(int nr) {
    if (lista_licitatie.size() < static_cast<std::size_t>(nr)) {
        return;
    }

    std::uniform_int_distribution<int> dis(0, static_cast<int>(lista_licitatie.size() - 1));

    for (int i = 0; i < nr; i++) {
        int numar_extras;
        bool este_duplicat;
        do {
            este_duplicat = false;
            numar_extras = dis(global_rng());
            for (int j = 0; j < i; j++) {
                if (display[j] == numar_extras) {
                    este_duplicat = true;
                }
            }
        } while (este_duplicat);

        display[i] = numar_extras;
    }
}

void lista::show_selected_itms(int more) const {
    std::cout << "\n=== ITEMS AVAILABLE THIS ROUND ===\n";
    for (int i = 0; i < more; i++) {
        std::cout << "[" << i + 1 << "] " << lista_licitatie[display[i]] << "\n";
    }
}

void lista::show_bidding_item(int n) const {
    if (n >= 0 && n < 4) {
        std::cout << "\nBidding for: " << lista_licitatie[display[n]] << "\n";
    }
}

void lista::copy_item(item& current_item, int i) {
    current_item = lista_licitatie[display[i]];
}

int lista::get_id(int i) const {
    return display[i];
}

int lista::get_display(int i) const {
    return display[i];
}

std::ostream& operator<<(std::ostream& os, const lista& l) {
    os << "The list contains " << l.lista_licitatie.size() << " valid offers.";
    return os;
}
