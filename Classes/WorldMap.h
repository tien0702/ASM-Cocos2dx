#ifndef __WORLD_MAP_H__
#define __WORLD_MAP_H__

#include "UIControl.h"
#include "cocos2d.h"
#include "Character.h"
#include "CameraFollowPlayer.h"
#include "CharacterController.h"
#include "Enemy.h"

enum TypeEnemy
{
    GOBLIN,
    SLIME,
    TOTAL_ENEMY
};

class WorldMap : public cocos2d::Scene
{
public:
	static Scene* createWorldMap(ECharacter player);
	virtual bool init();

    CREATE_FUNC(WorldMap);
private:
    cocos2d::PhysicsWorld* world;
    cocos2d::TMXTiledMap* _map;
    cocos2d::TMXLayer* _meta;
    cocos2d::TMXObjectGroup* _npc;
    cocos2d::TMXObjectGroup* _enemiesGr[TypeEnemy::TOTAL_ENEMY];
    CameraFollowPlayer* _cameraFollowPlayer;
    CharacterController* _charController;
    Character* _player;
    cocos2d::Rect* _visibleArea;

    std::map<int, Enemy*>* _enemies;

private:
    virtual void update(float dt);
    Enemy* findEnemyByID(int id);
    void fixedUpdate(float dt);
    cocos2d::Point getObjectLocationInMap(std::string nameGr, std::string nameObj);
    bool loadMapInformation();
    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { this->world = world; };
    bool initEnemies();

    cocos2d::Vec2 convertToPositonTileMap(cocos2d::Vec2 objectPos);
    bool canMoveOn(cocos2d::Vec2 target);

    bool onContactBegin(cocos2d::PhysicsContact& contact);
};

#endif // !__WORLD_MAP_H__
