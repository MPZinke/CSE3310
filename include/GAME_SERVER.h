#ifndef GAME_SERVER_inc
#define GAME_SERVER_inc

#include"chat_message.hpp"
#include "chat_participant.h"
#include "asio.hpp"
#include "PLAYER.h"
#include "DECK.h"
#include "ROUND.h"

#include<string>
#include <vector>

typedef std::shared_ptr<chat_participant> chat_participant_ptr;
typedef std::vector<std::map<int, chat_message>> MessageQueue;


class GAME_SERVER {
	public:
		GAME_SERVER();
		
		void addPlayer(chat_participant_ptr player, chat_message msg); 
		void start_game();
        void leave(chat_participant_ptr participant);
		void processInput(chat_message msg);
		bool has_started();

	private:
		void processRound();
		void send_queued_messages();
		void updatePlayers();

		bool updateRound();
		void start_new_round();

		bool game_started = false;
		ROUND* currentRound;
		std::vector<PLAYER*> players;
		std::vector<chat_participant_ptr> participants;
		MessageQueue message_queue;
};

#endif
