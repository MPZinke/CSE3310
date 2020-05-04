#include"DECK.h"
#include"CARD.h"

#include <random>
#include <ctime>

DECK::DECK() {
    std::srand(std::time(nullptr));
    for(int i = 0; i < 52; i++) {
        int suit = i%4;
        int rank = (i/4)+1;
        _cards.push_back(Card{suit, rank});
    }
}

Card DECK::draw_card() {
    if(_next_card_in_deck_index >= 52) throw "No more cards";
    return _cards[_next_card_in_deck_index++];
}


std::vector<Card> DECK::draw_card(int draw_amount) {
    if(_next_card_in_deck_index + draw_amount >= 52) throw "No more cards";
    std::vector<Card> new_cards;
    for(int x = 0; x < draw_amount; x++)
        new_cards.push_back(_cards[x + _next_card_in_deck_index++]);
    return new_cards;
}


bool DECK::still_has_cards() {
    return _next_card_in_deck_index < 52;
}

void DECK::shuffle(){
    for(int i = 0; i < 52; i++){
        for(int k = i; k < 52; k++){
            auto temp = _cards[i];
            int si = (std::rand()%52);
            _cards[i] = _cards[si];
            _cards[si] = temp;
        }
    }
}

void DECK::shuffleRemaining(){
    for(int i = _next_card_in_deck_index; i < (52-_next_card_in_deck_index)/2; i++){
        for(int k = i; k < 52; k++){
            auto temp = _cards[i];
            int si = (std::rand()%(52-_next_card_in_deck_index)) + _next_card_in_deck_index;
            _cards[i] = _cards[si];
            _cards[si] = temp;
        }
    }
}

void DECK::mergeDiscarded(){
    _next_card_in_deck_index = 0;
    shuffle();
}

void from_json(nlohmann::json& j, DECK& deck){
    j.at("cards").get_to(deck._cards);
    j.at("_next_card_in_deck_index").get_to(deck._next_card_in_deck_index);
}

void to_json(nlohmann::json& j, const DECK& deck){
    j = nlohmann::json{{"cards", deck._cards}, {"_next_card_in_deck_index", deck._next_card_in_deck_index}};
}
