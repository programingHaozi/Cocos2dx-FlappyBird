#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    
    initWorld();
    addbird();
    addbarsContainer();
    addGround();
    addGesture();
    
    scheduleOnce(schedule_selector(HelloWorld::startGame), 2);
    
    return true;
}


void HelloWorld::startGame(float dt){
    scheduleUpdate();
    schedule(schedule_selector(HelloWorld::addBars), 1);
}

void HelloWorld::stopGame(){
    unscheduleUpdate();
    unschedule(schedule_selector(HelloWorld::addBars));
}




void HelloWorld::initWorld(){
    world =new b2World(b2Vec2(0, -10));
    world ->SetContactListener(this);
}

void HelloWorld::BeginContact(b2Contact *contact){
    if (contact->GetFixtureA()->GetBody()->GetUserData() == bird||contact->GetFixtureB()->GetBody()->GetUserData() == bird) {
        stopGame();
        MessageBox("游戏失败", "游戏失败");
        
    }
}

void HelloWorld::update(float dt){
    world ->Step(dt, 8, 3);
    
    Sprite *allSprite;
    for (b2Body *b = world ->GetBodyList(); b; b = b ->GetNext()) {
        if (b ->GetUserData()) {
            //每一帧需要重新绘制所有sprite的位置，之前设置的只是B2body的位置，并不是sprite的位置
            allSprite = (Sprite *)b->GetUserData();
            allSprite ->setPosition(b ->GetPosition().x*RATIO, b ->GetPosition().y*RATIO);
            //删除离开屏幕的body
            if (b ->GetPosition().x<-3) {
                if (allSprite) {
                    allSprite ->removeFromParent();
                    log("remove");
                }
                world ->DestroyBody(b);
            }
        }
    }
}


void HelloWorld::addbird(){
    bird = Sprite::create("bird.png");
    auto birdSize = bird ->getContentSize();
    bird ->setPosition(visibleSize.width/2, 9*RATIO);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(visibleSize.width/2/RATIO, 9);
    b2Body *birdBody = world ->CreateBody(&bodyDef);
    
    birdBody ->SetUserData(bird);
    
    b2PolygonShape birdShape;
    birdShape.SetAsBox(birdSize.width/2/RATIO, birdSize.height/2/RATIO);
    b2FixtureDef birdFixtureDef;
    birdFixtureDef.shape = &birdShape;
    birdBody ->CreateFixture(&birdFixtureDef);
    addChild(bird);
}

void HelloWorld::addGround(){
    ground = Sprite::create("ground.png");
    auto groundSize = ground ->getContentSize();
    ground ->setPosition(groundSize.width/2, groundSize.height/2);
    addChild(ground);
    
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position = b2Vec2(groundSize.width/2/RATIO, groundSize.height/2/RATIO);
    b2Body *groundBody = world ->CreateBody(&groundBodyDef);
    
    
    b2PolygonShape groundShape;
    groundShape.SetAsBox(groundSize.width/2/RATIO, groundSize.height/2/RATIO);
    
    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundShape;
    groundBody ->CreateFixture(&groundFixtureDef);
    
    groundBody ->SetUserData(ground);
    
    
}

void HelloWorld::addGesture(){
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener ->onTouchBegan = [this](Touch *t,Event *e){
        for (b2Body *b = world ->GetBodyList(); b; b = b ->GetNext()) {
            if (b ->GetUserData() == bird) {
                b ->SetLinearVelocity(b2Vec2(0, 5));
            }
        }
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void HelloWorld::addBars(float dt){
    
    float offset = -rand()%5;
    //downBar
    auto downBar = Sprite::create("down_bar.png");
    auto downBarSize = downBar ->getContentSize();
    //由于刷新界面的操作处于update方法中，在一帧刚开始调用时，bar的初始位置在原点，刷新后才会显示正确位置。因此现在此处设置初始位置。
    downBar ->setPosition(visibleSize.width+2*RATIO, downBarSize.height/2+offset*RATIO);
    
    b2BodyDef downBarBodyDef;
    downBarBodyDef.type = b2_kinematicBody;
    downBarBodyDef.position = b2Vec2(visibleSize.width/RATIO+2, downBarSize.height/2/RATIO+offset);
    downBarBodyDef.linearVelocity = b2Vec2(-5, 0);
    b2Body *downBarbody = world ->CreateBody(&downBarBodyDef);
    downBarbody ->SetUserData(downBar);
    
    b2PolygonShape downBarShape;
    downBarShape.SetAsBox(downBarSize.width/2/RATIO, downBarSize.height/2/RATIO);
    b2FixtureDef downBarBodyFixtureDef;
    downBarBodyFixtureDef.shape = &downBarShape;
    downBarbody ->CreateFixture(&downBarBodyFixtureDef);
    barsContainer -> addChild(downBar);
    
    //upBar
    auto upBar = Sprite::create("up_bar.png");
    auto upBarSize = upBar ->getContentSize();
    //由于刷新界面的操作处于update方法中，在一帧刚开始调用时，bar的初始位置在原点，刷新后才会显示正确位置。因此现在此处设置初始位置。
    upBar ->setPosition(visibleSize.width+2*RATIO, downBarSize.height+offset*RATIO+upBarSize.height/2+2*RATIO);
    
    
    b2BodyDef upBarBodyDef;
    upBarBodyDef.type = b2_kinematicBody;
    upBarBodyDef.position = b2Vec2(visibleSize.width/RATIO+2, downBarSize.height/RATIO+offset+upBarSize.height/2/RATIO+2);
    upBarBodyDef.linearVelocity = b2Vec2(-5, 0);
    b2Body *upBarbody = world ->CreateBody(&upBarBodyDef);
    upBarbody ->SetUserData(upBar);
    
    b2PolygonShape upBarShape;
    upBarShape.SetAsBox(upBarSize.width/2/RATIO, upBarSize.height/2/RATIO);
    b2FixtureDef upBarBodyFixtureDef;
    upBarBodyFixtureDef.shape = &upBarShape;
    upBarbody ->CreateFixture(&upBarBodyFixtureDef);
    barsContainer -> addChild(upBar);

}

void HelloWorld::addbarsContainer(){
    barsContainer = Sprite::create();
    addChild(barsContainer);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
