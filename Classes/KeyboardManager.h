#ifndef __KEYBOARD_MANAGER_H__
#define __KEYBOARD_MANAGER_H__

#include "cocos2d.h"
#include <mutex>

class KeyboardManager : public cocos2d::Node
{
public:
	static KeyboardManager* GetInstance();
	float GetHorizontal() { return _horizontal; }
	float GetVertical() { return _vertical; }
	bool GetKey(cocos2d::EventKeyboard::KeyCode key);

	bool AddKey(cocos2d::EventKeyboard::KeyCode key);

	CREATE_FUNC(KeyboardManager);
private:
	static KeyboardManager* _instance;
	static KeyboardManager* createInputManager();
	bool init();
	void loadDefaultKey();

	//
	std::map<cocos2d::EventKeyboard::KeyCode, bool> _keyUsed;
	float _horizontal;
	float _vertical;

	// event listener keyboard
	void onKeyPress(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* ev);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* ev);
};

#endif // !__KEYBOARD_MANAGER_H__
