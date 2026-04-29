/*
 * Auction House - SFML UI version
 * Minimal SFML front-end for the existing auction game logic.
 */

#include <SFML/Graphics.hpp>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <ctime>
#include <cctype>
#include <optional>

static std::mt19937& global_rng() {
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
            total_items_created++;
        }

        item(const item& other) {
            name = other.name; price = other.price; stock = other.stock;
            total_items_created++;
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
};

int item::total_items_created = 0;

class player {
    private:
        std::string name;
        int balance;
        int* inv;

    public:
        player(const std::string& n = "Unknown", int b = 0) {
            name = n;
            balance = b;
            inv = new int[99];
            for (int i = 0; i < 99; i++) inv[i] = 0;
        }

        player(const player& other) {
            name = other.name; balance = other.balance;
            inv = new int[99];
            for (int i = 0; i < 99; i++) inv[i] = other.inv[i];
        }

        player& operator=(const player& other) {
            if (this != &other) {
                name = other.name; balance = other.balance;
                for (int i = 0; i < 99; i++) inv[i] = other.inv[i];
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
            if (id >= 0 && id < 99 && inv[id] > 0) {
                inv[id]--;
                balance += pret;
            }
        }

        inline int get_inv(int id) const { return (id >= 0 && id < 99) ? inv[id] : 0; }

        inline int check_balance() const { return balance; }
        inline std::string getName() const { return name; }
};

class bot {
    private:
        int bot_bid = 0;
        double max_price = 0.0;
        std::string name;
        double gen_mul() {
            std::uniform_real_distribution<double> dis(0.5, 3.0);
            return dis(global_rng());
        }
    public:
        bot(const std::string& n = "SimpleBOT") { name = n; }
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
        bool new_bidding(player& current_player, int player_price, int item_id) {
            if (player_price < max_price) {
                std::uniform_real_distribution<double> dis(0.5, 2.1);
                bot_bid = player_price + static_cast<int>(500 * dis(global_rng()));
                return true;
            }
            current_player.bought(item_id, player_price);
            return false;
        }
        inline int get_bid() const { return bot_bid; }
        inline std::string getName() const { return name; }
};

class lista {
    private:
        int display[4];
    public:
        std::vector<item> lista_licitatie;

        lista() {
            for (int i = 0; i < 4; i++)
                display[i] = 0;
        }

