#include "HelloWorldScene.h"

USING_NS_CC;

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

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    //CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile("test.plist","test.png");
    Node* node = Node::create();
    char name[32];
    for(int i  = 0;i<100;++i)
    {
        memset(name, 0, sizeof(name));
        sprintf(name, "%d.png",i%10);
		auto sprite = Sprite::create(name);
        //auto sprite = Sprite::createWithSpriteFrameName(name);
        sprite->setPosition(Point(i*5,i*5));
        node->addChild(sprite, 0);
    }
    this->addChild(node);
  
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent)
    {
        return true;
    };
    listener->onTouchMoved = [=](Touch *pTouch, Event *pEvent)
    {
        node->setPosition(node->getPosition()+pTouch->getDelta());
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}


