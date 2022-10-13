#include "Skill.h"

USING_NS_CC;

Skill* Skill::createSkill()
{
	return Skill::create();
}

bool Skill::init()
{
	if (!Sprite::init()) { return false; }

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprites/Effect/noraUltimate.plist", "sprites/Effect/noraUltimate.png");
	int numberFrame = 50;
	_isLocked = false;
	_cooldownTime = 5.0f;
	_cooldown = 0.0f;
	_executionTime = 2.5f;
	_operating_range = 450;
	_avatar = Sprite::create("sprites/Skill/skill_01.png");
	float duration = _executionTime / (float)numberFrame;
	_animation = CommonFuntion::createAnimation("nora-ultimate", numberFrame, duration);
	_animation->retain();
	_skillSprite = Sprite::createWithSpriteFrameName("nora-ultimate (1).png");
	_skillSprite->retain();
	_skillSprite->setScale(130 / _skillSprite->getContentSize().width);

	_sizeSkill = Size(130, 130);
	this->scheduleUpdate();
	return true;
}

void Skill::update(float dt)
{
	if (_cooldown > 0) { _cooldown -= dt; }
}

Skill* Skill::clone()
{
	return NULL;
}

int Skill::Trigger(Vec2 pos)
{
	if (_isLocked) { return STATUS_SKILL::LOCKED; }
	if (_cooldown > 0) { return STATUS_SKILL::NO_COOLDOWN_TIME; }

	this->setPosition(pos);
	_cooldown = _cooldownTime;
	auto delay1 = DelayTime::create(1.0f);
	auto delay2 = DelayTime::create(0.75f);
	auto delay3 = DelayTime::create(0.75f);

	this->addChild(_skillSprite);

	static int id1, id2;

	auto parse1 = CallFunc::create([=]() {
		id1 = AudioEngine::play2d("sound/skill01p2.mp3", false, 1);
		});
	auto parse2 = CallFunc::create([=]() {
		//AudioEngine::pause(id1);
		id2 = AudioEngine::play2d("sound/skill01p1.mp3", false, 1);
		auto body = PhysicsBody::createCircle(_sizeSkill.width / 2, PhysicsMaterial(0, 0, 0), Vec2::ZERO);
		body->setContactTestBitmask(true);
		body->setCollisionBitmask(CHAR_SKILL);
		//body->setCategoryBitmask(CHAR_SKILL);
		body->setDynamic(false);
		this->setPhysicsBody(body);
		});

	auto parse3 = CallFunc::create([=]() {
		this->getPhysicsBody()->removeFromWorld();
		});

	auto end = CallFunc::create([=]() {
		this->removeChild(_skillSprite, true);
		});


	_skillSprite->runAction(Animate::create(_animation->clone()));
	this->runAction(
		Sequence::create(parse1, delay1, parse2, delay2, parse3, delay3, end, NULL));

	return STATUS_SKILL::SUCCESSFUL;
}
