#include <iostream>
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"

Bullet::Bullet() : rect(), lifeTime(0.0f), isActive(false)
{
}

Bullet::Bullet(const SDL_Rect& aRect, const bool aIsActive) : rect(aRect), lifeTime(0.0f), isActive(aIsActive)
{
}

ModulePlayer::ModulePlayer() : bulletIndex(0U), mLaserSound(0)
{
	mPlayer.x = 0;
	mPlayer.y = 0;
	mPlayer.w = 30;
	mPlayer.h = 30;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;

	//todo 5: Load the laser .wav file.
	mLaserSound = App->audio->LoadFx("assets/laser.wav");

	//todo 6: Start playing any background music in .ogg format.
	App->audio->PlayMusic("assets/stage1.ogg");


	return ret;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	int speed = 1;
	float lifeIncrement = 0.016;

	if(App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT)
	{
		mPlayer.x += speed;
	}
	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT)
	{
		mPlayer.x -= speed;
	}
	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT)
	{
		mPlayer.y -= speed;
	}
	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT)
	{
		mPlayer.y += speed;
	}

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		CreateBullet();
	}

	for (size_t i = 0; i < MAX_BULLET_POOL; i++)
	{
		Bullet& cBullet = bulletPool[i];
		if (cBullet.isActive == true)
		{
			cBullet.lifeTime += lifeIncrement;
			cBullet.rect.x += 5;
		}

		if (cBullet.lifeTime >= BULLET_LIFETIME_SECONDS)
		{
			RestartBullet(cBullet);
		}
	}
	
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
	for (size_t i = 0; i < MAX_BULLET_POOL; i++)
	{
		Bullet& cBullet = bulletPool[i];
		if (cBullet.isActive == true)
		{
			App->render->BlitQuad(cBullet.rect);
		}
	}

	App->render->BlitQuad(mPlayer);

	return UPDATE_CONTINUE;
}

bool ModulePlayer::CleanUp()
{
	//todo 8: Unload the laser sound.
	App->audio->UnLoadFx(mLaserSound);
	return true;
}

void ModulePlayer::CreateBullet()
{
  	Bullet& cBullet = bulletPool[bulletIndex];
	if (cBullet.isActive == true)
	{
		return;
	}

	cBullet.isActive = true;
	cBullet.lifeTime = 0.0;
	cBullet.rect = mPlayer;

	cBullet.rect.x += mPlayer.w/2;
	cBullet.rect.y += mPlayer.h/2;

	cBullet.rect.w = 5;
	cBullet.rect.h = 2;

	//todo 7: Play the laser sound.
	App->audio->PlayFx(mLaserSound);

	if (bulletIndex >= MAX_BULLET_POOL-1)
	{
		bulletIndex = 0;
	}
	else
	{
		++bulletIndex;
	}
}

void ModulePlayer::RestartBullet(Bullet& aBullet)
{
	aBullet.isActive = false;
	aBullet.lifeTime = 0.0;
}
