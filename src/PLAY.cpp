#include "PLAY.h"
#include "json.hpp"
/*
 * @brief Default PLAY constructor
 */
PLAY::PLAY(): PLAY(BET, 0) {}
/*
 * @brief PLAY constructor with type
 */
PLAY::PLAY(PLAYTYPE type): type{type} {}
/*
 * @brief PLAY constructor with type and a HAND
 */
PLAY::PLAY(PLAYTYPE type, HAND hand): type{type}, currenthand{hand} {}
/*
 * @brief PLAY constructor with type and bet.
 */
PLAY::PLAY(PLAYTYPE type, int bet): type{type}, bet{bet} {}

void from_json(const nlohmann::json& j, PLAY& play) {
    play.type = j.at("type");
    play.bet = j.at("bet");
    play.phase = j.at("phase");
    play.message = j.at("message").get<std::string>();
    play.tradedCards = j.at("tradedCards").get<std::vector<Card>>();
    play.currenthand = j.at("currenthand").get<HAND>();
    play.ID = j.at("ID").get<std::string>();
}

void to_json(nlohmann::json& j, const PLAY& play) {
    j = nlohmann::json{{"type", play.type}, {"bet", play.bet}, {"phase", play.phase}, {"message", play.message}, {"tradedCards", play.tradedCards}, {"currenthand", play.currenthand}, {"ID", play.ID}};
}

