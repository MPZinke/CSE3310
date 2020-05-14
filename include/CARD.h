#ifndef CARD_inc
#define CARD_inc

#include <json.hpp>

/**
 *  @brief Enum for ranks
 */
enum RANK_ENUM {
    ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};

/**
 *  @brief Enum for suits
 */
 enum SUIT_ENUM {
    CLUBS,
    SPADES,
    DIAMONDS,
    HEARTS
};

/**
 * @brief Map for converting from rank enum to string
 */
const std::map<int, std::string> RANK_STRINGS =	{
    {2, "2"}, {3, "3"}, {4, "4"},
    {5, "5"}, {6, "6"}, {7, "7"},
    {8, "8"}, {9, "9"}, {10, "10"},
    {11, "Jack"}, {12, "Queen"},
    {13, "King"}, {1, "Ace"}
};

/**
 * @brief Map for converting from suit enum to string
 */
 const std::map<int, std::string> SUIT_STRINGS =	{
    {0, "Clubs"}, {1, "Spades"},
    {2, "Diamonds"}, {3, "Hearts"}
};

/**
 * @brief Card class, holds basic functionality for Card.
 * @details This card holds all of the functionality needed for a Card, including json parsers and helper functions to read the card as english text.
 */
class Card{
    public:
        Card();
        Card(int suit, int rank);
 
        friend void to_json(nlohmann::json& j, const Card& card);
        friend void from_json(const nlohmann::json& j, Card& card);
        int asInt();
        std::string toEnglish();
        friend bool operator==(const Card& card1, const Card& card2);

        int suit;
        int rank;
};

#endif
