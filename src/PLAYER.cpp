#include "PLAYER.h"

int PLAYER::id_count = 0;

/*
 * @brief Default PLAYER constructor.
 */
PLAYER::PLAYER(): _id{std::to_string(id_count++)} {

}

/*
 * @brief Set play
 */
void PLAYER::play(PLAY current_play) {
    _current_play = current_play;
}

/*
 * @brief Trade cards from hand for cards from deck.
 */
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

/*
 * @brief Overwrite cards.
 */
void PLAYER::setHand(std::vector<Card> cards) {
    _current_hand = HAND{cards};
}

/*
 * @brief Get ID
 */
std::string PLAYER::id() {
    return _id;
}

/*
 * @brief Get money.
 */
int PLAYER::money() {
    return _money;
}

/*
 * @brief Set money.
 */
void PLAYER::money(int new_amount)
{
    _money = new_amount;
}

/*
 * @brief Get current bet.
 */
int PLAYER::current_bet() {
    return _current_bet;
}

/*
 * @brief Get current HAND.
 */
HAND PLAYER::current_hand() {
    return _current_hand;
}

/*
 * @brief Get current play.
 */
PLAY PLAYER::current_play() {
    return _current_play;
}

/*
 * @brief Return current player state as a play.
 */
PLAY PLAYER::state_as_play() {
    auto temp = PLAY{PLAYTYPE::MATCHSTART};
	temp.ID = id();
    temp.bet = money();
    temp.currenthand = current_hand();
    return temp;
}
