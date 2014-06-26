#include "MainMenuLayer.h"

#include "extensions/cocos-ext.h"
#include "spritebuilder/SpriteBuilder.h"
#include "SimpleAudioEngine.h"

#include "global.h"
#include "cocos2dx/util.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;

Scene* MainMenuLayer::createScene()
{
	// 'scene' is an autorelease object
    auto scene = Scene::create();
	
    // 'layer' is an autorelease object
//    auto layer = MainMenuLayer::create();
	auto layer = MainMenuLayer::createFromUI();
	
	// add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
	return scene;
}

Node* MainMenuLayer::createFromUI()
{
	spritebuilder::NodeLoaderLibrary * ccNodeLoaderLibrary = spritebuilder::NodeLoaderLibrary::getInstance();
	ccNodeLoaderLibrary->registerNodeLoader("MainMenuLayer", MainMenuLayerLoader::loader());
	spritebuilder::CCBReader * ccbReader = new spritebuilder::CCBReader(ccNodeLoaderLibrary);
	auto node = ccbReader->readNodeGraphFromFile("MainMenuLayer.ccbi");
	
	return node;
}

void MainMenuLayer::onNodeLoaded(cocos2d::Node * pNode, spritebuilder::NodeLoader * pNodeLoader) {
	CCLOG("UI onnodeloaded");
	
	musicOn = 1;
	
	initDb();
	loadSetting();
	
	musicOn = atoi(drSetting["music_on"].c_str());
	
	// 控制音效开关
	setSwitchSoundButtonSprite();
}

void MainMenuLayer::switchSound(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent) {
	if (musicOn == 1)
	{
		musicOn = 0;
	}
	else
	{
		musicOn = 1;
	}
	
	saveSetting(musicOn);
	setSwitchSoundButtonSprite();
}

void MainMenuLayer::setSwitchSoundButtonSprite()
{
	SpriteFrame *musicSpriteFrame;
	if (musicOn == 1)
	{
		musicSpriteFrame = getSpriteFrame("assets_graphics_2x_sprite_60_0.png");
		
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(100);
		SimpleAudioEngine::getInstance()->setEffectsVolume(100);
	}
	else
	{
		musicSpriteFrame = getSpriteFrame("assets_graphics_2x_sprite_60_1.png");
		
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
		SimpleAudioEngine::getInstance()->setEffectsVolume(0);
	}
	
	switchSoundButton->setBackgroundSpriteFrameForState(musicSpriteFrame, Control::State::NORMAL);
	switchSoundButton->setBackgroundSpriteFrameForState(musicSpriteFrame, Control::State::HIGH_LIGHTED);
	switchSoundButton->setBackgroundSpriteFrameForState(musicSpriteFrame, Control::State::DISABLED);
	switchSoundButton->setBackgroundSpriteFrameForState(musicSpriteFrame, Control::State::SELECTED);
}

// on "init" you need to initialize your instance
//bool MainMenuLayer::init()
//{
//    //////////////////////////////
//    // 1. super init first
//    if ( !Layer::init() )
//    {
//        return false;
//    }
//
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Point origin = Director::getInstance()->getVisibleOrigin();
//
//    return true;
//}

//void MainMenuLayer::menuCloseCallback(Ref* pSender)
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//    return;
//#endif
//
//    Director::getInstance()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
//}
