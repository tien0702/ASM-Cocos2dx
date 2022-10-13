#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
#include "Definition.h"
#include "AudioEngine.h"
#include "CommonFuntion.h"
#include "GameDirector.h"

class Bullet : public cocos2d::Sprite
{
public:
	static Bullet* createBullet(cocos2d::Vec2 direction);
	virtual bool init();

	void setDame(float dame);
	Bullet* clone();
	cocos2d::TMXTiledMap* _map;
	cocos2d::TMXLayer* _meta;

	cocos2d::Point _originPos;
	CREATE_FUNC(Bullet);

protected:
	float _dame;
	float _speed;
	float _operatingRange;
	cocos2d::Sprite* _bulletSprite;
	cocos2d::Vec2 _direction;
	cocos2d::PhysicsBody* _body;
	cocos2d::Animation* _explode;
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	void update(float dt);
	void remove();
	bool canMoveOn(cocos2d::Vec2 target);
	cocos2d::Vec2 convertToPositonTileMap(cocos2d::Vec2 objectPos);
};

#endif // !__BULLET_H__
