#include "FinalScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "start.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocostudio::timeline;

int _Time;
int _score;
Scene* FinalScene::createScene(int score,double time)
{
	_Time = time;
	_score = score;
	auto scene = Scene::create();
	auto layer = FinalScene::create();
	scene->addChild(layer);

	return scene;
}

bool FinalScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//背景
	auto* BG = Sprite::create("UI/background1.png");
	BG->setPosition(640, 384);
	addChild(BG);
	//结算
	int sec = _Time % 60;
	int min = (_Time - sec) / 60;
	char Time[10];
	sprintf(Time, "%d:%d", min, sec);
	char Score[10];
	sprintf(Score, "%d", _score);
	TTFConfig ttf("fonts/chunkmuffinhollowwide.ttf", 66);
	auto* lable = Label::createWithTTF(ttf, Time);
	lable->enableShadow(Color4B::GRAY, Size(5, 5));
	lable->setPosition(380, 400);
	addChild(lable);
	auto* lable2 = Label::createWithTTF(ttf, Score);
	lable2->enableShadow(Color4B::GRAY, Size(5, 5));
	lable2->setPosition(600, 300);
	addChild(lable2);
	auto* lable3 = Label::createWithTTF(ttf, "Time :");
	lable3->enableShadow(Color4B::GRAY, Size(5, 5));
	lable3->setPosition(180, 400);
	addChild(lable3);
	auto* lable4 = Label::createWithTTF(ttf, "Score :");
	lable4->enableShadow(Color4B::GRAY, Size(5, 5));
	lable4->setPosition(400, 300);
	addChild(lable4);

	//回主菜单按钮
	auto pMenu = MenuItemImage::create("UI/menu.png", "UI/menu2.png", this, menu_selector(FinalScene::Restart));
	pMenu->setScale(1.2f);
	auto menu = Menu::create(pMenu, NULL, NULL);
	menu->setPosition(300, 200);
	this->addChild(menu);
	
	return true;
}

void FinalScene::Restart(Ref* p)
{
	Director::getInstance()->replaceScene(TransitionFlipAngular::create(2.4f, StartScene::createScene()));
}

