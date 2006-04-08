/* This source file is part of Rastullahs Lockenpracht.
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

#ifndef SOUNDPLAYLIST_H_
#define SOUNDPLAYLIST_H_

#include "CorePrerequisites.h"
#include "EventListener.h"
#include "SoundEvents.h"
#include <list>

namespace rl
{

class SoundObject;
typedef std::list<SoundObject*> SoundObjectList;

class _RlCoreExport SoundPlaylist : public EventListener<SoundEvent>
{
private:
    SoundObjectList mQueue;
	SoundObjectList::iterator mItem;
	bool mLooping;
    
public:
	SoundPlaylist();
	virtual ~SoundPlaylist();
    void add(SoundObject *object);
	void remove(SoundObject *object);
    void stop();
	void start();
	virtual bool eventRaised(SoundEvent *anEvent);
	void setLooping(bool looping);
	bool isLooping() const;
};

}

#endif /*SOUNDPLAYLIST_H_*/
