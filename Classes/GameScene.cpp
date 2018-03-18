#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include"physics\CCPhysicsShape.h"
#include "PauseScene.h"
#include "SimpleAudioEngine.h"
#include "FinalScene.h"

using namespace CocosDenshion;
using namespace cocostudio::timeline;

Scene* GameScene::createScene()
{
	//物理设置
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, -1000));

    auto layer = GameScene::create();
    scene->addChild(layer);
	
    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	Size visibleSize = Size(60 * 64, 24 * 64);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//背景载入
	BG = Sprite::create("bg.png");
	addChild(BG);

	//定义世界的边界  
	auto body = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(1.0f, 0.0f, 0.0f), 5.0f);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Vec2(60*64/2,24*64/2));
	edgeNode->setPhysicsBody(body);
	this->addChild(edgeNode);

	//创建人物
	_arrow = Sprite::create("arrow.png");
	_arrow->setScale(0.06f);
	addChild(_arrow);

	_player = Hero::create();
	_player->setPointer(_player);
	_player->setPosition(200, 200);
	addChild(_player);
	addChild(_player->getNode());

	//设置存档点
	_checkPoint.x = 200;
	_checkPoint.y = 200;

	//设置键盘监听
	this->_eventDispatcher->removeAllEventListeners();
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameScene::pressEventCheck, this);
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::releaseEventCheck, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//设置碰撞监听器
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//设置定时器
	schedule(schedule_selector(GameScene::update));

	//加载地图和方块
	map = TMXTiledMap::create("mapdesign1.tmx");
	TMXObjectGroup *objectGroup = map->objectGroupNamed("objects layer");
	ValueVector objects = objectGroup->getObjects();
	for (auto& rigid : objects)
	{
		ValueMap& dict_1 = rigid.asValueMap();
		if (dict_1["bord"].asInt() == 1)
		{
			auto x = dict_1["x"].asInt();
			auto y = dict_1["y"].asInt();
			float width = dict_1["width"].asFloat();
			float height = dict_1["height"].asFloat();
			this->createblock(Point(x + width / 2, y + height / 2));
			_block->setScale(width / 64, height / 64);
		}
	}

	for (auto& crak : objects)
	{
		ValueMap& dict_2 = crak.asValueMap();
		if (dict_2["breaker"].asInt() == 2)
		{
			auto x = dict_2["x"].asInt();
			auto y = dict_2["y"].asInt();
			this->createblock(Point(x + 32, y + 96));
			_block->setTag(3);
			_blockNode = Sprite::create("breaker.png");
			_block->setPicture(_blockNode);
			_blockNode->setPosition(Point(x + 32, y + 96));
			addChild(_blockNode);

		}
	}

	for (auto& bounce : objects)
	{
		ValueMap& dict_3 = bounce.asValueMap();
		if (dict_3["bouncer"].asInt() == 3)
		{
			auto x = dict_3["x"].asInt();
			auto y = dict_3["y"].asInt();
			this->createblock(Point(x + 32, y + 96));
			_block->setTag(4);
			_blockNode = Sprite::create("bouncer.png");
			_block->setPicture(_blockNode);
			_blockNode->setPosition(Point(x + 32, y + 96));
			addChild(_blockNode);
		}
	}
	for (auto& slide : objects)
	{
		ValueMap& dict_4 = slide.asValueMap();
		if (dict_4["slider"].asInt() == 4)
		{
			auto x = dict_4["x"].asInt();
			auto y = dict_4["y"].asInt();
			this->createblock(Point(x + 32, y + 96));
			_block->setTag(5);
			_blockNode = Sprite::create("slider.png");
			_block->setPicture(_blockNode);
			_blockNode->setPosition(Point(x + 32, y + 96));
			addChild(_blockNode);
		}
	}
	for (auto& warter : objects)
	{
		ValueMap& dict_5 = warter.asValueMap();
		if (dict_5["warter"].asInt() == 5)
		{
			auto x = dict_5["x"].asInt();
			auto y = dict_5["y"].asInt();
			float _width = dict_5["width"].asFloat();
			float _height = dict_5["height"].asFloat();
			this->createblock(Point(x + _width / 2, y + _height / 2));
			_block->setTag(6);
			_block->setScale(_width / 64, _height / 64);
		}
	}

	for (int i = 1; i <= 50; i++)
	{
		auto dict_6 = objectGroup->getObject("line" + Value(i).asString());
		auto x = dict_6["x"].asInt();
		auto y = dict_6["y"].asInt();
		float _width1 = dict_6["width"].asFloat();
		float _height1 = dict_6["height"].asFloat();
		this->createblock(Point(x + _width1 / 2, y + _height1 / 2));
		_block->setTag(7);
		_block->setScale(_width1 / 64, _height1 / 64);
		block_line.insert(i, _block);
	}
	for (auto& spike : objects)
	{
		ValueMap& dict_7 = spike.asValueMap();
		if (dict_7["spiker"].asInt() == 7)
		{
			auto x = dict_7["x"].asInt();
			auto y = dict_7["y"].asInt();
			float _width2 = dict_7["width"].asFloat();
			float _height2 = dict_7["height"].asFloat();
			this->createblock(Point(x + _width2 / 2, y + _height2 / 2));
			_block->setTag(8);
			_block->setScale(_width2 / 64, _height2 / 64);
		}
	}
	for (auto& diamond : objects)
	{
		ValueMap& dict_8 = diamond.asValueMap();
		if (dict_8["diamond"].asInt() == 8)
		{
			auto x = dict_8["x"].asInt();
			auto y = dict_8["y"].asInt();
			this->createblock(Point(x + 32, y + 96));
			_block->setTag(9);
			_blockNode = Sprite::create("diamond.png");
			_block->setPicture(_blockNode);
			_block->setScale(0.45f, 0.35f);
			_blockNode->setPosition(Point(x + 32, y + 96));
			addChild(_blockNode);
		}
	}
	for (auto& checkpoint : objects)
	{
		ValueMap& dict_9 = checkpoint.asValueMap();
		if (dict_9["checkpoint"].asInt() == 9)
		{
			auto x = dict_9["x"].asInt();
			auto y = dict_9["y"].asInt();
			this->createblock(Point(x + 32, y + 64));
			_block->setTag(10);
			_blockNode = Sprite::create("checkpoint.png");
			_block->setPicture(_blockNode);
			_blockNode->setPosition(Point(x + 32, y + 96));
			_block->setScale(0.5f, 0.05f);
			_finishPoint = _block->getPosition();
			addChild(_blockNode);
		}
	}
	for (auto& enemy : objects)
	{
		ValueMap& dict_10 = enemy.asValueMap();
		if (dict_10["enemy"].asInt() == 10)
		{
			auto x = dict_10["x"].asInt();
			auto y = dict_10["y"].asInt();
			_enemy = Enemy::create();
			_enemy->setPointer(_enemy);
			_enemy->setPosition(Point(x + 32, y + 64));
			_enemyList.pushBack(_enemy);
			addChild(_enemy);
			addChild(_enemy->getNode());
		}
	}
	addChild(map);

	//按钮生成
	UIlayer = Layer::create();
	auto pMenu1 = MenuItemImage::create("exit.png", "exit.png", this, menu_selector(GameScene::exit));
	pMenu1->setScale(1.2f);
	auto menu1 = Menu::create(pMenu1, NULL, NULL);
	menu1->setPosition(1400 / 4, 758 / 4);
	UIlayer->addChild(menu1);
	auto pMenu2 = MenuItemImage::create("pause.png", "pause.png", this, menu_selector(GameScene::pause));
	pMenu2->setScale(1.0f);
	auto menu2 = Menu::create(pMenu2, NULL, NULL);
	menu2->setPosition(1220 / 4, 758 / 4);
	UIlayer->addChild(menu2);
	UIlayer->setPosition(100, 400);
	addChild(UIlayer);

	//背景音乐
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("backgroundmusic.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("backgroundmusic.mp3", true);

	return true;
}

