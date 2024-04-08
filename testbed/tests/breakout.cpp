#ifndef BREAKOUT_H
#define BREAKOUT_H
#include "iostream"
#include <vector>
#include "test.h"
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


class Entity{
    public:
    enum class Type {
        Paddle,
        Brick,
        Bullet,
        Ground
    };
    
    Type entity_type;
    
    Entity(Type type)
    {
        entity_type = type;
    }
    Type getEntityType()
    {
        return entity_type;
    }
    void setEntityType(Type type)
    {
        entity_type = type;
    }
};



class MyContactListener: public b2ContactListener
{
private:
    std::vector<b2Body*>& bodiesToDelete; //Referenz auf einen vector von Bodies
public:
//Konstruktor
MyContactListener(std::vector<b2Body*>& bodiesToDelete):  bodiesToDelete(bodiesToDelete) {}


void BeginContact(b2Contact* contact)
    { 
        //std::cout << "BeginContact" << std::endl;  
    }
 
void EndContact(b2Contact* contact)
{ 
    //std::cout << "EndContact" << std::endl;
    // b2Fixture* a = contact->GetFixtureA();
    // b2Fixture* b = contact->GetFixtureA();
    // b2Body* bodyA = a->GetBody();
    // b2Body* bodyB = b->GetBody();
    // uintptr_t userDataValueA = bodyA->GetUserData().pointer;
    // Entity* EntityPtrA = reinterpret_cast<Entity*>(userDataValueA);

    // uintptr_t userDataValueB = bodyB->GetUserData().pointer;
    // Entity* EntityPtrB = reinterpret_cast<Entity*>(userDataValueB);

    // if ((EntityPtrA->entity_type == Entity::Type::Bullet && EntityPtrA->entity_type == Entity::Type::Brick) || 
    //     (EntityPtrA->entity_type == Entity::Type::Brick && EntityPtrA->entity_type == Entity::Type::Bullet)) {
    //    std::cout << "Bullet hits brick" << std::endl;
    // }

}
 
void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{  // Überprüfen, ob einer der Körper ein Kreiskörper ist
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();
        uintptr_t userDataValueA = bodyA->GetUserData().pointer;
        Entity* EntityPtrA = reinterpret_cast<Entity*>(userDataValueA);

        uintptr_t userDataValueB = bodyB->GetUserData().pointer;
        Entity* EntityPtrB = reinterpret_cast<Entity*>(userDataValueB);
        
        //Test if Bullet has hit a brick
        if (((EntityPtrA->entity_type == Entity::Type::Bullet) && (EntityPtrB->entity_type == Entity::Type::Brick)) || 
           ((EntityPtrA->entity_type == Entity::Type::Brick) && (EntityPtrB->entity_type == Entity::Type::Bullet))) {
           std::cout << "Bullet hits brick" << std::endl;
           if (EntityPtrA->entity_type == Entity::Type::Brick){
                std::cout << "To be destroyed" << std::endl; 
                bodiesToDelete.push_back(bodyA);
           }
           if (EntityPtrB->entity_type == Entity::Type::Brick){
                std::cout << "To be destroyed" << std::endl; 
                bodiesToDelete.push_back(bodyB);
           }     
        }

        //Test if Bullet has hit the Paddle
        if (((EntityPtrA->entity_type == Entity::Type::Bullet) && (EntityPtrB->entity_type == Entity::Type::Paddle)) || 
           ((EntityPtrA->entity_type == Entity::Type::Paddle) && (EntityPtrB->entity_type == Entity::Type::Bullet))) {
            //Figure out which collison partner is the bullet
            if (EntityPtrA->entity_type == Entity::Type::Bullet)
            {
                b2Vec2 additionalForce(0.0f, 1000.0f); // Beispiel: eine Kraft von (10.0f, 0.0f) auf den Kreiskörper ausüben
                //bodyA->ApplyForceToCenter(additionalForce, true);
                bodyA->ApplyLinearImpulseToCenter({0.0f,50.0f}, true);
            } 

            if (EntityPtrB->entity_type == Entity::Type::Bullet)
            {
                b2Vec2 additionalForce(0.0f, 1000.0f); // Beispiel: eine Kraft von (10.0f, 0.0f) auf den Kreiskörper ausüben
                //bodyB->ApplyForceToCenter(additionalForce, true);
                bodyB->ApplyLinearImpulseToCenter({0.0f,50.0f}, true);
            }
        
        }


        if (((EntityPtrA->entity_type == Entity::Type::Bullet) && (EntityPtrB->entity_type == Entity::Type::Ground)) || 
           ((EntityPtrA->entity_type == Entity::Type::Ground) && (EntityPtrB->entity_type == Entity::Type::Bullet))) {
           std::cout << "Bullet hits Ground" << std::endl;
        }

         
}
 
void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{ /* handle post-solve event */ }
};


class Breakout
{};

class Breakout_Test: public Test
{
    public:
    b2Body* Paddle;
    MyContactListener* my_contact_listener;
    std::vector<b2Body*> bodiesToDelete;
    
