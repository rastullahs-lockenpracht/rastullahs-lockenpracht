# this trigger is intended to be used by areahelper
# a trigger, that saves an command the appropriate zone and deletes it when finished
class LimitedTimesTrigger < Trigger
  def initialize(classname, name)
    super(classname, name, true);
    @_prop_enter_command = nil;
    @_prop_leave_command = nil;
    @_prop_zone = "";
    @_prop_number = 0; # 0 infty, -1 once, -2 deactivated
  end
  def setEnterCode(&code)
    @_prop_enter_command = code
  end
  def setLeaveCode(&code)
    @_prop_leave_command = code
  end

  def activate()
    if ( @_prop_number > 0 )
      @_prop_number = @_prop_number - 1;
      if (@_prop_number == 0 )
        @_prop_number = -1;
      end
    end
    
    if( @_prop_number >= -1 )
      unless ( @_prop_enter_command == nil )
        begin
          @_prop_enter_command.call
        rescue StandardError => se
          $SCRIPT.logError(se.to_s)
        end
      end
    end
    return false
  end
  def deactivate()
    if( @_prop_number >= -1 )
      unless ( @_prop_leave_command == nil )
        begin
          @_prop_leave_command.call
        rescue StandardError => se
          $SCRIPT.logError(se.to_s)
        end
      end
    end

    if ( @_prop_number == -1 )
      @_prop_number = -2
      unless ( @_prop_zone == "" )
        ZoneManager.getSingleton().destroyZone(@_prop_zone)
      end
      return true
    end
    return false
  end
  def setProperty(name, value)
    if (name == "number")
      @_prop_number = value;
    elsif (name == "zone")
      @_prop_zone = value
    else
      super(name, value)
    end
  end
  def getProperty(name)
    if (name == "zone")
      return @_prop_zone
    elsif (name == "number")
      return @_prop_number
    else
      super(name)
    end
  end
  def getAllProperties()
    ps = super();
    ps.setProperty("zone", @_prop_zone)
    ps.setProperty("number", @_prop_number)
    return ps
  end
end