void GameScene::createblock(Point pos)
{
	//物理方块生成
	_block = Block::create();
	_block->setPointer(_block);
	_block->setPosition(pos);
	addChild(_block);
}

void GameScene::createKunai(Hero* role)
{
	//飞镖生成
	Vec2 pos1 = role->getPosition();
	Vec2 pos2 = _player->getPosition();

	if (role->throwJudge() && fabs(pos1.x - pos2.x)<350 && fabs(pos1.y - pos2.y)<350)
	{
		Sprite* _Kunai = Sprite::create("Kunai.png");
		auto* kunai = PhysicsBody::createBox(_Kunai->getContentSize());
		_Kunai->setPhysicsBody(kunai);
		_Kunai->getPhysicsBody()->setGravityEnable(false);
		_Kunai->getPhysicsBody()->setContactTestBitmask(1);
		_Kunai->getPhysicsBody()->setRotationEnable(false);
		_Kunai->setTag(0);
		float x = role->getPositionX();
		float y = role->getPositionY();
		if (role->getDirection())
		{
			Vec2 v;
			v.x = 1000;
			v.y = 0;
			_Kunai->setScale(0.3f);
			_Kunai->getPhysicsBody()->setVelocity(v);
			_Kunai->setPosition(x + 70, y);
		}
		else
		{
			Vec2 v;
			v.x = -1000;
			v.y = 0;
			_Kunai->setScale(-0.3f);
			_Kunai->getPhysicsBody()->setVelocity(v);
			_Kunai->setPosition(x - 70, y);
		}
		addChild(_Kunai);
	}
}

