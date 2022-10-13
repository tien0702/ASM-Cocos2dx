#ifndef __CHARACTER_CHOICE_SCENE_H__
#define __CHARACTER_CHOICE_SCENE_H__

#include "cocos2d.h"
#include "CameraMap.h"
#include "Character.h"
#include "Definition.h"
#include "CharacterFactory.h"
#include "CharDetailsScene.h"
#include "GameDirector.h"

enum E_NPC
{
	SHOP,
	DAILY,
	TOTAL_NPC
};

class CharacterChoiceScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(CharacterChoiceScene);

private:
	cocos2d::TMXTiledMap* _map;
	cocos2d::Rect _visible;

	cocos2d::Menu* _menu;

	cocos2d::TMXObjectGroup* _characterGr;
	std::string _nameCharacters[ECharacter::TOTAL_CHAR];
	cocos2d::ValueMap _characters[ECharacter::TOTAL_CHAR];
	cocos2d::MenuItemSprite* _characterButton[ECharacter::TOTAL_CHAR];
	Character* _character;
	ECharacter choice;
	cocos2d::TMXObjectGroup* _NPCGr;

	CameraMap* _cameraMap;

private:
	virtual bool loadMapInformation();
	virtual void setNameCharacters();
	virtual bool loadCharacters();
	virtual bool loadNPC();

	void GotoCharDetailScene(Ref* pSender, ECharacter eChar);
};

#endif // !__CHARACTER_CHOICE_SCENE_H__
