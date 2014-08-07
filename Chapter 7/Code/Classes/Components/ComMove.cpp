#include "ComMove.h"

using namespace cocos2d;

const float NORMAL_SPEED = 150.0f;

ComMove::ComMove(std::vector<Point> path)
{
    setName("ComMove");
    m_path = path;
}

ComMove* ComMove::create(std::vector<Point> path)
{
    ComMove * ret = new ComMove(path);
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

void ComMove::onEnter()
{
    initPath(m_path);
}

void ComMove::startMove()
{
    auto owner = getOwner();
    if (!m_moveActions) {
        CCLOG("Error! Move Action is Empty");
        return;
    }
	owner->setPosition(m_startPos);
	owner->runAction(m_moveActions);
}

void ComMove::initPath(std::vector<cocos2d::Point> path)
{
    if (path.size() == 0)
	  {
		return ;
	  }
    Vector<FiniteTimeAction*> actionsArray;
	  m_startPos = path.at(0);
	  for (unsigned int i = 1;i<path.size();++i)
	  {
		float duration = path.at(i-1).getDistance(path.at(i));
		float time = duration/NORMAL_SPEED;
		actionsArray.pushBack(CCMoveTo::create(time,path.at(i)));
	  }
    m_moveActions = CCSequence::create(actionsArray);
}