void GameScene::resetPlayer(float dt)
{
	_player->reset(_checkPoint);
}

void GameScene::recreateBlock(float dt)
{
	//方块重现
	while (!_blockRecreateList.empty())
	{
		_block = *_blockRecreateList.begin();
		_block->getPhysicsBody()->setEnabled(true);
		_block->getPicture()->setVisible(true);
		_blockRecreateList.erase(0);
	}
}

void GameScene::deleteEnemy(float dt)
{
	//批量删除死亡敌人
	while (!_enemyDeleteList.empty())
	{
		_enemy = *_enemyDeleteList.begin();
		_enemy->setPosition(-100, -100);
		_enemy->getPhysicsBody()->removeFromWorld();
		_enemy->removeFromParentAndCleanup(true);
		_enemyDeleteList.erase(0);
	}
}

void GameScene::pressEventCheck(EventKeyboard::KeyCode code, Event* evt)
{
	//键盘控制
	switch ((int)code)
	{
	case 26:
		_player->moveLeft();
		break;
	case 27:
		_player->moveRight();
		break;
	case 28:
		_player->jump();
		break;
	case 124:
		for (_ienemy = _enemyList.begin(); _ienemy != _enemyList.end(); _ienemy++)
		{
			if (_player->attackJudge(*_ienemy))
			{
				_enemyDeleteList.pushBack(*_ienemy);
				scheduleOnce(schedule_selector(GameScene::deleteEnemy), 2.5f);
			}
		}
		break;
	case 142:
	{
		if (_player->throwJudge())
		{
			createKunai(_player);
		}
		break;
	}
	case 127:
		_player->slide();
		break;
	default:
		break;
	}
}

void GameScene::releaseEventCheck(EventKeyboard::KeyCode code, Event* evt)
{
	Vec2 velocity = _player->getPhysicsBody()->getVelocity();
	switch ((int)code)
	{
	case 26:
	case 27:
		_player->stop();
		break;
	case 28:
	default:
		break;
	}
}

