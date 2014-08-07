#include "HelloWorldScene.h"

USING_NS_CC;
using namespace ui;

#define PHYSICS_MATERIAL_BALL_WORLD PhysicsMaterial(1,1,0)
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    scene->getPhysicsWorld()->setGravity(Vect(0,0));

    
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
    auto visiableSize = Director::getInstance()->getVisibleSize();
    auto taskSize =Size(640,960);
    auto offsetX = (visiableSize.width - taskSize.width)/2;
    auto offsetY = (visiableSize.height - taskSize.height)/2;
    m_screenOffset= Point(offsetX,offsetY);
	
	gameControl(Welcome);	
    return true;
}

void HelloWorld::loadPageList(Widget *uiRoot)
{
    m_taskPage = dynamic_cast<PageView*>(uiRoot->getChildByName("taskList"));
    
    char name[32] = {0};
    for (int i = 1; i<=TASK_MAX; ++i) {
        memset(name, 0, sizeof(name));
        sprintf(name, "publish/task%d.json",i);
        auto task = dynamic_cast<Widget*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(name));
        task->setScale(0.5);
        m_taskPage->addWidgetToPage(task,i-1,true);
    }
}


void HelloWorld::touchButton(Ref *object,TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED) {
        auto buttonName = dynamic_cast<Widget*>(object)->getName();
		if (buttonName.compare("Start") == 0)
        {
            m_taskSceneNum = (int)m_taskPage->getCurPageIndex()+1;
            gameControl(Game);
        }
        else if (buttonName.compare("Restart") == 0)
        {
            gameControl(Game);
        }
        else if (buttonName.compare("BackMain") == 0)
        {
            gameControl(Welcome);
        }
    }
}



void HelloWorld::initPhysicsWorld()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    auto edgeSprite = Sprite::create();
    auto body = PhysicsBody::createEdgeBox(visibleSize,PHYSICS_MATERIAL_BALL_WORLD,3);
    edgeSprite->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    edgeSprite->setPhysicsBody(body);
    this->addChild(edgeSprite);

	auto dieLine = Sprite::create();
    auto dieLineBody = PhysicsBody::createEdgeBox(Size(visibleSize.width,6),PHYSICS_MATERIAL_BALL_WORLD,3);
    dieLineBody->setTag(DieLine);
	dieLineBody->setContactTestBitmask(0xFFFFFFFF);
    dieLine->setPosition(Point(visibleSize.width/2,10));
    dieLine->setPhysicsBody(dieLineBody);
    this->addChild(dieLine);

}
void HelloWorld::gameControl(GameState gameState)
{
    switch (gameState) {
        case Welcome:
        {
			removeAllChildren();
			auto welcome = dynamic_cast<Widget*>(cocostudio::GUIReader::getInstance()
				->widgetFromJsonFile("publish/welcome.json"));
			welcome->setPosition(m_screenOffset);
			loadPageList(welcome);
			addChild(welcome);
			auto start = dynamic_cast<Widget*>(welcome->getChildByName("Start"));
			start->addTouchEventListener(this, toucheventselector(HelloWorld::touchButton));
			break;
		}
		case Game:
        {
			this->removeAllChildren();
            Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
            initPhysicsWorld();
		    createBall(Point(100,100));
		    createPaddle(Point(100,60));
		    loadTaskScene();
			break;
		}
		case Result:
        {
            Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			removeAllChildren();         
			Director::getInstance()->getRunningScene()->getPhysicsWorld()->removeAllBodies();

            auto result = dynamic_cast<Widget*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("publish/GameOverDlg.json"));
            Text* text = dynamic_cast<Text*>(result->getChildByName("Background")->getChildByName("OverLabel"));
            if (m_taskTargetNum == 0)
            {
                text->setText("You Win!!");
            }
            else
            {
                text->setText("You Lose...");
            }

            auto start =dynamic_cast<Widget*>(result->getChildByName("Background")->getChildByName("Restart"));
            start->addTouchEventListener(this, toucheventselector(HelloWorld::touchButton));
           
            auto back = dynamic_cast<Widget*>(result->getChildByName("Background")->getChildByName("BackMain"));
            back->addTouchEventListener(this, toucheventselector(HelloWorld::touchButton));
            
            result->setPosition(m_screenOffset);
            this->addChild(result);
            break;
        }

		default:
            break;
	}
}

