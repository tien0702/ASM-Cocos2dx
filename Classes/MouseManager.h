#ifndef __MOUSE_MANAGER_H__
#define __MOUSE_MANAGER_H__

#include "cocos2d.h"

class MouseManager : public cocos2d::Node
{
public:

private:
	static MouseManager* createMouseManager();
	bool init();
};


#endif // !__MOUSE_MANAGER_H__
