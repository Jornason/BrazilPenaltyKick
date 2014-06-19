#ifndef __PlayScene_H__
#define __PlayScene_H__

#include "cocos2d.h"

#include "extensions/cocos-ext.h"
#include "spritebuilder/SpriteBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum GAME_STATUS{GAME_WAITING, GAME_RUNNING, GAME_STOP};

class PlayScene : public cocos2d::Layer
, public spritebuilder::CCBSelectorResolver
, public spritebuilder::CCBMemberVariableAssigner
, public spritebuilder::NodeLoaderListener
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
	
	static cocos2d::Node* createFromUI();
	
	virtual void onNodeLoaded(cocos2d::Node * pNode, spritebuilder::NodeLoader * pNodeLoader);
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
		SB_MEMBERVARIABLEASSIGNER_GLUE(this, "goalKeeper", Sprite*, goalKeeper);
		SB_MEMBERVARIABLEASSIGNER_GLUE(this, "shooter", Sprite*, shooter);
		SB_MEMBERVARIABLEASSIGNER_GLUE(this, "ball", Sprite*, ball);
		
		SB_MEMBERVARIABLEASSIGNER_GLUE(this, "scoreLabel", Label*, scoreLable);
		SB_MEMBERVARIABLEASSIGNER_GLUE(this, "timeLabel", Label*, timeLabel);
		
        return false;
    }
    
    virtual bool onAssignCCBCustomProperty(cocos2d::Ref* target, const char* memberVariableName, const cocos2d::Value& value) {
        return false;
    };
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
        return NULL;
    }
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
        return NULL;
    };
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
        CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "startPlay", PlayScene::startPlay);
        return NULL;
    }
	
	void initBallPosition();
	
	void startPlay(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent);
	
//  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
//  virtual bool init();
//
//  // a selector callback
//  void menuCloseCallback(cocos2d::Ref* pSender);
	
	bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
	
    // implement the "static create()" method manually
    CREATE_FUNC(PlayScene);

public:
	int count;
	
private:
	void checkTime(float dt);
	
	Sprite* goalKeeper;
	Sprite* shooter;
	Sprite* ball;
	
	int score;
	int seconds;

	Label* scoreLable;
	Label* timeLabel;
	
	GAME_STATUS gameStatus;
};

CREATE_SPRITEBUILDER_LOADER_CLASS(PlaySceneLoader, PlayScene);

#endif // __PlayScene_H__
