#ifndef __ScoreScene_H__
#define __ScoreScene_H__

#include "cocos2d.h"

#include "extensions/cocos-ext.h"
#include "spritebuilder/SpriteBuilder.h"

USING_NS_CC;

class ScoreScene : public cocos2d::Layer
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
		SB_MEMBERVARIABLEASSIGNER_GLUE(this, "lastScoreLabel", Label*, lastScoreLabel);
		SB_MEMBERVARIABLEASSIGNER_GLUE(this, "bestScoreLabel", Label*, bestScoreLabel);
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
        CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "play", ScoreScene::play);
		CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "goHome", ScoreScene::goHome);
        return NULL;
    }
	
	void play(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent);
	void goHome(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent);
	
//  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
//  virtual bool init();
//
//  // a selector callback
//  void menuCloseCallback(cocos2d::Ref* pSender);
	
    // implement the "static create()" method manually
    CREATE_FUNC(ScoreScene);
	
private:
	Label *lastScoreLabel, *bestScoreLabel;
};

CREATE_SPRITEBUILDER_LOADER_CLASS(ScoreSceneLoader, ScoreScene);

#endif // __ScoreScene_H__
