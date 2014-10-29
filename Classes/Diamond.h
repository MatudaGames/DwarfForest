//
//  Diamond.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 13.05.2013.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <cocos2d.h>

class GameScene;

class Diamond: public cocos2d::CCNode
{
public:
//	CREATE_FUNC(Diamond);
    static Diamond* create(GameScene* gameScene,int theTimeOnMap);
	
	Diamond();
    virtual ~Diamond();
	
	virtual bool init(GameScene* gameScene,int theTimeOnMap);
    
    void updateDiamondTimer();
	
    void createAnim();
    
    void onFinishedFlow();
    
    void expire();
    
    int mTimeOnMap;
	
private:
    GameScene* _gameScene;
    
	cocos2d::CCSprite* _diamondSprite;
	cocos2d::CCSprite* _shadowSprite;
    
    cocos2d::CCActionInterval* _diamondMoveUp;
    cocos2d::CCActionInterval* _diamondMoveDown;
	
	float _diamondTimer;
};
