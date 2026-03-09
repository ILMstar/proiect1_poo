#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
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
            cout << "Name: " << name << " -> Price: " << price << endl;
        }
};

class lista {
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
                    if (line[i] == '-') { 
                        cee++;
                        continue;
                    }
                    if (cee == 0) nume_produs += line[i];
                    if (cee == 1) pret = pret * 10 + (line[i] - '0');
                    if (cee == 2) stoc = stoc * 10 + (line[i] - '0');
                }
                lista_licitatie.push_back(item(nume_produs, pret, stoc));
            }
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
    lista l1; // creaza lista din input
    l1.citire_lista("input.txt");

    string name;
    cout << "Enter your name: ";
    cin >> name;
    system("cls");
    cout << "Your name has been registered" << endl; 
    player j1(name, 50000);
    
    // selecteaza ce poti sa cumperi in runda curenta
    int display[3];
    for (int i = 0; i < 4; i++) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, 100);
        display[i] = dis(gen);
    }

    // l1.lista_licitatie[display[0]].show(); // verificam si eu daca merge

}