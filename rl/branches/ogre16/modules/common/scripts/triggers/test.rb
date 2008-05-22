class TestTrigger < Trigger
  def initialize(classname, name)
    super(classname, name, false);
    @_prop_text = "<See your ad here!>";
  end

  def activate()
    print "activated trigger " + getName() + ", message: " + @_prop_text
    return false
  end
  def deactivate()
    print "deactivated trigger " + getName() + ", message: " + @_prop_text
    return false
  end
  def setProperty(name, value)
    if (name == "message")
      @_prop_text = value;
    else
      super(name, value)
    end
  end
  def getProperty(name)
    if (name == "message")
      return @_prop_text
    else
      super(name)
    end
  end
  def getAllProperties()
    ps = super();
    ps.setProperty("message", @_prop_text)
    return ps
  end
end

