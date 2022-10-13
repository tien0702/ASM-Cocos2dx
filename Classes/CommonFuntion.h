#ifndef __COMMON_FUNTION_H__
#define __COMMON_FUNTION_H__

#include "cocos2d.h"
#include "Definition.h"


class CommonFuntion
{
public:
	static cocos2d::TTFConfig labelConfig;
	static cocos2d::Animation* createAnimation(std::string fileName, int numberFrames, float duration);
	static cocos2d::Point convertToPosWindow(cocos2d::Point obj, cocos2d::Camera* cam, cocos2d::TMXTiledMap* _map);
	static void ShowVec2(cocos2d::Vec2 v);
	static void ShowVec3(cocos2d::Vec3 r);
	static void ShowRect(cocos2d::Rect v);
	static cocos2d::Node* DrawPoint(cocos2d::Point p);
	static cocos2d::DrawNode* DrawRect(cocos2d::Rect rect, cocos2d::Color4F color);
	static cocos2d::DrawNode* DrawLine(cocos2d::Vec2 v1, cocos2d::Vec2 v2);

};

#endif // !__COMMON_FUNTION_H__
