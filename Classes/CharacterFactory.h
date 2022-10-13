#ifndef __CHARACTER_FACTORY_H__
#define __CHARACTER_FACTORY_H__

#include "cocos2d.h"
#include "Character.h"

enum ECharacter
{
	NORA,
	CHAR2,
	CHAR3,
	TOTAL_CHAR
};

class CharacterFactory
{
public:
	static Character* GetCharacter(ECharacter name);
};

#endif // !__CHARACTER_FACTORY_H__
