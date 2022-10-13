//#include "Animator.h"
//
//USING_NS_CC;
//
//
//Animator* Animator::createAnimator()
//{
//	return Animator::create();
//}
//
//bool Animator::init()
//{
//	if (!Node::init()) { return false; }
//
//	if (!loadSpriteAnimation()) { return false; }
//
//
//	this->scheduleUpdate();
//	return true;
//}
//
//void Animator::update(float dt)
//{
//	int actionID = this->getParent()->getTag();
//	if (this->getActionByTag(actionID) != NULL) { return; }
//
//	/*if (_animations.find(actionID) == _animations.end())
//	{
//	}*/
//
//}
//
//Animation* Animator::createAnimation(std::string fileName, int numberFrames, float duration)
//{
//	Vector<SpriteFrame*> frames;
//
//	for (int i = 0; i < numberFrames; ++i)
//	{
//		std::string buffer;
//		buffer += fileName;
//		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(buffer);
//		if (frame == nullptr)
//		{
//			printf("Can't create frame name: %s\n", buffer.c_str());
//			break;
//		}
//		frames.pushBack(frame);
//	}
//
//	Animation* animation = Animation::createWithSpriteFrames(frames, duration);
//	return animation;
//}
//
//bool Animator::loadSpriteAnimation()
//{
//
//}