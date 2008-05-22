/*
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#ifndef _PAUSEOBJECT_H_
#define _PAUSEOBJECT_H_

#include "CommonPrerequisites.h"
#include "PlaylistObject.h"

namespace boost
{
	class thread;
}

namespace rl 
{

	class _RlCommonExport PauseObject : public PlaylistObject {
		private:
			/// Die Dauer der Pause
			long unsigned int mNSec;
			/// Der Timing-Thread
			boost::thread *mThread;
			struct ThreadFunctor {
				/// Die Funktion, die vom Thread ausgeführt wird.
				void operator()();
				/// Zum Zugriff auf das übergeordnete Objekt.
				PauseObject *that;
			} mThreadFunctor;
			friend struct ThreadFunctor;


		public:
			PauseObject(long unsigned int nsec);
			~PauseObject();
			const long unsigned int getPause() const;
			virtual void start();
			virtual void stop();
			virtual void pause();
	};

} // End Namespace

#endif
