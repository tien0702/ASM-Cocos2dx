#include "CharacterFactory.h"

Character* CharacterFactory::GetCharacter(ECharacter name)
{
	Character* characterResult = NULL;
	switch (name)
	{
	case NORA:
		characterResult = Character::createCharacter();
		break;
	case CHAR2:
		characterResult = Character::createCharacter();
		break;
	case CHAR3:
		characterResult = Character::createCharacter();
		break;
	case TOTAL_CHAR:
		break;
	default:
		break;
	}

	return characterResult;
}