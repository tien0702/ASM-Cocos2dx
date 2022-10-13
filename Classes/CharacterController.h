#ifndef __CHARACTER_CONTROLLER_H__
#define __CHARACTER_CONTROLLER_H__

#include "cocos2d.h"
#include "CharacterFactory.h"
#include "Movement.h"
#include "AudioEngine.h"
#include "InputManager.h"

class CharacterController : public cocos2d::Node
{
public:
	static CharacterController* createCharacterController(Character* character);
	virtual bool init();

	CREATE_FUNC(CharacterController);
public:
	Character* _character;
	Movement* _movement;
	cocos2d::Camera* _cam;
	cocos2d::Animation* _aniClick;
	InputManager* _input;
	cocos2d::Sprite* _circleRange;
	cocos2d::Sprite* _posSkill;
	POINT _cursorPostion;
private:
	void update(float dt);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* ev);
	void onKeyPress(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* ev);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* ev);
};

#endif // !__CHARACTER_CONTROLLER_H__
