#ifndef PLAY_inc
#define PLAY_inc

#include "json.hpp"
#include "HAND.h"
#include "CARD.h"

#include <string>

enum PLAYTYPE {
    CHECK=0,
    BET=1,
    RAISE=2,
    CALL=3,
    FOLD=4,
    MATCHSTART=5,
    TRADE=6,
    OUT=7
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

