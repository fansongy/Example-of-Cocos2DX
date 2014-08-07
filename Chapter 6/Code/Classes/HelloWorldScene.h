#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/Cocostudio.h"
#include "ui/CocosGUI.h"

enum BodyType {
    Paddle,
    Ball,
    Target,
    DieLine
};

enum GameState {
    Welcome = 0,
    Game,
    Result
};

const int TASK_MAX = 3;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	void initPhysicsWorld();
	void createBall(cocos2d::Point position);
	void createPaddle(cocos2d::Point position);
	void loadTaskScene();
	void addPhysicRect(cocos2d::Node* parent);
	void attachHitListener();
	void touchButton(Ref* object,cocos2d::ui::TouchEventType type);
	void loadPageList(cocos2d::ui::Widget* uiRoot);
	void gameControl(GameState gameState);

private:
    cocos2d::Sprite* m_ball;
	cocos2d::Point m_screenOffset;
	cocos2d::ui::PageView* m_taskPage;
	int m_taskSceneNum;
	int m_taskTargetNum;
};

#endif // __HELLOWORLD_SCENE_H__
