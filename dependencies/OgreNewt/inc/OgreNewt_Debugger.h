/* 
	OgreNewt Library

	Ogre implementation of Newton Game Dynamics SDK

	OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

		by Walaber

*/
#ifndef _INCLUDE_OGRENEWT_DEBUGGER
#define _INCLUDE_OGRENEWT_DEBUGGER


#include "OgreNewt_Prerequisites.h"
#include "OgreNewt_Tools.h"

namespace OgreNewt
{

    class World;
    class Body;
    class MaterialID;
    class Collision;

	//! For viewing the Newton rigid bodies visually.
	/*!
		This class implements a debug view of the Newton world. You can access it via World::getDebugger().
        It needs to be initialized (call World::getDebugger().init(...)).
	*/
	class _OgreNewtExport Debugger
	{
	
	public:
		~Debugger();

		//! init the debugger.
		/*
			\param smgr pointer to your Ogre::SceneManager
		*/
		void init( Ogre::SceneManager* smgr );

		//! de-init the debugger (cleantup)
		void deInit();

		//! show the newton world
		/*!
			Draws the Newton world as 3D lines with informative text above each body
		*/
		void showDebugInformation();

		//! remove lines and text drawn
		void hideDebugInformation();
	
        //! set default color
        void setDefaultColor(Ogre::ColourValue col);

        //! set Material color
        void setMaterialColor(const MaterialID* mat, Ogre::ColourValue col);

        //! enable additional raycast-debugging (this also enables displaying of recorded raycasts!)
        void startRaycastRecording(bool markhitbodies = false);

        //! returns true, if currently recording raycasts
        bool isRaycastRecording();

        //! returns true, if hit bodies are currently recording
        bool isRaycastRecordingHitBodies();

        //! clears all raycasts, that are currently shown, should probably be done once per frame!
        void clearRaycastsRecorded();

        //! disables raycast-debugging
        void stopRaycastRecording();

        //! set the color of the raycast-debug-lines
        void setRaycastRecordingColor(Ogre::ColourValue rayCol, Ogre::ColourValue convexCol, Ogre::ColourValue hitBodyCol, Ogre::ColourValue prefilterDiscardedBodyCol);

        //! this function is used internally
        void addRay(const Ogre::Vector3 &startpt, const Ogre::Vector3 &endpt);

        //! this function is used internally
        void addConvexRay(const OgreNewt::Collision* col, const Ogre::Vector3 &startpt, const Ogre::Quaternion &colori, const Ogre::Vector3 &endpt);

        //! this function is used internally
        void addDiscardedBody(const OgreNewt::Body* body);

        //! this function is used internally
        void addHitBody(const OgreNewt::Body* body);

	protected:
        friend class OgreNewt::World;
        //! this function must only be used by an instance of the OgreNewt::World class
        Debugger(const OgreNewt::World* world);

        const OgreNewt::World*        m_world;
		Ogre::SceneNode*		m_debugnode;
        typedef std::map<int, Ogre::ColourValue> MaterialIdColorMap;
        MaterialIdColorMap      m_materialcolors;
        Ogre::ColourValue       m_defaultcolor;

        // raycast-debugging
        Ogre::SceneNode*        m_raycastsnode;
        bool                    m_recordraycasts;
        Ogre::ColourValue       m_raycol, m_convexcol, m_hitbodycol, m_prefilterdiscardedcol;
        bool                    m_markhitbodies;

        struct BodyDebugData
        {
            BodyDebugData() : m_lastcol(NULL), m_node(NULL), m_lines(NULL), m_text(NULL), m_updated(false) {}
            const Collision* m_lastcol;
            Ogre::SceneNode* m_node;
            Ogre::ManualObject* m_lines;
            OgreNewt::OgreAddons::MovableText* m_text;
            int m_updated;
        };            
        typedef std::map<OgreNewt::Body*, BodyDebugData> BodyDebugDataMap;
        BodyDebugDataMap m_cachemap;

        //! create debug information for one body
        void processBody(Body* body);


    private:
        //! this function is declared private, so nobody can use it!
        Debugger() {}

        //! this function is declared private, so nobody can use it!
        Debugger(const Debugger&) {}

        //! this function is declared private, so nobody can use it!
        const Debugger& operator=(const Debugger& d) { return *this = d; } // actually this implementation must *never* be used!

		static void _CDECL newtonPerPoly( void* userData, int vertexCount, const float* faceVertec, int id );
	};

}	// end namespace OgreNewt


#endif	// _INCLUDE_OGRENEWT_DEBUGGER