        void citire_lista(const std::string& fisier) {
            std::ifstream file(fisier);
            if (!file.is_open()) {
                return;
            }

            std::string line;
            while (getline(file, line)) {
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

        void item_select(int nr) {
            if (lista_licitatie.size() < static_cast<std::size_t>(nr)) return;

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

        void copy_item(item& current_item, int i) {
            current_item = lista_licitatie[display[i]];
        }

        inline int get_id(int i) const { return display[i]; }
        inline int get_display(int i) const { return display[i]; }
};

static std::string sanitize_ascii(const std::string& in) {
    std::string out;
    out.reserve(in.size());
    for (unsigned char c : in) {
        if (c >= 32 && c <= 126) {
            out.push_back(static_cast<char>(c));
        } else {
            out.push_back('?');
        }
    }
    return out;
}

struct Button {
    sf::RectangleShape box;
    sf::Text label;
    bool hovered = false;

    Button(const sf::Font& font) : label(font, "") {}

    void setPosition(float x, float y) {
        box.setPosition(sf::Vector2f(x, y));
        sf::FloatRect bounds = label.getLocalBounds();
        label.setPosition(sf::Vector2f(
            x + (box.getSize().x - bounds.size.x) / 2.0f - bounds.position.x,
            y + (box.getSize().y - bounds.size.y) / 2.0f - bounds.position.y));
    }

    bool contains(sf::Vector2f p) const {
        return box.getGlobalBounds().contains(p);
    }

    void setHover(bool isHover) {
        hovered = isHover;
        box.setFillColor(hovered ? sf::Color(240, 200, 80) : sf::Color(220, 180, 60));
    }
};

enum class Screen {
    MainMenu,
    Tutorial,
    NameEntry,
    Round,
    Inventory,
    GameOver
};

struct GameUIState {
    Screen screen = Screen::MainMenu;
    lista offers;
    player currentPlayer = player("Player", 50000);
    bot currentBot = bot("Gigel");
    bool loadedItems = false;

    std::string nameInput;
    std::string infoMessage;

    bool inBidding = false;
    int biddingIndex = -1;
    item biddingItem;
    int botBid = 0;
    std::string bidInput;
    std::string bidMessage;
    double marketMultipliers[99];

    GameUIState() {
        for(int i = 0; i < 99; i++) marketMultipliers[i] = 1.0;
    }
};

static void setup_button(Button& btn, const std::string& text, sf::Vector2f size) {
    btn.box.setSize(size);
    btn.box.setFillColor(sf::Color(220, 180, 60));
    btn.box.setOutlineColor(sf::Color(90, 70, 20));
    btn.box.setOutlineThickness(2.0f);
    btn.label.setString(text);
    btn.label.setCharacterSize(20);
    btn.label.setFillColor(sf::Color(30, 20, 5));
}

static bool parse_int(const std::string& text, int& out) {
    if (text.empty()) return false;
    int value = 0;
    for (char c : text) {
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        int digit = c - '0';
        if (value > 100000000) return false;
        value = value * 10 + digit;
    }
    out = value;
    return true;
}

static void start_new_round(GameUIState& state) {
    state.offers.item_select(4);
    state.inBidding = false;
    state.biddingIndex = -1;
    state.botBid = 0;
    state.bidInput.clear();
    state.bidMessage.clear();
    
    std::uniform_real_distribution<double> dis(0.8, 2.5);
    for(int i = 0; i < 99; i++) {
        state.marketMultipliers[i] = dis(global_rng());
    }
}

static void begin_bidding(GameUIState& state, int index) {
    state.biddingIndex = index;
    state.offers.copy_item(state.biddingItem, index);
    state.currentBot.call_mpr(state.biddingItem.getPrice());
    state.botBid = 0;
    state.bidInput.clear();
    state.bidMessage = "Enter your bid.";
    state.inBidding = true;
}

static std::vector<std::string> wrap_text(const std::string& text, std::size_t maxLen) {
    std::vector<std::string> lines;
    std::string current;
    std::size_t pos = 0;
    while (pos < text.size()) {
        if (current.size() >= maxLen) {
            lines.push_back(current);
            current.clear();
        }
        current.push_back(text[pos]);
        pos++;
    }
    if (!current.empty()) lines.push_back(current);
    return lines;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1280, 720)), "Auction House - SFML");
    window.setFramerateLimit(60);

    sf::Font font;
    bool fontLoaded = font.openFromFile("C:/Windows/Fonts/consola.ttf");
    if (!fontLoaded) {
        fontLoaded = font.openFromFile("C:/Windows/Fonts/arial.ttf");
    }

    sf::Clock cursorClock;
    int inventoryScrollOffset = 0;

    GameUIState state;
    state.offers.citire_lista("obiecte.txt");
    state.loadedItems = !state.offers.lista_licitatie.empty();
    state.infoMessage = state.loadedItems ? "" : "Could not load obiecte.txt";

    sf::Text title(font, "Auction House", 32);
    title.setFillColor(sf::Color(245, 230, 200));
    title.setPosition(sf::Vector2f(40.0f, 20.0f));

    Button btnStart(font), btnTutorial(font), btnQuit(font), btnBack(font), btnContinue(font);
    Button btnNextRound(font), btnSubmitBid(font), btnWithdraw(font), btnInventory(font);
    setup_button(btnStart, "Start Game", {220.0f, 48.0f});
    setup_button(btnTutorial, "Tutorial", {220.0f, 48.0f});
    setup_button(btnQuit, "Quit", {220.0f, 48.0f});
    setup_button(btnBack, "Back", {160.0f, 42.0f});
    setup_button(btnContinue, "Continue", {200.0f, 42.0f});
    setup_button(btnNextRound, "Next Round", {200.0f, 42.0f});
    setup_button(btnSubmitBid, "Submit", {160.0f, 42.0f});
    setup_button(btnWithdraw, "Withdraw", {160.0f, 42.0f});
    setup_button(btnInventory, "Inventory", {160.0f, 42.0f});

    btnStart.setPosition(80.0f, 140.0f);
    btnTutorial.setPosition(80.0f, 200.0f);
    btnQuit.setPosition(80.0f, 260.0f);

    btnBack.setPosition(80.0f, 620.0f);
    btnContinue.setPosition(280.0f, 620.0f);

    btnNextRound.setPosition(80.0f, 620.0f);
    btnInventory.setPosition(520.0f, 620.0f);

