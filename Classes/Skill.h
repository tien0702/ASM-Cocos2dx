#ifndef __SKILL_H__
#define __SKILL_H__

#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "CommonFuntion.h"
#include "Definition.h"
#include "AudioEngine.h"

enum STATUS_SKILL
{
	NO_COOLDOWN_TIME = 0,
	LOCKED = 1,
	SUCCESSFUL = 2
};

class Skill : public cocos2d::Sprite
{
public:
	static Skill* createSkill();
	virtual bool init();
	Skill* clone();
	bool onCooldown() { return _cooldown > 0; }
	void setCooldownTime(float cooldownTime) { _cooldownTime = cooldownTime; }
	void setCooldown(float cooldown) { _cooldown = cooldown; }
	float getCooldown() { return _cooldownTime; }
	float getCooldownTime() { return _cooldownTime; }
	float getPercentCooldown() { return (_cooldown / _cooldownTime) * 100; }

	float getDame(float baseDame) { return baseDame += baseDame * 2; }

	int Trigger(cocos2d::Vec2 pos);

	float _executionTime;
	float _operating_range;
	cocos2d::Size _sizeSkill;

	CREATE_FUNC(Skill);
public:
	cocos2d::Sprite* _avatar;
protected:
	virtual void update(float dt);
	bool _isLocked;
	float _cooldownTime;
	float _cooldown;

	cocos2d::Sprite* _skillSprite;
	cocos2d::Animation* _animation;
};

#endif // !__SKILL_H__
