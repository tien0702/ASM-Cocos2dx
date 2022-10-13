#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CommonFuntion.h"
#include "Definition.h"
//#include "GameDirector.h"
#include <time.h>  
#include "Movement.h"

#define PATH_PLIST_SLIME(name) "sprites/Enemies/Slime/" + name + ".plist"
#define PATH_IMAGE_SMILE(name) "sprites/Enemies/Slime/" + name + ".png"

enum SlimeAni
{
	SLIME_ANI_IDLE,
	SLIME_ANI_WALK,
	SLIME_ANI_RUN,
	SLIME_ANI_ATTACK,
	SLIME_ANI_DAMAGE,
	SLIME_ANI_DEAD,
	TOTAL_SLIME_ANI
};

enum SlimeState
{
	SLIME_IDLE,
	SLIME_WALK,
	SLIME_RUN,
	SLIME_BACK_ORIGIN_POS,
	SLIME_CHASE,
	SLIME_ATTACK,
	SLIME_DAMAGE,
	SLIME_DEAD,
	TOTAL_SLIME_STATE
};

class Enemy : public cocos2d::Sprite
{
public:
	static Enemy* createEnemy();
	virtual bool init();

	//void Attack();
	void TakeDamage(float dame);
	void Attack();
	bool IsDead() { return (_state == SlimeState::SLIME_DEAD); };

	virtual void setOriginPos(cocos2d::Point pos) { _originPos = pos; };

	CREATE_FUNC(Enemy);
public:
	const cocos2d::Point* _target;
	
protected:
	int* _level;
	float* _max_hp;
	float* _hp;
	float* _dame;
	float* _speed;
	float* _speed_attack;
	float* _def;

	float* _attack_range;
	float* _move_range;
	float* _eyesight;

	SlimeState _state;
	SlimeAni _ani;

	cocos2d::Animation* animations[SlimeAni::TOTAL_SLIME_ANI];
	cocos2d::Sprite* _enemySprite;
	cocos2d::ui::LoadingBar* _hpBar;
	cocos2d::PhysicsBody* _body;

protected:
	virtual bool loadSpriteAni();
	virtual bool loadInfor();
	virtual bool loadUI();

	virtual void animator();
	virtual void aniIdle();
	virtual void aniWalk();
	virtual void aniRun();
	virtual void aniAttack();
	virtual void aniDamage();

	// bihavior
	virtual cocos2d::Vec2 changeDirection();
	virtual SlimeState getRandomStatus(float dt);

	void update(float dt);
	void fixedUpdate(float dt);
private:
	cocos2d::Point _originPos;
};

#endif // !__ENEMY_H__
