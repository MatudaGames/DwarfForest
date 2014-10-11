#pragma once

#include <cocos2d.h>

class BeforeQuitScene: public cocos2d::CCLayer
{
public:
	static BeforeQuitScene* create(bool endingGame);
    
    virtual bool init(bool endingGame);
    
    static cocos2d::CCScene* scene(bool endingGame = true);
    
private:
    
    bool _endingGame;
    void leaveCallback();
    void returnCallback();
};
