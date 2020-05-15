#include "ROUND.h"
const int ROUND::ANTE = 1;

#include<iostream>  //TESTING

/*
 * @brief Round constructor, builds a new round for game loop.
 */
ROUND::ROUND(int round_number, std::vector<PLAYER*> &remaining_players, MessageQueue* queue_ptr)
    : _round_number{round_number}, _remaining_players{remaining_players}, message_queue{*queue_ptr}, _current_player{0} {
    _deck = DECK();
    _deck.shuffle();
    for(unsigned int x = 0; x < _remaining_players.size(); x++) {
        _remaining_players[x]->setHand(_deck.draw_card(5));
        _remaining_players[x]->current_hand().sort();
        _remaining_players[x]->money(_remaining_players[x]->money()-ANTE);
    }
}

// ——————————————— GAME COMMUNICATION ———————————————
/*
 * @brief Process play from json.
 */
void ROUND::process_play(nlohmann::json playJson) {
    PLAY play = playJson.get<PLAY>();
    PLAYER* current_player = _remaining_players[_current_player];
    /* TODO fix ID check
    if(current_player->id() != play.ID)
    {
        std::cout << "Out of order player\n";
    }*/
    //PROCESS CURRENT PLAY
    switch(play.type) {
        case PLAYTYPE::RESIGN:
            remove_current_player();
            break;
        case PLAYTYPE::FOLD:
            _player_folds[_current_player] = true;
            break;
        case PLAYTYPE::TRADE: {
            if(currently_taking_bets()) return remove_current_player();  // CHEATER! (bad UI)
            _remaining_players[_current_player]->trade(play.tradedCards, _deck);
            break;
        }
        case PLAYTYPE::BET: {
            if(!currently_taking_bets()) return remove_current_player();  // CHEATER! (bad UI)
            current_player->money(current_player->money() - play.bet);  // decrement money
            if(play.bet < highest_bet()) {
                play.bet = highest_bet() - play.bet;  // they still need to match the other player's bet
                return add_message_to_queue(play);  // stay on current player and request more $$
            }
            break;
    }
    default:
        break;
    }

    //SET UP NEXT PLAYER
    _current_player++;
    if((unsigned int)_current_player == _remaining_players.size()) {
        _current_player %= _remaining_players.size();
        _round_phase++;
        //int required_bet = 0;  //TODO: bet amount
    } 
    if(is_finished()) finish_round();
    for(int i = 0; i < (int) _remaining_players.size(); i++) {
        auto &recipient = _remaining_players[i];
        for(int k = 0; k < (int) _remaining_players.size(); k++) {
            auto &dataOf = _remaining_players[k];
            if(i == k) {
                PLAY play = recipient->state_as_play();
                play.type = UPDATE;
                add_message_to_queue(i, (nlohmann::json) play);
            } else {
                PLAY play{UPDATE};
                play.ID = dataOf->id();
                play.bet = dataOf->money();
                add_message_to_queue(i, (nlohmann::json) play);
            }
        }
    }
    PLAY start{TURNSTART};
    start.phase = _round_phase%2==1;
    start.bet = highest_bet();
    add_message_to_queue(_current_player, start);

    std::cout << "Next player: " << _current_player << "\tID: " << current_player->id() << "\tNext round: " << _round_number << std::endl << std::endl;  //TESTING
}

/*
 * @brief Add a message to the queue.
 */
void ROUND::add_message_to_queue(PLAY current_play) {
    current_play.ID = _remaining_players[_current_player]->id();
    nlohmann::json message = current_play;
    std::cout << "MESSAGE QUEUED: " << message << std::endl;
    message_queue.push_back({{_current_player, chat_message{message}}});
}

/*
 * @brief Add a message to the queue for specific player.
 */
void ROUND::add_message_to_queue(int player, PLAY play) {
    if(play.ID.length() == 0) play.ID = _remaining_players[player]->id();
    nlohmann::json message = play;
    std::cout << "MESSAGE QUEUED: " << message << std::endl;
    message_queue.push_back({{player, chat_message{message}}});
}


// ——————————————————— UTILITY ———————————————————
/*
 * @brief Return phase.
 */
bool ROUND::is_finished() {
    return _round_phase == 5;
}


// —————————————————— PRIVATE ———————————————————
// —————————————————————————————————————————

