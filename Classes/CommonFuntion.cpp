#include "CommonFuntion.h"

USING_NS_CC;
// animation format
#define INDEX_START 1
#define ANIMATION_FORMAT(index) " (" + std::to_string(index + INDEX_START) + ").png"

TTFConfig CommonFuntion::labelConfig;

Animation* CommonFuntion::createAnimation(std::string fileName, int numberFrames, float duration)
{
	Vector<SpriteFrame*> frames;

	for (int i = 0; i < numberFrames; ++i)
	{
		std::string buffer;
		buffer += fileName + ANIMATION_FORMAT(i);
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buffer);
		if (frame == nullptr)
		{
			printf("Can't create frame name: %s\n", buffer.c_str());
			break;
		}
		frames.pushBack(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(frames, duration);
	return animation;
}

Point CommonFuntion::convertToPosWindow(Point obj, Camera* cam, TMXTiledMap* _map)
{
	float angle = tan(CONVERT_TO_RADIAN(FIELD_OF_VIEW / 2));
	Rect viewArea;
	viewArea.size.height = (cam->getPosition3D().z * angle) * 2;
	viewArea.size.width = (cam->getPosition3D().z * angle * (WIN_SIZE.width / WIN_SIZE.height)) * 2;

	Point botLeft = cam->getPosition() - (viewArea.size / 2);

	return obj - botLeft;
}

Node* CommonFuntion::DrawPoint(cocos2d::Point p)
{
	auto sprite = Sprite::create("CloseNormal.png");
	sprite->setScale(10 / sprite->getContentSize().width);

	return sprite;
}

void CommonFuntion::ShowVec2(cocos2d::Vec2 v)
{
	log("x: %lf", v.x);
	log("y: %lf", v.y);
}

void CommonFuntion::ShowVec3(cocos2d::Vec3 v) 
{
	log("x: %lf", v.x);
	log("y: %lf", v.y);
	log("z: %lf", v.z);
}

void CommonFuntion::ShowRect(cocos2d::Rect r)
{
	log("x: %lf", r.origin.x);
	log("y: %lf", r.origin.y);
	log("w: %lf", r.size.width);
	log("h: %lf", r.size.height);
	log("xMax: %lf", r.origin.x + r.size.width);
	log("yMax: %lf", r.origin.y + r.size.height);
}

DrawNode* CommonFuntion::DrawRect(cocos2d::Rect rect, Color4F color)
{
	auto rectNode = DrawNode::create();
	Vec2 rectangle[4];
	rectangle[0] = Vec2(rect.origin);
	rectangle[1] = Vec2(rect.origin.x + rect.size.width, rect.origin.y);
	rectangle[2] = Vec2(rect.origin + rect.size);
	rectangle[3] = Vec2(rect.origin.x, rect.origin.y + rect.size.height);
	color.a = 0.3;
	rectNode->drawPolygon(rectangle, 4, color, 1, color);
	return rectNode;
}

DrawNode* CommonFuntion::DrawLine(cocos2d::Vec2 v1, cocos2d::Vec2 v2)
{
	auto draw = DrawNode::create(); 
	draw->drawLine(v1, v2, Color4F::RED); 
	return draw;
}

