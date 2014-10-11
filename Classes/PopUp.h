//
//  Popup.h
//  DwarfForest
//
//  Created by Agris Dumins on 9/1/13.
//
//

#pragma once

#include <cocos2d.h>
#include <string>

typedef void (cocos2d::CCObject::*CallBack)();

class PopUp: public cocos2d::CCLayer
{
public:
    static PopUp* create(cocos2d::CCLayer* parent,
                         const std::string& message,
                         CallBack selector,
                         bool theCancelButton,
                         bool theExitGame = false);
    
    bool init(cocos2d::CCLayer* parent, const std::string& message, CallBack selector, bool theCancelButton,bool theExitGame=false);
    
private:
	void menuOkCallback(CCObject* sender);
    void menuCancel(CCObject* sender);
    
    bool _cancelButton;
    CallBack _callback;
};
