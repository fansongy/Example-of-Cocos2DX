#include "HelloWorldScene.h"
#import "iosPay.h"

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

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(cocos2d::Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(cocos2d::Point::ZERO);
    this->addChild(menu, 1);
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    label->setTag(100);
    label->setPosition(cocos2d::Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);
    auto sprite = Sprite::create("HelloWorld.png");
    sprite->setPosition(cocos2d::Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(sprite, 0);
    
    return true;
}

void HelloWorld::setInfo(std::string info)
{
    auto label = dynamic_cast<LabelTTF*>(getChildByTag(100));
    label->setString(info);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    [[iosPay getInstance] purchaseItem:@"Seed"];
}
