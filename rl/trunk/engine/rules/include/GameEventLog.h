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

#ifndef __GAMEEVENTLOG_H__
#define __GAMEEVENTLOG_H__

#include "RulesPrerequisites.h"

#include "RulesConstants.h"

namespace rl
{
	class _RlRulesExport GameEventLog : public Ogre::Singleton<GameEventLog>
    {
    public:
        GameEventLog();

		/// Set the number of event entries kept in memory.
		/// If more events are logged, oldest message is removed.
		void setEventQueueLength(size_t length);

		/// Get the number of event entries kept in memory.
		size_t getEventQueueLength() const;

		void logEvent(const CeGuiString& eventString, GameEventType type);

	private:
		typedef std::pair<GameEventType, CeGuiString> LogEntry;
		std::deque<LogEntry> mLog;
		size_t mLogSize;
    };

}

#endif //__GAMEEVENTLOG_H__
