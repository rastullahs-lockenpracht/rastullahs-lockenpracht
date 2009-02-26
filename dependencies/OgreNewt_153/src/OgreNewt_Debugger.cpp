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
    m_defaultcolor = Ogre::ColourValue::White;
}

Debugger::~Debugger()
{
	// natoka: I don't know but this creates an Access violation in deInit 
	// at line 39
	//Debugger::getSingleton().deInit();
}

Debugger& Debugger::getSingleton()
{
	static Debugger instance;
	return instance;
}
		
void Debugger::init( Ogre::SceneManager* smgr )
{
	m_debugnode = smgr->getRootSceneNode()->createChildSceneNode("__OgreNewt__Debugger__");
	m_debuglines = NULL;
	m_debuglines = new Ogre::ManualObject("__OgreNewt__Debugger__");
}

void Debugger::deInit()
{
	if (m_debugnode)
	{
		m_debugnode->detachAllObjects();
		if (m_debuglines) { delete m_debuglines; m_debuglines = NULL; }
		m_debugnode->getParentSceneNode()->removeAndDestroyChild( m_debugnode->getName() );
		m_debugnode = NULL;
	}
}

void Debugger::setMaterialColor(const MaterialID* mat, Ogre::ColourValue col)
{
    m_materialcolors[mat->getID()] = col;
}

void Debugger::setDefaultColor(Ogre::ColourValue col)
{
    m_defaultcolor = col;
}

void Debugger::showLines( OgreNewt::World* world )
{
	m_debugnode->detachAllObjects();
	m_debuglines->clear();
	m_debuglines->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    // make the new lines.
    NewtonWorldForEachBodyDo(world->getNewtonWorld(), newtonPerBody);

    m_debuglines->end();
	m_debugnode->attachObject(m_debuglines); 
	
}

void Debugger::hideLines()
{
	// erase any existing lines!
	m_debugnode->detachAllObjects();
	m_debuglines->clear(); 
}


void _CDECL Debugger::newtonPerBody( const NewtonBody* body )
{
    Debugger& debugger (Debugger::getSingleton());
    MaterialIdColorMap::iterator it = 
        debugger.m_materialcolors.find( NewtonBodyGetMaterialGroupID(body) );

    if( it != debugger.m_materialcolors.end() )
        debugger.m_debuglines->colour(it->second);
    else
        debugger.m_debuglines->colour(debugger.m_defaultcolor);
         
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
