#ifndef __CAMERA_MAP_H__
#define __CAMERA_MAP_H__

#include "cocos2d.h"
#include "Definition.h"
#include "CommonFuntion.h"

class CameraMap : public cocos2d::Node
{
public:
	static CameraMap* createCameraMap(cocos2d::Camera* cam, cocos2d::Rect visibleArea, float maxZoom, float minZoom);
	virtual bool init();

	CREATE_FUNC(CameraMap);
private:
	float _maxZoom;
	float _minZoom;
	float _xSensitivity;
	float _ySensitivity;
	float _speedScroll;
	cocos2d::Camera* _cam;
	cocos2d::Rect _visibleArea;

	cocos2d::Rect _viewInMap;
	cocos2d::Rect _limitSpaceCam;

	cocos2d::Vec2 _posTouchBegan;
	cocos2d::Vec3 _oldPos;

private:
	bool containsRect(cocos2d::Rect r1, cocos2d::Rect r2);
	void addEventListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev);
	void onMouseScroll(cocos2d::Event* ev);
private:
	void update(float dt);
};

#endif // !__CAMERA_MAP_H__
