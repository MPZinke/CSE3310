#include "PLAYER.h"

int PLAYER::id_count = 0;

PLAYER::PLAYER(): _id{std::to_string(id_count++)} {

}

void PLAYER::play(PLAY current_play) {
    _current_play = current_play;
}

void PLAYER::trade(std::vector<Card> cards, DECK deck) {
    std::vector<Card> newCards;
    auto currentCards = _current_hand.getCards();
    std::copy_if(cards.begin(), cards.end(), newCards.begin(), [&] (Card card) -> bool {
        return std::find(cards.begin(), cards.end(), card) == cards.end();
    });
    while(newCards.size() < 5) {
        newCards.push_back(deck.draw_card());
    }
    _current_hand = HAND{newCards};
}

void PLAYER::setHand(std::vector<Card> cards) {
    _current_hand = HAND{cards};
}

std::string PLAYER::id() {
    return _id;
}

int PLAYER::money() {
    return _money;
}


void PLAYER::money(int new_amount)
{
    _money = new_amount;
}


int PLAYER::current_bet() {
    return _current_bet;
}

HAND PLAYER::current_hand() {
    return _current_hand;
}

PLAY PLAYER::current_play() {
    return _current_play;
}
