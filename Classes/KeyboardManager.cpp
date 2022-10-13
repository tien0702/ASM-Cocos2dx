#include "KeyboardManager.h"

USING_NS_CC;

std::mutex mt;
KeyboardManager* KeyboardManager::_instance;

KeyboardManager* KeyboardManager::createInputManager()
{
	return KeyboardManager::create();
}

bool KeyboardManager::init()
{
	if (!Node::init())
	{
		return false;
	}

	_horizontal = 0;
	_vertical = 0;

	loadDefaultKey();

	auto eventListenerKeyboard = EventListenerKeyboard::create();

	eventListenerKeyboard->onKeyReleased = CC_CALLBACK_2(KeyboardManager::onKeyReleased, this);
	eventListenerKeyboard->onKeyPressed = CC_CALLBACK_2(KeyboardManager::onKeyPress, this);

	Director::getInstance()->getEventDispatcher()
		->addEventListenerWithSceneGraphPriority(eventListenerKeyboard, this);

	return true;
}

void KeyboardManager::loadDefaultKey()
{
	_keyUsed.insert(std::pair<EventKeyboard::KeyCode, bool>(EventKeyboard::KeyCode::KEY_W, false));
	_keyUsed.insert(std::pair<EventKeyboard::KeyCode, bool>(EventKeyboard::KeyCode::KEY_UP_ARROW, false));
	_keyUsed.insert(std::pair<EventKeyboard::KeyCode, bool>(EventKeyboard::KeyCode::KEY_S, false));
	_keyUsed.insert(std::pair<EventKeyboard::KeyCode, bool>(EventKeyboard::KeyCode::KEY_DOWN_ARROW, false));
	_keyUsed.insert(std::pair<EventKeyboard::KeyCode, bool>(EventKeyboard::KeyCode::KEY_D, false));
	_keyUsed.insert(std::pair<EventKeyboard::KeyCode, bool>(EventKeyboard::KeyCode::KEY_RIGHT_ARROW, false));
	_keyUsed.insert(std::pair<EventKeyboard::KeyCode, bool>(EventKeyboard::KeyCode::KEY_A, false));
	_keyUsed.insert(std::pair<EventKeyboard::KeyCode, bool>(EventKeyboard::KeyCode::KEY_LEFT_ARROW, false));
	_keyUsed.insert(std::pair<EventKeyboard::KeyCode, bool>(EventKeyboard::KeyCode::KEY_SPACE, false));
}

//
KeyboardManager* KeyboardManager::GetInstance()
{
	if (_instance == NULL)
	{
		mt.lock();
		_instance = KeyboardManager::createInputManager();
		mt.unlock();
	}
	return _instance;
}

bool KeyboardManager::GetKey(EventKeyboard::KeyCode key)
{
	if (_keyUsed.find(key) == _keyUsed.end())
	{
		log("Key does not exist!");
		return false;
	}
	return _keyUsed[key];
}

bool KeyboardManager::AddKey(EventKeyboard::KeyCode key)
{
	if (_keyUsed.find(key) != _keyUsed.end())
	{
		log("Key already exists!");
		return false;
	}
	_keyUsed.insert(std::pair<EventKeyboard::KeyCode, bool>(key, false));
	return true;
}

// event listener keyboard
void KeyboardManager::onKeyPress(EventKeyboard::KeyCode key, Event* ev)
{
	if (_keyUsed.find(key) == _keyUsed.end())
		return;
	_keyUsed[key] = true;

	switch (key)
	{
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		_vertical = 1;
		break;
	case EventKeyboard::KeyCode::KEY_S:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		_vertical = -1;
		break;
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		_horizontal = 1;
		break;
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		_horizontal = -1;
		break;
	default:
		break;
	}
}

void KeyboardManager::onKeyReleased(EventKeyboard::KeyCode key, Event* ev)
{
	if (_keyUsed.find(key) == _keyUsed.end())
		return;
	_keyUsed[key] = false;

	switch (key)
	{
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (_keyUsed[EventKeyboard::KeyCode::KEY_S] || _keyUsed[EventKeyboard::KeyCode::KEY_DOWN_ARROW])
			_vertical = -1;
		else
			_vertical = 0;
		break;
	case EventKeyboard::KeyCode::KEY_S:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		if (_keyUsed[EventKeyboard::KeyCode::KEY_W] || _keyUsed[EventKeyboard::KeyCode::KEY_UP_ARROW])
			_vertical = 1;
		else _vertical = 0;
		break;
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (_keyUsed[EventKeyboard::KeyCode::KEY_A] || _keyUsed[EventKeyboard::KeyCode::KEY_LEFT_ARROW])
			_horizontal = -1;
		else _horizontal = 0;
		break;
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (_keyUsed[EventKeyboard::KeyCode::KEY_D] || _keyUsed[EventKeyboard::KeyCode::KEY_RIGHT_ARROW])
			_horizontal = 1;
		else _horizontal = 0;
		break;
	default:
		break;
	}
}

