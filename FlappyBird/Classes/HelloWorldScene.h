#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>

USING_NS_CC;

#define RATIO 48.0
class HelloWorld : public cocos2d::Layer,public b2ContactListener
{
private:
    void startGame(float dt);
    void stopGame();
    
    void addbird();
    void initWorld();
    void addGround();
    void addGesture();
    void addbarsContainer();
    void addBars(float dt);
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
    
    virtual void BeginContact(b2Contact* contact);
    
    Size visibleSize;
    b2World *world;
    Sprite *bird;
    Sprite *ground;
    Sprite *barsContainer;
    
    
};

#endif // __HELLOWORLD_SCENE_H__
