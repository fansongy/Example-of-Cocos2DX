#include "HelloWorldScene.h"

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;

const int   HERO_BULLET_SPEED = 1000;
const int   ENEMY_BULLET_SPEED = 500;
const float HERO_FIRE_DURATION = 0.3;
const float ENEMY_FIRE_DURATION = 0.8;
const float BIG_EMENY_CREATE_DURATION = 5.0;
const float SMALL_EMENY_CREATE_DURATION = 1.5;


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


//更改init
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    srand((unsigned)time(NULL));


	ArmatureDataManager::getInstance()->addArmatureFileInfo("Hero/Hero.ExportJson"); 
	ArmatureDataManager::getInstance()->addArmatureFileInfo("BigEnemy/BigEnemy.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("SmallEnemy/SmallEnemy.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("Explode/Explode.ExportJson");
    
	if(!startGame())
    {
        return false;
    }
    
    return true;
}
bool HelloWorld::startGame()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    createHero();
    if(!m_hero)
    {
        CCLOG("Create Hero Error!");
        return false;
    }
    
	schedule(schedule_selector(HelloWorld::createBigEnemy),BIG_EMENY_CREATE_DURATION);
	schedule(schedule_selector(HelloWorld::createSmallEnemy),SMALL_EMENY_CREATE_DURATION);

    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent)
    {
        return true;
    };

    listener->onTouchMoved = [=](Touch *pTouch, Event *pEvent)
    {
        auto newPos = m_hero->getPosition()+pTouch->getDelta();

        Size planeSize = m_hero->getContentSize();
        if (newPos.x > visibleSize.width - planeSize.width/2) {
            newPos.x = visibleSize.width - planeSize.width/2;
        }
        else if(newPos.x < planeSize.width/2)
        {
            newPos.x =planeSize.width/2;
        }
        
        if (newPos.y > visibleSize.height - planeSize.height/2) {
            newPos.y = visibleSize.height - planeSize.height/2;
        }
        else if(newPos.y < planeSize.height/2)
        {
            newPos.y =planeSize.height/2;
        }
        
        m_hero->setPosition(newPos);
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
	initBackground();

	EventListenerPhysicsContact* hitListener = EventListenerPhysicsContact::create();
    
    hitListener->onContactBegin = [=](const PhysicsContact& contact)
    {
        auto tagA = contact.getShapeA()->getBody()->getTag();
        auto tagB = contact.getShapeB()->getBody()->getTag();
        
        if(tagA != tagB)
        {
            //Hero Die
            if ((tagA == ContactTag::HERO && tagB != ContactTag::HERO_BULLET) ||
                (tagB == ContactTag::HERO && tagA != ContactTag::HERO_BULLET))
            {
                this->unscheduleAllSelectors();
                Director::getInstance()->getEventDispatcher()->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
                
                //show RestartDlg
                auto gameOverDlg = GUIReader::getInstance()->widgetFromJsonFile("GameOverDlg/GameOverDlg.json");
                addChild(gameOverDlg,1);
                
                auto restart = dynamic_cast<Widget*>(gameOverDlg->getChildByName("Background")->getChildByName("Restart"));
                auto exit = dynamic_cast<Widget*>(gameOverDlg->getChildByName("Background")->getChildByName("ExitGame"));
                
                restart->addTouchEventListener(this, toucheventselector(HelloWorld::touchButton));
                exit->addTouchEventListener(this, toucheventselector(HelloWorld::touchButton));

                this->removeChild(contact.getShapeA()->getBody()->getNode());
                this->removeChild(contact.getShapeB()->getBody()->getNode());
            }
            
            //Enemy Die
            if((tagA == ContactTag::HERO_BULLET && tagB!=ContactTag::HERO) ||
               (tagB == ContactTag::HERO_BULLET && tagA!=ContactTag::HERO))
            {
                if (tagA == HERO_BULLET)
                {
                    auto enemy = contact.getShapeB()->getBody()->getNode();
                    enemy->unscheduleAllSelectors();

                    this->removeChild(enemy);
                    this->removeChild(contact.getShapeA()->getBody()->getNode());
                }
                else
                {
                    auto enemy = contact.getShapeA()->getBody()->getNode();
                    enemy->unscheduleAllSelectors();

                    this->removeChild(enemy);
                    this->removeChild(contact.getShapeB()->getBody()->getNode());
                }
                
            }
        }
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()-> addEventListenerWithSceneGraphPriority(hitListener,this);


    return true;
}

    

void HelloWorld::touchButton(Ref *object,TouchEventType type)
{
    if(type == TOUCH_EVENT_ENDED)
    {
        Widget* widget = dynamic_cast<Widget*>(object);
        if(widget)
        {
            auto name = widget->getName();
            if (name.compare("ExitGame") == 0)
            {
                 Director::getInstance()->end();
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                 exit(0);
            #endif
            }
            else if(name.compare("Restart") == 0)
            {
                removeAllChildren();
                Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
                Director::getInstance()->getRunningScene()->getPhysicsWorld()->   removeAllBodies();
                startGame();
            }
        }
    }
}


void HelloWorld::initBackground()
{
    m_background1 = Sprite::create("background.png");
    m_background1->setAnchorPoint(Point(0,0));
    m_background1->setPosition(Point(0,0));
    
    //缩放X，让背景在X方向上铺满屏幕
    Size bkSize = m_background1->getContentSize();
    auto xScale = Director::getInstance()->getVisibleSize().width / bkSize.width;
    m_background1->setScaleX(xScale);
    addChild(m_background1,-1);
    
    //另一张背景图片
    m_background2 = Sprite::create("background.png");
    m_background2->setAnchorPoint(Point(0,0));
    m_background2->setPosition(Point(0,bkSize.height));
    m_background2->setScaleX(xScale);
    addChild(m_background2,-1);
    
    schedule(schedule_selector(HelloWorld::moveBackground),0.01);
}

void HelloWorld::moveBackground(float dt)
{
    auto backgroundY = m_background1->getContentSize().height;
    if (m_background1->getPositionY() < -backgroundY)
    {
        m_background1->setPosition(Point(0,0));
        m_background2->setPosition(Point(0,backgroundY));
    }
    else
    {
        m_background1->setPosition(m_background1->getPosition()-Point(0,2));
        m_background2->setPosition(m_background2->getPosition()-Point(0,2));
    }
}


void HelloWorld::createSmallEnemy(float dt)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Armature *smallEnemy = Armature::create("SmallEnemy");
    Size planeSize = smallEnemy->getContentSize();
    int planeX = rand()%(int)(visibleSize.width-planeSize.width) + planeSize.width/2;
    Point startPos = Point(planeX, visibleSize.height+planeSize.height/2);
    Point endPos = Point(planeX, -planeSize.height/2);
    
	auto body = PhysicsBody::createBox(smallEnemy->getContentSize());
    body->setTag(ContactTag::ENEMY);
	body->setContactTestBitmask(0xFFFFFFFF);
    smallEnemy->setPhysicsBody(body);

    smallEnemy->getAnimation()->play("SmallEnemyIdle");
    smallEnemy->setPosition(startPos);
    addChild(smallEnemy);
    
    auto moveAct = MoveTo::create(2.5, endPos);
    auto acts = Sequence::create(moveAct,
                    CallFunc::create(
                      [=](){
                          smallEnemy->removeFromParent();
                      }
                    ),NULL);
    smallEnemy->runAction(acts);
}

