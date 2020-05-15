#include "DECK.h"
#include "json.hpp"

#include <iostream>

int main() {
    DECK deck{};

    deck.draw_card();

    nlohmann::json j = deck;

    std::cout << j << std::endl;

    return 0;
}
