#include "Movement.h"

USING_NS_CC;

PhysicsBody* tBody;

Movement* Movement::createMovement()
{
	return Movement::create();
}

bool Movement::init()
{
	if (!Node::init())
	{
		return false;
	}

	isActive = true;
	_input = InputManager::createInputManager();
	this->addChild(_input);
	this->schedule(SEL_SCHEDULE(&Movement::fixedUpdate), FIXED_UPDATE_TIME);
	return true;
}
void Movement::fixedUpdate(float dt)
{
	if (!isActive) 
	{ 
		_body->setVelocity(Vec2::ZERO);
		return; 
	}

	Vec2 direction = Vec2(_input->GetHorizontal(), _input->GetVertical());
	Vec2 s = (direction * (*_speed)) * dt;
	Vec2 nextPoint;
	if (_checkPoint == NULL) nextPoint = _body->getPosition() + s;
	else 
		nextPoint = _checkPoint->getParent()->convertToWorldSpace(_checkPoint->getPosition()) + s;
	if (canMoveOn(convertToPositonTileMap(nextPoint)))
	{
		Move(direction);
	}
	else
	{
		_body->setVelocity(Vec2::ZERO);
	}
}

void Movement::Move(cocos2d::Vec2 direction)
{
	direction.normalize();
	if (_body == NULL)
	{
		log("Parent is null!");
		return;
	}
	_body->setVelocity(direction * (*_speed));
}

Vec2 Movement::GetDirection()
{
	return cocos2d::Vec2(_input->GetHorizontal(), _input->GetVertical());
}

Vec2 Movement::convertToPositonTileMap(Vec2 objectPos)
{
	if (_map == NULL) { return Vec2::ZERO; }
	Size tileSize;
	tileSize.width = _map->getContentSize().width / _map->getMapSize().width;
	tileSize.height = _map->getContentSize().height / _map->getMapSize().height;

	int x = objectPos.x / tileSize.width;
	int y = (_map->getContentSize().height - objectPos.y) / tileSize.height;
	return Vec2(x, y);
}

bool Movement::canMoveOn(Vec2 target)
{
	if (_meta == NULL) return true;
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

