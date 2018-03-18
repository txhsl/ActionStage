#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"  
#include "cocostudio/CocoStudio.h"

using namespace cocostudio::timeline;

USING_NS_CC;

class Hero : public Sprite
{
public:
	//创建create方法，自动调用init和返回Hero类  
	virtual bool init();
	void setPointer(Hero* role) { _role = role; };
	Node* getNode() { return _myNode; };
	bool getDirection() { return _direction; }
	void moveLeft();
	void moveRight();
	void slide();
	void jump();
	void jumpReset();
	void jumpMinus();
	void stop();
	void attack() { _action->gotoFrameAndPlay(0, 49, false); }
	void jumpAttack() { _action->gotoFrameAndPlay(200, 249, false); }
	void Throw() { _action->gotoFrameAndPlay(400, 449, false); }
	void jumpThrow() { _action->gotoFrameAndPlay(250, 299, false); }
	void idle(){ _action->gotoFrameAndPlay(100, 149, true); }
	void die();
	void jumpJudge();
	bool attackJudge(Hero* _role);
	bool throwJudge();
	void reset(Vec2 checkPoint);
	CREATE_FUNC(Hero);
protected:
	Hero* _role;
	bool _direction;         //0代表左 1代表右
	bool _movable = 1;        //是否存活
	int _jumpTime = 0;
	ActionTimeline* _action;
	Node* _myNode;
};

class Enemy : public Hero
{
public:
	virtual bool init();
	bool catchHero(Hero* role);
	CREATE_FUNC(Enemy);
protected:
	int _num;
};
#endif