bool GameScene::onContactBegin(PhysicsContact& contact)
{
	//碰撞检测
	int type1 = contact.getShapeA()->getBody()->getNode()->getTag();
	int type2 = contact.getShapeB()->getBody()->getNode()->getTag();
	auto hero1 = (Hero*)contact.getShapeA()->getBody()->getNode();
	auto hero2 = (Hero*)contact.getShapeB()->getBody()->getNode();
	auto sprite1 = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto sprite2 = (Sprite*)contact.getShapeB()->getBody()->getNode();
	auto block1 = (Block*)contact.getShapeA()->getBody()->getNode();
	auto block2 = (Block*)contact.getShapeB()->getBody()->getNode();
	//跳跃重置
	if (type1 == 1)
	{
		hero1->jumpReset();
	}
	if (type2 == 1)
	{
		hero2->jumpReset();
	}
	//飞镖
	if (type1 == 0) 
	{
		if (type2 == 1) 
		{
			hero2->die();
			if (hero2 == _player)
			{
				scheduleOnce(schedule_selector(GameScene::resetPlayer), 2.5f);
			}
			for (_ienemy = _enemyList.begin(); _ienemy != _enemyList.end(); _ienemy++)
			{
				if (hero2 == *_ienemy)
				{
					_enemyDeleteList.pushBack(*_ienemy);
					scheduleOnce(schedule_selector(GameScene::deleteEnemy), 2.5f);
				}
			}
			_deleteList.pushBack(sprite1);
		}
		else
		{
			_deleteList.pushBack(sprite1);
		}
	}
	if (type2 == 0) 
	{
		if (type1 == 1) 
		{
			hero1->die();
			if (hero1 == _player)
			{
				scheduleOnce(schedule_selector(GameScene::resetPlayer), 2.5f);
			}
			for (_ienemy = _enemyList.begin(); _ienemy != _enemyList.end(); _ienemy++)
			{
				if (hero1 == *_ienemy)
				{
					_enemyDeleteList.pushBack(*_ienemy);
					scheduleOnce(schedule_selector(GameScene::deleteEnemy), 2.5f);
				}
			}
			_deleteList.pushBack(sprite2);
		}
		else
		{
			_deleteList.pushBack(sprite2);
		}
	}
	//消失的方块
	if (type1 == 3)
	{
		if (type2 == 1) 
		{ 
			_blockRecreateList.pushBack(block1);
			scheduleOnce(schedule_selector(GameScene::recreateBlock), 2.5f);
			block1->getPhysicsBody()->setEnabled(false);
			block1->getPicture()->setVisible(false);
		}
	}
	if (type2 == 3)
	{
		if (type1 == 1)
		{
			_blockRecreateList.pushBack(block2);
			scheduleOnce(schedule_selector(GameScene::recreateBlock), 2.5f);
			block2->getPhysicsBody()->setEnabled(false);
			block2->getPicture()->setVisible(false);
		}
	}
	//死亡方块
	if (type1 == 6 || type1 == 8) 
	{
		if (type2 == 1) 
		{
			hero2->die();
			if (hero2 == _player)
			{
				scheduleOnce(schedule_selector(GameScene::resetPlayer), 2.5f);
			}
			for (_ienemy = _enemyList.begin(); _ienemy != _enemyList.end(); _ienemy++)
			{
				if (hero2 == *_ienemy)
				{
					_enemyDeleteList.pushBack(*_ienemy);
					scheduleOnce(schedule_selector(GameScene::deleteEnemy), 2.5f);
				}
			}
		}
	}
	if (type2 == 6 || type2 == 8) 
	{
		if (type1 == 1)
		{
			hero1->die();
			if (hero1 == _player)
			{
				scheduleOnce(schedule_selector(GameScene::resetPlayer), 2.5f);
			}
			for (_ienemy = _enemyList.begin(); _ienemy != _enemyList.end(); _ienemy++)
			{
				if (hero1 == *_ienemy)
				{
					_enemyDeleteList.pushBack(*_ienemy);
					scheduleOnce(schedule_selector(GameScene::deleteEnemy), 2.5f);
				}
			}
		}
	}
	//弹射方块
	if (type1 == 4)
	{
		if (type2 == 1) 
		{
			if (hero2->getDirection()) 
			{
				Vec2 v;
				v.x = -2000;
				v.y = 1000;
				hero2->getPhysicsBody()->setVelocity(v);
			}
			else
			{
				Vec2 v;
				v.x = 2000;
				v.y = 1000;
				hero2->getPhysicsBody()->setVelocity(v);
			}
		}
	}
	if (type2 == 4) 
	{
		if (type1 == 1)
		{
			if (hero1->getDirection()) 
			{
				Vec2 v;
				v.x = -2000;
				v.y = 1000;
				hero1->getPhysicsBody()->setVelocity(v);
			}
			else
			{
				Vec2 v;
				v.x = 2000;
				v.y = 1000;
				hero1->getPhysicsBody()->setVelocity(v);
			}
		}
	}
	//冰块
	if (type1 == 5)
	{
		if (type2 == 1) 
		{
			if (hero2->getDirection())
			{
				Vec2 force;
				force.x = -10000;
				force.y = 0;
				hero2->getPhysicsBody()->applyForce(force);
			}
			else {
				Vec2 force;
				force.x = 10000;
				force.y = 0;
				hero2->getPhysicsBody()->applyForce(force);
			}
		}
	}
	if (type2 == 5)
	{
		if (type1 == 1) 
		{
			if (hero1->getDirection()) 
			{
				Vec2 force;
				force.x = -10000;
				force.y = 0;
				hero1->getPhysicsBody()->applyForce(force);
			}
			else
			{
				Vec2 force;
				force.x = 10000;
				force.y = 0;
				hero1->getPhysicsBody()->applyForce(force);
			}
		}
	}
	//钻石
	if (type1 == 9)
	{
		if (type2 == 1 && hero2 == _player)
		{
			_blockDeleteList.pushBack(block1);
			_score++;
		}
	}
	if (type2 == 9)
	{
		if (type1 == 1 && hero1 == _player)
		{
			_blockDeleteList.pushBack(block2);
			_score++;
		}
	}
	//存档点
	if (type1 == 10)
	{
		if (type2 == 1 && hero2 == _player)
		{
			_checkPoint = Vec2(block1->getPosition());
			if (_finishPoint == _checkPoint)
			{
				auto checkpoint_up = Sprite::create("finalpoint_up.png");
				checkpoint_up->setPosition(_checkPoint.x + 24, _checkPoint.y + 32);
				addChild(checkpoint_up);
				//完成游戏
				Director::getInstance()->replaceScene(TransitionFlipAngular::create(2.4f, FinalScene::createScene(_score,_timeCounter)));
			}
			else
			{
				auto checkpoint_up = Sprite::create("checkpoint_up.png");
				checkpoint_up->setPosition(_checkPoint.x + 24, _checkPoint.y + 32);
				addChild(checkpoint_up);
			}
			_blockDeleteList.pushBack(block1);
		}
	}
	if (type2 == 10)
	{
		if (type1 == 1 && hero1 == _player)
		{
			_checkPoint = Vec2(block2->getPosition());
			if (_finishPoint == _checkPoint)
			{
				auto checkpoint_up = Sprite::create("finalpoint_up.png");
				checkpoint_up->setPosition(_checkPoint.x + 24, _checkPoint.y + 32);
				addChild(checkpoint_up);
				//完成游戏
				Director::getInstance()->replaceScene(TransitionFlipAngular::create(2.4f, FinalScene::createScene(_score, _timeCounter)));
			}
			else
			{
				auto checkpoint_up = Sprite::create("checkpoint_up.png");
				checkpoint_up->setPosition(_checkPoint.x + 24, _checkPoint.y + 32);
				addChild(checkpoint_up);
			}
			_blockDeleteList.pushBack(block2);
		}
	}
	return true;
}

