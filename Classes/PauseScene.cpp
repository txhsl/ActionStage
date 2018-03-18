#include "PauseScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;

using namespace cocostudio::timeline;

int _time;
Scene* PauseScene::createScene(double time)
{
	_time = (int)time;
	auto scene = Scene::create();
    auto layer = PauseScene::create();
	scene->addChild(layer);
	
    return scene;
}

bool PauseScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    //背景
	auto* BG = Sprite::create("UI/background1.png");
	BG->setPosition(640, 384);
    addChild(BG);
	//时间
	int sec = _time % 60;
	int min = (_time - sec) / 60;
	char Time[10];
	sprintf(Time,"%d:%d", min, sec);
	TTFConfig ttf("fonts/chunkmuffinhollowwide.ttf",72);
	auto* lable = Label::createWithTTF(ttf, Time);
	lable->enableShadow(Color4B::GRAY, Size(5, 5));
	lable->setPosition(280, 400);
	addChild(lable);
	//继续游戏按钮
	auto pMenu = MenuItemImage::create("UI/resume.png", "UI/resume2.png", this, menu_selector(PauseScene::Restart));
	pMenu->setScale(0.7f);
	auto menu = Menu::create(pMenu, NULL, NULL);
	menu->setPosition(300, 300);
	this->addChild(menu);
    //音乐按钮
	auto* voice = Sprite::create("UI/voiceon.png");
	voice->setPosition(500, 360);
	voice->setScale(0.18f);
	addChild(voice);
	auto myListener = EventListenerTouchOneByOne::create();
	myListener->setSwallowTouches(true);
	myListener->onTouchBegan = [](Touch* touch, Event* event)
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
			//log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
			{
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
				Texture2D* texture = TextureCache::sharedTextureCache()->addImage("UI/voiceoff.png");
				target->setTexture(texture);
			}
			else
			{
				SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
				Texture2D* texture = TextureCache::sharedTextureCache()->addImage("UI/voiceon.png");
				target->setTexture(texture);
			}
			return true;
		}
		return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, voice);
	
	auto pMenu1 = MenuItemImage::create("UI/plus.png", "UI/plus2.png", this, menu_selector(PauseScene::louder));
	pMenu1->setScale(0.08f);
	auto menu1 = Menu::create(pMenu1, NULL, NULL);
	menu1->setPosition(640, 380);
	this->addChild(menu1);
	auto pMenu2 = MenuItemImage::create("UI/minus.png", "UI/minus2.png", this, menu_selector(PauseScene::lighter));
	pMenu2->setScale(0.08f);
	auto menu2 = Menu::create(pMenu2, NULL, NULL);
	menu2->setPosition(640, 320);
	this->addChild(menu2);
	auto pMenu3 = MenuItemImage::create("UI/exit2.png", "UI/exit3.png", this, menu_selector(PauseScene::EXIT));
	pMenu3->setScale(0.7f);
	auto menu3 = Menu::create(pMenu3, NULL, NULL);
	menu3->setPosition(300, 200);
	this->addChild(menu3);
	return true;
	

}

void PauseScene::Restart(Ref* p)
{
	Director::getInstance()->popScene();
}

void PauseScene::louder(Ref* p)
{
	float x = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(x+0.1);
}

void PauseScene::lighter(Ref * p)
{
	float x = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(x-0.1);
}

void PauseScene::EXIT(Ref * p)
{
	Director::getInstance()->end();
}
