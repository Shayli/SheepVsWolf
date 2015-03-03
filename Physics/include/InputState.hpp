#pragma once

#include <SDL2/SDL.h>
#include <map>

#include "WindowManager.hpp"

struct KeyState {
	bool pressed;
	KeyState() : pressed(false) {}
};

class InputState{
protected:
	glm::vec2 mousePosition;
	std::map<int, KeyState> keyboardState;
	WindowManager* window;

	InputState() = default;
	~InputState() = default;

public:
	bool isKeyPressed(int key){
		return keyboardState[key].pressed;
	}

	glm::vec2 getMouseDelta(){
		int x, y; 
		SDL_GetMouseState(&x,&y);
		return glm::vec2(x,y)-mousePosition;
	}

	void updateKey(int key, bool state) {
		keyboardState[key].pressed = state;
	}

	void update(){
		int x, y;
		if(window)
			window->lockMouse(true);
		SDL_GetMouseState(&x,&y);
		mousePosition = glm::vec2(x,y);
	}

	static InputState& get(){
		static InputState s;
		return s;
	}

	void setWindow(WindowManager* w){
		window = w;
	}
};