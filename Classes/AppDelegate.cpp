#include "AppDelegate.h"
#include "HelloWorldScene.h"

#include "MainMenuLayer.h"

#include "spritebuilder/SpriteBuilder.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
	
	Size designSize  = Size(320, 480);
	Size size = director->getWinSize();
	float scaleFactor = size.height / designSize.height;
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);
	director->setContentScaleFactor(scaleFactor / (size.height / 960)); //because the current resource is phonehd's
	spritebuilder::CCBReader::setupSpriteBuilder("resources-phonehd");
	
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/bg.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/bg.mp3", true);
	
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/warning.mp3");
	
	// create a scene. it's an autorelease object
    auto scene = MainMenuLayer::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
