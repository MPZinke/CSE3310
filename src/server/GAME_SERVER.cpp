#include <sstream>

#include "chat_server.h"
#include "GAME_SERVER.h"
#include "ROUND.h"
#include "PLAYER.h"
#include "PLAY.h"


GAME_SERVER::GAME_SERVER()
: players{std::vector<PLAYER*>{}}, participants{std::vector<chat_participant_ptr>{}} {

}


void GAME_SERVER::addPlayer(chat_participant_ptr player, chat_message msg) {
	nlohmann::json msgjson = msg.getJson();

	if(participants.size() == 4 || has_started()) return;
	participants.push_back(player);
	std::string name = "", id="";
	players.push_back(new PLAYER{});
	if(players.size() == 1) start_game();  //TESTING
}


void GAME_SERVER::start_game()
{
	game_started = true;
	currentRound = new ROUND(0, &players, &message_queue);

	for(int i = 0; i < (int) players.size(); i++){
        PLAY tempPlay{};
        tempPlay.type = PLAYTYPE::MATCHSTART;
        tempPlay.tradedCards = players[i]->current_hand().getCards();
        tempPlay.ID = players[i]->id();
		auto temp = nlohmann::json{tempPlay};
		participants[i]->deliver(chat_message{temp});
		std::cout << chat_message{temp}.body() << std::endl; 
	}
	// send first person bet message
	auto temp = PLAY{};
	temp.ID = players[0]->id();
	temp.bet = 500;
	auto tempJ = nlohmann::json{PLAY{}};
	participants[0]->deliver(chat_message{tempJ});
}


void GAME_SERVER::processInput(chat_message msg) {
	if(msg.decode_header())
	{
		std::stringstream str;
		str.write(msg.body(), msg.body_length());
		std::string json_string = str.str();
		json_string = json_string.substr(1, json_string.size()-2);
		nlohmann::json j = nlohmann::json::parse(json_string);
		std::cout << j << std::endl;

		currentRound->process_play(j);
		send_queued_messages();
		if(currentRound->is_finished()) start_new_round();
	}
}


void GAME_SERVER::start_new_round()
{
	// check for game over...or don't
	ROUND* next_round = new ROUND(currentRound->round_number() + 1, &players, &message_queue);
	delete currentRound;
	currentRound = next_round;
}


// ——————————————————— UTILITY ———————————————————

bool GAME_SERVER::has_started()
{
	return game_started;
}


void GAME_SERVER::send_queued_messages()
{
	while(message_queue.size())
	{
		auto message = message_queue[0].begin();
		participants[message->first]->deliver(message->second);
		std::cout << message->second.body() << std::endl; 
		message_queue.erase(message_queue.begin());
	}
}

void GAME_SERVER::leave(chat_participant_ptr participant){
    int i;
    bool flag = false;
    for(i = 0; i < (int) participants.size(); i++){
        if((flag |= (participants[i] == participant)))
            break;
    }
    if(flag){
        participants.erase(participants.begin()+i);
        players.erase(players.begin()+i);
    }
}
