#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include "cocos2d.h"
#include "InputManager.h"
#include "Definition.h"
#include "CommonFuntion.h"

class Movement : public cocos2d::Node
{
public:
	static Movement* createMovement();
	virtual bool init();
	virtual void Move(cocos2d::Vec2 direction);

	void SetBody(cocos2d::PhysicsBody* body) { _body = body; }
	void SetSpeed(int* speed) { _speed = speed; }
	int* GetSpeed() { return _speed; }
	void Disable() { isActive = false; }
	void Enable() { isActive = true; }
	bool IsActive() { return isActive; }
	cocos2d::Vec2 GetDirection();

	CREATE_FUNC(Movement);
public:
	cocos2d::TMXTiledMap* _map;
	cocos2d::TMXLayer* _meta;
	cocos2d::Node* _checkPoint;

protected:
	InputManager* _input;
	int *_speed;
	bool isActive;

	cocos2d::PhysicsBody* _body;

private:
	void fixedUpdate(float dt);
	cocos2d::Vec2 convertToPositonTileMap(cocos2d::Vec2 objectPos);
	bool canMoveOn(cocos2d::Vec2 target);
};


#endif // !__MOVEMENT_H__