void GameScene::update(float dt)
{
	//执行AI
	if (_timeOrder % 2000 == 0)
	{
		for (_ienemy = _enemyList.begin(); _ienemy != _enemyList.end(); _ienemy++)
		{
			createKunai(*_ienemy);
		}
	}
	if (_timeOrder++ % 80 == 0)
	{
		for (_ienemy = _enemyList.begin(); _ienemy != _enemyList.end(); _ienemy++)
		{
			_enemy = *_ienemy;
			if (_enemy->catchHero(_player))
			{
				scheduleOnce(schedule_selector(GameScene::resetPlayer), 2.5f);
			}
		}
		_timeOrder -= 80;
	}

	//设置精灵跟随
	for (_ienemy = _enemyList.begin(); _ienemy != _enemyList.end(); _ienemy++)
	{
		_enemy = *_ienemy;
		_enemy->getNode()->setPosition(_enemy->getPosition());
	}
	_timeCounter += dt;

	float x1 = _player->getPositionX();
	float y1 = _player->getPositionY();
	_player->getNode()->setPosition(x1, y1);
	_arrow->setPosition(x1, y1 + 50);
	Point p = setViewPointCenter(_player->getPosition());
	int x2 = p.x;
	int y2 = p.y;
	UIlayer->setPosition(x2 + 100, y2 + 100);
	BG->setPosition(x2, y2);

	//检测腾空
	_player->jumpJudge();
	for (_ienemy = _enemyList.begin(); _ienemy != _enemyList.end(); _ienemy++)
	{
		_enemy = *_ienemy;
		_enemy->jumpJudge();
	}

	//清理删除列表
	while (!_deleteList.empty())
	{
		auto deleteSprite = _deleteList.back();
		deleteSprite->getPhysicsBody()->removeFromWorld();
		deleteSprite->removeFromParentAndCleanup(true);
		_deleteList.popBack();
	}
	while (!_blockDeleteList.empty())
	{
		auto deleteSprite = _blockDeleteList.back();
		deleteSprite->remove();
		_blockDeleteList.popBack();
	}

	for (int j = 1; j <= block_line.size(); j++)
	{
		Block *temp = block_line.at(j);
		temp->get_through(_player->getPosition(), temp);
	}
}

Point GameScene::setViewPointCenter(Point position) 
{
	//设置视野中心
	auto winSize = Director::getInstance()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (map->getMapSize().width * this->map->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (map->getMapSize().height * this->map->getTileSize().height) - winSize.height / 2);
	auto actualPosition = Point(x, y);

	auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
	auto viewPoint = centerOfView - actualPosition;
	this->setPosition(viewPoint);
	auto p = Vec2(x, y);
	return p;
}

void GameScene::pause(Ref* pSender)
{
	Director::getInstance()->pushScene(PauseScene::createScene(_timeCounter));
}

void GameScene::exit(Ref * pSender)
{
	Director::getInstance()->end();
}
