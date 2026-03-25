/*
 * DESCRIEREA PROBLEMEI:
 * Tema 34: Licitatii (Joc de licitatie / Shop)
 * - Date de intrare: Se citeste dintr-un fisier text ("obiecte.txt") o lista de obiecte 
 * disponibile pentru licitatie (Nume, Pret de pornire, Stoc) parsate cu delimitatorul '@'.
 * - Operatii posibile: Jucatorul primeste un buget, sistemul selecteaza aleator 
 * obiecte unice, iar jucatorul poate alege pentru care obiecte doreste sa liciteze.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>

class item {
private:
    std::string name;
    int price;
    int stock;
    
public:
    item(const std::string& n = "", int p = 0, int s = 0) {
        name = n; price = p; stock = s;
    }
    
    item(const item& other) {
        name = other.name; price = other.price; stock = other.stock;
    }
    
    item& operator=(const item& other) {
        if (this != &other) {
            name = other.name; price = other.price; stock = other.stock;
        }
        return *this;
    }
    
    ~item() {}

    void stk_dcl() { stock--; }
    
    std::string getName() const { return name; }
    int getPrice() const { return price; }
    int getStock() const { return stock; }

    friend std::ostream& operator<<(std::ostream& os, const item& i) {
        os << "Item: " << i.name << " -> Price: " << i.price << " $ (Stoc: " << i.stock << ")";
        return os;
    }
};

class player {
private:
    std::string name;
    int balance;
    int inv[99]; 
    
public:
    player(const std::string& n = "Unknown", int b = 0) {
        name = n;
        balance = b;
        for(int i=0; i<99; i++) inv[i] = 0;
    }
    
    player(const player& other) {
        name = other.name; balance = other.balance;
        for(int i=0; i<99; i++) inv[i] = other.inv[i];
    }
    
    player& operator=(const player& other) {
        if (this != &other) {
            name = other.name; balance = other.balance;
            for(int i=0; i<99; i++) inv[i] = other.inv[i];
        }
        return *this;
    }
    
    ~player() {}

    void bought(int id, int pret) {
        inv[id]++;
        balance -= pret;
    }

    int check_balance() const { 
        return balance; 
    }
    
    friend std::ostream& operator<<(std::ostream& os, const player& p) { // returneaza prin consola
        os << "Jucator: " << p.name << " | Balanta: " << p.balance << " $";
        return os;
    }
};

class bot {
    private:
        int bot_bid;
        int max_price;
        std::string name;
        int gen_mul(){ //gen_mul se va folosi ca botul sa nu poata sa parieze mai mult de un anumit nivel
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dis(0.5, 2.1);
            return dis(gen);
        }
    public:
        bot(const std::string& n = "SimpleBOT"){
            name = n;
        }
        bot& operator=(const bot& other) {
            if (this != &other) {
                name = other.name;
            }
            return *this;
        }
        void call_mpr(int price_of_item) { // aici se stabileste pretul maxim pe care il poate avea la bid
            max_price = price_of_item * gen_mul();
        }
        int new_bidding(player& current_player, int player_price, int item_id, int item_pret){ // game engineul practic
            if (player_price < max_price) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<double> dis(0.5, 2.1);
                bot_bid = player_price + (100 * dis(gen));
                std::cout<<"The bot has bid "<< bot_bid <<"\n";
                return true;
            }
            else {
                std::cout << "You got the item!\n";
                current_player.bought(item_id, item_pret); // bought updateaza inventarul playerului si ii scade din balance
                return false;
            }
        }
};

class lista {
private:
    int display[4];
    
public:
    std::vector<item> lista_licitatie;

    void citire_lista(const std::string& fisier) { // face un vector de "item"e
        std::ifstream file(fisier);
        if(!file.is_open()) {
            std::cout << "File not found " << fisier << "\n";
            return;
        }
        
        std::string line;
        while (getline(file, line)) {
            int pret = 0, stoc = 0, cee = 0;
            std::string nume_produs = "";
            
            for (std::size_t i = 0; i < line.length(); i++) {
                if (line[i] == '@') { // in fisier numele pretul si cantitatea sunt desparitite de @
                    cee++; continue;
                }
                if (cee == 0) nume_produs += line[i];
                if (cee == 1) pret = pret * 10 + (line[i] - '0');
                if (cee == 2) stoc = stoc * 10 + (line[i] - '0');
            }
            lista_licitatie.push_back(item(nume_produs, pret, stoc));
        }
        file.close();
    }

    void item_select() { // alege 4 iteme la intamplare cu random bazat pe ceasul calculatorului
        if (lista_licitatie.size() < 4) return; // daca lista e mai mica decat 4 sa nu dea eroare

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, lista_licitatie.size() - 1);
        
        for (int i = 0; i < 4; i++) {
            int numar_extras;
            bool este_duplicat;
            do {
                este_duplicat = false;
                numar_extras = dis(gen);
                // check de duplicate
                for (int j = 0; j < i; j++) {
                    if (display[j] == numar_extras) {
                        este_duplicat = true;
                    }
                }
            } while (este_duplicat); // if true mai baga o fisa
            
            display[i] = numar_extras;
        }
    }

    void show_selected_itms() const {
        std::cout << "\n=== LA LICITATIE RUNDA ASTA ===\n";
        for (int i = 0; i < 4; i++) {
            std::cout << "[" << i + 1 << "] " << lista_licitatie[display[i]] << "\n"; 
        }
    }

    void show_bidding_item(int n) const {
        if (n >= 0 && n < 4) {
            std::cout << "\nSe liciteaza pentru: " << lista_licitatie[display[n]] << "\n";
        }
    }

    void copy_item(item& current_item, int i){ // constructor de copiere
        current_item = lista_licitatie[display[i]];
    }

    int get_id(int i) {
        return display[i];
    }
};

int main() {
    lista l1; 
    l1.citire_lista("obiecte.txt");

    std::string name;
    std::cout << "Enter your name: ";
    std::cin >> name;
    
    std::cout << "\nYour name has been registered.\n"; 
    player j1(name, 50000); // momentan jocul este facut cu un singur jucator si un singur bot dar
    bot gigel("Gigel");     // te poti juca in mai multi daca mai adaugi un while in game loop
    
    char cont = 'y', wnb;
    int cateIteme, ceItm;
    
    // Game loop
    while (cont == 'y') { 
        std::cout << "\n" << j1 << "\n"; // show status
        
        l1.item_select();
        l1.show_selected_itms();
        
        std::cout << "\nDo you want to buy something? (y/n): ";
        std::cin >> wnb;
        
        if (wnb == 'y') {
            std::cout << "How many items do you want to bid on? (1-4): ";
            std::cin >> cateIteme;
            
            for (int i = 0; i < cateIteme; i++) {
                std::cout << "Choose item no. " << i + 1 << " (1 to 4): ";
                std::cin >> ceItm;
                ceItm--;
    
                l1.show_bidding_item(ceItm);
                item clona;
                l1.copy_item(clona, ceItm);
                std::cout<<"Name your price: ";
                int player_bid_pricecall;
                std::cin >> player_bid_pricecall;
                while (player_bid_pricecall > j1.check_balance()) {
                    std::cout << "You can't bid that much money!!! Bid again: ";
                }
                char cont = 'y';
                gigel.call_mpr(clona.getPrice());
                while (cont == 'y' && gigel.new_bidding(j1, player_bid_pricecall, l1.get_id(ceItm), clona.getPrice()) == true) {
                    std::cout<<"Do you wnat to continue bidding? y/n: ";
                    std::cin >> cont;
                    if (cont == 'y') {
                        std::cout << "Your bid: ";
                        std::cin >> player_bid_pricecall;
                        while (player_bid_pricecall > j1.check_balance()) {
                            std::cout << "You can't bid that much money!!! Bid again: ";
                            std::cin >> player_bid_pricecall;
                        }
                        std::system("cls");
                        l1.show_bidding_item(ceItm);
                    }

                }
                //doar apelez functia de bidding si un while aici
                // Aici va veni: implementat bidding roboti
                // Aici va veni: implementat bidding player
                std::cout << "-> Licitatia s-a incheiat pentru acest item.\n";
            }
        }
        
        std::cout << "\nDo you want to continue to the next round? (y/n): ";
        std::cin >> cont;
    }

    std::cout << "\nJoc terminat! " << j1 << "\n";

    return 0;
}