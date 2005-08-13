/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include "DeletionPropagator.h"

using namespace Ogre;

template<> rl::DeletionPropagator* Singleton<rl::DeletionPropagator>::ms_Singleton = 0;

namespace rl {

    DeletionPropagator& DeletionPropagator::getSingleton(void)
	{
		return Singleton<DeletionPropagator>::getSingleton();
	}

	DeletionPropagator* DeletionPropagator::getSingletonPtr(void)
	{
		return Singleton<DeletionPropagator>::getSingletonPtr();
	}

    DeletionPropagator::DeletionPropagator()
        : m_DeletionListener( NULL )
    {
	}

    DeletionPropagator::~DeletionPropagator()
    {
    }

    void DeletionPropagator::setDeletionListener( DeletionListener* list )
    {
        m_DeletionListener = list;
    }

    void DeletionPropagator::notifyPointerDeleted( void* ptr )
    {
        if( m_DeletionListener != NULL )
        {
            m_DeletionListener->pointerDeleted( ptr );
        }
    }
}
