#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;


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
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto widget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Piano.json");
	addChild(widget);

	//auto buttonWidget = dynamic_cast<ui::Button*>(widget->getChildByName("Main")->getChildByTag(11));
	//buttonWidget->addTouchEventListener(this,toucheventselector(HelloWorld::touchButton));

	auto main = widget->getChildByName("Main");
	for(int i = 10;i<70;++i)
	{
		auto buttonWidget = dynamic_cast<ui::Button*>(main->getChildByTag(i));
		if(buttonWidget)
		{
			buttonWidget->addTouchEventListener (this,toucheventselector(HelloWorld::touchButton));
		}
	}
	
	ParticleSystemQuad* particle = ParticleSystemQuad::create("slowLight.plist");
	addChild(particle);

	auto heart = Sprite::create("heart.png");
    auto visibleSize = Director::getInstance()->getVisibleSize();
    heart->setPosition(visibleSize.width/5,visibleSize.height/5*4);
    auto move = MoveBy::create(5, Point(visibleSize.width/6,0));
    auto rev = move->reverse();
    auto actionSequence = Sequence::create(move,rev, NULL);
	heart->runAction(RepeatForever::create(actionSequence));
	addChild(heart);


	auto input = dynamic_cast<TextField*>(widget->getChildByName("input"));
	input->addEventListenerTextField(this,textfieldeventselector(HelloWorld::textInput));

//	std::string str = UserDefault::getInstance()->getStringForKey("userStr");
//	auto label = dynamic_cast<TextBMFont*>(widget->getChildByName("Words"));
//	label->setText(str.c_str());


    return true;
}


void HelloWorld::touchButton(Ref* object,ui::TouchEventType type)
{
	//CCLOG("Touch Button");
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Do.wav");

	auto button  =dynamic_cast<Button*>(object);
	if(button && type == TouchEventType::TOUCH_EVENT_BEGAN)
	{
		char buf[32]={0};
		std::string name = button->getName();
		sprintf(buf,"sound/%s.wav",name.c_str());
		CCLOG("Sound Name is:%s",buf);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(buf); 	
	}
}

void HelloWorld::textInput(Object* object,TextFiledEventType type)
{
    switch (type) {
        case TEXTFIELD_EVENT_DETACH_WITH_IME:
        {
            TextField* textField = dynamic_cast<TextField*>(object);
            std::string words = textField->getStringValue();
            TextBMFont* label = dynamic_cast<TextBMFont*>(textField->getParent()->getChildByTag(100));
            label->setText(words.c_str());
            textField->setText("");

			UserDefault::getInstance()->setStringForKey("userStr", words);
			UserDefault::getInstance()->flush();
        }
            break;
            
        default:
            break;
    }
}


