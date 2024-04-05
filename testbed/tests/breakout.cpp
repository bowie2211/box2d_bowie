#ifndef BREAKOUT_H
#define BREAKOUT_H
#include "test.h"

class Breakout: public Test 
{
    public:
    Breakout(){}
    void Step(Settings& settings) override
    {
        //run the default physics and rendering
        Test::Step(settings); 
    
        //show some text in the main screen
        g_debugDraw.DrawString(5, m_textLine, "My test");
		m_textLine += m_textIncrement;
    }
    
    static Test* Create()
    {
            return new Breakout;
    }
};
static int testIndex = RegisterTest("Examples", "Breakout", Breakout::Create);
#endif