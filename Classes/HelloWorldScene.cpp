#include "HelloWorldScene.h"
#include "Character.h"
#include "Bullet.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = HelloWorld::create();

	layer->SetPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	//--------------

	return scene;
}

bool HelloWorld::init()
{
	if (!Scene::init())
	{
		return false;
	}

	_skill = Skill::createSkill();
	this->addChild(_skill);

	_skill->retain();

	_skill->setPosition(VISIBLE_SIZE / 2);

	//_skill->Trigger();


	float sizeW = 70;

	auto skillImage = Sprite::create("sprites/Skill/skill_01.png");
	auto skillBorder = Sprite::create("sprites/Skill/skill-border.png");
	auto cooldownBar = cocos2d::ui::LoadingBar::create("sprites/Skill/skill-cooldown.png");

	skillImage->setScale(sizeW / skillImage->getContentSize().width);
	skillBorder->setScale(sizeW / skillBorder->getContentSize().width);
	cooldownBar->setScale(sizeW / cooldownBar->getContentSize().width);

	Vec2 pos = Vec2(sizeW, sizeW) / 2;
	skillImage->setPosition(pos);
	skillBorder->setPosition(pos);
	cooldownBar->setPosition(pos);

	cooldownBar->setDirection(ui::LoadingBar::Direction::LEFT);
	cooldownBar->setRotation(-90);
	cooldownBar->setOpacity(200);
	cooldownBar->setPercent(_skill->getPercentCooldown() + 50);
	cooldownBar->scheduleUpdate();


	auto m = MenuItemImage::create();
	m->setContentSize(Size(sizeW, sizeW));
	m->addChild(skillImage, 1);
	m->addChild(cooldownBar, 2);
	m->addChild(skillBorder, 3);

	m->setCallback(CC_CALLBACK_1(HelloWorld::UseSkill, this, Point(200, 200)));
	m->setPosition(Vec2::ZERO);
	Menu* menu = Menu::create(m, NULL);
	menu->setPosition(Vec2(1200, 100));

	this->addChild(menu);

	this->scheduleUpdate();
	return true;
}

void HelloWorld::UseSkill(Ref* pSender, cocos2d::Point& point)
{
	/*_skill->setPosition(point);
	switch (_skill->Trigger())
	{
	case STATUS_SKILL::NO_COOLDOWN_TIME:
		log("Skill is on cooldown");
		break;
	case STATUS_SKILL::LOCKED:
		log("Locked skills");
		break;
	default:
		break;
	}*/
}

void HelloWorld::update(float dt)
{

}