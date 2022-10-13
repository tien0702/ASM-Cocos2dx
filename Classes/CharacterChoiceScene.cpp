#include "CharacterChoiceScene.h"
#include "AudioEngine.h"
USING_NS_CC;

Scene* CharacterChoiceScene::createScene()
{
	auto scene = Scene::create();
	auto layer = CharacterChoiceScene::create();
	scene->addChild(layer);
	return scene;
}

bool CharacterChoiceScene::init()
{
	if (!Scene::init()) { return false; }
	if (!loadMapInformation()) { return false; }
	setNameCharacters();
	if (!loadCharacters()) { return false; }
	if (!loadNPC()) return false;

	_defaultCamera->initPerspective(FIELD_OF_VIEW, WIN_SIZE.width / WIN_SIZE.height, NEAR_PLANE, FAR_PLANE);

	float cotag = acos(CONVERT_TO_RADIAN(FIELD_OF_VIEW / 2));
	float sizeMap = _map->getContentSize().width;
	float maxZoom = (_visible.size.width / 2) * cotag;
	float minZoom = MIN_ZOOM;
	_cameraMap = CameraMap::createCameraMap(this->_defaultCamera, _visible, maxZoom, minZoom);

	_menu = Menu::create(_characterButton[ECharacter::CHAR2], _characterButton[ECharacter::CHAR3],
		_characterButton[ECharacter::NORA], NULL);
	_menu->setPosition(Vec2::ZERO);

	_defaultCamera->setPosition3D(Vec3(_visible.getMidX(), _visible.getMidY(), maxZoom - 300));
	Vec3 moveTarget = _defaultCamera->getPosition3D();
	moveTarget.z = maxZoom;
	auto move = MoveTo::create(TRANSITION_TIME, Vec3(moveTarget));
	_defaultCamera->runAction(move);


	{
		AudioEngine::play2d("sound/background/song-1.mp3", true, 0.2f);
	}

	this->addChild(_map, 0);
	this->addChild(_menu, 1);
	this->addChild(_cameraMap);
	return true;
}

bool CharacterChoiceScene::loadMapInformation()
{
	_map = new TMXTiledMap();
	_map->initWithTMXFile(MAP_FILE_PATH);

	if (_map == NULL)
	{
		log("Can't load map, file path: %s", MAP_FILE_PATH);
		return false;
	}
	_map->getLayer("meta")->setVisible(false);

	auto visibleGr = _map->getObjectGroup("visible");
	if (visibleGr == NULL)
	{
		log("Get visible error!");
		return false;
	}

	ValueMap visibleMap = visibleGr->getObject("visible");
	_visible = Rect(visibleMap["x"].asFloat(), visibleMap["y"].asFloat(),
		visibleMap["width"].asFloat(), visibleMap["height"].asFloat());

	_NPCGr = _map->getObjectGroup("npc");
	if (_NPCGr == NULL)
	{
		log("Get NPC error!");
		return false;
	}

	_characterGr = _map->getObjectGroup("characters");
	if (_characterGr == NULL)
	{
		log("Get characters error!");
		return false;
	}

	return true;
}

void CharacterChoiceScene::setNameCharacters()
{
	_nameCharacters[ECharacter::NORA] = "nora";
	_nameCharacters[ECharacter::CHAR2] = "char2";
	_nameCharacters[ECharacter::CHAR3] = "char3";
}