    btnSubmitBid.setPosition(400.0f, 420.0f);
    btnWithdraw.setPosition(580.0f, 420.0f);

    sf::RectangleShape inputBox;
    inputBox.setSize(sf::Vector2f(360.0f, 40.0f));
    inputBox.setFillColor(sf::Color(30, 20, 5));
    inputBox.setOutlineColor(sf::Color(200, 180, 140));
    inputBox.setOutlineThickness(2.0f);

    sf::Text inputText(font, "", 22);
    inputText.setFillColor(sf::Color(240, 220, 180));

    sf::Text infoText(font, "", 18);
    infoText.setFillColor(sf::Color(240, 220, 180));

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* scrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (state.screen == Screen::Inventory) {
                    inventoryScrollOffset -= static_cast<int>(scrolled->delta * 30.0f);
                    if (inventoryScrollOffset < 0) inventoryScrollOffset = 0;
                }
            }

            if (const auto* text = event->getIf<sf::Event::TextEntered>()) {
                if (state.screen == Screen::NameEntry) {
                    if (text->unicode == 8) {
                        if (!state.nameInput.empty()) state.nameInput.pop_back();
                    } else if (text->unicode >= 32 && text->unicode <= 126) {
                        if (state.nameInput.size() < 20) {
                            state.nameInput.push_back(static_cast<char>(text->unicode));
                        }
                    }
                } else if (state.screen == Screen::Round && state.inBidding) {
                    if (text->unicode == 8) {
                        if (!state.bidInput.empty()) state.bidInput.pop_back();
                    } else if (text->unicode >= 48 && text->unicode <= 57) {
                        if (state.bidInput.size() < 10) {
                            state.bidInput.push_back(static_cast<char>(text->unicode));
                        }
                    }
                }
            }

