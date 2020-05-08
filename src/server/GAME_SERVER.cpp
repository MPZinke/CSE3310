#include <sstream>

#include "chat_server.h"
#include "GAME_SERVER.h"
#include "ROUND.h"
#include "PLAYER.h"
#include "PLAY.h"

//Create Game Server
GAME_SERVER::GAME_SERVER()
: players{std::vector<PLAYER*>{}}, participants{std::vector<chat_participant_ptr>{}} {

}

//Add a player every time the server receives a new connection
void GAME_SERVER::addPlayer(chat_participant_ptr player) {
	if(participants.size() == 4 || has_started()) return;
	participants.push_back(player);
	players.push_back(new PLAYER{});
	if(players.size() == 4) start_game();  //TESTING
}

//Start the game, initialize a round and stop allowing new players to join
void GAME_SERVER::start_game()
{
	game_started = true;
	currentRound = new ROUND(0, players, &message_queue);

    //Loop sends begin game information to all players
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
	auto tempJ = nlohmann::json{PLAY{}};
	participants[0]->deliver(chat_message{tempJ});
}

//Process an input from the player
void GAME_SERVER::processInput(chat_message msg) {
	if(msg.decode_header())
	{
		std::stringstream str;
        str.write(msg.body(), msg.body_length());
        str << "\n";
        std::string msgstr = str.rdbuf()->str();
		nlohmann::json j = nlohmann::json::parse(msgstr.substr(1, msgstr.find_last_of('}')));
		std::cout << j << std::endl;
        PLAY play = j.get<PLAY>();
        if(stoi(play.ID) == currentRound->current_player()){
            currentRound->process_play(j);
            send_queued_messages();
            if(currentRound->is_finished()) start_new_round();
        }
	}
}

//Start a new round and destroy the old one
void GAME_SERVER::start_new_round()
{
    std::cout << "Starting new round" << std::endl;
	// check for game over...or don't
	ROUND* next_round = new ROUND(currentRound->round_number() + 1, players, &message_queue);
	delete currentRound;
	currentRound = next_round;
}


// ——————————————————— UTILITY ———————————————————

bool GAME_SERVER::has_started()
{
	return game_started;
}

//Dequeue the messages that have been queued
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

//Remove a player that has left the server
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
    //Implement round logic
    //currentRound.leave(i);
}
