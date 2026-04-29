/*
 * DESCRIEREA TEMEI ESTE IN README.md
 * Tema 34: Licitatii (Joc de licitatie / Shop)
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <windows.h>

#include "auction_exceptions.h"
#include "auction_house.h"
#include "bot.h"
#include "item.h"
#include "lista.h"
#include "player.h"

void place_shop(lista& lista_curenta, player& player_curent) {
    std::system("cls");
    char vect[4] = {'-', '\\', '|', '/'};
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 4; i++) {
            std::cout << "The trader has arrived and now is setting up the shop! Loading " << vect[i];
            Sleep(50);
            std::system("cls");
        }
    }
    lista_curenta.item_select(10);
    lista_curenta.show_selected_itms(10);
    for (int i = 0; i < 10; i++) {
        (void)lista_curenta.get_display(i);
    }
    char vrea_sa_vanda = 'n';
    std::cout << "Do you want to sell anything? y/n: ";
    std::cin >> vrea_sa_vanda;
    while (vrea_sa_vanda == 'y') {
        std::cout << "Enter the ID of the item you want to sell: ";
        int id_itemsold;
        std::cin >> id_itemsold;
        player_curent.sold(id_itemsold, lista_curenta.lista_licitatie[id_itemsold].getPrice());
        std::cout << "Do you want to sell anything else? y/n: ";
        std::cin >> vrea_sa_vanda;
    }
}

//arata rapid cum arata o licitatie fara input de la utilizator
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

    // Daca botul continua simulam o contra-oferta mai agresiva.
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
            // In demo aratam si scenariul in care jucatorul renunta.
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

// Tutorial automat: explica pasii unei runde reale folosind lista din fisier.
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
    
    // Clonam itemul ales pentru a lucra usor cu pretul lui de pornire.
    item clona;
    l1.copy_item(clona, ales);
    int pret_pornire = clona.getPrice();
    
    std::cout << "[TUTORIAL] The starting price is " << pret_pornire << "$.\n";
    std::cout << "[TUTORIAL] You offer a slightly higher initial amount.\n";
    
    int bid_curent = pret_pornire + 50;
    std::cout << "Your bid: " << bid_curent << " $\n";
    Sleep(5000);
    
    // Setam limita botului pornind de la pretul de baza al itemului.
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

//demo -> tutorial optional -> joc real in bucla.
int main() {
    try {
        lista l1;

        l1.citire_lista("obiecte.txt");
        runHardcodedDemo();
        std::system("cls");

        char vrea_tutorial;
        std::cout << "Do you want a tutorial (now the object list is included) y/n: ";
        std::cin >> vrea_tutorial;
        if (vrea_tutorial == 'y') {
            runTutorial(l1);
        }

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

        AuctionHouse house(gigel);
        std::cout << "Opponent registered: ";
        house.describeOpponent(std::cout);
        std::cout << "\n";
        if (house.opponentIsBot()) {
            std::cout << "Dynamic cast check: opponent is a bot.\n";
        }

        char cont = 'y', wnb;
        int cateIteme, ceItm;

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

                    std::cout << "Name your price: ";
                    int player_bid_pricecall;
                    std::cin >> player_bid_pricecall;

                    while (player_bid_pricecall > j1.check_balance()) {
                        std::cout << "You can't bid that much money!!! Bid again: ";
                        std::cin >> player_bid_pricecall;
                    }

                    char bidding_cont = 'y';
                    gigel.call_mpr(clona.getPrice());

                    while (bidding_cont == 'y' &&
                           gigel.new_bidding(j1, player_bid_pricecall, l1.get_id(ceItm), clona.getPrice()) == true) {
                        std::cout << "Do you want to continue bidding? y/n: ";
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
        std::cout << "Statistic: Total items loaded/cloned during the game: " << item::getTotalItems() << "\n";

        return 0;
    } catch (const AuctionError& err) {
        std::cout << "Auction error: " << err.what() << "\n";
    } catch (const std::exception& err) {
        std::cout << "Unexpected error: " << err.what() << "\n";
    }
    return 1;
}