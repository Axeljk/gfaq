#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "models/actor.h"
#include "models/blurb.h"
#include "models/effect.h"
#include "models/party.h"
#include "models/spell.h"
#include "models/xbfq.h"
#include "battle.h"
#include "chargen.h"
#include "console_util.h"
#include "motive.h"


const char kVersion[] = "0.01.000";

void MainLoop(xbfq *save_game, actor *player);
void MainMenu();
void NewGame();
void ContinueGame();

int main() {
	std::system("mode con: cols=80 lines=24");
	MainMenu();

	return 0;
}

void MainMenu() {
	char choice = '\0';
	bool selected = false;


	while (selected == false) {
		NewScreen();
		std::cout << '\n' << HorizontalLine() << '\n'
				  << "                                GENERIC FANTASY\n"
				  << "                                ADVENTURE QUEST\n\n"
				  << "                                 VER. " << kVersion
				  << "\n\n\n" << HorizontalLine() << "\n\n";
		std::cout << "                               [N]ew Game\n"
				  << "                               [C]ontinue\n"
				  << "                               [Q]uit\n"
				  << "                                ";
		std::cin >> choice;

		switch(choice) {
			case 'n':
			case 'N':
				NewGame();
				selected = true;
				break;
			case 'c':
			case 'C':
				ContinueGame();
				selected = true;
				break;
			case 'q':
			case 'Q':
				NewScreen();
				std::cout << "\n\n\n\n\n\n\n\n\n\n\n"
						  << "                           "
						  << "Another light vanquished.";
				std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
				selected = true;
				Pause();
		}
	}
}

void NewGame() {
	actor *player;
	xbfq *save_game = new xbfq("core");

	// Intro screen.
	NewScreen();
	std::cout << "\n\n\n\n\n\n\n\n\n\n\n"
			  << "             A druid accidentally reincarnates you instead "
			  << "of their\n           prettier, and much more successful, "
			  << "friend. Who are you?"
			  << "\n\n\n\n\n\n\n\n\n\n\n";
	Pause();

	// Character creation.
	player = save_game->actors.Add(actor());
	CharGen::NewActor(player, save_game);
	save_game->parties[0].members[0] = player;
	for (uint8_t i = 0; i < save_game->parties[0].count; ++i)
		save_game->parties[0].members[i]->playable = true;

	// Automatically save save01.fag for now.
	save_game->RenameFile("save01");
	save_game->Write();

	// Game loop.
	MainLoop(save_game, player);
}
void ContinueGame() {
	xbfq *save_game = new xbfq("Save01");
	actor *player = &save_game->actors[save_game->actors.Count() - 1];

	// Opening screen.
	NewScreen();
	std::cout << "\n\n\n\n\n\n\n\n\n\n\n"
			  << "             Curtains rise and the world watches. Silence. "
			  << "\n           Your destiny awaits."
			  << "\n\n\n\n\n\n\n\n\n\n\n";
	Pause();

	// Automatically load save01.fag for now.
	for (uint8_t i = 0; i < save_game->parties[0].count; ++i)
		save_game->parties[0].members[i]->playable = true;

	// Game loop.
	MainLoop(save_game, player);
}
void MainLoop(xbfq *save_game, actor *player) {
	int status = 0;
	party enemy;

	while (status == 0) {
		std::srand(std::time(nullptr));
		std::rand();
		enemy = save_game->parties[1 + std::rand()/((RAND_MAX + 1u)/3)];
		for (actor **i = save_game->parties[0].Begin(); i != save_game->parties[0].End(); ++i)
			(**i).Restore();
		for (actor **i = enemy.Begin(); i != enemy.End(); ++i)
			(**i).Restore();
		save_game->Write();
		battle test(&save_game->parties[0], &enemy, save_game);
	}
}