//
//  SupportPopup.h
//  DwarfForest
//
//  Created by Kristaps Auzins on 12/05/14.
//
//

#ifndef __DwarfForest__SupportPopup__
#define __DwarfForest__SupportPopup__

#include <iostream>

#endif /* defined(__DwarfForest__SupportPopup__) */

#include <cocos2d.h>
#include <string>

typedef void (cocos2d::CCObject::*CallBack)();

class SupportPopup: public cocos2d::CCLayer
{
public:
    static SupportPopup* create(cocos2d::CCLayer* parent);
    bool init(cocos2d::CCLayer* parent);
    
private:
	void menuOkCallback(CCObject* sender);
    void menuCancel(CCObject* sender);
    
    bool _cancelButton;
    CallBack _callback;
};