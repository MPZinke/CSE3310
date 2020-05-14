#include "HAND.h"

#include<algorithm>
#include<sstream>

/*
 * @brief Default HAND constructor
 */
HAND::HAND() {
    
}

/*
 * @brief Construct hand from cards.
 */
HAND::HAND(std::vector<Card> inCards): cards{inCards} { 
}

/*
 * @brief Construct HAND from json.
 */
HAND::HAND(nlohmann::json j) {
    j.get<HAND>();
}

/*
 * @brief Construct HAND from string (json string).
 */
HAND::HAND(std::string str) : HAND(nlohmann::json{str}) { 

}

/*
 * @brief Sort hand using asInt.
 */
void HAND::sort() {
    std::sort(cards.begin(), cards.end(), [](Card a, Card b) {
        return a.asInt() < b.asInt();
    });
}

/*
 * @brief Return all cards.
 */
const std::vector<Card>& HAND::getCards() {
    return cards;
}

/*
 * @brief Check for four of a kind
 */
bool HAND::checkFourOfKind(std::vector<Card> cards) {
    /*
    0&1&2&3!4
    0!1&2&3&4
    */
    bool flag = false;
    if(cards[0].rank == cards[3].rank){
        flag = true;
        swap(3, 4);
    } else if(cards[1].rank == cards[4].rank){
        flag = true;
    }
    return flag;
}

/*
 * @brief Check for full house
 */
bool HAND::checkFullHouse(std::vector<Card> cards) {
    bool flag = false;
    if(cards[0].rank == cards[1].rank && cards[2].rank == cards[4].rank) {
        flag = true; 
    } else if(cards[0].rank == cards[2].rank && cards[3].rank == cards[4].rank) {
        swap(2, 4);
       flag = true; 
    }
    return flag;
}

/*
 * @brief Check for Flush
 */
bool HAND::checkFlush(std::vector<Card> cards) {
    //Save suit of first card to compare to all other cards
    int suit = cards[0].suit;
    //Flag to be swapped if any of the cards fail either suit check or rank check.
    bool flag = true;
    for(int i = 0; i < 5; i++) {
        if(cards[i].suit != suit)
            flag = false;
    }
    return flag;
}

/*
 * @brief Check for straight
 */
bool HAND::checkStraight(std::vector<Card> cards) {
    int rank = cards[0].rank;
    bool flag = true;
    for(int i = 0; i < 5; i++) {
        if(cards[i].rank != rank++)
            flag = false;
    }
    return flag;
}

/*
 * @brief Checks for a set of three of a kind
 */
bool HAND::checkThreeOfAKind(std::vector<Card> cards) {
    /*
    0 == 2 && 2 != 3
    0 != 1 && 1 == 3 && 3 !=4
    1 != 2 && 2 == 4
    */
    bool flag = false;
    if(cards[0].rank == cards[2].rank && cards[2].rank != cards[3].rank) {
        flag = true;
        swap(2, 4);
    } else if(cards[0].rank != cards[1].rank && cards[1].rank == cards[3].rank && cards[3].rank != cards[4].rank) {
        flag = true;
        swap(3, 4);
    } else if(cards[1].rank != cards[2].rank && cards[2].rank == cards[4].rank) {
        flag = true;
    }
    return flag;
}

/*
 * @brief Helper function that runs == && == && != && != on 8 numbers to check for pairs exhaustively
 */
bool HAND::checkSpecificPairs(std::vector<Card> cards, int pairs[8]) {
    std::transform(pairs, pairs+8, pairs, [&] (int ind) -> int { return cards[ind].rank; });

    return (pairs[0] == pairs[1]) && (pairs[2] == pairs[3]) && (pairs[4] != pairs[5]) && (pairs[6] != pairs[7]);
}

/*
 * @brief Check for two pairs.
 */
bool HAND::checkTwoPairs(std::vector<Card> cards) {
    /*
    0&1 2&3 1!2 3!4
    0&1 3&4 1!2 2!3
    1&2 3&4 0!1 2!3
    */
    int pairs[3][8] = {{0,1,2,3,1,2,3,4},
        {0,1,3,4,1,2,2,3},
        {1,2,3,4,0,1,2,3}
    };
    bool checks[3];
    std::transform(pairs, pairs+3, checks,  [&] (int set[8]) -> bool { return checkSpecificPairs(cards, set); });
    bool flag = false;
    if(checks[0]) {
        flag = true;
        swap(3, 4);
    } else if(checks[1] || checks[2]) {
        flag = true;    
    }
    return  flag;
}

