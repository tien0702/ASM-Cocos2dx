#include "Bullet.h"

USING_NS_CC;

Vec2 gb_direction;

Bullet* Bullet::createBullet(cocos2d::Vec2 direction)
{
	gb_direction = direction;
	return Bullet::create();
}

bool Bullet::init()
{
	if (!Sprite::init()) { return false; }
	_direction = gb_direction;

	{
		_bulletSprite = Sprite::create("sprites/Bullet/laser/10.png");
		_bulletSprite->setScale(50 / _bulletSprite->getContentSize().width);
		float rotaion = Vec2::angle(Vec2::ANCHOR_BOTTOM_RIGHT, _direction);
		if (_direction.y > 0)
			rotaion = -rotaion;
		_bulletSprite->setRotation(CONVERT_TO_DEGRESS(rotaion));

		_body = PhysicsBody::createEdgeBox(Size(10, 10), PhysicsMaterial(1, 0, 1), 1);
		_body->setContactTestBitmask(1);
		_body->setCollisionBitmask(BULLET);
		_body->setCategoryBitmask(PLAYER);

		_dame = 100;
		_speed = 600;
		_operatingRange = 300;
		_direction.normalize();
		_body->setVelocity(_direction * _speed);
	}

	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprites/Effect/explode.plist", "sprites/Effect/explode.png");
		_explode = CommonFuntion::createAnimation("explode", 36, 0.03f);
		_explode->retain();
	}

	{
		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactBegin = CC_CALLBACK_1(Bullet::onContactBegin, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	}

	this->setPhysicsBody(_body);
	this->addChild(_bulletSprite);

	this->scheduleUpdate();
	return true;
}

bool Bullet::onContactBegin(cocos2d::PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if (a->getCollisionBitmask() == BULLET && b->getCollisionBitmask() == ENEMY ||
		a->getCollisionBitmask() == ENEMY && b->getCollisionBitmask() == BULLET)
	{
		remove();
	}

	return true;
}

void Bullet::setDame(float dame)
{
	if (dame > 0)
		_dame = dame;
}

void Bullet::update(float dt)
{
	if (!canMoveOn(convertToPositonTileMap(this->getPosition() + _direction * (_speed * dt))))
	{
		remove();
	}
	else if (this->getPosition().distance(_originPos) >= _operatingRange)
	{
		remove();
	}
}

void Bullet::remove()
{
	auto animate = Animate::create(_explode);
	animate->clone();
	AudioEngine::play2d("sound/bullet/boom.mp3", false, 0.1f);

	auto node = Sprite::create();
	node->setContentSize(Size(125, 125));
	node->setPosition(this->getPosition());
	node->setScale(40 / node->getContentSize().width);
	this->getParent()->addChild(node, 10);
	_explode->release();
	auto re = CallFunc::create([=]() {
		node->getParent()->removeChild(node, true);
		});
	node->runAction(Sequence::create(animate, re, NULL));
	this->getParent()->removeChild(this, true);
}

Bullet* Bullet::clone()
{
	return NULL;
}

Vec2 Bullet::convertToPositonTileMap(Vec2 objectPos)
{
	Size tileSize;
	tileSize.width = _map->getContentSize().width / _map->getMapSize().width;
	tileSize.height = _map->getContentSize().height / _map->getMapSize().height;

	int x = objectPos.x / tileSize.width;
	int y = (_map->getContentSize().height - objectPos.y) / tileSize.height;
	return Vec2(x, y);
}

bool Bullet::canMoveOn(Vec2 target)
{
	int tileGid = _meta->getTileGIDAt(target);

	if (tileGid != 0)
	{
		Value temp = _map->getPropertiesForGID(tileGid);
		if (!temp.isNull())
		{
			ValueMap properties = temp.asValueMap();
			if (properties.find("Collidable") != properties.end() && properties.at("Collidable").asString() == "false")
			{
				return false;
			}
		}
	}
	return true;
}