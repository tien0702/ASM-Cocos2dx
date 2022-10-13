#include "StartScene.h"

USING_NS_CC;

ECharacter eCharss;

Scene* StartScene::createStartScene(ECharacter character)
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
	scene->getPhysicsWorld()->setGravity(Vec2::ZERO);
	auto layer = StartScene::create();

	eCharss = character;
	auto s = scene->getPhysicsWorld();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

bool StartScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	if (!loadMapInformation())
	{
		log("Load map error!");
		return false;
	}

	{
		_player = CharacterFactory::GetCharacter(eCharss);
		_player->_map = _map;
		_player->_meta = _meta;
		_player->setPosition(700, 700);
		initNPC();
	}

	{
		/*_defaultCamera->initPerspective(FIELD_OF_VIEW, WIN_SIZE.width / WIN_SIZE.height, 1, 2000);
		Vec2 posCam;
		posCam.x = _visible.getMaxX() - _visible.size.width / 2;
		posCam.y = _visible.getMaxY() - _visible.size.height / 2;
		_defaultCamera->setPosition3D(Vec3(posCam.x, posCam.y, MIN_ZOOM));*/
	}

	{
		_cameraFollowPlayer = CameraFollowPlayer::createCam(_defaultCamera, &_visible, _player);
	}

	{
		_charController = CharacterController::createCharacterController(_player);
		_charController->_movement->_map = _map;
		_charController->_movement->_meta = _meta;
		_charController->_cam = this->_defaultCamera;
		_charController->_movement->_checkPoint = _player->_grounded;
	}

	auto gateWorldNode = Node::create();
	{
		ValueMap gateWorld = _teleportPoint->getObject("gateWorld");
		Vec2 posGateWorld = Vec2(gateWorld["x"].asFloat(), gateWorld["y"].asFloat());
		Size sizeGateWorld = Size(gateWorld["width"].asFloat(), gateWorld["height"].asFloat());
		posGateWorld.x += sizeGateWorld.width / 2;
		posGateWorld.y += sizeGateWorld.height / 2;

		gateWorldNode->setPosition(posGateWorld);
		auto bodyGateWorld = PhysicsBody::createEdgeBox(sizeGateWorld, PhysicsMaterial(0, 0, 0), 2);
		bodyGateWorld->setCollisionBitmask(5);
		bodyGateWorld->setContactTestBitmask(true);
		gateWorldNode->setPhysicsBody(bodyGateWorld);
	}

	{
		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactBegin = CC_CALLBACK_1(StartScene::onContactBegin, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	}

	{
		_map->addChild(_player, 10);
		this->addChild(_map);
		this->addChild(gateWorldNode);
		this->addChild(_charController);
		this->addChild(_cameraFollowPlayer);
	}

	this->scheduleUpdate();

	return true;
}


bool StartScene::loadMapInformation()
{
	_map = new TMXTiledMap();
	_map->initWithTMXFile(MAP_FILE_PATH);

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
	ValueMap visibleMap = visibleGr->getObject("visible");
	_visible = Rect(visibleMap["x"].asFloat(), visibleMap["y"].asFloat(),
		visibleMap["width"].asFloat(), visibleMap["height"].asFloat());

	_npc = _map->getObjectGroup("npc");
	if (_npc == NULL)
	{
		log("Get NPC error!");
		return false;
	}

	_character = _map->getObjectGroup("characters");
	if (_character == NULL)
	{
		log("Get characters error!");
		return false;
	}

	_teleportPoint = _map->getObjectGroup("teleportPoint");
	if (_teleportPoint == NULL)
	{
		log("Get teleport point error!");
		return false;
	}

	return true;
}

bool StartScene::initNPC()
{

	return true;
}

//
void StartScene::update(float dt)
{

}

bool StartScene::onContactBegin(PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	if (a->getCollisionBitmask() == PLAYER && b->getCollisionBitmask() == 5 ||
		a->getCollisionBitmask() == 5 && b->getCollisionBitmask() == PLAYER)
	{
		GotoWorldMap(this);
	}

	return true;
}

void StartScene::GotoWorldMap(Ref* pSender)
{
	auto scene = WorldMap::createWorldMap(eCharss);
	Director::getInstance()->replaceScene(scene);
}

