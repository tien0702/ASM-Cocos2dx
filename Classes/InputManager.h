#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "cocos2d.h"

class InputManager : public cocos2d::Node
{
public:
	static InputManager* createInputManager();
	virtual bool init();
	float GetHorizontal() { return _horizontal; }
	float GetVertical() { return _vertical; }
	void AddKey(cocos2d::EventKeyboard::KeyCode key);
	bool GetKey(cocos2d::EventKeyboard::KeyCode key);

	CREATE_FUNC(InputManager);
private:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> _keyUsed;
	float _horizontal;
	float _vertical;

private:
	void loadDefaultKey();
	// event listener keyboard
	void onKeyPress(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* ev);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* ev);
};

#endif // !__INPUT_MANAGER_H__
