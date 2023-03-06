#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Globals.h"

struct SDL_Texture;

#define MAX_BULLET_POOL 8
#define BULLET_LIFETIME_SECONDS 3

struct Bullet
{
	Bullet();
	Bullet(const SDL_Rect& aRect, const bool aIsActive);

	SDL_Rect rect;
	float lifeTime;
	bool isActive;
};

class ModulePlayer : public Module
{
	public:
		ModulePlayer();
		~ModulePlayer();

		bool Start() override;
		update_status Update() override;
		update_status PostUpdate() override;
		bool CleanUp();

		void CreateBullet();
		void RestartBullet(Bullet& aBullet);

	public:
		SDL_Rect		mPlayer;
		Bullet			bulletPool[MAX_BULLET_POOL];
		unsigned int	bulletIndex;
		
		//todo 4: Store the laser FBX index.
		uint			mLaserSound;
};

#endif