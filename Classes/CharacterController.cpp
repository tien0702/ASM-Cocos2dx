#include "CharacterController.h"

USING_NS_CC;

Character* gCharacterControl;

CharacterController* CharacterController::createCharacterController(Character* character)
{
	gCharacterControl = character;
	return CharacterController::create();
}

bool CharacterController::init()
{
	if (!Node::init()) { return false; }

	_character = gCharacterControl;

	if (_character == NULL)
	{
		_character = Character::createCharacter();
	}

	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprites/Effect/mouse-click.plist", "sprites/Effect/mouse-click.png");
		_aniClick = CommonFuntion::createAnimation("mouse-click", 15, 0.02f);
		_aniClick->retain();
		_circleRange = Sprite::create("sprites/Effect/c.png");
		_circleRange->retain();
		_posSkill = Sprite::create("sprites/Effect/c.png");
		_posSkill->retain();
	}

	{
		_movement = Movement::createMovement();
		_movement->SetSpeed(_character->_speed);
		_movement->SetBody(_character->getPhysicsBody());
		_movement->_checkPoint = _character->_grounded;
	}

	{
		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->onTouchBegan = CC_CALLBACK_2(CharacterController::onTouchBegan, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(CharacterController::onTouchMoved, this);
		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

		auto eventListenerKeyboard = EventListenerKeyboard::create();
		eventListenerKeyboard->onKeyReleased = CC_CALLBACK_2(CharacterController::onKeyReleased, this);
		eventListenerKeyboard->onKeyPressed = CC_CALLBACK_2(CharacterController::onKeyPress, this);
		Director::getInstance()->getEventDispatcher()
			->addEventListenerWithSceneGraphPriority(eventListenerKeyboard, this);
	}
	_input = InputManager::createInputManager();
	_input->AddKey(EventKeyboard::KeyCode::KEY_E);
	this->addChild(_input);
	this->addChild(_movement);
	this->scheduleUpdate();
	return true;
}


void CharacterController::update(float dt)
{
	if (_character->_state != NULL)
	{
		if (_circleRange != NULL && _circleRange->getParent() != NULL)
		{
			_circleRange->setPosition(_character->getPosition());
		}

		if (*(_character->_state) == CharState::CHAR_ATTACK || *(_character->_state) == CharState::CHAR_DEAD || *(_character->_state) == CharState::CHAR_USE_SKILL)
		{
			_movement->Disable();
		}
		else
		{
			_movement->Enable();
		}
	}

	{
		if (GetCursorPos(&_cursorPostion))
		{
			if (ScreenToClient(FindWindowA(NULL,
				Director::getInstance()->getOpenGLView()->getViewName().c_str()), &_cursorPostion)) {
			}
		}
	}

	if (_posSkill != NULL && _posSkill->getParent() != NULL)
	{
		Vec2 dir = ((_cam->getPosition() - WIN_SIZE / 2) + Vec2(_cursorPostion.x, WIN_SIZE.height - _cursorPostion.y)) - _character->getPosition();
		dir.normalize();
		dir = dir * _character->_skill->_operating_range / 2;
		_posSkill->setPosition(_character->getPosition() + dir);
	}
}

bool CharacterController::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev)
{
	auto aniSprite = Sprite::createWithSpriteFrameName("mouse-click (1).png");
	aniSprite->setScale(40 / aniSprite->getContentSize().width);
	aniSprite->runAction(Animate::create(_aniClick));
	aniSprite->setPosition((_cam->getPosition() - VISIBLE_SIZE / 2) + touch->getLocation());
	this->getParent()->addChild(aniSprite, 10);

	auto delay = FadeIn::create(0.5f);

	this->getParent()->runAction(Sequence::create(
		delay, CallFunc::create([=]() {
			this->getParent()->removeChild(aniSprite, true);
			aniSprite->release();
			}), NULL));
	AudioEngine::play2d("sound/click.mp3", false, 1.0f);
	float tanView = tan(CONVERT_TO_RADIAN(FIELD_OF_VIEW / 2));
	Rect r;
	r.size.height = (_cam->getPosition3D().z * tanView) * 2;
	r.size.width = (_cam->getPosition3D().z * tanView * (WIN_SIZE.width / WIN_SIZE.height)) * 2;

	_character->Attack((_cam->getPosition() - r.size / 2) + touch->getLocation());
	return true;
}

void CharacterController::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev)
{
	float tanView = tan(CONVERT_TO_RADIAN(FIELD_OF_VIEW / 2));
	Rect r;
	r.size.height = (_cam->getPosition3D().z * tanView) * 2;
	r.size.width = (_cam->getPosition3D().z * tanView * (WIN_SIZE.width / WIN_SIZE.height)) * 2;

	_character->Attack((_cam->getPosition() - r.size / 2) + touch->getLocation());
}

void CharacterController::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* ev)
{

}

void CharacterController::onKeyPress(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* ev)
{
	MoveTo* flyUp;
	auto fading = FadeOut::create(1.0f);
	Spawn* effect;
	Label* label;
	float x = _character->getPhysicsBody()->getVelocity().x * (_character->_skill->_operating_range);
	Vec2 target = Vec2(x, _character->getPosition().y);
	target = _character->getPosition() + Vec2(100, 0);


	Vec2 dir;
	switch (key)
	{
	case EventKeyboard::KeyCode::KEY_E:
		if (!_character->_skill->onCooldown())
		{
			{
				_posSkill->setScale(_character->_skill->_sizeSkill.width / _posSkill->getContentSize().width);
				dir = ((_cam->getPosition() - WIN_SIZE / 2) + Vec2(_cursorPostion.x, WIN_SIZE.height - _cursorPostion.y)) - _character->getPosition();
				dir.normalize();
				dir = dir * _character->_skill->_operating_range / 2;
				_posSkill->setPosition(_character->getPosition() + dir);
				this->getParent()->addChild(_posSkill, 11);
			}

			{
				_circleRange->setScale(_character->_skill->_operating_range / _circleRange->getContentSize().width);
				_circleRange->setPosition(_character->getPosition());
				this->getParent()->addChild(_circleRange, 10);
			}
		}
		else
		{
			label = Label::createWithTTF(CommonFuntion::labelConfig, "On cooldown");
			label->setPosition(_character->getPosition() + Vec2(0, 30));
			label->setColor(Color3B::WHITE);
			flyUp = MoveTo::create(1.0f, label->getPosition() + Vec2(0, 10));
			effect = Spawn::create(flyUp, fading, NULL);
			label->runAction(effect);
			this->getParent()->addChild(label);
		}
		break;
	default:
		break;
	}
}

void CharacterController::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* ev)
{
	Vec2 dir;
	switch (key)
	{
	case EventKeyboard::KeyCode::KEY_E:
		dir = ((_cam->getPosition() - WIN_SIZE / 2) + Vec2(_cursorPostion.x, WIN_SIZE.height - _cursorPostion.y)) - _character->getPosition();
		dir.normalize();
		dir = dir * _character->_skill->_operating_range / 2;
		if (_character->UseSkill(_character->getPosition() + dir) == STATUS_SKILL::SUCCESSFUL)
		{
			this->getParent()->removeChild(_circleRange, true);
			this->getParent()->removeChild(_posSkill, true);
		}
		break;
	default:
		break;
	}
}