#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Block.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	void createblock(cocos2d::Point pos);
	void recreateBlock(float dt);
	void createKunai(Hero* role);
	Point setViewPointCenter(Point position);
	void resetPlayer(float dt);
	void deleteEnemy(float dt);

	void pressEventCheck(EventKeyboard::KeyCode code, Event* evt);
	void releaseEventCheck(EventKeyboard::KeyCode code, Event* evt);
	bool onContactBegin(PhysicsContact& contact);
	void update(float dt);

	void pause(Ref* pSender);
	void exit(Ref* pSender);

    CREATE_FUNC(GameScene);
private:
	Hero* _player;
	Enemy* _enemy;
	Vector<Enemy*> _enemyList;//敌人列表
	Vector<Enemy*>::iterator _ienemy;
	Vector<Enemy*> _enemyDeleteList;//敌人删除列表
	Block* _block;
	Vector<Block*> _blockRecreateList;//方块重建列表
	Vector<Block*> _blockDeleteList;//方块删除列表
	Sprite* _arrow;
	Sprite* _Kunai;
	Sprite* _blockNode;
	TMXTiledMap*map;
	Vec2 _checkPoint;
	Vec2 _finishPoint;
	int _score = 0;
	int _timeOrder = 0;
	Map <int, Block*> block_line;//穿透边列表
	Vector<Sprite*> _deleteList;//飞镖删除列表
	Layer* UIlayer;
	Sprite* BG;
	double _timeCounter = 0;
};

#endif // __GAME_SCENE_H__
