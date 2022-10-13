#include "Enemy.h"

USING_NS_CC;

Enemy* Enemy::createEnemy()
{
	return Enemy::create();
}

bool Enemy::init()
{
	if (!Sprite::init()) { return false; }
	if (!loadSpriteAni()) { return false; }
	if (!loadInfor()) { return false; }
	if (!loadUI()) { return false; }

	_ani = SlimeAni::SLIME_ANI_ATTACK;
	srand(time(NULL));
	_originPos.x = this->getPosition().x;
	_originPos.y = this->getPosition().y;
	this->addChild(_hpBar);
	this->addChild(_enemySprite);
	this->scheduleUpdate();
	this->schedule(SEL_SCHEDULE(&Enemy::fixedUpdate), FIXED_UPDATE_TIME);
	return true;
}

bool Enemy::loadSpriteAni()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_SLIME("idle"), (std::string)PATH_IMAGE_SMILE("idle"));
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_SLIME("walk"), (std::string)PATH_IMAGE_SMILE("walk"));
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_SLIME("jump"), (std::string)PATH_IMAGE_SMILE("jump"));
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_SLIME("swinging"), (std::string)PATH_IMAGE_SMILE("swinging"));
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_SLIME("damage"), (std::string)PATH_IMAGE_SMILE("damage"));

	return true;
}

bool Enemy::loadInfor()
{
	_level = new int(100);
	_max_hp = new float(500);
	_hp = new float(500);
	_dame = new float(500);
	_speed = new float((rand() % 50 + 50));
	_speed_attack = new float(500);
	_def = new float(500);
	_attack_range = new float(20);
	_eyesight = new float((rand() % 250 + 200));
	_move_range = new float (400);

	_body = PhysicsBody::createEdgeBox(Size(SIZE_SLIME, SIZE_SLIME), PhysicsMaterial(1, 0, 1), 1);

	_body->setContactTestBitmask(1);
	_body->setCollisionBitmask(ENEMY);
	_body->setCategoryBitmask(1);

	this->setPhysicsBody(_body);
	return true;
}

bool Enemy::loadUI()
{
	{
		_hpBar = ui::LoadingBar::create("sprites/GameUI/hp-bar.png");
		if (_hpBar == NULL)
		{
			log("HP bar is null!");
			return false;
		}
		_hpBar->setDirection(ui::LoadingBar::Direction::LEFT);
		_hpBar->setScale(200 / _hpBar->getContentSize().width);
		_hpBar->setPosition(Vec2(0, SIZE_SLIME / 2 + 5));
		_hpBar->setPercent(((*_hp) / (*_max_hp)) * 100);
	}

	{
		_enemySprite = Sprite::createWithSpriteFrameName("slime-idle (1).png");
		if (_enemySprite == NULL)
		{
			log("HP bar is null!");
			return false;
		}
		_enemySprite->setScale(100 / _enemySprite->getContentSize().width);
		_enemySprite->setPosition(0, 20);
	}

	return true;
}

void Enemy::update(float dt)
{
	animator();
}

void Enemy::fixedUpdate(float dt)
{
	if (*_hp <= 0)
	{
		// dead
		_state = SlimeState::SLIME_DEAD;
	}
	else if (_state == SlimeState::SLIME_DAMAGE)
	{
		_body->setVelocity(Vec2::ZERO);
		return;
	}
	else if (_target->distance(this->getPosition()) <= *_attack_range)
	{
		_body->setVelocity(Vec2::ZERO);
		Attack();
	}
	else if ( (_target->distance(this->getPosition()) + _originPos.distance(this->getPosition()) ) <= *_eyesight)
	{
		Vec2 direction = *_target - this->getPosition();
		direction.normalize();
		_body->setVelocity(direction * (*_speed));

		_state = SlimeState::SLIME_CHASE;
	}
	else if (this->getPosition().distance(_originPos) >= 10)
	{
		Vec2 dir = _originPos - this->getPosition();
		dir.normalize();
		_body->setVelocity(dir * (*_speed));
		_state = SlimeState::SLIME_BACK_ORIGIN_POS;
	}
	else
	{
		_body->setVelocity(Vec2::ZERO);
		_state = SlimeState::SLIME_IDLE;
	}

	int xDir = _body->getVelocity().x;
	if (xDir != 0) { _enemySprite->setFlippedX((xDir > 0) ? (true) : false); }
}

Vec2 Enemy::changeDirection()
{
	int horizontal = rand() % 101;
	int vertical = rand() % 101;

	return Vec2(horizontal, vertical);
}

void Enemy::TakeDamage(float dame)
{
	if (_state == SlimeState::SLIME_DEAD ||_state == SlimeState::SLIME_DAMAGE || *_hp <= 0)
		return;
	log("s");
	_state = SlimeState::SLIME_DAMAGE;
	*_hp -= dame;
	if ((*_hp < 0)) *_hp = 0;
	auto showDame = Label::createWithTTF(CommonFuntion::labelConfig, std::to_string((int)dame));
	showDame->setTextColor(Color4B::RED);

	auto flyUp = MoveTo::create(0.5f, this->getPosition() + Vec2(0, 10));
	auto fading = FadeOut::create(0.5f);
	auto effect = Spawn::create(flyUp, fading, NULL);
	showDame->setPosition(this->getPosition());
	showDame->runAction(effect);
	this->getParent()->addChild(showDame, 10);

	_hpBar->setPercent(((*_hp) / (*_max_hp)) * 100);
	if (*_hp <= 0)
	{
		_state = SlimeState::SLIME_DEAD;
		this->getPhysicsBody()->removeFromWorld();
		this->getParent()->removeChild(this, true);
	}
	else
	{
		auto unlock = CallFunc::create([=]() {_state = SlimeState::SLIME_IDLE; });
		this->runAction(Sequence::create(DelayTime::create(0.5f), unlock, NULL));
	}
}

