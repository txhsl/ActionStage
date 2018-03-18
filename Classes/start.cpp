#include "Start.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameScene.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* StartScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

bool StartScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto* BG = Sprite::create("UI/mainbg.png");
	BG->setPosition(480, 384);
	addChild(BG);
	//°´Å¥
	auto pMenu11 = MenuItemImage::create("UI/start.png", "UI/start2.png", this, menu_selector(StartScene::start));
	pMenu11->setScale(1.20f);
	auto menu11 = Menu::create(pMenu11, NULL, NULL);
	menu11->setPosition(250, 230);
	this->addChild(menu11);
	auto pMenu12 = MenuItemImage::create("UI/exit2.png", "UI/exit3.png", this, menu_selector(StartScene::EXIT));
	pMenu12->setScale(1.20f);
	auto menu12 = Menu::create(pMenu12, NULL, NULL);
	menu12->setPosition(250, 150);
	this->addChild(menu12);
	//×ÖÌå
	TTFConfig ttf("fonts/chunkmuffinhollowwide.ttf", 72);
	auto* lable = Label::createWithTTF(ttf, "Action");
	lable->enableShadow(Color4B::GRAY, Size(5, 5));
	lable->setPosition(260, 500);
	auto* lable2 = Label::createWithTTF(ttf, "Stage");
	lable2->enableShadow(Color4B::GRAY, Size(5, 5));
	lable2->setPosition(420, 400);
	addChild(lable);
	addChild(lable2);
	return true;
}

void StartScene::start(Ref* p)
{
	Director::getInstance()->replaceScene(TransitionFlipAngular::create(2.4f, GameScene::createScene()));
}

void StartScene::EXIT(Ref * p)
{
	Director::getInstance()->end();
}
