#include "Character.h"

USING_NS_CC;

Character* Character::createCharacter()
{
	return Character::create();
}

void message(const std::string message)
{
	log(message.c_str());
}

bool Character::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	if (!loadSpriteAnimation())
	{
		message("Sprite animation loading error!");
		return false;
	}

	if (!loadInformation())
	{
		message("Information loading error!");
		return false;
	}

	_skill = Skill::createSkill();

	_grounded = Node::create();
	_grounded->setPosition(0, -20);
	this->addChild(_grounded);
	states = CharAni::AniIdle;
	this->addChild(_characterSprite);
	this->scheduleUpdate();
	this->schedule(SEL_SCHEDULE(&Character::fixedUpdate), FIXED_UPDATE_TIME);
	return true;
}

bool Character::loadSpriteAnimation()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_CHAR1("idle"), (std::string)PATH_IMAGE_CHAR1("idle"));
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_CHAR1("damage"), (std::string)PATH_IMAGE_CHAR1("damage"));
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_CHAR1("run"), (std::string)PATH_IMAGE_CHAR1("run"));
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprites/Characters/Char_1/shot.plist", "sprites/Characters/Char_1/shot.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprites/Characters/Char_1/jump.plist", "sprites/Characters/Char_1/jump.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprites/Characters/Char_1/stabbing.plist", "sprites/Characters/Char_1/stabbing.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprites/Characters/Char_2/sick.plist", "sprites/Characters/Char_2/sick.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprites/Characters/Char_2/item.plist", "sprites/Characters/Char_2/item.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_CHAR1("skill"), (std::string)PATH_IMAGE_CHAR1("skill"));
	_characterSprite = Sprite::createWithSpriteFrameName("run (1).png");

	return true;
}

bool Character::loadInformation()
{
	if (_characterSprite == NULL)
	{
		message("Can't load texture, because character is null!");
		return false;
	}



	_state = new CharState(CharState::CHAR_IDLE);
	_entityName = "nora";
	_max_hp = new float(1000);
	_hp = new float(1000);
	_speed = new int(200);
	_attack_speed = new int(100);
	_dame = new int(100);
	_def = new int(100);

	_characterSprite->setScale((CHAR_NORMAL_SIZE + 20) / _characterSprite->getContentSize().width);
	this->setPosition(Vec2::ZERO);
	_body = PhysicsBody::createEdgeBox(Size(CHAR_NORMAL_SIZE - 20, CHAR_NORMAL_SIZE - 20),
		PhysicsMaterial(1, 0, 1), EDGE_BOX_SIZE);
	_body->setRotationEnable(false);
	_body->setMass(45);
	_body->setCollisionBitmask(PLAYER);
	_body->setContactTestBitmask(true);


	this->setPhysicsBody(_body);

	return true;
}

void Character::update(float dt)
{
	animator();
}

void Character::fixedUpdate(float dt)
{
	if (*_hp <= 0)
	{
		*_state = CharState::CHAR_DEAD;
		return;
	}
	else if (*_state == CharState::CHAR_USE_SKILL)
	{
		_body->setVelocity(Vec2::ZERO);
	}
	else if (*_state != CharState::CHAR_ATTACK)
	{
		if (_body->getVelocity() != Vec2::ZERO)
			*_state = CharState::CHAR_RUN;
		else
			*_state = CharState::CHAR_IDLE;
		characterFlip();
	}
	else
	{
		_body->setVelocity(Vec2::ZERO);
	}
}

void Character::characterFlip()
{
	if (_body->getVelocity() != Vec2::ZERO && _body->getVelocity().x != 0)
	{
		_characterSprite->setFlippedX(_body->getVelocity().x > 0);
	}
}

// action

void Character::Attack(Vec2 target)
{
	if (*_state == CharState::CHAR_ATTACK || *_state == CharState::CHAR_DAMAGE)
		return;
	*_state = CharState::CHAR_ATTACK;
	Vec2 direction = target - this->getPosition();
	direction.normalize();

	if (direction.x != 0)
		_characterSprite->setFlippedX((direction.x > 0) ? (true) : (false));

	AudioEngine::play2d("sound/attack.mp3", false, 1);
	CallFunc* initBullet = CallFunc::create([=]()
		{
			auto bullet = Bullet::createBullet(direction);
			bullet->_map = this->_map;
			bullet->_meta = this->_meta;
			bullet->setPosition(this->getPosition() + direction * (CHAR_NORMAL_SIZE / 2));
			bullet->setDame(*_dame);
			bullet->_originPos = this->getPosition();

			this->getParent()->addChild(bullet, 4);
		});

	DelayTime* delay = DelayTime::create(0.5f);
	CallFunc* unlockAttack = CallFunc::create([=]() { *_state = CharState::CHAR_IDLE; });

	Sequence* attack = Sequence::create(delay, initBullet, unlockAttack, NULL);
	this->runAction(attack);
}

