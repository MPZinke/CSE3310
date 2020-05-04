#ifndef ROUND_inc
#define ROUND_inc

#include"chat_message.hpp"
#include"CARD.h"
#include "PLAYER.h"

#include<map>
#include<string>
#include<vector>

typedef std::vector<std::map<int, chat_message>> MessageQueue;

class ROUND {
public:
	ROUND(int, std::vector<PLAYER*>*, MessageQueue*);

	Card draw_card();
	std::vector<Card> draw_card(int);
	void finish_round();

	std::string return_message();
	void process_play(nlohmann::json play);

	int round_number();
	bool is_finished();
	bool all_other_players_have_folded();
	int highest_bet();
	bool currently_taking_bets();
	void remove_current_player();
	void add_message_to_queue(PLAY);
	void add_message_to_queue(int, PLAY);

	~ROUND();

private:
	int _round_number;
	int _round_phase = 0;  // whether the round is betting or trading (even bet; odd trade) 5 = end
	std::vector<PLAYER*>* _remaining_players;
	MessageQueue* message_queue;
	DECK _deck;

	int _current_pot;
	int _current_player;

	bool _betting = false;  // round is currently set to betting
	int _player_bets[5] = {0, 0, 0, 0, 0};
	bool _player_folds[5] = {false, false, false, false, false};
	int _bet_player;

	bool bet_amount_exceeds_other_players_worth(int);
};

#endif