            if (const auto* moved = event->getIf<sf::Event::MouseMoved>()) {
                sf::Vector2f mouse(static_cast<float>(moved->position.x), static_cast<float>(moved->position.y));
                btnStart.setHover(btnStart.contains(mouse));
                btnTutorial.setHover(btnTutorial.contains(mouse));
                btnQuit.setHover(btnQuit.contains(mouse));
                btnBack.setHover(btnBack.contains(mouse));
                btnContinue.setHover(btnContinue.contains(mouse));
                btnNextRound.setHover(btnNextRound.contains(mouse));
                btnSubmitBid.setHover(btnSubmitBid.contains(mouse));
                btnWithdraw.setHover(btnWithdraw.contains(mouse));
                btnInventory.setHover(btnInventory.contains(mouse));
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button != sf::Mouse::Button::Left) {
                    continue;
                }
                sf::Vector2f mouse(static_cast<float>(mousePressed->position.x), static_cast<float>(mousePressed->position.y));
                if (state.screen == Screen::MainMenu) {
                    if (btnStart.contains(mouse)) {
                        state.screen = Screen::NameEntry;
                        state.infoMessage.clear();
                    } else if (btnTutorial.contains(mouse)) {
                        state.screen = Screen::Tutorial;
                    } else if (btnQuit.contains(mouse)) {
                        window.close();
                    }
                } else if (state.screen == Screen::Tutorial) {
                    if (btnBack.contains(mouse)) {
                        state.screen = Screen::MainMenu;
                    }
                } else if (state.screen == Screen::NameEntry) {
                    if (btnBack.contains(mouse)) {
                        state.screen = Screen::MainMenu;
                    } else if (btnContinue.contains(mouse)) {
                        if (state.nameInput.empty()) {
                            state.infoMessage = "Please enter a name.";
                        } else if (!state.loadedItems) {
                            state.infoMessage = "Items not loaded. Check obiecte.txt";
                        } else {
                            state.currentPlayer = player(state.nameInput, 50000);
                            state.currentBot = bot("Gigel");
                            start_new_round(state);
                            state.screen = Screen::Round;
                        }
                    }
                } else if (state.screen == Screen::Round) {
                    if (btnNextRound.contains(mouse)) {
                        start_new_round(state);
                    }
                    if (btnQuit.contains(mouse)) {
                        state.screen = Screen::GameOver;
                    }
                    if (state.inBidding) {
                        if (btnSubmitBid.contains(mouse)) {
                            int bidValue = 0;
                            if (!parse_int(state.bidInput, bidValue)) {
                                state.bidMessage = "Enter a valid number.";
                            } else if (bidValue > state.currentPlayer.check_balance()) {
                                state.bidMessage = "Not enough balance.";
                            } else if (state.botBid > 0 && bidValue < state.botBid) {
                                state.bidMessage = "Bid must be >= bot bid.";
                            } else {
                                bool botContinues = state.currentBot.new_bidding(state.currentPlayer, bidValue, state.offers.get_id(state.biddingIndex));
                                if (botContinues) {
                                    state.botBid = state.currentBot.get_bid();
                                    state.bidMessage = "Bot bid: " + std::to_string(state.botBid);
                                } else {
                                    state.bidMessage = "You won the item for " + std::to_string(bidValue) + ".";
                                    state.inBidding = false;
                                    state.bidInput.clear();
                                }
                            }
                        } else if (btnWithdraw.contains(mouse)) {
                            state.bidMessage = "You withdrew from the auction.";
                            state.inBidding = false;
                            state.bidInput.clear();
                        }
                    } else {
                        for (int i = 0; i < 4; i++) {
                            sf::FloatRect bidRect(sf::Vector2f(860.0f, 160.0f + i * 120.0f), sf::Vector2f(100.0f, 36.0f));
                            if (bidRect.contains(mouse)) {
                                begin_bidding(state, i);
                            }
                        }
                        if (btnInventory.contains(mouse)) {
                            state.screen = Screen::Inventory;
                            inventoryScrollOffset = 0;
                        }
                    }
                } else if (state.screen == Screen::Inventory) {
                    if (btnBack.contains(mouse)) {
                        state.screen = Screen::Round;
                    } else {
                        int drawn_idx = 0;
                        for (std::size_t i = 0; i < state.offers.lista_licitatie.size(); i++) {
                            if (state.currentPlayer.get_inv(i) > 0) {
                                float yPos = 140.0f + drawn_idx * 120.0f - inventoryScrollOffset;
                                if (yPos > 50.0f && yPos < 650.0f) {
                                    sf::FloatRect sellRect(sf::Vector2f(860.0f, yPos + 20.0f), sf::Vector2f(100.0f, 36.0f));
                                    if (sellRect.contains(mouse)) {
                                        int sellPrice = static_cast<int>(state.offers.lista_licitatie[i].getPrice() * state.marketMultipliers[i]);
                                        state.currentPlayer.sold(i, sellPrice);
                                    }
                                }
                                drawn_idx++;
                            }
                        }
                    }
                } else if (state.screen == Screen::GameOver) {
                    if (btnBack.contains(mouse)) {
                        state.screen = Screen::MainMenu;
                    } else if (btnQuit.contains(mouse)) {
                        window.close();
                    }
                }
            }
        }

        btnBack.setPosition(80.0f, 620.0f);
        if (state.screen == Screen::Inventory) {
            btnBack.setPosition(80.0f, 660.0f);
        }

        window.clear(sf::Color(25, 18, 10));

        sf::RectangleShape background;
        background.setSize({1280.0f, 720.0f});
        background.setFillColor(sf::Color(30, 22, 12));
        window.draw(background);

        if (fontLoaded) {
            window.draw(title);
        }

        if (state.screen == Screen::MainMenu) {
            if (fontLoaded) {
                window.draw(btnStart.box);
                window.draw(btnStart.label);
                window.draw(btnTutorial.box);
                window.draw(btnTutorial.label);
                window.draw(btnQuit.box);
                window.draw(btnQuit.label);
            }
        } else if (state.screen == Screen::Tutorial) {
            if (fontLoaded) {
                infoText.setCharacterSize(18);
                infoText.setPosition(sf::Vector2f(80.0f, 120.0f));
                infoText.setString("Tutorial:\n- Select an item each round.\n- Enter a bid. The bot may outbid you.\n- If you bid above its limit, you win the item.\n- Use Next Round to refresh offers.");
                window.draw(infoText);
                window.draw(btnBack.box);
                window.draw(btnBack.label);
            }
        } else if (state.screen == Screen::NameEntry) {
            if (fontLoaded) {
                infoText.setCharacterSize(20);
                infoText.setPosition(sf::Vector2f(80.0f, 140.0f));
                infoText.setString("Enter your player name:");
                window.draw(infoText);

                inputBox.setPosition(sf::Vector2f(80.0f, 180.0f));
                window.draw(inputBox);

                bool showCursor = (cursorClock.getElapsedTime().asMilliseconds() / 500) % 2 == 0;
                inputText.setString(state.nameInput + (showCursor ? "_" : ""));
                inputText.setPosition(sf::Vector2f(90.0f, 186.0f));
                window.draw(inputText);

                if (!state.infoMessage.empty()) {
                    infoText.setCharacterSize(16);
                    infoText.setPosition(sf::Vector2f(80.0f, 240.0f));
                    infoText.setString(state.infoMessage);
                    window.draw(infoText);
                }

                window.draw(btnBack.box);
                window.draw(btnBack.label);
                window.draw(btnContinue.box);
                window.draw(btnContinue.label);
            }
        } else if (state.screen == Screen::Round) {
            if (fontLoaded) {
                infoText.setCharacterSize(18);
                infoText.setPosition(sf::Vector2f(80.0f, 80.0f));
                infoText.setString("Player: " + state.currentPlayer.getName() + " | Balance: " + std::to_string(state.currentPlayer.check_balance()) + " $");
                window.draw(infoText);

                for (int i = 0; i < 4; i++) {
                    int idx = state.offers.get_display(i);
                    if (idx < 0 || idx >= static_cast<int>(state.offers.lista_licitatie.size())) continue;
                    item displayItem = state.offers.lista_licitatie[idx];
                    std::string safeName = sanitize_ascii(displayItem.getName());
                    auto lines = wrap_text(safeName, 40);

                    sf::RectangleShape card;
                    card.setSize(sf::Vector2f(760.0f, 100.0f));
                    card.setPosition(sf::Vector2f(80.0f, 140.0f + i * 120.0f));
                    card.setFillColor(sf::Color(50, 36, 18));
                    card.setOutlineColor(sf::Color(100, 80, 30));
                    card.setOutlineThickness(2.0f);
                    window.draw(card);

                    float textY = 150.0f + i * 120.0f;
                    for (std::size_t li = 0; li < lines.size() && li < 2; li++) {
                        infoText.setCharacterSize(16);
                        infoText.setPosition(sf::Vector2f(95.0f, textY + li * 18.0f));
                        infoText.setString(lines[li]);
                        window.draw(infoText);
                    }

                    infoText.setCharacterSize(16);
                    infoText.setPosition(sf::Vector2f(95.0f, 190.0f + i * 120.0f));
                    infoText.setString("Price: " + std::to_string(displayItem.getPrice()) + " $ | Stock: " + std::to_string(displayItem.getStock()));
                    window.draw(infoText);

                    sf::RectangleShape bidBox;
                    bidBox.setSize(sf::Vector2f(100.0f, 36.0f));
                    bidBox.setPosition(sf::Vector2f(860.0f, 160.0f + i * 120.0f));
                    bidBox.setFillColor(sf::Color(220, 180, 60));
                    bidBox.setOutlineColor(sf::Color(90, 70, 20));
                    bidBox.setOutlineThickness(2.0f);
                    window.draw(bidBox);

                    sf::Text bidLabel(font, "Bid", 18);
                    bidLabel.setFillColor(sf::Color(30, 20, 5));
                    bidLabel.setPosition(sf::Vector2f(892.0f, 165.0f + i * 120.0f));
                    window.draw(bidLabel);
                }

                window.draw(btnNextRound.box);
                window.draw(btnNextRound.label);

                btnQuit.setPosition(300.0f, 620.0f);
                window.draw(btnQuit.box);
                window.draw(btnQuit.label);

                window.draw(btnInventory.box);
                window.draw(btnInventory.label);

                if (state.inBidding) {
                    sf::RectangleShape overlay;
                    overlay.setSize(sf::Vector2f(520.0f, 260.0f));
                    overlay.setPosition(sf::Vector2f(380.0f, 230.0f));
                    overlay.setFillColor(sf::Color(40, 28, 14, 240));
                    overlay.setOutlineColor(sf::Color(150, 120, 60));
                    overlay.setOutlineThickness(2.0f);
                    window.draw(overlay);

                    infoText.setCharacterSize(16);
                    infoText.setPosition(sf::Vector2f(400.0f, 250.0f));
                    infoText.setString("Bidding on: " + sanitize_ascii(state.biddingItem.getName()));
                    window.draw(infoText);

                    infoText.setPosition(sf::Vector2f(400.0f, 275.0f));
                    infoText.setString("Base price: " + std::to_string(state.biddingItem.getPrice()) + " $");
                    window.draw(infoText);

                    if (state.botBid > 0) {
                        infoText.setPosition(sf::Vector2f(400.0f, 300.0f));
                        infoText.setString("Bot bid: " + std::to_string(state.botBid) + " $");
                        window.draw(infoText);
                    }

                    inputBox.setPosition(sf::Vector2f(400.0f, 330.0f));
                    window.draw(inputBox);
                    
                    bool showCursor = (cursorClock.getElapsedTime().asMilliseconds() / 500) % 2 == 0;
                    inputText.setString(state.bidInput + (showCursor ? "_" : ""));
                    inputText.setPosition(sf::Vector2f(410.0f, 336.0f));
                    window.draw(inputText);

                    infoText.setPosition(sf::Vector2f(400.0f, 375.0f));
                    infoText.setString(state.bidMessage);
                    window.draw(infoText);

                    window.draw(btnSubmitBid.box);
                    window.draw(btnSubmitBid.label);
                    window.draw(btnWithdraw.box);
                    window.draw(btnWithdraw.label);
                }
            }
        } else if (state.screen == Screen::Inventory) {
            if (fontLoaded) {
                infoText.setCharacterSize(22);
                infoText.setPosition(sf::Vector2f(80.0f, 40.0f));
                infoText.setString("Inventory - Player: " + state.currentPlayer.getName() + " | Balance: " + std::to_string(state.currentPlayer.check_balance()) + " $");
                window.draw(infoText);

                int drawn_idx = 0;
                for (std::size_t i = 0; i < state.offers.lista_licitatie.size(); i++) {
                    int qty = state.currentPlayer.get_inv(i);
                    if (qty > 0) {
                        float yPos = 140.0f + drawn_idx * 120.0f - inventoryScrollOffset;
                        if (yPos > 50.0f && yPos < 650.0f) {
                            item displayItem = state.offers.lista_licitatie[i];
                            std::string safeName = sanitize_ascii(displayItem.getName());

                            sf::RectangleShape card;
                            card.setSize(sf::Vector2f(760.0f, 80.0f));
                            card.setPosition(sf::Vector2f(80.0f, yPos));
                            card.setFillColor(sf::Color(40, 40, 20));
                            card.setOutlineColor(sf::Color(100, 80, 30));
                            card.setOutlineThickness(2.0f);
                            window.draw(card);

                            infoText.setCharacterSize(18);
                            infoText.setPosition(sf::Vector2f(95.0f, yPos + 10.0f));
                            infoText.setString(safeName + " (x" + std::to_string(qty) + ")");
                            window.draw(infoText);

                            infoText.setCharacterSize(16);
                            infoText.setPosition(sf::Vector2f(95.0f, yPos + 40.0f));
                            int sellPrice = static_cast<int>(displayItem.getPrice() * state.marketMultipliers[i]);
                            infoText.setString("Market Sell Value: " + std::to_string(sellPrice) + " $");
                            window.draw(infoText);

                            sf::RectangleShape sellBox;
                            sellBox.setSize(sf::Vector2f(100.0f, 36.0f));
                            sellBox.setPosition(sf::Vector2f(860.0f, yPos + 20.0f));
                            sellBox.setFillColor(sf::Color(220, 80, 60));
                            sellBox.setOutlineColor(sf::Color(90, 20, 20));
                            sellBox.setOutlineThickness(2.0f);
                            window.draw(sellBox);

                            sf::Text sellLabel(font, "Sell", 18);
                            sellLabel.setFillColor(sf::Color(30, 5, 5));
                            sellLabel.setPosition(sf::Vector2f(890.0f, yPos + 25.0f));
                            window.draw(sellLabel);
                        }
                        drawn_idx++;
                    }
                }

                window.draw(btnBack.box);
                window.draw(btnBack.label);
            }
        } else if (state.screen == Screen::GameOver) {
            if (fontLoaded) {
                infoText.setCharacterSize(22);
                infoText.setPosition(sf::Vector2f(80.0f, 140.0f));
                infoText.setString("Game Over! Final balance: " + std::to_string(state.currentPlayer.check_balance()) + " $");
                window.draw(infoText);
                window.draw(btnBack.box);
                window.draw(btnBack.label);
                btnQuit.setPosition(260.0f, 620.0f);
                window.draw(btnQuit.box);
                window.draw(btnQuit.label);
            }
        }

        window.display();
    }

    return 0;
}
