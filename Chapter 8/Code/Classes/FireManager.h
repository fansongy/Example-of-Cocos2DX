#include "Components/Constants.h"

USING_NS_CC;

class FireManager
	:public Layer
{
public:
	CREATE_FUNC(FireManager);
	virtual bool init();

	void moveBullet(float dt);

public:
    std::list<ComBullet*> m_bullets;
	std::list<ComTower*>  m_towers;
	std::list<ComMove*>   m_monster;
	std::list<ComMove*>   m_tmpMonster;
};