void Character::TakeDame(float dame)
{
	if (*_state == CharState::CHAR_DEAD)
	{
		return;
	}
	if (*_state == CharState::CHAR_USE_SKILL)
	{
		dame = dame / 2;
	}
	else
	{
		*_state = CharState::CHAR_DAMAGE;
	}

	*_hp -= dame;
	auto showDame = Label::createWithTTF(CommonFuntion::labelConfig, std::to_string((int)dame));
	showDame->setTextColor(Color4B::WHITE);

	auto flyUp = MoveTo::create(2, this->getPosition() + Vec2(0, 20));
	auto fading = FadeOut::create(2);
	auto effect = Spawn::create(flyUp, fading, NULL);
	showDame->setPosition(this->getPosition());
	showDame->runAction(effect);
	this->getParent()->addChild(showDame, 15);

	if (*_hp <= 0)
	{
		*_state = CharState::CHAR_DEAD;
	}

	this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]() {
		*_state = CharState::CHAR_IDLE;
		}), NULL));
}

void Character::animator()
{
	switch (*_state)
	{
	case CHAR_RUN:
		aniRun();
		break;
	case CHAR_ATTACK:
		aniAttack();
		break;
	case CHAR_EVADE:
		aniEvade();
		break;
	case CHAR_DAMAGE:
		aniDamage();
		break;
	case CHAR_DEAD:
		aniDead();
		break;
	case CHAR_USE_SKILL:
		aniUseSkill();
		break;
	default:
		aniIdle();
		break;
	}
}

void Character::revival()
{
	auto objGr = _map->getObjectGroup("teleport");

	if (objGr == NULL)
	{
		log("Can't get object name %s", "revival");
		return;
	}

	ValueMap object = objGr->getObject("revival");

	Point result;
	result.x = object["x"].asFloat();
	result.y = object["y"].asFloat();
	*_hp = *_max_hp;
	this->setPosition(result);
}

void Character::aniDead()
{
	if (_characterSprite->getActionByTag(CharAni::AniDead))
		return;

	_characterSprite->stopAllActions();
	if (animations[CharAni::AniDead] == NULL)
	{
		int numberFrame = 20;
		float duration = 0.05f;
		animations[CharAni::AniDead] = CommonFuntion::createAnimation("sick", numberFrame, duration);
		animations[CharAni::AniDead]->retain();
	}

	auto dead = Animate::create(animations[CharAni::AniDead]->clone());

	auto callRevival = CallFunc::create([=]()
		{
			*_state = CharState::CHAR_IDLE;
			revival();
		}
	);

	Rect rect;
	rect.origin = this->getPosition();
	rect.size = VISIBLE_SIZE;
	auto node = CommonFuntion::DrawRect(rect, Color4F::BLACK);
	node->setOpacity(0);

	auto action = Sequence::create(dead, callRevival, NULL);
	action->setTag(CharAni::AniDead);

	_characterSprite->runAction(action);
}

void Character::aniUseSkill()
{
	if (_characterSprite->getActionByTag(CharAni::AniUseSkill))
		return;

	_characterSprite->stopAllActions();
	if (animations[CharAni::AniUseSkill] == NULL)
	{
		int numberFrame = 15;
		float duration = 0.067f;
		animations[CharAni::AniUseSkill] = CommonFuntion::createAnimation("item", numberFrame, duration);
		animations[CharAni::AniUseSkill]->retain();
	}

	auto useSkill = Animate::create(animations[CharAni::AniUseSkill]->clone());
	useSkill->setTag(CharAni::AniUseSkill);

	_characterSprite->runAction(useSkill);
}

void Character::aniIdle()
{
	if (_characterSprite->getActionByTag(CharAni::AniIdle))
		return;

	_characterSprite->stopAllActions();
	if (animations[CharAni::AniIdle] == NULL)
	{
		int numberFrame = 14;
		float duration = 0.05f;
		animations[CharAni::AniIdle] = CommonFuntion::createAnimation("idle", numberFrame, duration);
		animations[CharAni::AniIdle]->retain();
	}

	auto idle = Animate::create(animations[CharAni::AniIdle]->clone());
	idle->setTag(CharAni::AniIdle);

	_characterSprite->runAction(idle);
}

