#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Skill.h"
#include "InputManager.h"
#include "Definition.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void UseSkill(cocos2d::Ref* pSender, cocos2d::Point& point);
    CREATE_FUNC(HelloWorld);
private:
    cocos2d::PhysicsWorld* world;
    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { this->world = world; };
    void update(float dt);
    InputManager* _input;

    Skill* _skill;

};

#endif // __HELLOWORLD_SCENE_H__
