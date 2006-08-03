# A simple GameAreaListener which will call predefined procedures upon entering
# or leaving the Area.
class SimpleAreaListener < GameAreaListener
    def initialize( inProc, outProc )
        super()
        @inProc = inProc
        @outProc = outProc
    end

	def areaEntered( anEvent )
        if( @inProc != nil )
            @inProc.call( anEvent )
        end
	end
    
    def areaLeft( anEvent )
        if( @outProc != nil )
            @outProc.call( anEvent )
        end
	end
end

# A more sophisticated GameAreaListener which will call predefined procedures upon
#  entering or leaving the Area, but only a given number of times.
class LimitedTimesAreaListener < SimpleAreaListener
    def initialize( inProc, outProc, inTimes, outTimes, deleteActor )
        super( inProc, outProc )
        @inTimes = inTimes
        @outTimes = outTimes
        @deleteActor = deleteActor
    end

    def areaEntered( anEvent )
        if( @inTimes != 0 )
            super( anEvent )
        end

        if( @inTimes > 0 )
            @inTimes = @inTimes-1
        end

        if( @inTimes == 0 )
            $GameEveMgr.removeAreaListener(self)
            if( @deleteActor )
                $AM.destroyActor( anEvent.getSource().getActor() )
            end
        end        
	end

    def areaLeft( anEvent )        
        if( @outTimes != 0 )
            super( anEvent )
        end

        if( @outTimes > 0 )
            @outTimes = @outTimes-1
        end

        if( @outTimes == 0 )
            $GameEveMgr.removeAreaListener(self)
            if( @deleteActor )
                $AM.destroyActor( anEvent.getSource().getActor() )
            end
        end   
	end
end

# A Helper class for creating areas.
class AreaHelper 
    # Creates an SphereArea and a Listener for the given Actor or at the given Position.
    #
    # @param actorOrPosition The Actor or the Position of the area
    # @param radius The radius of the sphere in meters, default 1.0
    # @param flags The types of Actors on which the Area will react, default Actor::QGF_ALL
    # @param inProc The procedure to be called when the Area is entered, default nil
    # @param outProc The procedure to be called when the Area is left, default nil
    # @param inTimes The number of times the Area can be entered before its deleted, default 1
    # @param outTimes The number of times the Area can be left before its deleted, default 1
    #
    # @throws ArgumentError when the first parameter can not be interpreted
    #
    # @returns The created listener
    #
    # @example  AreaHelper::createLimitedTimesSphereArea( [0.0,0.0,0.0], 1.0, Actor::QGF_PLAYER, Proc.new {|e| p "enter"}, Proc.new {|e| p "exit"}, -1, 0 )
    #           Creates a Sphere at 0,0,0 which will react on all players, will
    #           be enterable unlimited times, but remove itself on leaving. 
    #           It will print 'enter' on entering, and 'exit' on leaving, 
    #           which will never occur.
    # @example  AreaHelper::createLimitedTimesSphereArea( [0.0,0.0,0.0], 1.0, Actor::QGF_PLAYER, Proc.new { p "enter" } )
    #           Creates a Sphere at 0,0,0 which will react on all players, will
    #           be enterable one time.
    #           It will print 'enter' on entering, and does nothing on leaving.
    #
    def AreaHelper.createLimitedTimesSphereArea( actorOrPosition, radius=1.0, flags=Actor::QGF_ALL, inProc=nil, outProc=nil, inTimes=1, outTimes=1 )
        # We have not yet created an Actor
        actorCreated = false
        # It's an array, possibly a Position
        if( actorOrPosition.is_a? Array )
            begin  
                # Try to create a new empty actor and to place it at the wanted position
                actor = $AM.createEmptyActor( "EmptyAreaActor" + rand().to_s )
                actor.placeIntoScene( actorOrPosition )
                # We have created an Actor, it should be removed upon removal of the Area
                actorCreated = true 
            rescue
                # If creation or placement fails
                if( actor != nil )
                    $AM.destroyActor( actor )
                end
                raise
            end
        # It's an Actor
        elsif( actorOrPosition.is_a? Actor ) 
            actor = actorOrPosition
        else
            raise ArgumentError, "first parameter must be an Actor or a Position (3-Numbers Array)", caller
        end

        if( actor == nil )
            raise ArgumentError, "Actor must not be nil", caller
        end

        # Create the listener
        listener = LimitedTimesAreaListener.new( inProc, outProc, inTimes, outTimes, actorCreated )

        # Add the Listener
        $GameEveMgr.addSphereAreaListener( actor, radius, listener, flags )

        return listener
    end
end


# AreaHelper::createLimitedTimesSphereArea( [0.0,0.0,0.0], 1.0, Actor::QGF_PLAYER, Proc.new {|e| p "enter"}, Proc.new {|e| p "exit"}, -1, 0 )
# AreaHelper::createLimitedTimesSphereArea( [0.0,0.0,0.0], 1.0, Actor::QGF_PLAYER, Proc.new { p "enter" } )


