//#ifndef __ANIMATOR_H__
//#define __ANIMATOR_H__
//
//#include "cocos2d.h"
//
//class Animator : public cocos2d::Node
//{
//public:
//	static Animator* createAnimator();
//	virtual bool init();
//
//	void SetNameAnimation(std::string nameSprite) { _nameSprite = nameSprite; };
//	void SetPath(std::string path) { _path = path; };
//	void SetFormat(std::string format) { _format = format; }
//
//	CREATE_FUNC(Animator);
//private:
//	std::string _nameSprite;
//	std::string _path;
//	std::string _format;
//
//	std::map< int, Animation* > _animations;
//protected:
//	cocos2d::Animation* createAnimation(std::string fileName, int numberFrames, float duration);
//	virtual bool loadSpriteAnimation();
//	void update(float dt);
//};
//
//#endif // !__ANIMATOR_H__
