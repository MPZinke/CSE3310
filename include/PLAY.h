#ifndef PLAY_inc
#define PLAY_inc

#include "json.hpp"
#include "HAND.h"
#include "CARD.h"

#include <string>

enum PLAYTYPE {
    BET,        //SERVER: Remove money being added to player bet. 
    CHECK,      //SERVER: Remove no money, do not change current bet. 
    RAISE,      //SERVER: Remove money needed to raise bet. 
    CALL,       //SERVER: Remove money needed to match bet. 
    FOLD,       //SERVER: Drop out of current round.
    TRADE,      //SERVER: Receive cards to remove from hand.
    RESIGN,        //CLIENT: Remove from game, no longer has money.
    MATCHSTART, //CLIENT: Receive new deck, money, current phase, and ante.
    UPDATE,     //CLIENT: Receive new deck, money, current phase, and bet change. 
    TURNSTART,  //CLIENT: Receive turn start.
    MESSAGE     //CLIENT: Receive text message.
                //SERVER: Receive and send text message.
};

class PLAY{
    public:
        PLAY();
        PLAY(PLAYTYPE type);
        PLAY(PLAYTYPE type, HAND play);
        PLAY(PLAYTYPE type, int bet);

        friend void to_json(nlohmann::json& j, const PLAY& play);
        friend void from_json(const nlohmann::json& j, PLAY& play);

        PLAYTYPE type;
        int bet;
        bool phase;
        std::string ID;
        std::string message;
        std::vector<Card> tradedCards;
        HAND currenthand;
};

#endif

