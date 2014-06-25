#include "PlayScene.h"
#include "ScoreScene.h"

#include "extensions/cocos-ext.h"
#include "spritebuilder/SpriteBuilder.h"
#include "SimpleAudioEngine.h"

#include "cocos2dx/util.h"
#include "util/util.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;

enum {BALL_SHAPE = 1, DOOR_SHAPE = 2, KEEPER_SHAPE = 4, TIME_BONUS_SHAPE = 8, SCORE_BONUS_SHAPE = 16};

Scene* PlayScene::createScene()
{
	// 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	
    // 'layer' is an autorelease object
//    auto layer = PlayScene::create();
	auto layer = PlayScene::createFromUI();
	
	scene->getPhysicsWorld()->setGravity(Vect::ZERO);
//	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
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
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	score = 0;
	seconds = 40;
	
	gameStatus = GAME_WAITING;
	
	bonusSprite = NULL;
	
	scoreLable->setString(StringUtils::format("%d", score));
	timeLabel->setString(StringUtils::format("0:%02d", seconds));
	
	initBallPosition();
	
	PhysicsBody* ballBody = PhysicsBody::createCircle(13.00);
	ballBody->setCategoryBitmask(BALL_SHAPE);
	ballBody->setContactTestBitmask(DOOR_SHAPE | TIME_BONUS_SHAPE | SCORE_BONUS_SHAPE);
	ballBody->setCollisionBitmask(0);
	ball->setPhysicsBody(ballBody);
	
	Point points[] = {Point(-112.08, -39.02), Point(-112.10, 39.72), Point(114.62, 39.45), Point(114.62, -39.02)};
	auto doorBody = PhysicsBody::createPolygon(points, 4);
	doorBody->setCategoryBitmask(DOOR_SHAPE);
	doorBody->setContactTestBitmask(BALL_SHAPE);
	doorBody->setCollisionBitmask(0);
	door->setPhysicsBody(doorBody);
	
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void PlayScene::initBallPosition()
{
	ball->setPosition(Point(165, 100));
	
	goalKeeper->setPosition(160, 372);
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

bool PlayScene::onContactBegin(const cocos2d::PhysicsContact &contact)
{
	auto shapea = contact.getShapeA();
	auto shapeb = contact.getShapeB();
	
	int hitScore = 0, hitTime = 0;
	
	if ((shapea->getCategoryBitmask() == DOOR_SHAPE) || (shapeb->getCategoryBitmask() == DOOR_SHAPE))
	{
		hitScore = 20;
		
		showTips("assets_graphics_2x_sprite_62_0.png");
	}
	else if ((shapea->getCategoryBitmask() == SCORE_BONUS_SHAPE) || (shapeb->getCategoryBitmask() == SCORE_BONUS_SHAPE))
	{
		// 多个碰撞会发生多次，所以这里不能把龙门的碰撞分数加进来，因为上面的碰撞检测已经加上
		hitScore = genRandom(5, 20);
		
		Texture2D *bonusTexture = Director::getInstance()->getTextureCache()->addImage("assets_graphics_2x_sprite_41_2.png");
		Vector<SpriteFrame*> animateFrames;
		addAnimationFrame(animateFrames, bonusTexture, 2);
		
		Sequence* bonusActionSeq = Sequence::create(getFrameAnimation(animateFrames, 1.8), CallFunc::create(CC_CALLBACK_0(PlayScene::ballActionDone, this)), NULL);
		
		bonusSprite->runAction(bonusActionSeq);
		
		showTips("assets_graphics_2x_sprite_62_1.png");
	}
	else if ((shapea->getCategoryBitmask() == TIME_BONUS_SHAPE) || (shapeb->getCategoryBitmask() == TIME_BONUS_SHAPE))
	{
		hitTime = 3;
		
		Texture2D *bonusTexture = Director::getInstance()->getTextureCache()->addImage("assets_graphics_2x_sprite_41_3.png");
		Vector<SpriteFrame*> animateFrames;
		addAnimationFrame(animateFrames, bonusTexture, 2);
		
		Sequence* bonusActionSeq = Sequence::create(getFrameAnimation(animateFrames, 1.8), CallFunc::create(CC_CALLBACK_0(PlayScene::ballActionDone, this)), NULL);
		
		bonusSprite->runAction(bonusActionSeq);
		
		showTips("assets_graphics_2x_sprite_62_2.png");
	}
	else if ((shapea->getCategoryBitmask() == KEEPER_SHAPE) || (shapeb->getCategoryBitmask() == KEEPER_SHAPE))
	{
		CCLOG("keep shape");
		ballActionDone();
		
		return false;
	}
	
	if (hitScore > 0)
	{
		score += hitScore;
		scoreLable->setString(StringUtils::format("%d", score));
	}
	
	if (hitTime > 0)
	{
		seconds += hitTime;
		timeLabel->setString(StringUtils::format("0:%02d", seconds));
	}
	
	return false;
}

void PlayScene::showTips(std::string spriteFileName)
{
	Sprite* tips = Sprite::create(spriteFileName);
	tips->setPosition(160, 270);
	tips->setScale(0.2f);
	this->addChild(tips, 100);
	
	ScaleTo *scaleTo = ScaleTo::create(0.3, 1);
	MoveTo *moveTo = MoveTo::create(0.3, Point(tips->getPositionX(), tips->getPositionY() + 5));
	Sequence *seq = Sequence::create(scaleTo, moveTo, CallFuncN::create(CC_CALLBACK_1(PlayScene::hideTips, this)), NULL);
	tips->runAction(seq);
}

void PlayScene::hideTips(Node *node)
{
	node->removeFromParentAndCleanup(true);
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
	hasMoved = false;
	
	Point convertedLocation = touch->getLocation();
	if (ball->getBoundingBox().containsPoint(convertedLocation))
	{
		isTouchBall = true;
	}
	else
	{
		isTouchBall = false;
	}
	
	return true;
}

void PlayScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
	if (isTouchBall)
	{
		hasMoved = true;
	}
}

void PlayScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
	if (gameStatus != GAME_RUNNING)
	{
		return;
	}
	
	if (!hasMoved)
	{
		return;
	}
	
	Point convertedLocation = touch->getLocation();
	
	// goalKepper action detect
	int goalKeeperJumpSide = rand() % 6;
	goalKeeper->setTexture(StringUtils::format("assets_graphics_2x_sprite_50_%d.png", goalKeeperJumpSide));
	
	// shooter animation
	Texture2D *shooterTexture = Director::getInstance()->getTextureCache()->addImage("assets_graphics_2x_sprite_47_0.png");
	Texture2D *shooterTexture2 = Director::getInstance()->getTextureCache()->addImage("assets_graphics_2x_sprite_47_1.png");
	
	Vector<SpriteFrame*> shooterFrames;
	addAnimationFrame(shooterFrames, shooterTexture, 4);
	addAnimationFrame(shooterFrames, shooterTexture2, 4);
	
	SpriteFrame *originFrame = shooter->getSpriteFrame();
	shooterFrames.pushBack(originFrame);
	
	Animate* animate = getFrameAnimation(shooterFrames, 0.08f);
	shooter->runAction(animate);
	
	// ball action
	Texture2D *ballAnimationTexture = Director::getInstance()->getTextureCache()->addImage("assets_graphics_2x_sprite_1_1.png");
	Vector<SpriteFrame*> ballFrames;
	addAnimationFrame(ballFrames, ballAnimationTexture, 3);
	Animate* ballAnimate = getFrameAnimation(ballFrames, 0.1f);
	ball->runAction(RepeatForever::create(ballAnimate));
	
	MoveTo* moveto = getAvgMoveToAction(ball->getPosition(), convertedLocation, 450);
	ScaleTo* scaleto = ScaleTo::create(moveto->getDuration(), 0.5);
	Sequence* ballActionSeq = Sequence::create(Spawn::create(moveto, scaleto, NULL), CallFunc::create(CC_CALLBACK_0(PlayScene::ballActionDone, this)), NULL);
	ball->runAction(ballActionSeq);
}

