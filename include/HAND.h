#ifndef hand_inc
#define hand_inc

#include "json.hpp"
#include "CARD.h"

#include <map>

enum HAND_ENUM {
    NOTHING,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
};

const std::map<int, std::string> handNameStrings{
    {0, "NOTHING"},
    {1, "ONE_PAIR"},
    {2, "TWO_PAIR"},
    {3, "THREE_OF_A_KIND"},
    {4, "STRAIGHT"},
    {5, "FLUSH"},
    {6, "FULL_HOUSE"},
    {7, "FOUR_OF_A_KIND"},
    {8, "STRAIGHT_FLUSH"},
    {9, "ROYAL_FLUSH"}
};

/**
 * @brief Implements a hand of cards, primarily provides way to value 5 cards.
 */
class HAND {
public:
    HAND();
    HAND(std::vector<Card> cards);
    HAND(nlohmann::json);
    HAND(std::string);  // JSON usage

    void sort();

    unsigned char value();
    std::string valueStr();

    friend void to_json(nlohmann::json& j, const HAND& hand);
    friend void from_json(const nlohmann::json& j, HAND& hand);
    friend std::ostream& operator<<(std::ostream&, HAND);

    const std::vector<Card>& getCards();

    //TODO
    //std::string operator+(std::string);

private:
    bool checkFourOfKind(std::vector<Card> cards); 
    bool checkFullHouse(std::vector<Card> cards); 
    bool checkFlush(std::vector<Card> cards); 
    bool checkStraight(std::vector<Card> cards); 
    bool checkThreeOfAKind(std::vector<Card> cards); 
    bool checkSpecificPairs(std::vector<Card> cards, int pairs[8]); 
    bool checkTwoPairs(std::vector<Card> cards); 
    bool checkSinglePair(std::vector<Card> cards, int pairs[6]); 
    bool checkOnePair(std::vector<Card> cards); 
    bool checkSFlush(std::vector<Card> cards); 
    bool checkRFlush(std::vector<Card> cards); 

    void swap(int ind1, int ind2);

    std::vector<Card> cards;
    unsigned char currentValue;
};

#endif
