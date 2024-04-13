#include "breakout.hpp"
    
    Breakout::Breakout(b2World* m_world)
    {
        //Übernommen aus Tutorial
        //vector von Entities 
        bodiesToDelete = {}; 
        
        //aus testbed übernommen, breakout ist von b2EventListener abgeleitet
        //und implementiert PreSolve.
        m_world->SetContactListener(this);   
        
        //Erzeugung der Bricks
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
        
        
        //Paddle
        Paddle = nullptr;
        
        b2Body* Bullet = nullptr;
        
        {
        b2BodyDef bd;
        Entity* paddle_entity = new Entity(Entity::Type::Paddle); 
        bd.userData.pointer = reinterpret_cast<uintptr_t>(paddle_entity); //To be clarified
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

void Breakout::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{  // Überprüfen, ob einer der Körper ein Kreiskörper ist
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();
        uintptr_t userDataValueA = bodyA->GetUserData().pointer;
        Entity* EntityPtrA = reinterpret_cast<Entity*>(userDataValueA); //to be clarified

        uintptr_t userDataValueB = bodyB->GetUserData().pointer;
        Entity* EntityPtrB = reinterpret_cast<Entity*>(userDataValueB);
        
        //Test if Bullet has hit a brick
        if (((EntityPtrA->entity_type == Entity::Type::Bullet) && (EntityPtrB->entity_type == Entity::Type::Brick)) || 
           ((EntityPtrA->entity_type == Entity::Type::Brick) && (EntityPtrB->entity_type == Entity::Type::Bullet))) {
           if (EntityPtrA->entity_type == Entity::Type::Brick){ 
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
        }

         
}
