#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

enum ContactTag {
    HERO = 0,
    HERO_BULLET,
    ENEMY_BULLET,
    ENEMY
};


enum GameState {
    Welcome = 0,
    Game,
    HighScore,
    Result
};

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void createBigEnemy(float dt);
	void createSmallEnemy(float dt);

	void createHero();
	void HeroFire(float dt);

	void EnemyFire(float dt);
	
	void initBackground();
	void moveBackground(float dt);

	void touchButton(Ref* object,cocos2d::ui::TouchEventType type);
	bool startGame();
	    
	void gameControl(GameState gameState);
	void damageHero();
	void addScore();
	void setResultDlg(bool isBreakRecord);

private:
	cocostudio::Armature* m_hero;
	cocos2d::Sprite* m_background1;
    cocos2d::Sprite* m_background2;
	GameState m_gameState;
	cocos2d::ui::LoadingBar* m_lifeBar;
	cocos2d::ui::TextAtlas* m_scoreNum;
	cocos2d::ui::Widget* m_resultDlg;
	cocos2d::ui::TextField* m_input;
};

#endif // __HELLOWORLD_SCENE_H__
