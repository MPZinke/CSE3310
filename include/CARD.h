#ifndef CARD_inc
#define CARD_inc

#include <json.hpp>

enum RANK_ENUM {
    TWO = 2,
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


enum SUIT_ENUM {
    CLUBS,
    SPADES,
    DIAMONDS,
    HEARTS
};

const std::map<int, std::string> RANK_STRINGS =	{
    {2, "2"}, {3, "3"}, {4, "4"},
    {5, "5"}, {6, "6"}, {7, "7"},
    {8, "8"}, {9, "9"}, {10, "10"},
    {11, "Jack"}, {12, "Queen"},
    {13, "King"}, {1, "Ace"}
};


const std::map<int, std::string> SUIT_STRINGS =	{
    {0, "Clubs"}, {1, "Spades"},
    {2, "Diamonds"}, {3, "Hearts"}
};

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
