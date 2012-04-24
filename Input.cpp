#include "Input.h"

bool Input::poll(Player &p, Sound &s, Gamestate &gs, Level &l) {
	l.shiftX(shiftingLevel.x);
	l.shiftY(shiftingLevel.y);
	l.shiftZ(shiftingLevel.z);
	
	SDL_Event event;
	while ( SDL_PollEvent(&event) ) {
		if ( event.type == SDL_QUIT ) {
 			return false;
		}
		
		// mouse click
		if ( event.type == SDL_MOUSEBUTTONDOWN ) {
			if(gs.getOperationState() != WON) {
				if(!p.isMoving()) {
					p.setMovement(Coord3d(1,0,0));
					gs.setOperationState(PLAY);
				} else {
					p.shoot(s);
				}
			}
		}
		
		// release key
		if ( event.type == SDL_KEYUP ) {
			switch(event.key.keysym.sym) {
				case SDLK_w:
				case SDLK_s:
				case SDLK_UP:
				case SDLK_DOWN:
					shiftingLevel.z = 0;
					break;
				case SDLK_a:
				case SDLK_d:
				case SDLK_LEFT:
				case SDLK_RIGHT:
					shiftingLevel.x = 0;
					break;
				default:
					break;
			}
		}

		// key press
		if ( event.type == SDL_KEYDOWN ) {
			switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					return false;
					break;
				case SDLK_w:
					shiftingLevel.z = 1;
					break;
				case SDLK_s:
					shiftingLevel.z = -1; 
					break;
				case SDLK_a:
					shiftingLevel.x = 1;
					break;
				case SDLK_d:
					shiftingLevel.x = -1;
					break;
				case SDLK_r:
					shiftingLevel.x = 0;
					shiftingLevel.y = 0;
					shiftingLevel.z = 0;
					l.resetShift();
					break;
#ifdef Debug					
				case SDLK_DOWN:
					p.setMovement(Coord3d(0,0,-1));
					break;
				case SDLK_UP:
					p.setMovement(Coord3d(0,0,1));
					break;
				case SDLK_LEFT:
					p.setMovement(Coord3d(-1,0,0));
					break;
				case SDLK_RIGHT:
					p.setMovement(Coord3d(1,0,0));
					break;
				case SDLK_RETURN:
					if(gs.getOperationState() != WON) {
						if(!p.isMoving()) {
							p.setMovement(Coord3d(1,0,0));
							gs.setOperationState(PLAY);
						} else {
							p.setMovement(Coord3d(0,0,0));
						}
					}
					break;
#else
				case SDLK_UP:
					shiftingLevel.z = 1;
					break;
				case SDLK_DOWN:
					shiftingLevel.z = -1; 
					break;
				case SDLK_LEFT:
					shiftingLevel.x = 1;
					break;
				case SDLK_RIGHT:
					shiftingLevel.x = -1;
					break;
#endif
				case SDLK_SPACE:
					if(gs.getOperationState() != WON) {
						if(!p.isMoving()) {
							p.setMovement(Coord3d(1,0,0));
							gs.setOperationState(PLAY);
						} else {
							p.shoot(s);
						}
					}
					break;
				default:
					if(gs.getOperationState() != WON) {
						if(!p.isMoving()) {
							p.setMovement(Coord3d(1,0,0));
							gs.setOperationState(PLAY);
						}
					}
					break;
			}
		}
		if( event.type == SDL_MOUSEMOTION ) {
			p.setPitch(p.getPitch() + (double)event.motion.yrel);	
			p.setYaw(p.getYaw() + (double)event.motion.xrel);
		}
		SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
		SDL_WarpMouse(320, 240);
		SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
	}
	return true;
}

Input::Input() { }

Input::~Input() { }
