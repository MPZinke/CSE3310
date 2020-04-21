#ifndef PLAY_inc
#define PLAY_inc

#include "json.hpp"
#include "HAND.h"
#include "CARD.h"

enum PLAYTYPE {
    BET=0,
    CHECK=1,
    FOLD=2,
    TRADE=3,
    OUT=4
};

class PLAY{
    public:
        PLAY();
        PLAY(PLAYTYPE type, int bet);

        friend void to_json(nlohmann::json& j, const PLAY& play);
        friend void from_json(const nlohmann::json& j, PLAY& play);

        PLAYTYPE type;
        int bet;
        std::vector<Card> tradedCards;
        HAND currenthand;
};

#endif
