#include "HelloWorldScene.h"


USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "platform/android/jni/jniHelper.h"

#ifdef __cplusplus
extern "C" {
#endif
    /*
     * Class:     com_fansy_mySDK_MainActivity
     * Method:    OnLogin
     * Signature: (ILjava/lang/String;)V
     */
    JNIEXPORT void JNICALL Java_com_fansy_mySDK_MainActivity_OnLogin
    (JNIEnv *env, jobject thiz, jint jnResult, jstring jstrRet)
    {
        const char* pText = NULL;
		if(jstrRet) {
			pText = env->GetStringUTFChars(jstrRet, NULL);
		}
        auto children = Director::getInstance()->getRunningScene()->getChildren();
        for(auto child:children)
        {
            auto layer = dynamic_cast<HelloWorld*>(child);
            if (layer) {
                cocos2d::LabelTTF* label = dynamic_cast<cocos2d::LabelTTF*>(layer->getChildByTag(100));
                label->setString(pText);
            }
        }
    }
    
#ifdef __cplusplus
}
#endif

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
    JniMethodInfo method;
    std::string className = "com/fansy/mySDK/MainActivity";
    if(JniHelper::getStaticMethodInfo(method,className.c_str() , "rtnObject", "()Ljava/lang/Object;"))
    {
        jobject jobj;
        jobj = method.env->CallStaticObjectMethod(method.classID, method.methodID);
        
        if(JniHelper::getMethodInfo(method, className.c_str(), "LogIn", "()V"))
        {
            method.env->CallVoidMethod(jobj, method.methodID);
            return;
        }
    }
    
    auto label = dynamic_cast<LabelTTF*>(getChildByTag(100));
    label->setString("Call Android");
#else
    Director::getInstance()->end();
    exit(0);
#endif
}

