#include "ComTower.h"

using namespace cocos2d;

ComTower::ComTower()
:m_isFiring(false)
{
    setName("ComTower");
}

ComTower* ComTower::create()
{
    ComTower * ret = new ComTower();
    if (ret != nullptr && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}


void ComTower::onEnter()
{
    m_range = 100;
    m_reloadTime = 0.3;
}

int ComTower::getRange() const
{
    return m_range;
}
