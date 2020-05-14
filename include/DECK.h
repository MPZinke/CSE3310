
#ifndef _DECK_
#define _DECK_

#include<string>
#include<vector>

#include"CARD.h"

/**
 * @brief Class to hold a deck of cards, manages shuffling and keeps track of remaining cards.
 */
class DECK {
public:
    DECK();

    Card draw_card();
    std::vector<Card> draw_card(int);  // draw multiple cards
    
    void shuffle();
    void shuffleRemaining();
    void mergeDiscarded();

    friend void from_json(nlohmann::json& j, DECK& deck);
    friend void to_json(nlohmann::json& j, const DECK& deck);

    bool still_has_cards();

private:
    std::vector<Card> _cards;
    int _next_card_in_deck_index = 0;
};

#endif
