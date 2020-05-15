#include <json.hpp>
#include <sstream>

#include "CARD.h"

/**
 * @brief Main constructor for card.
 * @param Suit (1-13)
 * @param Rank (0-3)
 */
Card::Card(int suit, int rank) : suit{suit}, rank{rank} {

}

/**
 * @brief Default constructor, makes 0,0 card.
 */
Card::Card() : Card{0,0} {

}

void to_json(nlohmann::json& j, const Card& card) {
    j = nlohmann::json{{"rank", card.rank}, {"suit", card.suit}};
}

void from_json(const nlohmann::json& j, Card& card) {
    card.rank = j.at("rank");
    card.suit = j.at("suit");
}

/**
 * @brief Turn card in to integer.
 */
int Card::asInt() {
    return suit+(rank<<2);
}

/**
 * @brief Turn card in to English text.
 */
std::string Card::toEnglish() {
    std::stringstream ss;
    ss << RANK_STRINGS.at(rank) << " of " << SUIT_STRINGS.at(suit);
    return ss.str();
}

/**
 * @brief Comparison operator.
 */
bool operator==(const Card& card1, const Card& card2) {
    return (card1.rank == card2.rank) && (card1.suit == card2.suit);
}
