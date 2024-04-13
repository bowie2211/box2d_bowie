#include "box2d\box2d.h"
#include "Entity.hpp"
#include <vector>
#include "iostream"

#define PADDLE_POS_IN_METER 3.0f, 0.5f
#define PADDLE_SIZE_IN_METER 1.0f, 0.025f 
#define PADDLE_MOVE_SPEED_MPS 0.001

#define BULLET_POS_IN_METER 8.0f, 5.0f
#define BULLET_RADIUS_IN_METER 1



#define GROUND_LEFT_X_MTR 0.5f
#define GROUND_RIGHT_X_MTR 10.0f
#define GROUND_BOTTOM_Y_MTR 0.1f
#define GROUND_TOP_Y_MTR 20.0f

#define TOP_GROUND_BRICK_DIST_MTR 0.1f
#define LEFT_GROUND_BRICK_DIST_MTR 0.1f

#define BRICK_WIDTH_IN_METER 1.0f
#define BRICK_HEIGHT_IN_METER 0.25f 
#define BRICK_POS_IN_METER_X GROUND_LEFT_X_MTR + BRICK_WIDTH_IN_METER + LEFT_GROUND_BRICK_DIST_MTR
#define BRICK_POS_IN_METER_Y GROUND_TOP_Y_MTR - BRICK_HEIGHT_IN_METER - TOP_GROUND_BRICK_DIST_MTR
#define BRICK_POS_IN_METER BRICK_POS_IN_METER_X, BRICK_POS_IN_METER_Y

#define BRICK_2_BRICK_DIST_METER 0.1f 
#define BRICK_ROWS 2
#define BRICK_COLS 4


class Breakout : public b2ContactListener
{
    public:
    b2Body* Paddle;
    std::vector<b2Body*> bodiesToDelete;//Referenz auf einen vector von Bodies
    Breakout(b2World* m_world);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
};
