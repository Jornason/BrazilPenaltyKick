#include "ScoreScene.h"

#include "extensions/cocos-ext.h"
#include "spritebuilder/SpriteBuilder.h"

#include "PlayScene.h"
#include "MainMenuLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

Scene* ScoreScene::createScene()
{
	// 'scene' is an autorelease object
    auto scene = Scene::create();
	
    // 'layer' is an autorelease object
//    auto layer = ScoreScene::create();
	auto layer = ScoreScene::createFromUI();
	
	// add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
	return scene;
}

Node* ScoreScene::createFromUI()
{
	spritebuilder::NodeLoaderLibrary * ccNodeLoaderLibrary = spritebuilder::NodeLoaderLibrary::getInstance();
	ccNodeLoaderLibrary->registerNodeLoader("ScoreScene", ScoreSceneLoader::loader());
	spritebuilder::CCBReader * ccbReader = new spritebuilder::CCBReader(ccNodeLoaderLibrary);
	auto node = ccbReader->readNodeGraphFromFile("ScoreScene.ccbi");
	return node;
};

void ScoreScene::play(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent) {
	auto scene = PlayScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void ScoreScene::goHome(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent) {
	auto scene = MainMenuLayer::createScene();
	Director::getInstance()->replaceScene(scene);
}

// on "init" you need to initialize your instance
//bool ScoreScene::init()
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

//void ScoreScene::menuCloseCallback(Ref* pSender)
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
