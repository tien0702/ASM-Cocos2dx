#include "CharDetailsScene.h"

USING_NS_CC;

Scene* CharDetailsScene::createScene()
{
	auto scene = Scene::create();
	auto layer = CharDetailsScene::create();
	scene->addChild(layer);

	return scene;
}

bool CharDetailsScene::init()
{
	if (!Scene::init()) { return false; }
	if (!loadInforCharacter()) { return false; }
	_character = CharacterFactory::GetCharacter(ECharacter::NORA);
	auto bkgr = Sprite::create("bkgr.jpg");
	bkgr->setPosition(VISIBLE_SIZE / 2);
	bkgr->setScale(VISIBLE_SIZE.width / bkgr->getContentSize().width);

	//_character = GameDirector::GetInstance()->_playerDir;
	if (_character == NULL) { return false; }

	auto tableDetails = _character->GetInfor();
	tableDetails->setPosition(210, 350);

	auto charAnimation = _character->GetIdle();
	charAnimation->setScale(600 / charAnimation->getContentSize().width);
	charAnimation->setPosition(900, 350);

	float gobackBtSize = 60;
	auto goBackBt = MenuItemImage::create("sprites/GameUI/back/normal.png", "sprites/GameUI/back/hove.png", "sprites/GameUI/back/clicked.png");
	goBackBt->setCallback(CC_CALLBACK_1(CharDetailsScene::GoBack, this));
	goBackBt->setScale(gobackBtSize / goBackBt->getContentSize().width);
	goBackBt->setPosition((gobackBtSize / 2), VISIBLE_SIZE.height - (goBackBt->getContentSize().height * goBackBt->getScale()) / 2);

	float gotoStartSceneBtSize = 150;
	auto gotoStartSceneBt = MenuItemImage::create("sprites/GameUI/next/normal.png", "sprites/GameUI/next/hove.png", "sprites/GameUI/next/clicked.png");
	gotoStartSceneBt->setCallback(CC_CALLBACK_1(CharDetailsScene::GotoStartScene, this, eChar));
	gotoStartSceneBt->setScale(gotoStartSceneBtSize / gotoStartSceneBt->getContentSize().width);
	gotoStartSceneBt->setPosition(
		VISIBLE_SIZE.width - gotoStartSceneBtSize / 2,
		(gotoStartSceneBt->getContentSize().height * gotoStartSceneBt->getScale()) / 2
	);

	_navigation = Menu::create(goBackBt, gotoStartSceneBt, NULL);
	_navigation->setPosition(Point::ZERO);

	this->addChild(bkgr);
	this->addChild(tableDetails);
	this->addChild(charAnimation);
	this->addChild(_navigation);
	return true;
}
void CharDetailsScene::GotoStartScene(Ref* pSender, ECharacter eChar)
{
	Director::getInstance()->resume();
	Director::getInstance()->popScene();
	auto scene = StartScene::createStartScene(eChar);
	Director::getInstance()->replaceScene(scene);
}

bool CharDetailsScene::loadInforCharacter()
{
	return true;
}

void CharDetailsScene::GoBack(Ref* pSender)
{
	Director::getInstance()->popScene();
}
