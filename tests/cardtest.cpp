#include "CARD.h"

#include <vector>
#include <iostream>
int main(/*int argc, char *argv[]*/) {
    std::vector<Card> cards;
    for(int i = 0; i < 52; i++) {
        cards.push_back(Card{(i/13), (i%13)+1});
        std::cout << cards[i].toEnglish() << std::endl;
    }

    nlohmann::json j{{"cards", cards}};

    std::cout << j << std::endl;

    return 0;
}
