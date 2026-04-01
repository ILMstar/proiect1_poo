/*
 * DESCRIEREA TEMEI ESTE IN README.md
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
        static int total_items_created;   
    public:
        item(const std::string& n = "", int p = 0, int s = 0) {
            name = n; price = p; stock = s;
            total_items_created++; // incrementam contorul
        }
        
        item(const item& other) {
            name = other.name; price = other.price; stock = other.stock;
            total_items_created++; // incrementam contorul la copiere
        }
        
        item& operator=(const item& other) {
            if (this != &other) {
                name = other.name; price = other.price; stock = other.stock;
            }
            return *this;
        }
        
        ~item() {}

        inline void stk_dcl() { stock--; }
        
        static int getTotalItems() {
            return total_items_created;
        }

        inline std::string getName() const { return name; }
        inline int getPrice() const { return price; }
        inline int getStock() const { return stock; }

        friend std::ostream& operator<<(std::ostream& os, const item& i) {
            os << "Item: " << i.name << " -> Price: " << i.price << " $ (Stock: " << i.stock << ")";
            return os;
        }
};

// Initializare atribut static
int item::total_items_created = 0;

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

        void sold(int id, int pret) { 
            if (inv[id] > 0) {        // mai vedem daca mai trb ceva adaugat aici -------------------------------------------------------
                inv[id]--;
                balance += pret;
            }
            else
                std::cout << "You don't have this item\n";
        }

        inline int check_balance() const {
            return balance;
        }
        
        friend std::ostream& operator<<(std::ostream& os, const player& p) { 
            os << "Player: " << p.name << " | Balance: " << p.balance << " $";
            return os;
        }
};

class bot {
    private:
        int bot_bid;
        double max_price;
        std::string name;
        double gen_mul(){ // genereaza inmultitorul care arata ori cat poate sa pluseze pretul
            std::uniform_real_distribution<double> dis(0.5, 3.0);
            return dis(global_rng());
        }
    public:
        bot(const std::string& n = "SimpleBOT"){
            name = n;
        }
        bot(const bot& other) {
            name = other.name;
            bot_bid = other.bot_bid;
            max_price = other.max_price;
        }
        bot& operator=(const bot& other) {
            if (this != &other) {
                name = other.name;
                bot_bid = other.bot_bid;
                max_price = other.max_price;
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

        inline int get_bid() const {return bot_bid;}; 

        friend std::ostream& operator<<(std::ostream& os, const bot& b) {
            os << "Bot " << b.name;
            return os;
        }
};

class lista {
    private:
        int display[4];
        int shopItms[10]; 
    public:
        std::vector<item> lista_licitatie;

        lista() {
            for (int i = 0; i < 4; i++)
                display[i] = 0;
        }

        lista(const lista& other) {
            for (int i = 0; i < 4; i++)
                display[i] = other.display[i];
            lista_licitatie = other.lista_licitatie;
        }

        lista& operator=(const lista& other) {
            if (this != &other) {
                for (int i = 0; i < 4; i++)
                    display[i] = other.display[i];
                lista_licitatie = other.lista_licitatie;
            }
            return *this;
        }

        void citire_lista(const std::string& fisier) { 
            std::ifstream file(fisier);
            if(!file.is_open()) {
                std::cout << "File not found: " << fisier << "\n";
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

        void item_select(int nr) { // selecteaza 4 iteme pt licitatie
            if (lista_licitatie.size() < nr) return; 

            std::uniform_int_distribution<int> dis(0, lista_licitatie.size() - 1);
            
            for (int i = 0; i < nr; i++) { // selecteaza 4 id-uri de iteme
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

        void show_selected_itms(int more) const {
            std::cout << "\n=== UP FOR AUCTION THIS ROUND ===\n";
            for (int i = 0; i < more; i++) {
                std::cout << "[" << i + 1 << "] " << lista_licitatie[display[i]] << "\n";
            }
        }

        void show_bidding_item(int n) const {
            if (n >= 0 && n < 4) {
                std::cout << "\nBidding for: " << lista_licitatie[display[n]] << "\n";
            }
        }

        void copy_item(item& current_item, int i){ 
            current_item = lista_licitatie[display[i]];
        }

        inline int get_id(int i) const {
            return display[i];
        }

        friend std::ostream& operator<<(std::ostream& os, const lista& l) { // afisare prin returnare de consola
            os << "The list contains " << l.lista_licitatie.size() << " valid offers.";
            return os;
        }

        inline int get_display(int i) const {
            return display[i];
        }
};

void place_shop(lista& lista_curenta) {
    std::system("cls");
    char v[4] = {'-', '\\', '|', '/'};
    for (int j = 0; j < 8; j++)
        for (int i = 0; i < 4; i++) {
            std::cout << "The trader has arrived and now is setting up the shop! Loading " << v[i];
            Sleep(50);
            std::system("cls");
        }
    lista_curenta.item_select(10); // selecteaza 10 iteme
    lista_curenta.show_selected_itms(10);
    int v[4]; // iau id-ul itemelor
    for (int i = 0; i < 4; i++) {
        v[i] = lista_curenta.get_display(i);
    }
    // si logica de cumparat
}

void runHardcodedDemo() {
    std::system("cls");
    std::cout << "\n============================================\n";
    std::cout << "           AUTO-PLAY DEMO STARTED           \n";
    std::cout << "============================================\n\n";

    // Creating a specific player and bot for this demo
    player demoPlayer("Alex_Gamer", 25000);
    bot demoBot("SniperBot");

    std::cout << "[DEMO] Player is connecting...\n";
    std::cout << demoPlayer << "\n";
    Sleep(2000);

    // Hardcoding a few items instead of using the "lista" class
    item itm1("Diamond Sword", 5000, 1);
    item itm2("Ancient Shield", 3000, 1);
    item itm3("Health Potion", 500, 3);

    std::cout << "\n[DEMO] The trader brought the following special offers today:\n";
    std::cout << "[1] " << itm1 << "\n";
    std::cout << "[2] " << itm2 << "\n";
    std::cout << "[3] " << itm3 << "\n";
    Sleep(3500);

    std::cout << "\n[DEMO] Alex_Gamer is looking over the offers...\n";
    Sleep(2000);
    
    std::cout << "[DEMO] Alex_Gamer decided to bid on: " << itm1.getName() << "!\n";
    std::cout << "[DEMO] The starting price is: " << itm1.getPrice() << " $\n";
    Sleep(2000);

    // Setting the bot's limits
    demoBot.call_mpr(itm1.getPrice());

    // The player makes the first call
    int current_bid = itm1.getPrice() + 500;
    std::cout << "Alex_Gamer bids: " << current_bid << " $\n";
    Sleep(2000);

    // Using an arbitrary ID for the item (e.g., 0) since we're not taking it from the list
    bool bot_continua = demoBot.new_bidding(demoPlayer, current_bid, 0, itm1.getPrice());

    if (bot_continua) {
        Sleep(2000);
        std::cout << "\n[DEMO] Alex_Gamer gets annoyed and raises the bid aggressively!\n";
        
        int aggressive_bid = demoBot.get_bid() + 1500;
        std::cout << "Alex_Gamer bids: " << aggressive_bid << " $\n";
        Sleep(2000);

        // Second check round with the bot
        bot_continua = demoBot.new_bidding(demoPlayer, aggressive_bid, 0, itm1.getPrice());
        
        if (bot_continua) {
            Sleep(2000);
            std::cout << "\n[DEMO] The price got too high. Alex_Gamer withdraws from the auction.\n";
        }
    } 

    Sleep(2000);
    std::cout << "\n[DEMO] The player's final balance after this session:\n";
    std::cout << demoPlayer << "\n";

    std::cout << "\n============================================\n";
    std::cout << "            AUTO-PLAY DEMO ENDED            \n";
    std::cout << "============================================\n\n";
    char cnt;
    std::cout << "Press any key to continue... ";
    std::cin >> cnt;
}

void runTutorial(lista& l1) {
    std::cout << "\n============================================\n";
    std::cout << "          AUTOMATIC TUTORIAL STARTED        \n";
    std::cout << "============================================\n";
    
    player tut_player("Jucator_Tutorial", 500000);
    bot tut_bot("Bot_Tutorial");

    std::cout << "[TUTORIAL] You receive a starter account.\n";
    std::cout << tut_player << "\n";
    
    std::cout << "[TUTORIAL] The system selects 4 random items from the store...\n";
    l1.item_select(4);
    l1.show_selected_itms(4);
    
    std::cout << "\n[TUTORIAL] Let's assume you want to bid on the first item [1].\n";
    Sleep(5000);
    
    int ales = 0; 
    l1.show_bidding_item(ales);
    
    item clona;
    l1.copy_item(clona, ales);
    int pret_pornire = clona.getPrice();
    
    std::cout << "[TUTORIAL] The starting price is " << pret_pornire << "$.\n";
    std::cout << "[TUTORIAL] You offer a slightly higher initial amount.\n";
    
    int bid_curent = pret_pornire + 50;
    std::cout << "Your bid: " << bid_curent << " $\n";
    Sleep(5000);
    
    tut_bot.call_mpr(pret_pornire);
    
    bool bot_continua = tut_bot.new_bidding(tut_player, bid_curent, l1.get_id(ales), pret_pornire);
    
    if (bot_continua) {
        std::cout << "[TUTORIAL] The bot outbid you!\n";
        std::cout << "[TUTORIAL] At this point you can choose to bid higher or withdraw.\n";
        std::cout << "[TUTORIAL] We will withdraw from this demonstration round.\n";
    } else {
        std::cout << "[TUTORIAL] You were lucky! You won the auction on your first try!\n";
        std::cout << "[TUTORIAL] Here is what your balance looks like now:\n";
        std::cout << tut_player << "\n";
    }

    std::cout << "\n============================================\n";
    std::cout << "          AUTOMATIC TUTORIAL ENDED          \n";
    std::cout << "============================================\n\n";
}

int main() {
    lista l1;

    l1.citire_lista("obiecte.txt"); 
    runHardcodedDemo();
    std::system("cls");

    char vrea_tutorial;
    std::cout << "Do you want a tutorial (now the object list is included) y/n: ";
    std::cin >> vrea_tutorial; 
    if (vrea_tutorial == 'y')
        runTutorial(l1);
    
    char vrea_sa_joace;
    std::cout << "Do you want to start the real game? (y/n): ";
    std::cin >> vrea_sa_joace;

    if (vrea_sa_joace == 'n' || vrea_sa_joace == 'N') {
        std::cout << "Goodbye!\n";
        return 0;
    }

    std::system("cls");
    std::string name;
    std::cout << "\nEnter your name: ";
    std::cin >> name;
    
    std::cout << "Your name has been registered.\n";
    player j1(name, 50000); 
    bot gigel("Gigel");     
    
    char cont = 'y', wnb;
    int cateIteme, ceItm;
    
    // Bucla jocului
    while (cont == 'y' || cont == 'Y') {
        std::cout << "\n" << j1 << "\n"; 
        
        l1.item_select(4);
        l1.show_selected_itms(4);
        
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
                std::cout << "-> The auction has ended for this item.\n";
            }
        }
        
        std::cout << "\nDo you want to continue to the next round? (y/n): ";
        std::cin >> cont;
        if (cont == 'y' || cont == 'Y') {
            std::system("cls");
        }
    }

    std::cout << "\nGame Over! " << j1 << "\n";
    // Afisam utilizarea metodei statice la final pentru a demonstra ca functioneaza
    std::cout << "Statistic: Total items loaded/cloned during the game: " << item::getTotalItems() << "\n";

    return 0;
}