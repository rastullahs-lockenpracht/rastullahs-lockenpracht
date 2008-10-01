#include "OgreNewt_Debugger.h"
#include <sstream>

namespace OgreNewt
{

//////////////////////////////////////////////////////////
// DEUBBER FUNCTIONS
//////////////////////////////////////////////////////////
Debugger::Debugger()
{
    m_debugnode = NULL;
    m_defaultcolor = Ogre::ColourValue::White;
}

Debugger::~Debugger()
{
    Debugger::getSingleton().deInit();
}

Debugger& Debugger::getSingleton()
{
    static Debugger instance;
    return instance;
}
		
void Debugger::init( Ogre::SceneManager* smgr )
{
    if( !m_debugnode )
    {
        m_debugnode = smgr->getRootSceneNode()->createChildSceneNode("__OgreNewt__Debugger__Node__");
        //m_debuglines = new Ogre::ManualObject("__OgreNewt__Debugger__Lines__");
    }
}

void Debugger::deInit()
{
	if (m_debugnode)
	{
		m_debugnode->removeAndDestroyAllChildren();
		m_debugnode->getParentSceneNode()->removeAndDestroyChild( m_debugnode->getName() );
		m_debugnode = NULL;
	}

        for(BodyDebugDataMap::iterator it = m_cachemap.begin(); it != m_cachemap.end(); it++)
        {
            Ogre::ManualObject* mo = it->second.m_lines;
            if( mo )
                delete mo;
        }
        m_cachemap.clear();
}


void Debugger::showDebugInformation( OgreNewt::World* world )
{
    m_debugnode->removeAllChildren();

    // make the new lines.
    for( const NewtonBody* body = NewtonWorldGetFirstBody(world->getNewtonWorld()); body; body = NewtonWorldGetNextBody(world->getNewtonWorld(), body) )
        newtonPerBody(body);


    // delete old entries
    BodyDebugDataMap newmap;
    for(BodyDebugDataMap::iterator it = Debugger::getSingleton().m_cachemap.begin(); it != Debugger::getSingleton().m_cachemap.end(); it++)
    {
        if( it->second.m_updated )
            newmap.insert(*it);
    }
    Debugger::getSingleton().m_cachemap.swap(newmap);
}

void Debugger::hideDebugInformation()
{
	// erase any existing lines!
	m_debugnode->removeAllChildren();
}

void Debugger::setMaterialColor(const MaterialID* mat, Ogre::ColourValue col)
{
    m_materialcolors[mat->getID()] = col;
}

void Debugger::setDefaultColor(Ogre::ColourValue col)
{
    m_defaultcolor = col;
}

void _CDECL Debugger::newtonPerBody( const NewtonBody* body )
{
    Debugger& debugger (Debugger::getSingleton());
    MaterialIdColorMap::iterator it = 
        debugger.m_materialcolors.find( NewtonBodyGetMaterialGroupID(body) );


    Body *bod = (OgreNewt::Body*)NewtonBodyGetUserData(body);
    Ogre::Vector3 pos;
    Ogre::Quaternion ori;
    bod->getPositionOrientation(pos, ori);


    // ----------- create debug-text ------------
    std::ostringstream oss_name;
    oss_name << "__OgreNewt__Debugger__Body__" << bod << "__";
    std::ostringstream oss_info;
    oss_info.precision(2);
    oss_info.setf(std::ios::fixed,std::ios::floatfield);
    Ogre::Vector3 inertia;
    Ogre::Real mass;
    bod->getMassMatrix(mass, inertia);
    oss_info << "mass: " << mass << std::endl << "inertia: " << inertia << std::endl;
    oss_info << "pos: " << pos << std::endl << "ori: " << ori << std::endl;
    oss_info << "vel: " << bod->getVelocity() << std::endl << "omega: " << bod->getOmega() << std::endl;
    // ----------- ------------------ ------------



    // look for cached data
    BodyDebugData* data = &Debugger::getSingleton().m_cachemap[bod];
    if( data->m_lastcol == bod->getCollision() ) // use cached data
    {
        // set new position...
        data->m_node->setPosition(pos);
        data->m_node->setOrientation(ori);
        data->m_updated = 1;
        Debugger::getSingleton().m_debugnode->addChild(data->m_node);
        data->m_text->setCaption(oss_info.str());
        data->m_text->setLocalTranslation(bod->getAABB().getSize().y*1.1*Ogre::Vector3::UNIT_Y);
    }
    else
    {
        data->m_lastcol = bod->getCollision();
        data->m_updated = 1;

        if( data->m_node )
        {
            data->m_node->detachAllObjects();
            data->m_node->setPosition(pos);
            data->m_node->setOrientation(ori);
        }
        else
            data->m_node = Debugger::getSingleton().m_debugnode->createChildSceneNode(pos, ori);

        if( data->m_lines )
            data->m_lines->clear();
        else
        {
            std::ostringstream oss;
            oss << "__OgreNewt__Debugger__Lines__" << body << "__";
            data->m_lines = new Ogre::ManualObject(oss.str());
        }

        if( data->m_text )
        {
            data->m_text->setCaption(oss_info.str());
            data->m_text->setLocalTranslation(bod->getAABB().getMaximum().y*1.1*Ogre::Vector3::UNIT_Y);
        }
        else
        {
            data->m_text = new OgreNewt::OgreAddons::MovableText( oss_name.str(), oss_info.str(), "VeraSerifBold-16",0.2);
            data->m_text->setLocalTranslation(bod->getAABB().getMaximum().y/2*Ogre::Vector3::UNIT_Y+Ogre::Vector3::UNIT_Y*0.1);
            data->m_text->setTextAlignment( OgreNewt::OgreAddons::MovableText::H_LEFT, OgreNewt::OgreAddons::MovableText::V_ABOVE );
        }
        data->m_node->attachObject(data->m_text);
        


        data->m_lines->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

        // set color
        if( it != debugger.m_materialcolors.end() )
            data->m_lines->colour(it->second);
        else
            data->m_lines->colour(debugger.m_defaultcolor);

        float matrix[16];
        Converters::QuatPosToMatrix(Ogre::Quaternion::IDENTITY, Ogre::Vector3::ZERO, &matrix[0]);
        
        NewtonCollisionForEachPolygonDo( NewtonBodyGetCollision(body), &matrix[0], newtonPerPoly, data );
        
        data->m_lines->end();
        data->m_node->attachObject(data->m_lines);
    }
}

void _CDECL Debugger::newtonPerPoly( void* userData, int vertexCount, const float* faceVertec, int id )
{
        BodyDebugData* data = (BodyDebugData*)userData;
	Ogre::Vector3 p0, p1;

        if( vertexCount < 2 )
            return;

	int i= vertexCount - 1;
	p0 = Ogre::Vector3( faceVertec[(i*3) + 0], faceVertec[(i*3) + 1], faceVertec[(i*3) + 2] );


	for (i=0;i<vertexCount;i++)
	{
		p1 = Ogre::Vector3( faceVertec[(i*3) + 0], faceVertec[(i*3) + 1], faceVertec[(i*3) + 2] );

		data->m_lines->position( p0 );
		data->m_lines->position( p1 );

		p0 = p1;
	}
}

}	// end namespace OgreNewt

