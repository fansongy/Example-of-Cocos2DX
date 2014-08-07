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
    
	gameControl(Welcome);

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
				if (contact.getShapeA()->getBody()->getNode() != m_hero) {
					this->removeChild(contact.getShapeA()->getBody()->getNode());
				}
				else
				{
					this->removeChild(contact.getShapeB()->getBody()->getNode());
				}
				this->damageHero();
            }
            
            //Enemy Die
            if((tagA == ContactTag::HERO_BULLET && tagB!=ContactTag::HERO) ||
               (tagB == ContactTag::HERO_BULLET && tagA!=ContactTag::HERO))
            {
				this->addScore();
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

	auto hud = GUIReader::getInstance()->widgetFromJsonFile("HUD/HUD.json");
    Size screen=Director::getInstance()->getVisibleSize();
    auto offset = Point(0,screen.height-hud->getContentSize().height);
    hud->setPosition(offset);
    this->addChild(hud,1);    
    
    m_lifeBar = dynamic_cast<LoadingBar*>(hud->getChildByName("LifeBar"));
    m_scoreNum = dynamic_cast<TextAtlas*>(hud->getChildByName("ScoreNum"));
    
    m_scoreNum->setStringValue("0");
    m_lifeBar->setPercent(100);
   
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
            else if(name.compare("Start") == 0 || name.compare("StartGame") == 0)
            {
				gameControl(Game);
            }
			else if(name.compare("SaveName") == 0)
			{
				auto name = m_input->getStringValue();
				auto score = m_scoreNum->getStringValue();
				CCLOG("input is %s,Score is %s",name.c_str(),score.c_str());
				//save data and Name
				UserDefault::getInstance()->setStringForKey("High_Name", name);
				UserDefault::getInstance()->setIntegerForKey("High_Score", atoi(score.c_str()));
				UserDefault::getInstance()->flush();

				//after save show other button
				setResultDlg(false);
			}
			else if(name.compare("HighScore") == 0)
            {
                gameControl(HighScore);
            }
        }
    }
}

void HelloWorld::damageHero()
{
    if (m_gameState == Game) {
        int precent = m_lifeBar->getPercent();
        precent -=20;
        m_lifeBar->setPercent(precent);
        if (precent == 0) {
            gameControl(Result);
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

void HelloWorld::gameControl(GameState gameState)
{
    switch (gameState) 
    {
        case GameState::Welcome:
        {
			CCLOG("Welcome");
            m_gameState = Welcome;
			initBackground();
            auto welcome = GUIReader::getInstance()->widgetFromJsonFile("Welcome/Welcome.json");
            this->addChild(welcome);
            auto start = dynamic_cast<Widget*>(welcome->getChildByName("StartGame"));
            start->addTouchEventListener(this, toucheventselector(HelloWorld::touchButton));
            auto highScore = dynamic_cast<Widget*>(welcome->getChildByName("HighScore"));
            highScore ->addTouchEventListener(this, toucheventselector(HelloWorld::touchButton));
            break;
		}
		case GameState::Game:
        {
            CCLOG("Game");
            m_gameState = Game;
            removeAllChildren();
            Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
            Director::getInstance()->getRunningScene()->getPhysicsWorld()->removeAllBodies();
            startGame();
            break;
        }
        case GameState::HighScore:
        {
            CCLOG("HighScore");
			m_gameState = HighScore;
			Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
			auto highScore = GUIReader::getInstance()->widgetFromJsonFile("HighScore/HighScore.json");
			auto num = dynamic_cast<TextAtlas*>(highScore->getChildByName("HighScoreBK")->getChildByName("ScoreNum"));
			auto name = dynamic_cast<Text*>(highScore->getChildByName("HighScoreBK")->getChildByName("UserName"));

			int scoreNum = UserDefault::getInstance()->getIntegerForKey("High_Score");
			char scoreStr[256]={0};
			sprintf(scoreStr, "%d",scoreNum);
			num->setStringValue(scoreStr);
			name->setText(UserDefault::getInstance()->getStringForKey("High_Name", "$_$"));
			auto start = dynamic_cast<Widget*>(highScore->getChildByName("HighScoreBK")->getChildByName("Start"));
			start->addTouchEventListener(this, toucheventselector(HelloWorld::touchButton));
			addChild(highScore,1);
			break;
        }
        case GameState::Result:
        {
            CCLOG("Result");
            m_gameState = Result;
			this->unscheduleAllSelectors();
			Director::getInstance()->getEventDispatcher()->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
                
			if(m_hero)
			{
				m_hero->removeFromParent();
			}

			m_resultDlg = GUIReader::getInstance()->widgetFromJsonFile("Result/Result.json");
			int score = UserDefault::getInstance()->getIntegerForKey("High_Score");
			int currentScore = atoi(m_scoreNum->getStringValue().c_str());
			if (score< currentScore) {
				setResultDlg(true);
			}
			else
			{
				setResultDlg(false);
			}

			addChild(m_resultDlg,1);
            break;
        }
        default:
            break;
	}
}
void HelloWorld::setResultDlg(bool isBreakRecord)
{
    auto start = dynamic_cast<Widget*>(m_resultDlg->getChildByName("ResultBK")->getChildByName("Start"));
    auto save = dynamic_cast<Widget*>(m_resultDlg->getChildByName("ResultBK")->getChildByName("SaveName"));
    auto highScore =dynamic_cast<Widget*>(m_resultDlg->getChildByName("ResultBK")->getChildByName("HighScore"));
    auto breakLabel = m_resultDlg->getChildByName("ResultBK")->getChildByName("BreakLabel");
    auto unBreakLabel = m_resultDlg->getChildByName("ResultBK")->getChildByName("UnBreakLabel");
    auto scoreNum = m_resultDlg->getChildByName("ResultBK")->getChildByName("ScoreNum");

    dynamic_cast<TextAtlas*>(scoreNum)->setStringValue(m_scoreNum->getStringValue());
    m_input = dynamic_cast<TextField*>(m_resultDlg->getChildByName("ResultBK")->getChildByName("Input"));
    
    if(isBreakRecord)
    {
        breakLabel->setVisible(true);
        unBreakLabel->setVisible(false);
        highScore->setVisible(false);
        start->setVisible(false);
    
        save->addTouchEventListener(this, toucheventselector(HelloWorld::touchButton));
        
    }
    else
    {
        // from save
        save->setEnabled(false);
        save->setVisible(false);
        m_input->setVisible(false);
        m_input->setEnabled(false);
        
        breakLabel->setVisible(false);
        highScore->setVisible(true);
        start->setVisible(true);
        scoreNum->setPosition(scoreNum->getPosition()+Point(0,-70));
        
        start->addTouchEventListener(this, toucheventselector(HelloWorld::touchButton));
        highScore->addTouchEventListener(this, toucheventselector(HelloWorld::touchButton));
    }
}

void HelloWorld::addScore()
{
    if (m_gameState == Game) {
        auto str = m_scoreNum->getStringValue();
        int num = atoi(str.c_str());
        num+= 15;
        char add_str[256] ={0};
        sprintf(add_str, "%d",num);
        m_scoreNum->setStringValue(add_str);
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
