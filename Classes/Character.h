#ifndef __CHARACTER_H__
#define __CHARACTER_H__


#include "cocos2d.h"
#include "Entity.h"
#include "Skill.h"
#include "Bullet.h"
#include "Definition.h"
#include "CommonFuntion.h"
#include "AudioEngine.h"

enum CharAni
{
	AniIdle,
	AniRun,
	AniAttack,
	AniEvade,
	AniDamage,
	AniDead,
	AniUseSkill,
	TotalAni
};

enum CharState
{
	CHAR_IDLE,
	CHAR_RUN,
	CHAR_ATTACK,
	CHAR_DAMAGE,
	CHAR_EVADE,
	CHAR_DEAD,
	CHAR_USE_SKILL,
	TOTAL_STATES
};

class Character : public cocos2d::Sprite, public Entity
{
public:
	static Character* createCharacter();
	virtual bool init();

	virtual void Evade(cocos2d::Vec2& direction);
	virtual int UseSkill(cocos2d::Point& target);
	virtual void Attack(cocos2d::Vec2 target);
	virtual void TakeDame(float dame);
	cocos2d::Sprite* GetInfor();
	cocos2d::Sprite* GetIdle();
	CharState* _state;
	// 
	CREATE_FUNC(Character);

public:
	cocos2d::TMXTiledMap* _map;
	cocos2d::TMXLayer* _meta;
	cocos2d::Node* _grounded;
	Skill* _skill;
protected:
	cocos2d::Animation* animations[CharAni::TotalAni];
	cocos2d::Sprite* _characterSprite;
	cocos2d::PhysicsBody* _body;
	CharAni states;
	Bullet* _bullet;
protected:
	virtual bool loadSpriteAnimation();
	virtual bool loadInformation();

	// animations

	virtual void animator();
	virtual void aniDead();
	virtual void aniIdle();
	virtual void aniDamage();
	virtual void aniRun();
	virtual void aniAttack();
	virtual void aniEvade();
	virtual void aniUseSkill();
	//
	void revival();
	void characterFlip();

	// 
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
};

#endif // !__CHARACTER_H__
