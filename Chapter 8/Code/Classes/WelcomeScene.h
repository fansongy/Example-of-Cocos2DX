#ifndef Tower_WelcomeScene_h
#define Tower_WelcomeScene_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class WelcomeScene : public cocos2d::Scene
{
public:
    
    virtual bool init();
    CREATE_FUNC(WelcomeScene);
    void touchButton(cocos2d::Ref* object,cocos2d::ui::TouchEventType type);
 
};

#endif
