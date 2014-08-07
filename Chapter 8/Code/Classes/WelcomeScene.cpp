#include "WelcomeScene.h"
#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

bool WelcomeScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    auto layer = Layer::create();
    auto welcomeNode = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile("publish/WelcomeScene.json");
    auto uiComponent = dynamic_cast<ComRender*>(welcomeNode->getChildByTag(10004)->getComponent("GUIComponent"));
    auto ui = dynamic_cast<ui::Widget*>(uiComponent->getNode());
    auto start = dynamic_cast<Widget*>(ui->getChildByName("Start"));
    start->addTouchEventListener(this, toucheventselector(WelcomeScene::touchButton));
    
	auto back = Sprite::create("background.png");
    back->setAnchorPoint(Point::ZERO);
    addChild(back,0);

    layer->addChild(welcomeNode);
    this->addChild(layer);
    
    return true;
}

void WelcomeScene::touchButton(Ref *object, TouchEventType type)
{
    auto scene = HelloWorld::createScene();
    auto transScene = TransitionFadeTR::create(1,scene);
    Director::getInstance()->replaceScene(transScene);
}
