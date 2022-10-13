#ifndef __DEFINITION_H__
#define __DEFINITION_H__

#include "cocos2d.h"


#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()
#define ORIGIN Director::getInstance()->getVisibleOrigin()
#define WIN_SIZE Director::getInstance()->getWinSize()

#define FIXED_UPDATE_TIME 0.02f

// size
#define SIZE_OF_AVATAR 70
#define SIZE_NORMAL_ATTACK 70
#define SIZE_PAUSE 40

#define CHAR_NORMAL_SIZE 70
#define EDGE_BOX_SIZE 1

#define SIZE_GOBLIN 50
#define SIZE_SLIME 20
#define MIN_ZOOM 400
#define FAR_PLANE 2000
#define NEAR_PLANE 1
//
#define PLAYER 0x01 // 0001
#define ENEMY 0x02  // 0010
#define ENEMY_SKILL 0x05  // 0010
#define CHAR_SKILL 0x07  // 0010
#define BULLET 0x03  // 0011


// convert
#define PI 3.14159265359
#define CONVERT_TO_DEGRESS(radian) (radian * 180) / PI
#define CONVERT_TO_RADIAN(degress) degress * (PI / 180)

// time 
#define FIXED_UPDATE_TIME 0.02f
#define TRANSITION_TIME 0.5

// path
#define MAP_FILE_PATH "sprites/tileset/startMap.tmx"
#define WORLD_MAP_FILE_PATH "sprites/tileset/worldMap.tmx"
#define PATH_PLIST_CHAR1(name) "sprites/Characters/Char_2/" + name + ".plist"
#define PATH_IMAGE_CHAR1(name) "sprites/Characters/Char_2/" + name + ".png"

// camera
#define FIELD_OF_VIEW 60


#endif // !__DEFINITION_H__
