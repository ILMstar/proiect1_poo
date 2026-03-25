/*
 * DESCRIEREA PROBLEMEI ESTE IN README.md
 * Tema 34: Licitatii (Joc de licitatie / Shop)
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <ctime>
#include <windows.h> // pt sleep
#include <cstdlib> // pt std::system

static std::mt19937& global_rng() {
    // Seed luat de la ceasul calculatorului pt randomness
    static std::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
    return gen;
}

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
    int* inv; // pt alocare dinamica
    
public:
    player(const std::string& n = "Unknown", int b = 0) {
        name = n;
        balance = b;
        inv = new int[99];
        for(int i=0; i<99; i++) inv[i] = 0;
    }
    
    player(const player& other) {
        name = other.name; balance = other.balance;
        inv = new int[99]; // alocam memorie pt copiere
        for(int i=0; i<99; i++) inv[i] = other.inv[i];
    }
    
    player& operator=(const player& other) {
        if (this != &other) {
            name = other.name; balance = other.balance;
            // memorie incarcata doar scriem peste
            for(int i=0; i<99; i++) inv[i] = other.inv[i];
        }
        return *this;
    }
    
    ~player() {
        delete[] inv;
    }

    void bought(int id, int pret) {
        inv[id]++;
        balance -= pret; 
    }

    int check_balance() const {
        return balance;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const player& p) { 
        os << "Jucator: " << p.name << " | Balanta: " << p.balance << " $";
        return os;
    }
};

class bot {
    private:
        int bot_bid;
        int max_price;
        std::string name;
        int gen_mul(){ // genereaza * cat poate sa pluseze pretul
            std::uniform_real_distribution<double> dis(1.0, 3.0);
            return std::max(1, static_cast<int>(dis(global_rng())));
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
        void call_mpr(int price_of_item) { 
            max_price = price_of_item * gen_mul();
        }
        int new_bidding(player& current_player, int player_price, int item_id, int item_pret){ // game engine
            if (player_price < max_price) {
                std::uniform_real_distribution<double> dis(0.5, 2.1);
                bot_bid = player_price + static_cast<int>(500 * dis(global_rng()));
                std::cout<<"The bot has bid "<< bot_bid <<"\n";
                return true; 
            }
            else {
                std::cout << "You got the item!\n";
                current_player.bought(item_id, player_price); 
                return false; 
            }
        }

        int get_bid() const {return bot_bid;};

        friend std::ostream& operator<<(std::ostream& os, const bot& b) {
            os << "Bot-ul " << b.name;
            return os;
        }
};

class lista {
private:
    int display[4];
    
public:
    std::vector<item> lista_licitatie;

    void citire_lista(const std::string& fisier) { 
        std::ifstream file(fisier);
        if(!file.is_open()) {
            std::cout << "Fisierul nu a fost gasit: " << fisier << "\n";
            return;
        }
        
        std::string line;
        while (getline(file, line)) { // interpretare fisier simbolul @ delimiteaza nume pret si cantitate
            int pret = 0, stoc = 0, cee = 0;
            std::string nume_produs = "";
            
            for (std::size_t i = 0; i < line.length(); i++) {
                if (line[i] == '@') { 
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

    void item_select() { // selecteaza 4 iteme pt licitatie
        if (lista_licitatie.size() < 4) return; 

        std::uniform_int_distribution<int> dis(0, lista_licitatie.size() - 1);
        
        for (int i = 0; i < 4; i++) { // selecteaza 4 id-uri de iteme
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

    void copy_item(item& current_item, int i){ 
        current_item = lista_licitatie[display[i]];
    }

    int get_id(int i) const {
        return display[i];
    }

    friend std::ostream& operator<<(std::ostream& os, const lista& l) { // afisare prin returnare de consola
        os << "Lista contine " << l.lista_licitatie.size() << " oferte valabile.";
        return os;
    }
};

void runTutorial(lista& l1) {
    std::cout << "\n============================================\n";
    std::cout << "          TUTORIAL AUTOMAT PORNIT           \n";
    std::cout << "============================================\n";
    
    player tut_player("Jucator_Tutorial", 5000);
    bot tut_bot("Bot_Tutorial");

    std::cout << "[TUTORIAL] Primesti un cont de start.\n";
    std::cout << tut_player << "\n";
    
    std::cout << "[TUTORIAL] Sistemul selecteaza 4 iteme aleatorii din magazin...\n";
    l1.item_select();
    l1.show_selected_itms();
    
    std::cout << "\n[TUTORIAL] Sa presupunem ca vrei sa licitezi pentru primul item [1].\n";
    Sleep(2000);
    
    int ales = 0; 
    l1.show_bidding_item(ales);
    
    item clona;
    l1.copy_item(clona, ales);
    int pret_pornire = clona.getPrice();
    
    std::cout << "[TUTORIAL] Pretul de pornire este " << pret_pornire << "$.\n";
    std::cout << "[TUTORIAL] Tu oferi o suma initiala putin mai mare.\n";
    
    int bid_curent = pret_pornire + 50;
    std::cout << "Your bid: " << bid_curent << " $\n";
    Sleep(2000);
    
    tut_bot.call_mpr(pret_pornire);
    
    bool bot_continua = tut_bot.new_bidding(tut_player, bid_curent, l1.get_id(ales), pret_pornire);
    
    if (bot_continua) {
        std::cout << "[TUTORIAL] Botul a licitat peste tine!\n";
        std::cout << "[TUTORIAL] In acest punct poti alege sa plusezi sau sa te retragi.\n";
        std::cout << "[TUTORIAL] Ne vom retrage din aceasta runda demonstrativa.\n";
    } else {
        std::cout << "[TUTORIAL] Ai fost norocos! Ai castigat licitatia din prima!\n";
        std::cout << "[TUTORIAL] Iata cum arata balanta ta acum:\n";
        std::cout << tut_player << "\n";
    }

    std::cout << "\n============================================\n";
    std::cout << "          TUTORIAL AUTOMAT INCHEIAT         \n";
    std::cout << "============================================\n\n";
}

int main() {
    lista l1;
    l1.citire_lista("obiecte.txt"); 

    runTutorial(l1);

    char vrea_sa_joace;
    std::cout << "Vrei sa incepi jocul real? (y/n): ";
    std::cin >> vrea_sa_joace;

    if (vrea_sa_joace == 'n' || vrea_sa_joace == 'N') {
        std::cout << "La revedere!\n";
        return 0;
    }

    std::string name;
    std::cout << "\nEnter your name: ";
    std::cin >> name;
    
    std::cout << "Your name has been registered.\n";
    player j1(name, 50000); 
    bot gigel("Gigel");     
    
    char cont = 'y', wnb;
    int cateIteme, ceItm;
    
    // Game loop
    while (cont == 'y' || cont == 'Y') {
        std::cout << "\n" << j1 << "\n"; 
        
        l1.item_select();
        l1.show_selected_itms();
        
        std::cout << "\nDo you want to buy something? (y/n): ";
        std::cin >> wnb;
        
        if (wnb == 'y') {
            std::cout << "How many items do you want to bid on? (1-4): ";
            std::cin >> cateIteme;
            
            for (int i = 0; i < cateIteme; i++) {
                std::cout << "\nChoose item no. " << i + 1 << " (1 to 4): ";
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
                    std::cin >> player_bid_pricecall;
                }
                
                char bidding_cont = 'y'; 
                gigel.call_mpr(clona.getPrice());
                
                while (bidding_cont == 'y' && gigel.new_bidding(j1, player_bid_pricecall, l1.get_id(ceItm), clona.getPrice()) == true) {
                    std::cout<<"Do you want to continue bidding? y/n: ";
                    std::cin >> bidding_cont;
                    
                    if (bidding_cont == 'y') {
                        std::cout << "Your bid: ";
                        std::cin >> player_bid_pricecall;
                        while (player_bid_pricecall > j1.check_balance() || player_bid_pricecall < gigel.get_bid()) {
                            std::cout << "You can't bid that much money!!! Bid again: ";
                            std::cin >> player_bid_pricecall;
                        }
                        std::system("cls");
                        l1.show_bidding_item(ceItm);
                    }
                }
                std::cout << "-> Licitatia s-a incheiat pentru acest item.\n";
            }
        }
        
        std::cout << "\nDo you want to continue to the next round? (y/n): ";
        std::cin >> cont;
        if (cont == 'y' || cont == 'Y') {
            std::system("cls");
        }
    }

    std::cout << "\nJoc terminat! " << j1 << "\n";

    return 0;
}