void Character::aniRun()
{
	if (_characterSprite->getActionByTag(CharAni::AniRun) != NULL)
		return;
	_characterSprite->stopAllActions();

	float duration = 0.05f;
	if (animations[CharAni::AniRun] == NULL)
	{
		int numberFrame = 15;
		animations[CharAni::AniRun] = CommonFuntion::createAnimation("run", numberFrame, duration);
		animations[CharAni::AniRun]->retain();
	}

	auto run = Animate::create(animations[CharAni::AniRun]->clone());
	run->setTag(CharAni::AniRun);

	_characterSprite->runAction(run);
}

void Character::aniDamage()
{
	if (_characterSprite->getActionByTag(CharAni::AniDamage) != NULL)
	{
		return;
	}

	_characterSprite->stopAllActions();

	int numberFrame = 2;
	//float duration = (10.0f / (*_attack_speed)) / (float)numberFrame;
	float duration = 0.05f;
	if (animations[CharAni::AniDamage] == NULL)
	{
		animations[CharAni::AniDamage] = CommonFuntion::createAnimation("skill", numberFrame, duration);
		animations[CharAni::AniDamage]->retain();
	}

	auto damage = Animate::create(animations[CharAni::AniDamage]->clone());
	damage->setTag(CharAni::AniDamage);

	_characterSprite->runAction(damage);
}

void Character::aniAttack()
{
	if (_characterSprite->getActionByTag(CharAni::AniAttack) != NULL)
	{
		return;
	}

	_characterSprite->stopAllActions();

	int numberFrame = 18;
	//float duration = (10.0f / (*_attack_speed)) / (float)numberFrame;
	float duration = 0.05f;
	if (animations[CharAni::AniAttack] == NULL)
	{
		animations[CharAni::AniAttack] = CommonFuntion::createAnimation("skill", numberFrame, duration);
		animations[CharAni::AniAttack]->retain();
	}

	auto attack = Animate::create(animations[CharAni::AniAttack]->clone());
	attack->setTag(CharAni::AniAttack);

	_characterSprite->runAction(attack);

}

void Character::aniEvade()
{

}

Sprite* Character::GetInfor()
{
	Sprite* tableDetails = Sprite::create("sprites/GameUI/Info/table-details.png");
	tableDetails->setScaleX(400 / tableDetails->getContentSize().width);
	tableDetails->setScaleY(500 / tableDetails->getContentSize().height);

	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/arial.ttf";
	labelConfig.fontSize = 40;

	std::string cName = "Name           : " + _entityName + "\n";
	std::string cHp = "HP             : " + std::to_string((int)*_max_hp) + "\n";
	std::string cAtk = "Attack         : " + std::to_string((int)*_dame) + "\n";
	std::string cAtackSpeed = "Attack Speed   : " + std::to_string((int)*_attack_speed) + "\n";
	std::string cSpeed = "Speed          : " + std::to_string((int)*_attack_speed) + "\n";
	std::string cDef = "Def            : " + std::to_string((int)*_def) + "\n";

	std::string text = cName + cHp + cAtk + cAtackSpeed + cSpeed + cDef;
	Label* textLabel = Label::createWithTTF(labelConfig, text);
	textLabel->setPosition(Vec2(220, 250));

	textLabel->setColor(Color3B::WHITE);

	tableDetails->addChild(textLabel);

	return tableDetails;
}

Sprite* Character::GetIdle()
{
	aniIdle();
	auto idle = Sprite::createWithSpriteFrameName("idle (1).png");
	idle->runAction(RepeatForever::create(Animate::create((animations[CharAni::AniIdle]))));
	return idle;
}

void Character::Evade(cocos2d::Vec2& direction)
{
	if (*_state == CharState::CHAR_EVADE)
		return;
}

int Character::UseSkill(cocos2d::Point& target)
{
	int result = _skill->Trigger(target);
	if (result == STATUS_SKILL::SUCCESSFUL)
	{
		AudioEngine::play2d("sound/useSkill01.mp3", false, 1);
		*_state = CharState::CHAR_USE_SKILL;
		_characterSprite->setFlippedX((target - this->getPosition()).x > 0);
		auto unlock = CallFunc::create([=]() {*_state = CharState::CHAR_IDLE; });
		auto action = Sequence::create(DelayTime::create(1.0f), unlock, NULL);
		this->runAction(action);
	}
	return result;
}
