#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;
class PauseScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(double time);
    virtual bool init();
    CREATE_FUNC(PauseScene);
	void Restart(Ref* p);
	void louder(Ref* p);
	void lighter(Ref* p);
	void EXIT(Ref* p);
};
#endif

