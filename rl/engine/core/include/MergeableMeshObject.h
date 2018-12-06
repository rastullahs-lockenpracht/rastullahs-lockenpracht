/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Clarified Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Clarified Artistic License for more details.
*
*  You should have received a copy of the Clarified Artistic License
*  along with this program; if not you can get it here
*  http://www.jpaulmorrison.com/fbp/artistic2.htm.
*/
#ifndef __MergeableMeshObject_H__
#define __MergeableMeshObject_H__

#include "CorePrerequisites.h"

#include <map>

#include "CoreDefines.h"
#include "MeshObject.h"

namespace rl
{
	class _RlCoreExport MergeableMeshObject : public MeshObject
	{
	public:
		MergeableMeshObject(const Ogre::String& name, const Ogre::String& meshname);
		~MergeableMeshObject();

		void addSubmesh(const Ogre::String& partname, const Ogre::String& meshfile);
		void removeSubmesh(const Ogre::String& partToRemove);
		void replaceSubmesh(const Ogre::String& partToReplace, const Ogre::String& substituteMeshname);
		void setBaseMeshPart(const Ogre::String& partname);

	private:
		Ogre::MeshPtr mBaseMesh;
		MeshPartMap mMeshes;
		Ogre::String mCombinedMeshName;

		void switchTo(const Ogre::MeshPtr& newMesh);
		void updateMesh();
	};

}

#endif //__MergeableMeshObject_H__
