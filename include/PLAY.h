#ifndef PLAY_inc
#define PLAY_inc

#include "json.hpp"
#include "HAND.h"
#include "CARD.h"

#include <string>

enum PLAYTYPE {
    BET=0,
    CHECK=1,
    FOLD=2,
    TRADE=3,
    OUT=4,
    MATCHSTART=5
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
        std::string ID;
        std::vector<Card> tradedCards;
        HAND currenthand;
};

#endif

