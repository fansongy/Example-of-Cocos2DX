#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "FireManager.h"
#include "ui/CocosGUI.h"


class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
	void onEnter() override;

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
    std::vector<cocos2d::Point> getWalkPath( const char* key);
	void makeMonster(float dt);
	void attachTowerBuild();
	cocos2d::Point worldToTile(cocos2d::Point& point);
	std::string getValue(std::string key,cocos2d::Point& posInGL,cocos2d::TMXLayer* layer, cocos2d::TMXTiledMap* map );
	void createTower(cocos2d::Point touchPos);
	void createWaveRusher();
	void touchButton(cocos2d::Ref* object,cocos2d::ui::TouchEventType type);
public:
    void changeGold(int num);
	int changeTowerLife(int num);
	void removeMonster(Node* monster);
	void endGame(bool isWin);
private:
    cocos2d::TMXTiledMap* m_tileMap;
	std::vector<cocos2d::Point> m_pathVec;
	cocos2d::TMXLayer* m_build;
    FireManager* m_fireManager;
	int m_curRound;
    int m_monsterCreateLeft;
	int m_gold;
	int m_curTowerLife;
	cocos2d::Node*  m_monsters;

};

#endif // __HELLOWORLD_SCENE_H__
