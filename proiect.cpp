#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <ctime>
using namespace std;

class player {
    private:
        string name;
        int balance;
        int inv[99] = {0};
    public:
        player (string n, int b) {
            name = n;
            balance = b;
        }
        void bought(int id, int pret){
            inv[id]++;
            balance -= pret;
        }
        int check_balance() {
            return balance;
        }
};

class bot {
    private:
        double lic_pr; // il folosim ca botul sa nu liciteze mai mult sau mai putin cat x% din pretul produsului
    
    public:
        void call_price() {

        }
};

class item {
    private:
        string name;
        int price;
        int stock;
    public:
        item (string n, int p, int s) {
            name = n;
            price = p;
            stock = s;
        }
        void stk_dcl(){
            stock--;
        }
        void show(){
            cout << "Item: " << name << " -> Price: " << price << " $" << endl;
        }
};

class lista {
    private:
        int display[4];
        int nrelem = -1;
    public:
        vector<item> lista_licitatie;
        void citire_lista(string fisier) {
            ifstream file(fisier);
            string line;
            int i = 0;
            while (getline(file, line)) {
                int id = 0, pret = 0, stoc = 0, cee = 0;
                string nume_produs = "";
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == '@') { 
                        cee++;
                        continue;
                    }
                    if (cee == 0) nume_produs += line[i];
                    if (cee == 1) pret = pret * 10 + (line[i] - '0');
                    if (cee == 2) stoc = stoc * 10 + (line[i] - '0');
                }
                lista_licitatie.push_back(item(nume_produs, pret, stoc));
                nrelem++;
            }
        }
        void item_select () { // selecteaza 4 elemente random pt licitatie
            mt19937 gen(time(nullptr)); // de adaugat verificarea daca inca e in stoc
            for (int i = 0; i < 4; i++) {
                uniform_int_distribution<int> dis(0, nrelem);
                if (i > 0 && display[i - 1] == dis(gen)) {
                    for(int j = 0; j < i; j++) {
                        if (display[j] == dis(gen))
                            uniform_int_distribution<int> dis(0, nrelem);
                    }
                }
                display[i] = dis(gen);
            }
        }
        void show_selected_itms () {
            for (int i = 0; i < 4; i++) {
                lista_licitatie[display[i]].show();
            }
        }
        void show_bidding_item (int n) {
            lista_licitatie[display[n]].show();
        }
};

class shop {
    private:
        double mul; // un multiplier care e facut cu random de la 0.5 pana la 2 care influenteza pretul itemelor displayed

    public:
        void gen_rnd(){
            // 1. Obtain a random seed from the hardware
            random_device rd;
            // 2. Initialize the Standard mersenne_twister_engine with the seed
            mt19937 gen(rd());
            uniform_real_distribution<double> dis(0.5, 2.1);
            // 4. Generate the random number
            mul = dis(gen);
        }
};

int main () {
    lista l1; // creaza lista din fisier
    l1.citire_lista("obiecte.txt");

    string name;
    cout << "Enter your name: ";
    cin >> name;
    system("cls");
    cout << "Your name has been registered" << endl; 
    player j1(name, 50000);
    
    char cont = 'y', wnb;
    int cateIteme, ceItm;
    while (j1.check_balance() > 0 && cont == 'y') { // cat timp balance > 0 si jucatorul alege y dintre y/n
        l1.item_select();
        l1.show_selected_itms();
        cout << "Do you want to buy something?" << endl << "y/n ";
        cin >> wnb;
        if (wnb == 'y') {
            system("cls");
            l1.item_select();
            l1.show_selected_itms();
            cout << "How many items do you want to buy? -> ...";
            cin >> cateIteme;
            for (int i = 0; i < cateIteme; i++) {
                cout << "Choose item no. " << i + 1 << " (ex: 4) -> ";
                cin >> ceItm;
                ceItm --;
                system("cls");
                l1.show_bidding_item(ceItm);
                // implementat bidding roboti
                // implementat bidding player
            }
        }
        cout << "Do you want to continue? y/n ";
        cin >> cont;
    }

    // selecteaza ce poti sa cumperi in runda curenta si le arata
    // l1.item_select();
    // l1.show_selected_itms();

    // l1.lista_licitatie[display[0]].show(); // verificam si eu daca merge

}