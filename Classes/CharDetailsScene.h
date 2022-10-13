#ifndef __CHAR_DETAILS_SCENE_H__
#define __CHAR_DETAILS_SCENE_H__

#include "cocos2d.h"
#include "Character.h"
#include "Definition.h"
#include "CharacterFactory.h"
#include "StartScene.h"
#include "GameDirector.h"
#include "Enemy.h"
#include "WorldMap.h"

class CharDetailsScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(CharDetailsScene);
private:
	Character* _character;
	cocos2d::Menu* _navigation;
	ECharacter eChar;

private:
	void GotoStartScene(Ref* pSender, ECharacter eChar);
	bool loadInforCharacter();
	void GoBack(Ref* pSender);
};

#endif // !__CHAR_DETAILS_SCENE_H__
