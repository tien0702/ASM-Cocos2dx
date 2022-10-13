#include "UIControl.h"
#include "CharDetailsScene.h"

USING_NS_CC;


UIControl* UIControl::_instance;

UIControl* UIControl::createUIControl()
{
	return UIControl::create();
}

bool UIControl::init()
{
	if (!Layer::init()) { return false; }


	{
		isOpenSetting = false;

		_tableSetting = Layer::create();

		auto stopAudio = cocos2d::ui::Button::create();

	}

	{
		Size size = Size(50, 50);
		_setting = MenuItemImage::create("sprites/GameUI/pause.png", "sprites/GameUI/pause-clicked.png");
		//if (_setting == NULL) { return false; }
		_setting->setScale(size.width / _setting->getContentSize().width);
		_setting->setPosition((WIN_SIZE / 2) - (size / 2));
		_setting->setCallback(CC_CALLBACK_0(UIControl::gotoSetting, this));
	}

	{
		Size size = Size(100, 100);
		_charInfor = MenuItemImage::create("sprites/GameUI/avt.png", "sprites/GameUI/avt.png");
		_charInfor->setScale(size.width / _charInfor->getContentSize().width);
		_charInfor->setPosition((Vec2(-WIN_SIZE.width, WIN_SIZE.height) / 2) + Vec2(size.width / 2, -size.height / 2));
		_charInfor->setCallback(CC_CALLBACK_1(UIControl::gotoDetailScene, this));

		_hpBar = cocos2d::ui::LoadingBar::create("sprites/GameUI/hp.png");
		_hpBar->setDirection(ui::LoadingBar::Direction::LEFT);
		_hpBar->setScale(100 / _hpBar->getContentSize().width);
		_hpBar->setPercent(100);
		_hpBar->setPosition(Vec2(_charInfor->getPosition().x + size.width + 45,
			_charInfor->getPosition().y + (size.height / 2 - 10 - (_hpBar->getContentSize().height * _hpBar->getScale()) / 2)));
		auto hpFrame = Sprite::create("sprites/GameUI/hp_frame.png");
		hpFrame->setScale(_hpBar->getScale());
		hpFrame->setPosition(_hpBar->getPosition() - Vec2(12, 0));
		this->addChild(hpFrame);
		this->addChild(_hpBar);
	}

	{
		_skillBorder = Sprite::create("sprites/Skill/skill-border.png");
		_skillBorder->retain();
		_cooldownOverlay1 = ui::LoadingBar::create("sprites/Skill/skill-cooldown.png");
		_cooldownOverlay1->retain();
		_skill1 = MenuItemImage::create("sprites/Skill/skill-size.png", "");
		_skill1->setPosition(Vec2(480, -270));
		_skill1->setScale(60 / _skill1->getContentSize().width);
	}


	{
		//Todo: _skill2 = MenuItemSprite::create();
	}

	{
		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->onTouchBegan = CC_CALLBACK_2(UIControl::onTouchBegan, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(UIControl::onTouchEnded, this);
		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	}
	{
		this->setTag(22);
		_menu = Menu::create(_setting, _charInfor, _skill1, _skill2, NULL);
		_menu->setPosition(Vec2::ZERO);
		this->addChild(_menu);
		this->scheduleUpdate();
	}

	return true;
}

UIControl* UIControl::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = UIControl::createUIControl();
	}

	return _instance;
}

void UIControl::update(float dt)
{
	if (_player != NULL) _hpBar->setPercent(((*_player->_hp) / (*_player->_max_hp)) * 100.0f);
	if (_cooldownOverlay1 != NULL) {
		_cooldownOverlay1->setPercent(_player->_skill->getPercentCooldown());
	}
}

bool UIControl::initSkillButton1()
{
	if (_player == NULL) {
		log("Player is null!");
		return false;
	}

	auto skill = _player->_skill;
	if (skill == NULL) {
		log("Skill is null!");
		return false;
	}

	auto avatar = skill->_avatar;
	if (avatar == NULL) {
		log("Avatar is null!");
		return false;
	}

	{
		float sizeW = 60;
		auto borderClone = new Sprite();
		borderClone = _skillBorder;

		_cooldownOverlay1->setDirection(ui::LoadingBar::Direction::LEFT);
		_cooldownOverlay1->setRotation(-90);
		_cooldownOverlay1->setOpacity(200);
		_cooldownOverlay1->setPercent(_player->_skill->getPercentCooldown());

		avatar->setScale(_skill1->getContentSize().width / avatar->getContentSize().width);
		borderClone->setScale(_skill1->getContentSize().width / borderClone->getContentSize().width);
		_cooldownOverlay1->setScale(_skill1->getContentSize().width / _cooldownOverlay1->getContentSize().width);

		Vec2 pos = Vec2(sizeW, sizeW);
		avatar->setPosition(pos);
		borderClone->setPosition(pos);
		_cooldownOverlay1->setPosition(pos);

		_skill1->addChild(avatar, 1);
		if (_cooldownOverlay1->getParent() != NULL) { _cooldownOverlay1->removeFromParent(); }
		_skill1->addChild(_cooldownOverlay1, 2);
		if (borderClone->getParent() != NULL) { borderClone->removeFromParent(); }
		_skill1->addChild(borderClone, 3);
		_skill1->setCallback(CC_CALLBACK_1(UIControl::UseSkill, this, Point(400, 400)));
	}

	return true;
}

void UIControl::UseSkill(cocos2d::Ref* pSender, Point& point)
{
	auto skill = _player->_skill;

	MoveTo* flyUp;
	auto fading = FadeOut::create(1.0f);
	Spawn* effect;
	Label* label;
	float x = _player->getPhysicsBody()->getVelocity().x* (_player->_skill->_operating_range);
	Vec2 target = Vec2(x, _player->getPosition().y);
	 target = _player->getPosition() + Vec2(100, 0);
	
	switch (_player->UseSkill(target))
	{
	case STATUS_SKILL::NO_COOLDOWN_TIME:
		label = Label::createWithTTF(CommonFuntion::labelConfig, "On cooldown");
		label->setPosition(_player->getPosition() + Vec2(0, 30));
		label->setColor(Color3B::WHITE);
		flyUp = MoveTo::create(1.0f, label->getPosition() + Vec2(0, 10));
		effect = Spawn::create(flyUp, fading, NULL);
		label->runAction(effect);
		this->getParent()->addChild(label);
		break;
	case STATUS_SKILL::LOCKED:
		log("Locked skills");
		break;
	default:
		break;
	}
}

bool UIControl::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev)
{
	auto target = static_cast<Sprite*>(ev->getCurrentTarget());
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
	{
		return true;
	}
	return true;
}

void UIControl::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* ev)
{

}

void UIControl::gotoDetailScene(Ref* pSender)
{
	log("goto ...");
}

void UIControl::gotoSetting()
{
	auto allChild = Director::getInstance()->getRunningScene()->getChildren();

	if (isOpenSetting)
	{
		Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1);
		for (auto x : allChild)
		{
			if (x->getTag() != this->getTag())
			{
				x->resume();
			}
		}
	}
	else
	{
		Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0);
		for (auto x : allChild)
		{
			if (x->getTag() != this->getTag())
			{
				x->pause();
			}
		}
	}
	isOpenSetting = !isOpenSetting;
}
