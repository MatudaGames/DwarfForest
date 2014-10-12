#include "HelloWorldScene.h"
//#include <../extensions/cocos-ext.h>
//#include <../extensions/network/HttpClient.h>

//#include "../../DwarfForest/cocos2d/extensions/network/HttpClient.h"

#include "stdio.h"
#include "stdlib.h"
#include "curl/curl.h"



USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

size_t static pWriteCallback(void *pData, size_t n, size_t nDataSize, FILE *stream)
{
    CCLOG("wut 1");
    size_t nWritten = fwrite(pData,n,nDataSize,(FILE *)stream);
    return nWritten;
}

int static DownProgresss(void* clientp,double fDownLoadTotal,double fDownLoaded,double fUpTotal,double fUpLoaded)
{
    if (fDownLoaded >= 0 && fDownLoadTotal != 0)
        CCLOG("%6.2f%%\n", 100*fDownLoaded/fDownLoadTotal);//Download percentage
    return 0;
}


void HelloWorld::downloaded()
{
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath()+"DF_Missions2.plist";
    
    cocos2d::CCDictionary* dict = cocos2d::CCDictionary::createWithContentsOfFile(path.c_str());
    CCLOG("CCDictionary %u",dict->count());
    CCLOG("finish");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    //magic
//    cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
//    cocos2d::extension::CCHttpRequest* req = new cocos2d::extension::CCHttpRequest();
    
    CURL *pCurl;
    CURLcode nResCode;
    
    pCurl = curl_easy_init();
    
    if (pCurl != NULL)
    {
        std::string saveFileName;
        saveFileName = "DF_Missions2.plist";
        saveFileName = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + saveFileName;
        
        pFile = fopen(saveFileName.c_str(), "w+");
        curl_easy_setopt(pCurl,CURLOPT_URL,"http://matudagames.com/DF/DF_Missions.plist");
        
        if(pFile != NULL)
        {
            curl_easy_setopt(pCurl,CURLOPT_FILE,pFile);                   //The specified file write
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, pWriteCallback);//Callback function to write data
            curl_easy_setopt(pCurl, CURLOPT_VERBOSE, true);                //Let CURL report every suddenness
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 60);                  //Setting the timeout
            curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS,0L);
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, DownProgresss);//Specify a callback function
            nResCode = curl_easy_perform(pCurl);//Executing the above a set operation and return a status code
            curl_easy_cleanup(pCurl);           //Release the related resources
            fputs ("fopen example",pFile);
            fclose(pFile);
//            nResCode == CURLE_OK ? downloaded() : CCLOG("CODE: %d",nResCode);
            downloaded();
//            nResCode == CURLE_OK ? downloaded(): CCLOG("finish");
        }
    }

    
    
    
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
