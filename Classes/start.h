#ifndef __START_SCENE_H__
#define __START_SCENE_H__
#include "cocos2d.h"

USING_NS_CC;

class StartScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void start(Ref* p);
	void EXIT(Ref* p);
	CREATE_FUNC(StartScene);
};
#endif