/*
 * @brief Helper function to check for a single pair by applying a != && == && != set of operations
 */
bool HAND::checkSinglePair(std::vector<Card> cards, int pairs[6]) {
    std::transform(pairs, pairs+6, pairs, [&] (int ind) -> int { return cards[ind].rank; });
    return (pairs[0] != pairs[1]) && (pairs[2] == pairs[3]) && (pairs[4] != pairs[5]);
}

/*
 * @brief Check for a single pair
 */
bool HAND::checkOnePair(std::vector<Card> cards) {
    /*
    1!2 0&1 1!2
    0!1 1&2 2!3
    1!2 2&3 3!4
    2!3 3&4 2!3
    */
    int pairs[5][6] = {{2, 0, 0, 1, 1, 2},
        {0, 1, 1, 2, 2, 3},
        {1, 2, 2, 3, 3, 4},
        {2, 3, 3, 4, 2, 3},
    };
    bool checks[4];
    std::transform(pairs, pairs+4, checks, [&] (int pair[4]) -> bool { return checkSinglePair(cards, pair); });
    bool flag = false;
    if(checks[0]) {
        flag = true;
        swap(1, 4);
    } else if(checks[1]) {
        flag = true;
        swap(2, 4);
    } else if(checks[2]) {
        flag = true;
        swap(3, 4);
    } else if(checks[3]) {
        flag = true;
    } 
    return flag;
}

/*
 * @brief Check for both a straight and a flush
 */
bool HAND::checkSFlush(std::vector<Card> cards) {
    return checkFlush(cards) && checkStraight(cards);
}

/*
 * @brief Check for Flush and verify ranks match royal ranks
 */
bool HAND::checkRFlush(std::vector<Card> cards) {
    if(checkFlush(cards)) {
        //Const set of ranks to check royals
        const int ROYALS[5] = {1, 10, 11, 12, 13};
        //Flag to be swapped if any of the cards either fail the suit check or rank check.
        bool flag = true;
        for(int i = 0; i < 5; i++) {
            if(cards[i].rank != ROYALS[i])
                flag = false;
        }
        return flag;
    }
    return false;
}

/*
 * @brief Value single hand exhaustively.
 */
unsigned char HAND::value() {
    sort();
    unsigned char value = 0x00;
    //Royal Flush
    if(checkRFlush(cards))
        value += (9<<4);
    //Straight Flush
    else if(checkSFlush(cards))
        value += (8<<4);
    //Four of a Kind
    else if(checkFourOfKind(cards))
        value += (7<<4);
    //Full House
    else if(checkFullHouse(cards))
        value += (6<<4);
    //Flush
    else if(checkFlush(cards))
        value += (5<<4);
    //Straight
    else if(checkStraight(cards))
        value += (4<<4);
    //Three of a Kind
    else if(checkThreeOfAKind(cards))
        value += (3<<4);
    //Two Pairs
    else if(checkTwoPairs(cards))
        value += (2<<4);
    //One Pair
    else if(checkOnePair(cards))
        value += (1<<4);
    //Lone High Card (do nothing)
    value += cards[4].rank;
    currentValue = value;
    return value;
}

/*
 * @brief Value as a string.
 */
std::string HAND::valueStr(){
    if(currentValue == 0x00)
        value();
    std::stringstream strstr{};
    strstr << handNameStrings.at((int) currentValue>>4) << " With Rank of " << (int) (currentValue&0x0F); 
    return strstr.str();
}


void to_json(nlohmann::json& j, const HAND& hand) {
    j = nlohmann::json{{"cards", hand.cards}};
}

void from_json(const nlohmann::json& j, HAND& hand) {
    j.at("cards").get_to(hand.cards);
}

std::ostream& operator<<(std::ostream& os, const HAND& hand) {
    nlohmann::json j = hand;
    return os << j; 
}

/*
 * @brief Swap to cards in hand.
 */
void HAND::swap(int ind1, int ind2){
    Card temp = cards[ind1];
    cards[ind1] = cards[ind2];
    cards[ind2] = temp;
}
