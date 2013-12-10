/*
 * Project: zikzak
 * Filename: Main.cpp
 * Author: Andreas Textor
 * Version: 0.1
 * Date: 2006-06-17
 * Contents:
 *   Parse the commandline, instantiate all systems (graphics, sound, input
 *   and so on) and start the main game loop.
 */

#include <iostream>
#include <cstring>
#include "Graphics.h"
#include "Input.h"
#include "Level.h"
#include "Player.h"
#include "Gamestate.h"
#include "Sound.h"
#include "Config.h"
#include "Coord3d.h"
using std::cout;

void displayHelp() {
	cout	<< "Zik Zak - v0.1 - Andreas Textor\n"
			<< "Usage: zikzak [-h|--help][-f][-x number][-y number][-nm]\n\n"
			<< "Options:\n"
			<< "\t-h  --help  - Display this information\n"
			<< "\t-f          - Run the game in fullscreen (default: off)\n"
			<< "\t-x number   - Set the horizontal resolution (default: 640)\n"
			<< "\t-y number   - Set the vertical resolution (default: 480)\n"
			<< "\t-nm         - Don't play music\n";
}

int main(int argc, char* argv[]) {
	bool fullscreen = false;
	bool music = true;
	int xres = 800;
	int yres = 600;
	int lastlevel = 1;

	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

	// load config first, to let commandline arguments override
	// the config settings
	Config c;
	c.load(music, fullscreen, xres, yres, lastlevel);

	// check commandline
	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-f") == 0)
			fullscreen = true;
		if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			displayHelp();
			return 0;
		}
		if(strcmp(argv[i], "-x") == 0) {
			if(argc-1 > i)
				xres = atoi(argv[i+1]);
			else {
				displayHelp();
				return 0;
			}
		}
		if(strcmp(argv[i], "-y") == 0) {
			if(argc-1 > i)
				yres = atoi(argv[i+1]);
			else {
				displayHelp();
				return 0;
			}
		}
		if(strcmp(argv[i], "-nm") == 0)
			music = false;
	}

	// run game
	bool running = true;

	Graphics g;
	Sound s;
	Input i;
	Level l;
	Gamestate gs;

	int status = g.init(xres, yres, fullscreen);
	if(status != 0)
		return status;

	Player player1(1.0, -1.0, 1.0, 0.0, 60.0);

	if(l.load(0) != 0) {
		return 1;
	}

	status = s.init(music);
	if(music)
		s.playMusic();

	gs.resetTimer();
	gs.setOperationState(TITLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);

	while(running) {
		g.renderscene(l, player1, s, gs);
		if(i.poll(player1, s, gs, l)) {
			// advance one level
			if(!player1.move(l, s)) {
				c.save(music, fullscreen, xres, yres, lastlevel);
				if(l.load(lastlevel) == 0) {
					lastlevel++;
					player1.setX(1.0);
					player1.setY(-1.0);
					player1.setZ(1.0);
					player1.setMovement(Coord3d(0,0,0));
					player1.clearBullets();
					gs.setOperationState(PLAY);
				} else {
					// no more levels found - that means
					// we have won \o/
					player1.setX(1.0);
					player1.setY(-1.0);
					player1.setZ(1.0);
					player1.setMovement(Coord3d(0,0,0));
					player1.clearBullets();
					gs.setOperationState(WON);
				}
			}
		} else {
			running = false;
		}

		if(l.getLevelNumber() == 0 || !player1.isMoving()) {
			gs.resetTimer();
		} else {
			// game over
			if(gs.getTimeLeft(l) <= 0) {
				l.load(lastlevel-1);
				player1.setX(1.0);
				player1.setY(-1.0);
				player1.setZ(1.0);
				player1.setMovement(Coord3d(0,0,0));
				player1.clearBullets();
				gs.setOperationState(GAMEOVER);
			}
		}
		gs.loopDelay();
	}

	//c.save(music, fullscreen, xres, yres, lastlevel-1);
	s.cleanUp();
	g.cleanUp();

	return 0;
}
