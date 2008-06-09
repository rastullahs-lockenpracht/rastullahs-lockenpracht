# this trigger is intended to be used by areahelper
# a trigger, that saves an command the appropriate zone and deletes it when finished
# 
# In order to save the trigger, we need the code for enter- and leave-command to
# be a string, because Proc's cannot be serialized
# The string will be transformed to a Proc, so local variables etc can be used
#
# Example:
# tr.setEnterCode( %q{ print "hello"} )
class LimitedTimesTrigger < Trigger
  def initialize(classname, name)
    super(classname, name, true);
    @_prop_enter_command = nil;
    @_prop_leave_command = nil;
    @_prop_zone = "";
    @_prop_number = 0; # 0 infty, -1 once, -2 deactivated
    @_prop_enter_command_string = nil;
    @_prop_leave_command_string = nil;
  end
  def setEnterCode(code)
    if( code == nil )
      @_prop_enter_command = nil
    else	
      @_prop_enter_command_string = code
    end
    @_prop_enter_command = eval "Proc.new{ #{code} }"
  end
  def setLeaveCode(code)
    if( code == nil )
      @_prop_leave_command = nil
    else
      @_prop_leave_command_string = code
    end
    @_prop_leave_command = eval "Proc.new{ #{code} }"
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
      @_prop_zone = value;
    elsif (name == "enter_code")
      setEnterCode(value);
    elsif (name == "leave_code")
      setLeaveCode(value);
    else
      super(name, value)
    end
  end
  def getProperty(name)
    if (name == "zone")
      return @_prop_zone
    elsif (name == "number")
      return @_prop_number
    elsif (name == "enter_code")
      return @_prop_enter_command_string
    elsif (name == "leave_code")
      return @_prop_leave_command_string
    else
      super(name)
    end
  end
  def getAllPropertyKeys()
    keys = super();
    keys.push("zone");
    keys.push("number");
    keys.push("enter_code");
    keys.push("leave_code");
    return ps
  end
end

