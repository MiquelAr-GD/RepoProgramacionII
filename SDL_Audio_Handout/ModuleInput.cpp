#include <iostream>
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

ModuleInput::ModuleInput() : Module()
{
	memset(keyboard, static_cast<int>(KEY_STATE::KEY_IDLE), sizeof(KEY_STATE) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate()
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		ProcessKeyState(keyboard[i], bool(keys[i] == 1));
	}

	if(keyboard[SDL_SCANCODE_ESCAPE] == KEY_STATE::KEY_DOWN)
		return update_status::UPDATE_STOP;

	return update_status::UPDATE_CONTINUE;
}

void ModuleInput::ProcessKeyState(KEY_STATE& aKey, const bool aIsPressed)
{
	if (aIsPressed)
	{
		if (aKey == KEY_STATE::KEY_IDLE)
		{
			aKey = KEY_STATE::KEY_DOWN;
		}
		else
		{
			aKey = KEY_STATE::KEY_REPEAT;
		}
	}
	else
	{
		if (aKey == KEY_STATE::KEY_REPEAT || aKey == KEY_STATE::KEY_DOWN)
		{
			aKey = KEY_STATE::KEY_UP;
		}
		else
		{
			aKey = KEY_STATE::KEY_IDLE;
		}
	}
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}