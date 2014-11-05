#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "B2Sprite.h"
class HelloWorld : public cocos2d::Layer
{
private:
    void addbird();
    void initWorld();
    Size visibleSize;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual void update(float dt);
    
    b2World *world;
   
    B2Sprite *bird;
};

#endif // __HELLOWORLD_SCENE_H__
