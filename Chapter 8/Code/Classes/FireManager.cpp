#include "FireManager.h"

bool FireManager::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	this->schedule(schedule_selector(FireManager::moveBullet));
	return true;
}

void FireManager::moveBullet( float dt )
{
	if (m_tmpMonster.size() >0) {
        for(auto tmp: m_tmpMonster)
        {
            m_monster.pop_front();
            tmp->getOwner()->removeFromParent();
        }
        m_tmpMonster.clear();
    }

	Size winSize = Director::getInstance()->getWinSize();
	bool hitMonster = false;
	//bullets
	for(auto bullet:m_bullets)
	{
		hitMonster = false;
        auto owner = bullet->getOwner();
		Point realPos = Point(bullet->getSpeedX()+owner->getPositionX(),bullet->getSpeedY()+owner->getPositionY());
        
		for (auto monster: m_monster)
		{
            auto distance = realPos.getDistance(monster->getOwner()->getPosition());
            if (distance < monster->getOwner()->getContentSize().width/2)
			{
				auto comLife = dynamic_cast<ComLife*> (monster->getOwner()->getComponent("ComLife"));
				bool isDead = comLife->attacked(bullet->getFireDamage());
				hitMonster = true;
				if (isDead)
				{
                    hitMonster = true;
				    m_monster.remove(monster);
                    monster->getOwner()->removeFromParent();
                    break;
                }
			}
		}
        
		if (hitMonster||winSize.width < realPos.x || winSize.height <realPos.y || 0 > realPos.x || 0>realPos.y)
		{
			owner->removeFromParent();
			m_bullets.remove(bullet);
			break;
		}
		else
		{
			owner->setPosition(realPos);
		}
	}
	//tower
    for(auto tower:m_towers)
    {
        auto owner = tower->getOwner();
		if (tower->getIsFire())
		{
			continue;
		}
		else
		{
			int fireRange = tower->getRange();
			Point towerPos = tower->getOwner()->getPosition();
            for(auto monster:m_monster)
			{
				if (towerPos.getDistance(monster->getOwner()->getPosition())<= fireRange)
				{
                    Sprite* bullet = Sprite::create("Bullet.png");
                    bullet->setPosition(owner->getPosition());
                    auto comBullet = ComBullet::create(30, 10);
                    bullet->addComponent(comBullet);
                    m_bullets.push_back(comBullet);
                    
                    float angle = comBullet->setSpeedXY(owner->getPosition(),monster->getOwner()->getPosition());
                    
                    owner->getParent()->addChild(bullet,2);
                    tower->setIsFire(true);
                    runAction(Sequence::create(DelayTime::create(tower->getReloadTime()),
                                               CallFunc::create([=]{
                        tower->setIsFire(false);
                    }) ,nullptr));
                    
                    bullet->setRotation(angle);
                    owner->setRotation(angle);
                    break;
				}
			}
		}
    }
}
