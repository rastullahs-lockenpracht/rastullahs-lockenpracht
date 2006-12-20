/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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
#ifndef PHYSICALOBSTACLE_H_
#define PHYSICALOBSTACLE_H_

#include "AiPrerequisites.h"

namespace OpenSteer {
    class Obstacle;
}

namespace rl {
    class PhysicalThing;
    
    class _RlAiExport PhysicalObstacle {
        
        public:
            PhysicalObstacle(PhysicalThing *thing);
            virtual ~PhysicalObstacle();
            
            OpenSteer::Obstacle *getObstacle() const;
            
            PhysicalThing *getPhysicalThing() const;
            void setPhysicalThing(PhysicalThing *thing);
            
        protected:
            void _update();
            
        private:
            void setObstacle(OpenSteer::Obstacle *obstacle);

            OpenSteer::Obstacle *mObstacle;
            PhysicalThing *mPhysicalThing;
    };
}


#endif /*PHYSICALOBSTACLE_H_*/
