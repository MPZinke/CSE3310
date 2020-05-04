#include "PLAY.h"
#include "json.hpp"

PLAY::PLAY(): PLAY(BET, 0) {}
PLAY::PLAY(PLAYTYPE type, HAND hand): type{type}, currenthand{hand} {}
PLAY::PLAY(PLAYTYPE type, int bet): type{type}, bet{bet} {}

void from_json(const nlohmann::json& j, PLAY& play) {
    play.type = j.at("type");
    play.bet = j.at("bet");
    play.tradedCards = j.at("tradedCards").get<std::vector<Card>>();
    play.currenthand = j.at("currenthand").get<HAND>();
    play.ID = j.at("ID").get<std::string>();
}

void to_json(nlohmann::json& j, const PLAY& play) {
    j = nlohmann::json{{"type", play.type}, {"bet", play.bet}, {"tradedCards", play.tradedCards}, {"currenthand", play.currenthand}, {"ID", play.ID}};
}

