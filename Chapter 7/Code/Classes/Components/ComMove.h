//ComMove.h

#ifndef Tower_ComMove_h
#define Tower_ComMove_h

#include "cocos2d.h"

class ComMove : public cocos2d::Component
{
public:
    static ComMove* create(std::vector<cocos2d::Point> path);
    virtual void onEnter() override;
	void startMove();

private:
    void initPath(std::vector<cocos2d::Point> path);

protected:
    ComMove(std::vector<cocos2d::Point> path);
    
private:
    std::vector<cocos2d::Point> m_path;
    cocos2d::Point m_startPos;
    cocos2d::Action* m_moveActions;
};

#endif
