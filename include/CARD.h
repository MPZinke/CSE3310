#ifndef CARD_inc
#define CARD_inc

#include <json.hpp>

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


enum SUIT_ENUM {
    CLUBS,
    SPADES,
    DIAMONDS,
    HEARTS
};

const std::map<int, std::string> RANK_STRINGS =	{
    {2, "Two"}, {3, "Three"}, {4, "Four"},
    {5, "Five"}, {6, "Six"}, {7, "Seven"},
    {8, "Eight"}, {9, "Nine"}, {10, "Ten"},
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
