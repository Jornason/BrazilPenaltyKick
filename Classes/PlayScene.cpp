#include "PlayScene.h"
#include "ScoreScene.h"

#include "extensions/cocos-ext.h"
#include "spritebuilder/SpriteBuilder.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;

Scene* PlayScene::createScene()
{
	// 'scene' is an autorelease object
    auto scene = Scene::create();
	
    // 'layer' is an autorelease object
//    auto layer = PlayScene::create();
	auto layer = PlayScene::createFromUI();
	
	// add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
	return scene;
}

Node* PlayScene::createFromUI()
{
	spritebuilder::NodeLoaderLibrary * ccNodeLoaderLibrary = spritebuilder::NodeLoaderLibrary::getInstance();
	ccNodeLoaderLibrary->registerNodeLoader("PlayScene", PlaySceneLoader::loader());
	spritebuilder::CCBReader * ccbReader = new spritebuilder::CCBReader(ccNodeLoaderLibrary);
	auto node = ccbReader->readNodeGraphFromFile("PlayScene.ccbi");
	
	return node;
};

void PlayScene::onNodeLoaded(cocos2d::Node * pNode, spritebuilder::NodeLoader * pNodeLoader) {
	score = 0;
	seconds = 40;
	
	gameStatus = GAME_WAITING;
	
	scoreLable->setString(StringUtils::format("%d", score));
	timeLabel->setString(StringUtils::format("0:%02d", seconds));
	
	initBallPosition();
}

void PlayScene::initBallPosition()
{
	ball->setPosition(Point(165, 100));
}

void PlayScene::startPlay(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent) {
	ControlButton* button = (ControlButton*)sender;
	button->removeFromParentAndCleanup(true);
	
	gameStatus = GAME_RUNNING;
	
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(PlayScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	
	schedule(schedule_selector(PlayScene::checkTime), 1);
}

// on "init" you need to initialize your instance
//bool PlayScene::init()
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

//void PlayScene::menuCloseCallback(Ref* pSender)
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

bool PlayScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	return true;
}

void PlayScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
}

void PlayScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
	if (gameStatus != GAME_RUNNING)
	{
		return;
	}
	
	Point convertedLocation = touch->getLocation();
	
	// goalKepper action detect
	int moveTo = rand() / 8;
//	SpriteFrame* newFrame = SpriteFrameCache::getInstance()->get
//	goalKeeper->setspr(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("assets_graphics_2x_sprite_50_%d.png", moveTo)));
	
	// shooter action
	
	// ball action
//	MoveTo* moveTo = MoveTo::create(0.5f, convertedLocation);
//	ball->runAction(moveTo);
}

void PlayScene::checkTime(float dt)
{
	if (gameStatus != GAME_RUNNING)
	{
		return;
	}
	
	seconds -= 1;
	
	timeLabel->setString(StringUtils::format("0:%02d", seconds));
	
	if (seconds <= 5)
	{
		SimpleAudioEngine::getInstance()->playEffect("sounds/warning.mp3");
	}
	
	if (seconds <= 0)
	{
		gameStatus = GAME_STOP;
		
		auto scene = ScoreScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}