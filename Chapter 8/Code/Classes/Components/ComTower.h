#ifndef testTower_ComTower_h
#define testTower_ComTower_h

#include "cocos2d.h"

class ComTower: public cocos2d::Component
{
public:
    static ComTower* create();
    
    virtual void onEnter() override;
    
    CC_SYNTHESIZE(bool,m_isFiring,IsFire);
    CC_PROPERTY_READONLY(int, m_range, Range);
    CC_SYNTHESIZE_READONLY(float, m_reloadTime, ReloadTime);

protected:
    ComTower();
    
};

#endif