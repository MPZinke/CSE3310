#ifndef GAME_SERVER_inc
#define GAME_SERVER_inc

#include"chat_message.hpp"
#include "chat_participant.h"
#include "asio.hpp"
#include "PLAYER.h"
#include "DECK.h"
#include "ROUND.h"

#include <vector>
#include<string>

typedef std::shared_ptr<chat_participant> chat_participant_ptr;


class GAME_SERVER {
	public:
		GAME_SERVER();
		
		void addPlayer(chat_participant_ptr player, chat_message msg); 
		void start_game();

		std::string processPacket(const chat_message& message);
		void processRound();
        void processInput(chat_message msg);
		void updatePlayers();

		bool updateRound();
        bool has_started();

	private:
		bool game_started = false;
		ROUND* currentRound;
		std::vector<PLAYER*> players;
		std::vector<chat_participant_ptr> participants;
};

#endif
