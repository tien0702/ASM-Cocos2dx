#include "CameraFollowPlayer.h"

USING_NS_CC;

Camera* gCam;
Rect* gVisibleArea;
Node* gTarget;

CameraFollowPlayer* CameraFollowPlayer::createCam(Camera* cam, Rect* visibleArea, Node* target)
{
	gCam = cam;
	gVisibleArea = visibleArea;
	gTarget = target;
	return CameraFollowPlayer::create();
}

bool CameraFollowPlayer::init()
{
	if (!Node::init()) return false;

	_cam = gCam;
	_visibleArea = gVisibleArea;
	_target = gTarget;

	if (_cam == NULL || _target == NULL || _visibleArea == NULL) return false;
	
	float tanView = tan(CONVERT_TO_RADIAN(FIELD_OF_VIEW / 2));
	_cameraViewArea.size.height = (_cam->getPosition3D().z * tanView) * 2;
	_cameraViewArea.size.width = (_cam->getPosition3D().z * tanView * (WIN_SIZE.width / WIN_SIZE.height)) * 2;
	_cameraViewArea.origin.x = _cam->getPosition().x - _cameraViewArea.size.width / 2;
	_cameraViewArea.origin.y = _cam->getPosition().y - _cameraViewArea.size.height / 2;

	_limitSpaceCam.origin = _visibleArea->origin + (_cameraViewArea.size / 2);
	_limitSpaceCam.size = _visibleArea->size - _cameraViewArea.size;

	this->scheduleUpdate();
	return true;
}

void CameraFollowPlayer::update(float dt)
{
	float offsetLeft = _visibleArea->origin.x + _cameraViewArea.size.width / 2;
	float offsetRight = _visibleArea->getMaxX() - (_cameraViewArea.size.width / 2);

	float offsetTop = _visibleArea->getMaxY() - (_cameraViewArea.size.height / 2);
	float offsetBottom = _visibleArea->origin.y + (_cameraViewArea.size.height / 2);

	Vec2 posTarget = _target->getPosition();
	float xNewPos = posTarget.x;
	float yNewPos = posTarget.y;

	if (xNewPos < _limitSpaceCam.origin.x) xNewPos = offsetLeft;
	else if (xNewPos > _limitSpaceCam.getMaxX()) xNewPos = offsetRight;

	if (yNewPos < _limitSpaceCam.origin.y) yNewPos = offsetBottom;
	else if (yNewPos > _limitSpaceCam.getMaxY()) yNewPos = offsetTop;

	_cam->setPosition(xNewPos, yNewPos);
}

bool CameraFollowPlayer::ChangeTarget(cocos2d::Node* target)
{
	if (!_visibleArea->containsPoint(target->getPosition()))
		return false;
	return true;
}
