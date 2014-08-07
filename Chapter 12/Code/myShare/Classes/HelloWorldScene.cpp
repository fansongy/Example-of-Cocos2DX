#include "HelloWorldScene.h"


USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "platform/android/jni/jniHelper.h"
#endif

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width/2 - closeItem->getContentSize().width/2 ,origin.y +visibleSize.height/2+ closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    label->setPosition(Point(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height - 300));
    label->setTag(100);
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    std::string className = "com/fansy/shareSDK/MainActivity";
    std::string paramStr = "hi,I'm Fansy,Why not Join Me?";
    if(JniHelper::getStaticMethodInfo(t,className.c_str() , "rtnObject", "()Ljava/lang/Object;"))
    {
        jobject jobj;
        jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        
        if(JniHelper::getMethodInfo(t, className.c_str(),"sendToWeiXin", "(Ljava/lang/String;)V"))
        {
            jstring jstrParam1 = t.env->NewStringUTF(paramStr.c_str());
            t.env->CallVoidMethod(jobj, t.methodID, jstrParam1);
            t.env->DeleteLocalRef(jstrParam1);
        }
    }
    
    auto label = dynamic_cast<LabelTTF*>(getChildByTag(100));
    label->setString("Call Android");
#else
    Director::getInstance()->end();
    exit(0);
#endif
}