bool CharacterChoiceScene::loadCharacters()
{
	// get characters in map
	for (int i = 0; i < ECharacter::TOTAL_CHAR; ++i)
		_characters[i] = _characterGr->getObject(_nameCharacters[i].c_str());

	// check character existence
	for (int i = 0; i < ECharacter::TOTAL_CHAR; ++i)
	{
		if (_characters[i].size() == 0)
			log("Get '%s' failed!", _nameCharacters[i].c_str());
	}

	{
		// sprite
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_CHAR1("idle"), (std::string)PATH_IMAGE_CHAR1("idle"));
		auto noraSprite = Sprite::createWithSpriteFrameName("idle (1).png");
		auto noraIdle = CommonFuntion::createAnimation("idle", 14, 0.05f);
		noraSprite->runAction(RepeatForever::create(Animate::create(noraIdle)));

		// button
		_characterButton[ECharacter::NORA] = MenuItemSprite::create(noraSprite, NULL);
		_characterButton[ECharacter::NORA]->setCallback(CC_CALLBACK_1(CharacterChoiceScene::GotoCharDetailScene, this, ECharacter::NORA));
		Vec2 posNora = Vec2(_characters[ECharacter::NORA]["x"].asFloat(), _characters[ECharacter::NORA]["y"].asFloat());
		_characterButton[ECharacter::NORA]->setPosition(posNora);
		_characterButton[ECharacter::NORA]->setScale(CHAR_NORMAL_SIZE / _characterButton[ECharacter::NORA]->getContentSize().width);
	}

	{
		//SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_CHAR1("idle"), (std::string)PATH_IMAGE_CHAR1("idle"));
		auto char2 = Sprite::createWithSpriteFrameName("idle (1).png");
		char2->setFlippedX(true);
		auto char2Idle = CommonFuntion::createAnimation("idle", 14, 0.05f);
		char2->runAction(RepeatForever::create(Animate::create(char2Idle)));

		// button
		_characterButton[ECharacter::CHAR2] = MenuItemSprite::create(char2, NULL);
		_characterButton[ECharacter::CHAR2]->setCallback(CC_CALLBACK_1(CharacterChoiceScene::GotoCharDetailScene, this, ECharacter::CHAR2));
		Vec2 posChar2 = Vec2(_characters[ECharacter::CHAR2]["x"].asFloat(), _characters[ECharacter::CHAR2]["y"].asFloat());
		_characterButton[ECharacter::CHAR2]->setPosition(posChar2);
		_characterButton[ECharacter::CHAR2]->setScale(CHAR_NORMAL_SIZE / _characterButton[ECharacter::CHAR2]->getContentSize().width);
	}

	{
		//SpriteFrameCache::getInstance()->addSpriteFramesWithFile((std::string)PATH_PLIST_CHAR1("idle"), (std::string)PATH_IMAGE_CHAR1("idle"));
		auto char3 = Sprite::createWithSpriteFrameName("idle (1).png");
		auto char3Idle = CommonFuntion::createAnimation("idle", 14, 0.05f);
		char3->runAction(RepeatForever::create(Animate::create(char3Idle)));

		// button
		_characterButton[ECharacter::CHAR3] = MenuItemSprite::create(char3, NULL);
		_characterButton[ECharacter::CHAR3]->setCallback(CC_CALLBACK_1(CharacterChoiceScene::GotoCharDetailScene, this, ECharacter::CHAR3));
		Vec2 posChar3 = Vec2(_characters[ECharacter::CHAR3]["x"].asFloat(), _characters[ECharacter::CHAR3]["y"].asFloat());
		_characterButton[ECharacter::CHAR3]->setPosition(posChar3);
		_characterButton[ECharacter::CHAR3]->setScale(CHAR_NORMAL_SIZE / _characterButton[ECharacter::CHAR3]->getContentSize().width);
	}

	return true;
}

bool CharacterChoiceScene::loadNPC()
{
	return true;
}

void CharacterChoiceScene::GotoCharDetailScene(Ref* pSender, ECharacter eChar)
{
	Character* player = NULL;
	switch (eChar)
	{
	case NORA:
		_character = CharacterFactory::GetCharacter(ECharacter::NORA);
		break;
	case CHAR2:
		_character = CharacterFactory::GetCharacter(ECharacter::NORA);
		break;
	case CHAR3:
		_character = CharacterFactory::GetCharacter(ECharacter::NORA);
		break;
	default:
		_character = NULL;
		return;
	}

	auto scene = CharDetailsScene::createScene();
	Director::getInstance()->pushScene(scene);
}