void HelloWorld::createBigEnemy(float dt)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Armature *bigEnemy = Armature::create("BigEnemy");
    Size planeSize = bigEnemy->getContentSize();
    int planeX = rand()%(int)(visibleSize.width-planeSize.width) + planeSize.width/2;
    Point startPos = Point(planeX, visibleSize.height+planeSize.height/2);
    Point endPos = Point(planeX, visibleSize.height/5*4);
    
	auto body = PhysicsBody::createBox(bigEnemy->getContentSize());
    body->setTag(ContactTag::ENEMY);
	body->setContactTestBitmask(0xFFFFFFFF);
    bigEnemy->setPhysicsBody(body);


    bigEnemy->getAnimation()->play("BigEnemyIdle");
    bigEnemy->setPosition(startPos);
    addChild(bigEnemy);
    
    auto moveAct = MoveTo::create(1, endPos);
    auto acts = Sequence::create(moveAct,
                     CallFunc::create(
                          [=](){
                              //create bullet
							  bigEnemy->schedule( schedule_selector
                                    (HelloWorld::EnemyFire),ENEMY_FIRE_DURATION);
                          }
                      ),NULL);
    bigEnemy->runAction(acts);
}

void HelloWorld::createHero()
{
    m_hero = Armature::create("Hero");
    m_hero->getAnimation()->play("HeroIdle");
	    
	auto body = PhysicsBody::createBox(m_hero->getContentSize());
    body->setTag(ContactTag::HERO);
	body->setContactTestBitmask(0xFFFFFFFF);
    m_hero->setPhysicsBody(body);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    m_hero->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    addChild(m_hero);
    
    this->schedule(schedule_selector(HelloWorld::HeroFire),HERO_FIRE_DURATION);
}
void HelloWorld::HeroFire(float dt)
{
    auto bullet = Sprite::create("HeroBullet.png");
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto startPos = m_hero->getPosition()+Point(0,m_hero->getContentSize().height/2);
    auto endPos = Point(m_hero->getPositionX(),visibleSize.height +        
                                      bullet->getContentSize().height/2);
    auto duration = (endPos.y- startPos.y) / HERO_BULLET_SPEED;
        
	auto body = PhysicsBody::createBox(bullet->getContentSize());
    body->setTag(ContactTag::HERO_BULLET);
	body->setContactTestBitmask(0xFFFFFFFF);
    bullet->setPhysicsBody(body);

    bullet->setPosition(startPos);
    addChild(bullet);
    
    auto fire = Sequence::create(MoveTo::create(duration, endPos),
                     CallFunc::create(
                              [=](){
                                  bullet->removeFromParent();
                              }
                      ),NULL);
    
    bullet->runAction(fire);
}

void HelloWorld::EnemyFire(float dt)
{
    auto bullet = Sprite::create("EnemyBullet.png");
    auto startPos = this->getPosition()-Point(0,this->getContentSize().height/2);
    auto endPos = Point(this->getPositionX(),- bullet->getContentSize().height/2);
    auto duration = (endPos.y + startPos.y) / ENEMY_BULLET_SPEED;
    
	auto body = PhysicsBody::createBox(bullet->getContentSize());
    body->setTag(ContactTag::ENEMY_BULLET);
	body->setContactTestBitmask(0xFFFFFFFF);
    bullet->setPhysicsBody(body);
    
    bullet->setPosition(startPos);
    getParent()->addChild(bullet);
    
    auto fire = Sequence::create(MoveTo::create(duration, endPos),
                     CallFunc::create(
                                      [bullet](){
                                          bullet->removeFromParent();
                                      }
                      ),NULL);
    
    bullet->runAction(fire);
}
