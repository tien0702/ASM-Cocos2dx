#ifndef __CAMERA_FOLLOW_PLAYER_H__
#define __CAMERA_FOLLOW_PLAYER_H__

#include "cocos2d.h"
#include "Definition.h"
#include "CommonFuntion.h"

class CameraFollowPlayer : public cocos2d::Node
{
public:
	static CameraFollowPlayer* createCam(cocos2d::Camera* cam, cocos2d::Rect* visibleArea, cocos2d::Node* target);
	virtual bool init();

	void SetCam(cocos2d::Camera* cam) { _cam = cam; }
	void SetVisibleArea(cocos2d::Rect* visibleArea) { _visibleArea = visibleArea; }
	bool ChangeTarget(cocos2d::Node* target);

	CREATE_FUNC(CameraFollowPlayer);

private:
	cocos2d::Camera* _cam; 
	cocos2d::Rect* _visibleArea;
	cocos2d::Node* _target;
	cocos2d::Rect _limitSpaceCam;
	cocos2d::Rect _cameraViewArea;

private:
	void update(float dt);
};

#endif // !__CAMERA_FOLLOW_PLAYER_H__