    Breakout_Test()
    {
        my_contact_listener = new MyContactListener(bodiesToDelete);
        m_world->SetContactListener(my_contact_listener);
        //Bricks
        b2Body* Brick = nullptr;
        b2Vec2 brick_pos = {BRICK_POS_IN_METER_X,BRICK_POS_IN_METER_Y};
        for (int row = 0; row<BRICK_ROWS; row++)
        {   
            for (int col = 0; col<BRICK_COLS; col++){
                b2BodyDef bd;
                Entity* brick_entity = new Entity(Entity::Type::Brick); 
                bd.userData.pointer = reinterpret_cast<uintptr_t>(brick_entity);
                bd.type = b2_kinematicBody;
                bd.position = {brick_pos};
                Brick = m_world->CreateBody(&bd);
                b2PolygonShape box;
                box.SetAsBox(BRICK_WIDTH_IN_METER, BRICK_HEIGHT_IN_METER);
                b2FixtureDef fd;
                fd.shape = &box;
		        fd.density = 3.0f;
                fd.restitution = 0.75f;
	            Brick->CreateFixture(&fd);
                brick_pos.x += (2 * BRICK_WIDTH_IN_METER+BRICK_2_BRICK_DIST_METER);
            }
            brick_pos.y -= (2 * BRICK_HEIGHT_IN_METER+BRICK_2_BRICK_DIST_METER);
            brick_pos.x = BRICK_POS_IN_METER_X;
        }
        
        
        Paddle = nullptr;
        //Paddle_2 = nullptr;
        
        b2Body* Bullet = nullptr;
        //Paddle
        {
        b2BodyDef bd;
        Entity* paddle_entity = new Entity(Entity::Type::Paddle); 
        bd.userData.pointer = reinterpret_cast<uintptr_t>(paddle_entity);
        bd.type = b2_kinematicBody;
        bd.position = {PADDLE_POS_IN_METER};
        Paddle = m_world->CreateBody(&bd);
        b2PolygonShape box;
        box.SetAsBox(PADDLE_SIZE_IN_METER);
        b2FixtureDef fd;
        fd.shape = &box;
		fd.density = 3.0f;
        fd.restitution = 0.75f;
	    Paddle->CreateFixture(&fd);
	}
    

    // Ground body
	b2Body* ground = NULL;
		{
			b2BodyDef bd;
            Entity* ground_entity = new Entity(Entity::Type::Ground); 
            bd.userData.pointer = reinterpret_cast<uintptr_t>(ground_entity);
            bd.type = b2_staticBody;
			ground = m_world->CreateBody(&bd);

			b2Vec2 vs[4];
		
			vs[0].Set(GROUND_LEFT_X_MTR, GROUND_BOTTOM_Y_MTR);
			vs[1].Set(GROUND_LEFT_X_MTR, GROUND_TOP_Y_MTR);
			vs[2].Set(GROUND_RIGHT_X_MTR, GROUND_TOP_Y_MTR);
			vs[3].Set(GROUND_RIGHT_X_MTR, GROUND_BOTTOM_Y_MTR);

			b2ChainShape loop;
			loop.CreateLoop(vs, 4);
			b2FixtureDef fd;
			fd.shape = &loop;
			fd.density = 4.0f;
            fd.restitution = .75f;
			ground->CreateFixture(&fd);
		}
   
    // Bullet
	{
		Entity* bullet_entity = new Entity(Entity::Type::Bullet);
        b2BodyDef bd;
		bd.position.Set(BULLET_POS_IN_METER);
		bd.type = b2_dynamicBody;
        bd.linearDamping = 0.0;
        bd.angularDamping = 0.0;
		bd.bullet = true;
        bd.userData.pointer = reinterpret_cast<uintptr_t>(bullet_entity);
        Bullet = m_world->CreateBody(&bd);
		Bullet->SetGravityScale(0.0);
        b2CircleShape shape;
		shape.m_radius = BULLET_RADIUS_IN_METER;
		b2FixtureDef fd;
		fd.shape = &shape;
        fd.friction = 0.0;
		fd.density = 1.0f;
        fd.restitution = 0.8f;
		Bullet->CreateFixture(&fd);
	}

    }
    void Step(Settings& settings) override
    {
        
        if (glfwGetKey(g_mainWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
            Paddle->SetTransform({Paddle->GetPosition().x-0.1f ,Paddle->GetPosition().y}, 0.0f);
        }

		if (glfwGetKey(g_mainWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			Paddle->SetTransform({Paddle->GetPosition().x+0.1f ,Paddle->GetPosition().y}, 0.0f);
        }
        
        //run the default physics and rendering
        Test::Step(settings); 
        for (b2Body* body: bodiesToDelete){
            this->m_world->DestroyBody(body);
        }
        bodiesToDelete = {};
        //show some text in the main screen
        g_debugDraw.DrawString(5, m_textLine, "My test");
		m_textLine += m_textIncrement;
    }
    
    static Test* Create()
    {
            return new Breakout_Test;
    }
};
static int testIndex = RegisterTest("Examples", "Breakout_Test", Breakout_Test::Create);
#endif