void PlayScene::ballActionDone()
{
	ball->stopAllActions();
	
	ball->setTexture("assets_graphics_2x_sprite_1_0.png");
	initBallPosition();
	ball->setScale(1);
	
	goalKeeper->setTexture("assets_graphics_2x_sprite_50_5.png");
	
	if (bonusSprite != NULL)
	{
		bonusSprite->removeFromParentAndCleanup(true);
		bonusSprite = NULL;
	}
	
	CCLOG("balldone");
}

void PlayScene::checkTime(float dt)
{
	if (gameStatus != GAME_RUNNING)
	{
		return;
	}
	
	seconds -= 1;
	timeLabel->setString(StringUtils::format("0:%02d", seconds));
	
	if (bonusSprite == NULL)
	{
		// TODO 是否生成奖励
		int randno = rand() % 10;
		if (randno == 5) // 可以生成奖励
		{
			
		}
		
		int bonusNo = rand() % 2;
		
		PhysicsBody* bonusBody = PhysicsBody::createCircle(13);
		
		if (bonusNo == 0)
		{
			bonusBody->setCategoryBitmask(SCORE_BONUS_SHAPE);
			bonusBody->setContactTestBitmask(BALL_SHAPE);
			bonusBody->setCollisionBitmask(0);
			
			bonusSprite = Sprite::create("assets_graphics_2x_sprite_41_0.png");
		}
		else
		{
			bonusBody->setCategoryBitmask(TIME_BONUS_SHAPE);
			bonusBody->setContactTestBitmask(BALL_SHAPE);
			bonusBody->setCollisionBitmask(0);
			
			bonusSprite = Sprite::create("assets_graphics_2x_sprite_41_1.png");
		}
		
		bonusSprite->setPosition(genRandom(62, 260), genRandom(360, 412));
		bonusSprite->setPhysicsBody(bonusBody);
		bonusLayer->addChild(bonusSprite);
	}
	
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
