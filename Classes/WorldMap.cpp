#include "WorldMap.h"

USING_NS_CC;

ECharacter gCharWM;

Scene* WorldMap::createWorldMap(ECharacter player)
{
	gCharWM = player;

	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

	auto layer = WorldMap::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer);

	return scene;
}

bool WorldMap::init()
{
	if (!Scene::init()) { return false; }
	if (!loadMapInformation()) { return false; }
	{
		CommonFuntion::labelConfig.fontFilePath = "fonts/arial.ttf";
		CommonFuntion::labelConfig.fontSize = 15;
	}

	{
		_player = CharacterFactory::GetCharacter(ECharacter::NORA);
		_player->_map = this->_map;
		_player->_meta = _meta;
		_player->setPosition(getObjectLocationInMap("player", "pointStart"));
	}
	if (!initEnemies()) { return false; }
	{
		_cameraFollowPlayer = CameraFollowPlayer::createCam(_defaultCamera, _visibleArea, _player);
	}

	{
		_charController = CharacterController::createCharacterController(_player);
		_charController->_movement->_map = _map;
		_charController->_movement->_meta = _meta;
		_charController->_cam = _defaultCamera;
	}

	{
		if (_player->getParent() != NULL)
			_player->removeFromParentAndCleanup(false);
		_map->addChild(_player);

		auto above = _map->getLayer("above");
		_player->setLocalZOrder(above->getLocalZOrder() - 1);
	}

	{
		this->addChild(_map);
		this->addChild(_cameraFollowPlayer);
		this->addChild(_charController);
		if (_enemies != NULL)
		{
			for (auto it = _enemies->begin(); it != _enemies->end(); ++it)
			{
				this->addChild(it->second);
			}
		}

		if (UIControl::GetInstance()->getParent() != NULL)
		{
			UIControl::GetInstance()->getParent()->removeChild(UIControl::GetInstance(), false);
		}
		UIControl::GetInstance()->setPosition(_defaultCamera->getPosition());
		UIControl::GetInstance()->_player = _player;
		UIControl::GetInstance()->_world = this->world;
		if (!UIControl::GetInstance()->initSkillButton1()) { return false; };
		this->addChild(UIControl::GetInstance(), 11);
		if (_player->_skill->getParent() != NULL)
		{
			_player->_skill->removeFromParent();
		}
		this->addChild(_player->_skill);
	}

	{
		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactBegin = CC_CALLBACK_1(WorldMap::onContactBegin, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	}

	this->scheduleUpdate();
	this->schedule(SEL_SCHEDULE(&WorldMap::fixedUpdate), FIXED_UPDATE_TIME);
	return true;
}

bool WorldMap::loadMapInformation()
{
	_map = new TMXTiledMap();
	_map->initWithTMXFile(WORLD_MAP_FILE_PATH);

	if (_map == NULL)
	{
		log("Can't load map, file path: %s", MAP_FILE_PATH);
		return false;
	}

	_meta = _map->getLayer("meta");
	if (_meta == NULL)
	{
		log("Get meta error!");
		return false;
	}
	_meta->setVisible(false);

	auto visibleGr = _map->getObjectGroup("visible");
	if (visibleGr == NULL)
	{
		log("Get visible error!");
		return false;
	}
	ValueMap visibleArea = visibleGr->getObject("visible");
	_visibleArea = new Rect();
	_visibleArea->origin = Vec2(visibleArea["x"].asFloat(), visibleArea["y"].asFloat());
	_visibleArea->size = Size(visibleArea["width"].asFloat(), visibleArea["height"].asFloat());

	_enemiesGr[TypeEnemy::SLIME] = _map->getObjectGroup("slime");
	_enemiesGr[TypeEnemy::GOBLIN] = _map->getObjectGroup("goblin");

	return true;
}

void WorldMap::update(float dt)
{
	UIControl::GetInstance()->setPosition(_defaultCamera->getPosition());
}

void WorldMap::fixedUpdate(float dt)
{

}

bool WorldMap::initEnemies()
{
	_enemies = new std::map<int, Enemy*>();
	if (_enemiesGr[TypeEnemy::SLIME] != NULL)
	{
		ValueVector listSlime = _enemiesGr[TypeEnemy::SLIME]->getObjects();
		for (int i = 0; i < listSlime.size(); ++i)
		{
			Enemy* enemySlime = Enemy::createEnemy();
			enemySlime->getPhysicsBody()->_ID = listSlime.at(i).asValueMap()["id"].asInt();
			Point pos = Point(listSlime.at(i).asValueMap()["x"].asFloat(), listSlime.at(i).asValueMap()["y"].asFloat());
			enemySlime->setPosition(pos);
			enemySlime->setOriginPos(pos);
			enemySlime->setTag(listSlime.at(i).asValueMap()["id"].asInt());
			enemySlime->_target = &_player->getPosition();
			_enemies->insert(std::pair<int, Enemy*>(listSlime.at(i).asValueMap()["id"].asInt(), enemySlime));
		}
	}

	return !_enemies->empty();
}

Point WorldMap::getObjectLocationInMap(std::string nameGr, std::string nameObj)
{
	auto objGr = _map->getObjectGroup(nameGr.c_str());

	if (objGr == NULL)
	{
		log("Can't get object name %s", nameGr.c_str());
		return Point::ZERO;
	}

	ValueMap object = objGr->getObject(nameObj.c_str());

	Point result;
	result.x = object["x"].asFloat();
	result.y = object["y"].asFloat();

	return result;
}

Vec2 WorldMap::convertToPositonTileMap(Vec2 objectPos)
{
	Size tileSize;
	tileSize.width = _map->getContentSize().width / _map->getMapSize().width;
	tileSize.height = _map->getContentSize().height / _map->getMapSize().height;

	int x = objectPos.x / tileSize.width;
	int y = (_map->getContentSize().height - objectPos.y) / tileSize.height;
	return Vec2(x, y);
}

bool WorldMap::canMoveOn(Vec2 target)
{
	int tileGid = _meta->getTileGIDAt(target);

	if (tileGid != 0)
	{
		Value temp = _map->getPropertiesForGID(tileGid);
		if (!temp.isNull())
		{
			ValueMap properties = temp.asValueMap();
			if (properties.find("Collidable") != properties.end() && properties.at("Collidable").asString() == "false")
			{
				return false;
			}
		}
	}
	return true;
}

Enemy* WorldMap::findEnemyByID(int id)
{
	Enemy* result = new Enemy();
	if (this->getChildByTag(id) != NULL)
	{
		Enemy* result = _enemies->at(id);
	}

	return result;
}

bool WorldMap::onContactBegin(cocos2d::PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if (a->getCollisionBitmask() == BULLET && b->getCollisionBitmask() == ENEMY ||
		a->getCollisionBitmask() == ENEMY && b->getCollisionBitmask() == BULLET)
	{
		int id = (a->getCollisionBitmask() == ENEMY) ? (a->_ID) : (b->_ID);
		if (this->getChildByTag(id) != NULL)
		{
			Enemy* e = _enemies->at(id);
			if (!e->IsDead())
			{
				e->TakeDamage(*_player->_dame);
			}
			else
			{
				this->removeChild(e, true);
				_enemies->erase(id);
			}
		}
	}
	else if (a->getCollisionBitmask() == PLAYER && b->getCollisionBitmask() == ENEMY_SKILL ||
		a->getCollisionBitmask() == ENEMY_SKILL && b->getCollisionBitmask() == PLAYER)
	{
		(a->getCollisionBitmask() == ENEMY_SKILL) ? (this->removeChild(a->getNode(), true)) : (this->removeChild(b->getNode(), true));
		_player->TakeDame(50);
	}
	else if (a->getCollisionBitmask() == CHAR_SKILL && b->getCollisionBitmask() == ENEMY ||
		a->getCollisionBitmask() == ENEMY && b->getCollisionBitmask() == CHAR_SKILL)
	{
		int id = (a->getCollisionBitmask() == ENEMY) ? (a->_ID) : (b->_ID);
		if (this->getChildByTag(id) != NULL)
		{
			Enemy* e = _enemies->at(id);
			if (e != NULL)
			{
				if (!e->IsDead())
				{
					e->TakeDamage(_player->_skill->getDame(*_player->_dame));
				}
				else
				{
					this->removeChild(e, true);
					_enemies->erase(id);
				}
			}
		}
	}

	return true;
}