void HelloWorld::loadTaskScene()
{
	char name[32] = {0};
    sprintf(name, "publish/taskScene%d.json", m_taskSceneNum);
  	auto task = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile(name);
    auto realTask = static_cast<cocostudio::ComRender*>(task->getChildByTag(10003)->getComponent("taskComponent"))->getNode();
    realTask->setPosition(m_screenOffset);    
	addPhysicRect(realTask->getChildByTag(2));
	attachHitListener();
	addChild(task);
}

void HelloWorld::attachHitListener()
{
    EventListenerPhysicsContact* hitListener = EventListenerPhysicsContact::create();
    
    hitListener->onContactBegin=[=](const PhysicsContact& contact)
    {
        auto tagA = contact.getShapeA()->getBody()->getTag();
        auto tagB = contact.getShapeB()->getBody()->getTag();
        
        if(tagA == Target || tagB == Target)
        {
            Node* block = nullptr;
            if (tagA == Target) {
                block = contact.getShapeA()->getBody()->getNode();
            }
            else
            {
                block = contact.getShapeB()->getBody()->getNode();
            }
            block->removeFromParent();

			m_taskTargetNum--;
            if (m_taskTargetNum == 0) {
                gameControl(Result);
            }

        }
		else if(tagA == DieLine || tagB == DieLine){
			gameControl(Result);
		}

        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(hitListener,this);
}

void HelloWorld::addPhysicRect(cocos2d::Node* parent)
{
	m_taskTargetNum = 0;;
    for (auto child:parent->getChildren())
    {
        auto body = PhysicsBody::createBox(child->getContentSize(),PHYSICS_MATERIAL_BALL_WORLD);
        body->setDynamic(false);
		body->setContactTestBitmask(0xFFFFFFFF);
        body->setTag(Target);
        child->setPhysicsBody(body);
		m_taskTargetNum++;
    }
}



void HelloWorld::createPaddle(Point position)
{
    auto paddle = Sprite::create("paddle.png");
    auto paddleBody = PhysicsBody::createBox(paddle->getContentSize(),PHYSICS_MATERIAL_BALL_WORLD);
    paddleBody->setDynamic(false);
	paddleBody->setContactTestBitmask(0xFFFFFFFF);
	paddleBody->setTag(Paddle);

    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    paddle->setPhysicsBody(paddleBody);
    paddle->setPosition(position);
    addChild(paddle);  

	auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent)
    {
        return true;
    };
    
    listener->onTouchMoved = [=](Touch *pTouch, Event *pEvent)
    {
        auto offset = pTouch->getDelta();
        auto newPos =paddle->getPosition()+offset;
        auto paddleSize = paddle->getContentSize();
        if (newPos.x<paddleSize.width/2) {
            newPos.x = paddleSize.width/2;
        }
        else if (newPos.x>visibleSize.width-paddleSize.width/2)
        {
            newPos.x = visibleSize.width-paddleSize.width/2;
        }
        newPos.y = paddle->getPosition().y;
        paddle->setPosition(newPos);
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

}


void HelloWorld::createBall(Point position)
{
    auto ballBody = PhysicsBody::createCircle(25,PHYSICS_MATERIAL_BALL_WORLD);
    ballBody->setVelocity(Point(300,300));
    ballBody->setVelocityLimit(600);
   	ballBody->setContactTestBitmask(0xFFFFFFFF);
	ballBody->setTag(Ball);

    m_ball = Sprite::create("ball.png");
    m_ball->setPhysicsBody(ballBody);
    m_ball->setPosition(position);
    addChild(m_ball);
}
