#ifndef __FINAL_SCENE_H__
#define __FINAL_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class FinalScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(int score,double time);
	virtual bool init();
	CREATE_FUNC(FinalScene);
	void Restart(Ref* p);
};

#endif

