#ifndef BREAKOUT_TEST_H
#define BREAKOUT_TEST_H
#include "iostream"
#include <vector>
#include "test.h"
#include "breakout.hpp"

class Breakout_Test: public Test, public Breakout
{
    public:


    Breakout_Test():Test(),Breakout(m_world)
    {

    
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