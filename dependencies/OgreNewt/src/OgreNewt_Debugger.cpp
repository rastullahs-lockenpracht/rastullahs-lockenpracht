#include "OgreNewt_Debugger.h"

namespace OgreNewt
{

//////////////////////////////////////////////////////////
// DEUBBER FUNCTIONS
//////////////////////////////////////////////////////////
Debugger::Debugger()
{
	m_debuglines = NULL;
	m_debugnode = NULL;
}


Debugger& Debugger::getSingleton()
{
	static Debugger instance;
	return instance;
}
		
void Debugger::init( Ogre::SceneManager* smgr )
{
    m_debugnode = smgr->getRootSceneNode()->createChildSceneNode("__OgreNewt__Debugger__");
    m_debuglines = new Ogre::ManualObject("__OgreNewt__Debugger__");
    m_debugnode->attachObject(m_debuglines); 
}


void Debugger::showLines( OgreNewt::World* world )
{
	m_debuglines->clear();
	m_debuglines->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

	// make the new lines.
    NewtonWorldForEachBodyDo(world->getNewtonWorld(), newtonPerBody);

    m_debuglines->end();	
}

void Debugger::hideLines()
{
	// erase any existing lines!
	m_debuglines->clear(); 
}

void _CDECL Debugger::newtonPerBody( const NewtonBody* body )
{
	NewtonBodyForEachPolygonDo( body, newtonPerPoly );
}

void _CDECL Debugger::newtonPerPoly( const NewtonBody* body, int vertexCount, const float* faceVertec, int id )
{
	Ogre::Vector3 p0, p1;

	int i= vertexCount - 1;
	p0 = Ogre::Vector3( faceVertec[(i*3) + 0], faceVertec[(i*3) + 1], faceVertec[(i*3) + 2] );

	for (i=0;i<vertexCount;i++)
	{
		p1 = Ogre::Vector3( faceVertec[(i*3) + 0], faceVertec[(i*3) + 1], faceVertec[(i*3) + 2] );
		Debugger::getSingleton().m_debuglines->position( p0 );
		Debugger::getSingleton().m_debuglines->position( p1 );
		p0 = p1;
	}
}

}	// end namespace OgreNewt
