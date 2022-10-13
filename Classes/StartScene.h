#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "Character.h"
#include "CharacterFactory.h"
#include "CameraFollowPlayer.h"
#include "CharacterController.h"
#include "CameraMap.h"
#include "Movement.h"

#include "Definition.h"
#include "CommonFuntion.h"
#include "WorldMap.h"
#include "GameDirector.h"

class StartScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createStartScene(ECharacter character);
	virtual bool init();

	CREATE_FUNC(StartScene);
private:
	cocos2d::TMXTiledMap* _map;
	cocos2d::TMXLayer* _meta;
	cocos2d::Rect _visible;
	cocos2d::PhysicsWorld* _world;

	cocos2d::TMXObjectGroup* _character;
	cocos2d::TMXObjectGroup* _npc;
	cocos2d::TMXObjectGroup* _teleportPoint;

	Character* _player;
	CharacterController* _charController;
	CameraFollowPlayer* _cameraFollowPlayer;
	CameraMap* _cameraMap;

private:
	void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { _world = world; };
	bool loadMapInformation();
	bool initNPC();

	//
	void update(float dt);
	bool onContactBegin(cocos2d::PhysicsContact& contact);

	// 
	void GotoWorldMap(Ref* pSender);


};

#endif // !__START_SCENE_H__
