#pragma once

#include "cocos2d.h"

/* For demonstrating using one design resolution to match different resources,
   or one resource to match different design resolutions.

   [Situation 1] Using one design resolution to match different resources.
     Please look into Appdelegate::applicationDidFinishLaunching.
     We check current device frame size to decide which resource need to be selected.
     So if you want to test this situation which said in title '[Situation 1]',
     you should change ios simulator to different device(e.g. iphone, iphone-retina3.5, iphone-retina4.0, ipad, ipad-retina),
     or change the window size in "proj.XXX/main.cpp" by "CCEGLView::setFrameSize" if you are using win32 or linux plaform
     and modify "proj.mac/AppController.mm" by changing the window rectangle.

   [Situation 2] Using one resource to match different design resolutions.
     The coordinates in your codes is based on your current design resolution rather than resource size.
     Therefore, your design resolution could be very large and your resource size could be small.
     To test this, just define the marco 'TARGET_DESIGN_RESOLUTION_SIZE' to 'DESIGN_RESOLUTION_2048X1536'
     and open iphone simulator or create a window of 480x320 size.

   [Note] Normally, developer just need to define one design resolution(e.g. 960x640) with one or more resources.
 */

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
}Resource;

static Resource smallResource  =  { cocos2d::CCSizeMake(426, 320),   "small" };
static Resource mediumResource =  { cocos2d::CCSizeMake(470, 320),  "medium"   };
static Resource largeResource  =  { cocos2d::CCSizeMake(640, 480), "large" };
static Resource xlargeResource  =  { cocos2d::CCSizeMake(960, 720), "xlarge" };

static Resource iphoneResource  =  { cocos2d::CCSizeMake(480, 320),   "iphone" };
static Resource iphonehdResource  =  { cocos2d::CCSizeMake(960, 640),   "iphonehd" };
static Resource ipadResource =  { cocos2d::CCSizeMake(1024, 768),  "ipad"   };
static Resource ipadhdResource  =  { cocos2d::CCSizeMake(2048, 1536), "ipadhd" };

static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(960, 720);

// The font size 24 is designed for small resolution, so we should change it to fit for current design resolution
#define TITLE_FONT_SIZE  (float)(cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / smallResource.size.width * 24)

#define FONT_SKRANJI        "Skranji.ttf"
#define FONT_SKRANJI_BOLD   "Skranji-Bold.ttf"
