#ifndef PLAYER_inc
#define PLAYER_inc

#include <string>
#include <vector>

#include "DECK.h"
#include "HAND.h"
#include "CARD.h"
#include "PLAY.h"

class PLAYER {
public:
    PLAYER();

    void play(PLAY current_play);

    void trade(std::vector<Card> cards, DECK deck);
    void setHand(std::vector<Card> cards);

    std::string id();
    int money();
    void money(int);  // set current money level
    int current_bet();
    HAND current_hand();
    PLAY current_play();

private:
    std::string _id;
    int _money=200;

    HAND _current_hand;
    int _current_bet;
    PLAY _current_play;
    static int id_count;
};

#endif

