#include "OgreNewt_Debugger.h"
#include "OgreNewt_World.h"
#include "OgreNewt_Body.h"
#include "OgreNewt_Collision.h"
#include <sstream>

namespace OgreNewt
{

//////////////////////////////////////////////////////////
// DEUBBER FUNCTIONS
//////////////////////////////////////////////////////////
Debugger::Debugger(const OgreNewt::World* world)
{
    m_world = world;
    m_debugnode = NULL;
    m_raycastsnode = NULL;
    m_defaultcolor = Ogre::ColourValue::White;

    m_recordraycasts = false;
    m_markhitbodies = false;
    m_raycol = Ogre::ColourValue::Green;
    m_convexcol = Ogre::ColourValue::Blue;
    m_hitbodycol = Ogre::ColourValue::Red;
    m_prefilterdiscardedcol = Ogre::ColourValue::Black;
}

Debugger::~Debugger()
{
    deInit();
}

void Debugger::init( Ogre::SceneManager* smgr )
{
    if( !m_debugnode )
    {
        m_debugnode = smgr->getRootSceneNode()->createChildSceneNode("__OgreNewt__Debugger__Node__");
        m_debugnode->setListener(this);
    }

    if( !m_raycastsnode )
    {
        m_raycastsnode = smgr->getRootSceneNode()->createChildSceneNode("__OgreNewt__Raycasts_Debugger__Node__");
        m_raycastsnode->setListener(this);
    }
}

void Debugger::deInit()
{
	if (m_debugnode)
	{
        m_debugnode->setListener(NULL);
		m_debugnode->removeAllChildren();
		m_debugnode->getParentSceneNode()->removeAndDestroyChild( m_debugnode->getName() );
		m_debugnode = NULL;
	}


    clearBodyDebugDataCache();


    clearRaycastsRecorded();
    if( m_raycastsnode )
    {
        m_raycastsnode->setListener(NULL);
		m_raycastsnode->removeAndDestroyAllChildren();
		m_raycastsnode->getParentSceneNode()->removeAndDestroyChild( m_raycastsnode->getName() );
		m_raycastsnode = NULL;
    }
}

void Debugger::nodeDestroyed (const Ogre::Node *node)
{
    if(node == m_debugnode)
    {
        clearBodyDebugDataCache();
        m_debugnode = NULL;
    }

    if(node == m_raycastsnode)
    {
        //!TODO: check this... this accesses the node, I'm not shure if this is allowed in this function
        clearRaycastsRecorded();
        m_raycastsnode = NULL;
    }
}


void Debugger::clearBodyDebugDataCache()
{
        for(BodyDebugDataMap::iterator it = m_cachemap.begin(); it != m_cachemap.end(); it++)
        {
            Ogre::ManualObject* mo = it->second.m_lines;
            if( mo )
                delete mo;
        }
        m_cachemap.clear();

}


void Debugger::showDebugInformation( )
{
    if (!m_debugnode)
        return;

    m_debugnode->removeAllChildren();

    // make the new lines.
    for( Body* body = m_world->getFirstBody(); body; body = body->getNext() )
    {
        processBody(body);
    }
        


    // delete old entries
    BodyDebugDataMap newmap;
    for(BodyDebugDataMap::iterator it = m_cachemap.begin(); it != m_cachemap.end(); it++)
    {
        if( it->second.m_updated )
            newmap.insert(*it);
        else
        {
            Ogre::ManualObject* mo = it->second.m_lines;
            if( mo )
                delete mo;
        }
    }
    m_cachemap.swap(newmap);
}

void Debugger::hideDebugInformation()
{
	// erase any existing lines!
    if( m_debugnode )
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

void Debugger::processBody( OgreNewt::Body* bod )
{
    NewtonBody* newtonBody = bod->getNewtonBody();
    MaterialIdColorMap::iterator it = m_materialcolors.find( NewtonBodyGetMaterialGroupID(newtonBody) );


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
    BodyDebugData* data = &m_cachemap[bod];
    if( data->m_lastcol == bod->getCollision() ) // use cached data
    {
        // set new position...
        data->m_node->setPosition(pos);
        data->m_node->setOrientation(ori);
        data->m_updated = 1;
        m_debugnode->addChild(data->m_node);
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
            data->m_node = m_debugnode->createChildSceneNode(pos, ori);

        if( data->m_lines )
            data->m_lines->clear();
        else
        {
            std::ostringstream oss;
            oss << "__OgreNewt__Debugger__Lines__" << bod << "__";
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
        if( it != m_materialcolors.end() )
            data->m_lines->colour(it->second);
        else
            data->m_lines->colour(m_defaultcolor);

        float matrix[16];
        Converters::QuatPosToMatrix(Ogre::Quaternion::IDENTITY, Ogre::Vector3::ZERO, &matrix[0]);
        
        NewtonCollisionForEachPolygonDo( NewtonBodyGetCollision(newtonBody), &matrix[0], newtonPerPoly, data->m_lines );
        
        data->m_lines->end();
        data->m_node->attachObject(data->m_lines);
    }
}

void _CDECL Debugger::newtonPerPoly( void* userData, int vertexCount, const float* faceVertec, int id )
{
    Ogre::ManualObject* lines = (Ogre::ManualObject*)userData;
	Ogre::Vector3 p0, p1;

        if( vertexCount < 2 )
            return;

	int i= vertexCount - 1;
	p0 = Ogre::Vector3( faceVertec[(i*3) + 0], faceVertec[(i*3) + 1], faceVertec[(i*3) + 2] );


	for (i=0;i<vertexCount;i++)
	{
		p1 = Ogre::Vector3( faceVertec[(i*3) + 0], faceVertec[(i*3) + 1], faceVertec[(i*3) + 2] );

		lines->position( p0 );
		lines->position( p1 );

		p0 = p1;
	}
}


// ----------------- raycast-debugging -----------------------
void Debugger::startRaycastRecording(bool markhitbodies)
{
    m_recordraycasts = true;
    m_markhitbodies = markhitbodies;
    clearRaycastsRecorded();
}

bool Debugger::isRaycastRecording()
{
    return m_recordraycasts;
}

bool Debugger::isRaycastRecordingHitBodies()
{
    return m_markhitbodies;
}

void Debugger::clearRaycastsRecorded()
{
    if( m_raycastsnode )
    {
        while( m_raycastsnode->numAttachedObjects() > 0 )
        {
            delete m_raycastsnode->detachObject((unsigned short)0);
        }

        m_raycastsnode->detachAllObjects();
    }
}

void Debugger::stopRaycastRecording()
{
    m_recordraycasts = false;
}

void Debugger::setRaycastRecordingColor(Ogre::ColourValue rayCol, Ogre::ColourValue convexCol, Ogre::ColourValue hitBodyCol, Ogre::ColourValue prefilterDiscardedBodyCol)
{
    m_raycol = rayCol;
    m_convexcol = convexCol;
    m_hitbodycol = hitBodyCol;
    m_prefilterdiscardedcol = prefilterDiscardedBodyCol;
}

void Debugger::addRay(const Ogre::Vector3 &startpt, const Ogre::Vector3 &endpt)
{
    if (!m_raycastsnode)
        return;

    static int i = 0;
    std::ostringstream oss;
    oss << "__OgreNewt__Raycast_Debugger__Lines__Raycastline__" << i++ << "__";
    Ogre::ManualObject *line = new Ogre::ManualObject(oss.str());

    line->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );
    line->colour(m_raycol);
    line->position(startpt);
    line->position(endpt);
    line->end();

    m_raycastsnode->attachObject(line);    
}

void Debugger::addConvexRay(const OgreNewt::Collision* col, const Ogre::Vector3 &startpt, const Ogre::Quaternion &colori, const Ogre::Vector3 &endpt)
{
    if (!m_raycastsnode)
        return;

    static int i = 0;
    // lines from aab
    std::ostringstream oss;
    oss << "__OgreNewt__Raycast_Debugger__Lines__Convexcastlines__" << i++ << "__";
    Ogre::ManualObject *line = new Ogre::ManualObject(oss.str());

    line->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );
    line->colour(m_convexcol);

    // aab1
    Ogre::AxisAlignedBox aab1 = col->getAABB(colori, startpt);
    const Ogre::Vector3* corners1 = aab1.getAllCorners();
    Ogre::AxisAlignedBox aab2 = col->getAABB(colori, endpt);
    const Ogre::Vector3* corners2 = aab2.getAllCorners();
    for(int i = 0; i < 4; i++)
    {
        line->position(corners1[i]); line->position(corners1[(i+1)%4]);
        line->position(corners1[i+4]); line->position(corners1[(i+1)%4+4]);
        line->position(corners2[i]); line->position(corners2[(i+1)%4]);
        line->position(corners2[i+4]); line->position(corners2[(i+1)%4+4]);
        line->position(corners1[i]); line->position(corners2[i]);
        line->position(corners1[i+4]); line->position(corners2[i+4]);
    }
    line->position(corners1[0]); line->position(corners1[6]);
    line->position(corners1[1]); line->position(corners1[5]);
    line->position(corners1[2]); line->position(corners1[4]);
    line->position(corners1[3]); line->position(corners1[7]);
    line->position(corners2[0]); line->position(corners2[6]);
    line->position(corners2[1]); line->position(corners2[5]);
    line->position(corners2[2]); line->position(corners2[4]);
    line->position(corners2[3]); line->position(corners2[7]);

    // bodies
    float matrix[16];
    Converters::QuatPosToMatrix(colori, startpt, &matrix[0]);
    NewtonCollisionForEachPolygonDo( col->getNewtonCollision(), &matrix[0], newtonPerPoly, line );
    Converters::QuatPosToMatrix(colori, endpt, &matrix[0]);
    NewtonCollisionForEachPolygonDo( col->getNewtonCollision(), &matrix[0], newtonPerPoly, line );


    line->end();
    m_raycastsnode->attachObject(line);
}

void Debugger::addDiscardedBody(const OgreNewt::Body* body)
{
    if (!m_raycastsnode)
        return;

    static int i = 0;
    float matrix[16];
    Ogre::Vector3 pos;
    Ogre::Quaternion ori;

    std::ostringstream oss;
    oss << "__OgreNewt__Raycast_Debugger__Lines__DiscardedBody__" << i++ << "__";
    Ogre::ManualObject *line = new Ogre::ManualObject(oss.str());

    line->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );
    line->colour(m_prefilterdiscardedcol);

    body->getPositionOrientation(pos, ori);
    Converters::QuatPosToMatrix(ori, pos, &matrix[0]);
    NewtonCollisionForEachPolygonDo( body->getCollision()->getNewtonCollision() , &matrix[0], newtonPerPoly, line );

    line->end();
    m_raycastsnode->attachObject(line);
}

void Debugger::addHitBody(const OgreNewt::Body* body)
{
    if (!m_raycastsnode)
        return;

    static int i = 0;
    float matrix[16];
    Ogre::Vector3 pos;
    Ogre::Quaternion ori;

    std::ostringstream oss;
    oss << "__OgreNewt__Raycast_Debugger__Lines__HitBody__" << i++ << "__";
    Ogre::ManualObject *line = new Ogre::ManualObject(oss.str());

    line->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );
    line->colour(m_hitbodycol);

    body->getPositionOrientation(pos, ori);
    Converters::QuatPosToMatrix(ori, pos, &matrix[0]);
    NewtonCollisionForEachPolygonDo( body->getCollision()->getNewtonCollision() , &matrix[0], newtonPerPoly, line );

    line->end();
    m_raycastsnode->attachObject(line);
}

}	// end namespace OgreNewt

