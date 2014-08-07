#ifndef Tower_ComLife_h
#define Tower_ComLife_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class ComLife: public cocos2d::Component
{
public:
    static ComLife* create(int maxLife);
    
    virtual void onEnter() override;
    bool attacked(int damage);
    
    
protected:
    ComLife(int maxLife);
        
private:
    cocos2d::ui::LoadingBar* m_hpBar;
    float m_maxLife;
    float m_currentHp;
};

#endif
