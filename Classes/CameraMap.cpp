#include "CameraMap.h"

USING_NS_CC;


float gMaxZoom;
float gMinZoom;
Camera* gCamMap;
Rect gVisibleArea;
Rect gVisibleSizeCam;

CameraMap* CameraMap::createCameraMap(cocos2d::Camera* cam, cocos2d::Rect visibleArea, float maxZoom, float minZoom)
{
	gCamMap = cam;
	gMaxZoom = maxZoom;
	gMinZoom = minZoom;
	gVisibleArea = visibleArea;
	return CameraMap::create();
}

bool CameraMap::init()
{
	if (!Node::init())
	{
		return false;
	}

	_maxZoom = gMaxZoom;
	_minZoom = gMinZoom;
	_cam = gCamMap;
	_visibleArea = gVisibleArea;

	if (_cam == NULL || (_maxZoom < _minZoom)) { return false; }
	_cam->setPositionZ(MIN_ZOOM);

	float tanView = tan(CONVERT_TO_RADIAN(FIELD_OF_VIEW / 2));
	_viewInMap.size.height = (_cam->getPosition3D().z * tanView) * 2;
	_viewInMap.size.width = (_cam->getPosition3D().z * tanView * (WIN_SIZE.width / WIN_SIZE.height)) * 2;
	_viewInMap.origin.x = _cam->getPosition().x - _viewInMap.size.width / 2;
	_viewInMap.origin.y = _cam->getPosition().y - _viewInMap.size.height / 2;

	_limitSpaceCam.origin = _visibleArea.origin + (_viewInMap.size / 2);
	_limitSpaceCam.size = _visibleArea.size - _viewInMap.size;

	_xSensitivity = 0.5;
	_ySensitivity = 0.5;
	_speedScroll = 20;

	addEventListener();
	return true;
}

void CameraMap::update(float dt)
{

}

void CameraMap::addEventListener()
{
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseScroll = CC_CALLBACK_1(CameraMap::onMouseScroll, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(CameraMap::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(CameraMap::onTouchMoved, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool CameraMap::containsRect(Rect r1, Rect r2)
{
	return (r1.origin.x	 < r2.origin.x  &&
			r1.origin.y	 > r2.origin.y  &&
			r1.getMaxX() > r2.getMaxX() &&
			r1.getMaxY() > r2.getMaxY());
}

bool CameraMap::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev)
{
	EventMouse* e = (EventMouse*)ev;
	_posTouchBegan = touch->getLocationInView();
	return true;
}

void CameraMap::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev)
{
	EventMouse* e = (EventMouse*)ev;
	Vec2 posMouse = touch->getLocationInView();

	Vec2 offset = _posTouchBegan - posMouse;

	offset.x = offset.x * _xSensitivity;
	offset.y = -(offset.y * _ySensitivity);

	auto temp = _viewInMap;
	temp.origin += offset;

	/*if (!_visibleArea.containsPoint(temp.origin) || !_visibleArea.containsPoint(Vec2(temp.getMaxX(), temp.getMaxY())))
		return;*/

	_viewInMap.origin += offset;
	Vec2 newPosCam = _cam->getPosition() + offset;
	_cam->setPosition(newPosCam.x, newPosCam.y);
	_posTouchBegan = posMouse;
}

void CameraMap::onMouseScroll(Event* ev)
{
	EventMouse* e = (EventMouse*)ev;
	Vec3 newPosCam = _cam->getPosition3D();
	newPosCam.z += e->getScrollY() * _speedScroll;
	if (newPosCam.z > _maxZoom || newPosCam.z < _minZoom) { return; }

	float tanView = tan(CONVERT_TO_RADIAN(FIELD_OF_VIEW / 2));
	_viewInMap.size.height = (_cam->getPosition3D().z * tanView) * 2;
	_viewInMap.size.width = (_cam->getPosition3D().z * tanView * (WIN_SIZE.width / WIN_SIZE.height)) * 2;
	_viewInMap.origin.x = _cam->getPosition().x - _viewInMap.size.width / 2;
	_viewInMap.origin.y = _cam->getPosition().y - _viewInMap.size.height / 2;


	float right = _viewInMap.getMaxX();
	float left = _viewInMap.origin.x;
	float top = _viewInMap.getMaxY();
	float bot = _viewInMap.origin.y;

	Vec2 offset = Vec2::ZERO;

	if (left < _visibleArea.origin.x)
		offset.x += (_visibleArea.origin.x - left);
	else if (right > _viewInMap.getMaxX())
		offset.x += (_visibleArea.origin.x - right);

	if (bot < _visibleArea.origin.y)
		offset.y += (_visibleArea.origin.y - bot);
	else if (top > _viewInMap.getMaxY())
		offset.y += (_visibleArea.origin.y - top);

	_viewInMap.origin += offset;

	_limitSpaceCam.origin = _visibleArea.origin + (_viewInMap.size / 2);
	_limitSpaceCam.size = _visibleArea.size - _viewInMap.size;

	_cam->setPosition3D(newPosCam);
	_cam->setPosition(_cam->getPosition() + offset);
}