// ————————————————— WHOLE GAME —————————————————

/*
 * @brief Remove current player from the round.
 */
void ROUND::remove_current_player() { 
    for(int i = message_queue.size()-1; i >= 0; i--){
        auto tuple = message_queue[i].begin();
        nlohmann::json j = tuple->second.getJson();
        bool readd = false;
        if(tuple->first > _current_player){
            readd = true;
        }
        if(j.get<PLAY>().ID > std::to_string(_current_player)){
            j = tuple->second.getJson();
            j["ID"] = std::to_string(std::stoi(j["ID"].get<std::string>())-1);
            readd = true;
        }
        if(readd) add_message_to_queue(tuple->first-1, j);
        if(tuple->first == _current_player){
            message_queue.erase(message_queue.begin()+i);
        }
    }

    for(unsigned int x = _current_player; x < _remaining_players.size() - 1; x++) {
        _player_folds[x] = _player_folds[x+1];
        _player_bets[x] = _player_bets[x+1];
    }
    delete _remaining_players[_current_player];
    _remaining_players.erase(_remaining_players.begin() + _current_player);
}

/*
 * @brief Run round finish up.
 */
void ROUND::finish_round() {
    int highest_hand_value = 0;
    int pot_total = 0;
    for(unsigned int x = 0; x < _remaining_players.size(); x++) { // highest hand and total pot
        pot_total += _player_bets[x];
        if(_remaining_players[x]->current_hand().value() > highest_hand_value)
            highest_hand_value = _remaining_players[x]->current_hand().value();
    }

    std::vector<std::map<int, PLAYER*>> winners;
    for(unsigned int x = 0; x < _remaining_players.size(); x++)
        if(_remaining_players[x]->current_hand().value() == highest_hand_value)
            winners.push_back({{x, _remaining_players[x]}});

    // split winnings and message client(s)
    int pot_split = pot_total / winners.size();  // integer division to round down (tip the dealer)
    for(unsigned int x = 0; x < winners.size(); x++) {
        auto iter = winners[x].begin();
        iter->second->money(iter->second->money() + pot_split);
        add_message_to_queue(iter->first, PLAY{BET, pot_split});
    }

    // eliminate anyone without money
    for(unsigned int x = 0; x < _remaining_players.size();) { // while() with initialization (keep x hidden)
        if(_remaining_players[x]->money()) x++;
        else { // do not increment x because vector just shifted left
            delete _remaining_players[x];
            _remaining_players.erase(_remaining_players.begin() + _current_player);
        }
    }
}


// ———————————————— INDIVIDUAL PLAY —————————————————


// ——————————————————— UTILITY ———————————————————

/*
 * @brief Check and return if all other players have folded.
 */
bool ROUND::all_other_players_have_folded() {
    for(unsigned int x = _current_player; x < _remaining_players.size(); x++) {
        if(!_player_folds[_current_player]) return false;
        _current_player++;
    }
    return true;
}

/*
 * @brief Check to see if the current bet is higher than anyone's value.
 */
bool ROUND::bet_amount_exceeds_other_players_worth(int current_bet) {
    for(unsigned int x = 0; x < _remaining_players.size(); x++)
        if(_remaining_players[x]->money() + _player_bets[x] > current_bet) return true;
    return false;
}

/*
 * @brief Taking bets?
 */
bool ROUND::currently_taking_bets() {
    return _round_phase % 2 == 0;
}

/*
 * @brief Get round number.
 */
int ROUND::round_number() {
    return _round_number;
}

/*
 * @brief Get current player.
 */
int ROUND::current_player() {
    return _current_player;
}

/*
 * @brief Draw a single card from the deck.
 */
Card ROUND::draw_card() {
    return _deck.draw_card();
}

/*
 * @brief Draw cards from the deck.
 */
std::vector<Card> ROUND::draw_card(int draw_amount) {
    return _deck.draw_card(draw_amount);
}

/*
 * @brief Return the currnet highest bet.
 */
int ROUND::highest_bet() {
    int current_highest = _player_bets[0];
    for(unsigned int x = 0; x < _remaining_players.size(); x++)
        if(_player_bets[x] > current_highest) current_highest = _player_bets[x];
    return current_highest;
}

/*
 * @brief Round destructor
 */
ROUND::~ROUND() {

};

