#ifndef __UI_CONTROL_H__
#define __UI_CONTROL_H__

#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "Definition.h"
#include "Character.h"
#include "AudioEngine.h"

class UIControl : public cocos2d::Layer
{
public:
	static UIControl* GetInstance();
	Character* _player;
	cocos2d::PhysicsWorld* _world;

	bool initSkillButton1();
	bool initSkillButton2();

	CREATE_FUNC(UIControl);
protected:
	bool isOpenSetting;
	bool isMuted;
	static UIControl* createUIControl();
	virtual bool init();
	static UIControl* _instance;
protected:
	cocos2d::Menu* _menu;
	cocos2d::Menu* _menuSkills;
	cocos2d::ui::LoadingBar* _hpBar;
	cocos2d::Layer* _tableSetting;
	cocos2d::MenuItemImage* _setting;
	cocos2d::MenuItemImage* _charInfor;
	cocos2d::MenuItemImage* _skill1;
	cocos2d::MenuItemSprite* _skill2;

	cocos2d::ui::LoadingBar* _cooldownBar1;
	cocos2d::ui::LoadingBar* _cooldownBar2;

	cocos2d::Sprite* _skillBorder;
	cocos2d::ui::LoadingBar* _cooldownOverlay1;

protected:
	void update(float dt);
	void UseSkill(cocos2d::Ref* pSender, cocos2d::Point& point);
	virtual void gotoSetting();
	virtual void gotoDetailScene(cocos2d::Ref* pSender);

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* ev);
};

#endif // !__UI_CONTROL_H__
