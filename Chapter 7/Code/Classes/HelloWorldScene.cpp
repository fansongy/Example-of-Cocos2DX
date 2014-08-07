#include "HelloWorldScene.h"
#include "Components/Constants.h"

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
    
    auto back = Sprite::create("background.png");
    back->setAnchorPoint(Point::ZERO);
    addChild(back,0);

    m_tileMap = TMXTiledMap::create("TowerTileMap/gate1.tmx");
    m_tileMap->setPosition(Point::ZERO);
    addChild(m_tileMap,1);
    
	m_pathVec = getWalkPath("Walk");
	m_curRound = 1;
	createWaveRusher();

	attachTowerBuild();

	m_build = m_tileMap->getLayer("meta");
	m_build->setVisible(false);

	m_fireManager = FireManager::create();
	addChild(m_fireManager);
    
//    Director::getInstance()->getTextureCache()->addImage("Monster.png");
    

    return true;
}

void HelloWorld::createWaveRusher()
{
    m_monsterCreateLeft = 10 + m_curRound*3;
    this->schedule(schedule_selector(HelloWorld::makeMonster), 0.5);
}

void HelloWorld::createTower( Point touchPos )
{
    std::string str = getValue("Build",touchPos,m_build,m_tileMap);
    
    int offsetX = - (int)touchPos.x % (int)m_tileMap->getTileSize().width + m_tileMap->getTileSize().width/2;
	int offsetY = - (int)touchPos.y % (int)m_tileMap->getTileSize().height + m_tileMap->getTileSize().height/2;
	auto blockCenter = Point((int)(touchPos.x+offsetX),(int)(touchPos.y+offsetY));
    
    if (str.size() != 0)
    {
		CCLOG("answer:%s",str.c_str());
		if (str.compare("True") == 0)
		{
			Sprite* tower = Sprite::create("Tower.png");
			tower->setPosition(blockCenter);
			auto comTower = ComTower::create();
			tower->addComponent(comTower);
			m_fireManager->m_towers.push_back(comTower);
			this->addChild(tower,2);
            return;
		}
    }
    Sprite* errorPos = Sprite::create("ErrorPos.png");
    errorPos->setPosition(blockCenter);
    this->addChild(errorPos,2);
    auto action = Sequence::create(FadeOut::create(3), CallFunc::create([=]{
        errorPos->removeFromParent();
    }),NULL);
    errorPos->runAction(action);
    return ;
}


Point HelloWorld::worldToTile( Point& point )
{
	int x = point.x / m_tileMap->getTileSize().width;
	int y = ( m_tileMap->getContentSize().height - point.y)/m_tileMap->getTileSize().height;
	return Point(x,y);
}

std::string HelloWorld::getValue( std::string key,Point& posInGL,TMXLayer* layer,TMXTiledMap* map )
{
    
	Point pos = worldToTile(posInGL);
	int tileGID = layer->getTileGIDAt(pos);
	if(tileGID)
	{
		Value pro = map->getPropertiesForGID(tileGID);
        auto map = pro.asValueMap();
        auto pos = map.find(key);
        if (pos != map.end()) {
            return pos->second.asString();
        }
        else{
            return "";
        }
	}
	else
	{
		return "";
	}
}

void HelloWorld::attachTowerBuild()
{
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = [=](Touch *pTouch, Event *pEvent)
    {
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *pTouch, Event *pEvent)
    {
        auto touchPos = pTouch->getLocation();
        createTower(touchPos);
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

std::vector<cocos2d::Point> HelloWorld::getWalkPath( const char* key)
{
	std::vector<cocos2d::Point> pathVec;
	if (!m_tileMap)
	{
		CCLOG("Can't Find Map.Please Load Map First!");
		return pathVec;
	}
	TMXObjectGroup* walk = m_tileMap->getObjectGroup(key);
	if (!walk)
	{
		CCLOG("No Such Key = %s In TileMap!",key);
		return pathVec;
	}
	ValueVector path = walk->getObjects();
	int myX = 0;
	int myY = 0;
	for(auto pos : path)
	{
		myX = pos.asValueMap()["x"].asInt();
		myY = pos.asValueMap()["y"].asInt();
		pathVec.push_back(Point(myX,myY));
	}

	return pathVec;
}
void HelloWorld::makeMonster(float dt)
{
//    auto texture = Director::getInstance()->getTextureCache()->addImage("Monster.png");
    auto monster =Sprite::create("Monster.png");
//    auto monster = Sprite::createWithTexture(texture);
    auto path = m_pathVec;
    if (path.size() == 0)
	  {
		return ;
	  }
	auto comMove = ComMove::create(m_pathVec);
    monster->addComponent(comMove);
    auto comLife = ComLife::create(30*m_curRound);
	m_fireManager->m_monster.push_back(comMove);
    monster->addComponent(comLife);
    addChild(monster,2);
    comMove->startMove();

	    
	m_monsterCreateLeft--;
    if (m_monsterCreateLeft == 0) {
        unschedule(schedule_selector(HelloWorld::makeMonster));
        runAction(Sequence::create(DelayTime::create(3+m_curRound),CallFunc::create([=](){
            m_curRound++;
            createWaveRusher();
            
        }),nullptr));
    }
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