void Enemy::Attack()
{
	if (_state == SlimeState::SLIME_ATTACK)
		return;
	_state = SlimeState::SLIME_ATTACK;
	aniAttack();
	auto node = Node::create();
	node->setPosition(this->getPosition());
	auto body = PhysicsBody::createBox(Size(*_attack_range, *_attack_range), PhysicsMaterial(0, 0, 0));
	body->setContactTestBitmask(true);
	body->setCategoryBitmask(ENEMY_SKILL);
	body->setCollisionBitmask(ENEMY_SKILL);

	node->setPhysicsBody(body);
	this->runAction(Sequence::create(DelayTime::create(2.0f),
		CallFunc::create([=]() {_state = SlimeState::SLIME_IDLE; }), NULL));
	this->getParent()->addChild(node);
}

SlimeState Enemy::getRandomStatus(float dt)
{
	const int size = 2;
	SlimeState status[size];
	status[0] = SlimeState::SLIME_IDLE;
	status[1] = SlimeState::SLIME_WALK;

	return status[rand() % size];
}

void Enemy::animator()
{
	switch (_state)
	{
	case SLIME_WALK:
		aniWalk();
		break;
	case SLIME_CHASE:
		aniRun();
		break;
	case SLIME_ATTACK:
		aniAttack();
		break;
	case SLIME_BACK_ORIGIN_POS:
		aniRun();
		break;
	case SLIME_DAMAGE:
		aniDamage();
		break;
	default:
		aniIdle();
		break;
	}
}

void Enemy::aniIdle()
{
	if (_enemySprite->getActionByTag(SlimeAni::SLIME_ANI_IDLE) != NULL) { return; }
	_enemySprite->stopAllActions();

	if (animations[SlimeAni::SLIME_ANI_IDLE] == NULL)
	{
		int numbeFrame = 14;
		float duration = 0.05f;
		animations[SlimeAni::SLIME_ANI_IDLE] = CommonFuntion::createAnimation("slime-idle", numbeFrame, duration);
		animations[SlimeAni::SLIME_ANI_IDLE]->retain();
	}

	auto idle = Animate::create(animations[SlimeAni::SLIME_ANI_IDLE]->clone());
	idle->setTag(SlimeAni::SLIME_ANI_IDLE);

	_enemySprite->runAction(idle);
}

void Enemy::aniWalk()
{
	if (_enemySprite->getActionByTag(SlimeAni::SLIME_ANI_WALK) != NULL) { return; }
	_enemySprite->stopAllActions();

	if (animations[SlimeAni::SLIME_ANI_WALK] == NULL)
	{
		int numbeFrame = 28;
		float duration = 0.05f;
		animations[SlimeAni::SLIME_ANI_WALK] = CommonFuntion::createAnimation("slime-walk", numbeFrame, duration);
		animations[SlimeAni::SLIME_ANI_WALK]->retain();
	}

	auto walk = Animate::create(animations[SlimeAni::SLIME_ANI_WALK]->clone());
	walk->setTag(SlimeAni::SLIME_ANI_WALK);

	_enemySprite->runAction(walk);
}

void Enemy::aniRun()
{
	if (_enemySprite->getActionByTag(SlimeAni::SLIME_ANI_RUN) != NULL) { return; }
	_enemySprite->stopAllActions();

	if (animations[SlimeAni::SLIME_ANI_RUN] == NULL)
	{
		int numbeFrame = 28;
		float duration = 0.03f;
		animations[SlimeAni::SLIME_ANI_RUN] = CommonFuntion::createAnimation("slime-walk", numbeFrame, duration);
		animations[SlimeAni::SLIME_ANI_RUN]->retain();
	}

	auto run = Animate::create(animations[SlimeAni::SLIME_ANI_RUN]->clone());
	run->setTag(SlimeAni::SLIME_ANI_RUN);

	_enemySprite->runAction(run);
}

void Enemy::aniAttack()
{
	if (_enemySprite->getActionByTag(SlimeAni::SLIME_ANI_ATTACK) != NULL) { return; }
	_enemySprite->stopAllActions();

	if (animations[SlimeAni::SLIME_ANI_ATTACK] == NULL)
	{
		int numbeFrame = 16;
		float duration = 0.05f;
		animations[SlimeAni::SLIME_ANI_ATTACK] = CommonFuntion::createAnimation("slime-swinging", numbeFrame, duration);
		animations[SlimeAni::SLIME_ANI_ATTACK]->retain();
	}

	auto attack = Animate::create(animations[SlimeAni::SLIME_ANI_ATTACK]->clone());
	attack->setTag(SlimeAni::SLIME_ANI_ATTACK);

	_enemySprite->runAction(attack);
}

void Enemy::aniDamage()
{
	if (_enemySprite->getActionByTag(SlimeAni::SLIME_ANI_DAMAGE) != NULL) { return; }
	_enemySprite->stopAllActions();

	if (animations[SlimeAni::SLIME_ANI_DAMAGE] == NULL)
	{
		int numbeFrame = 12;
		float duration = 0.05f;
		animations[SlimeAni::SLIME_ANI_DAMAGE] = CommonFuntion::createAnimation("slime-damage", numbeFrame, duration);
		animations[SlimeAni::SLIME_ANI_DAMAGE]->retain();
	}

	auto damage = Animate::create(animations[SlimeAni::SLIME_ANI_DAMAGE]->clone());
	damage->setTag(SlimeAni::SLIME_ANI_DAMAGE);

	_enemySprite->runAction(damage);
}
