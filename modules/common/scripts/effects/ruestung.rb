class Ruestung < Effect
  def initialize(stufe)
    super(stufe);
    setName("Ruestung");
    setDescription("Ruestungsschutz verringert den erlittenen Schaden.");
    @_prop_g_rs = 0
    @_prop_g_be = 0
  end

  def setProperty(name, value)
    if( name == Armor::PROPERTY_G_BE )
      @_prop_g_be = value;
    elsif( name == Armor::PROPERTY_G_RS )
      @_prop_g_rs = value;
    else
      super(name, value)
    end
  end
  
  def getMod(target, type, tag)
    if ((target == Creature::WERT_BE) &&
        (type == MODTYPE_SUM))
      @_prop_g_be
    elsif ((target == Creature::WERT_RS) &&
        (type == MODTYPE_SUM))
      @_prop_g_rs;
    else
      0
    end
  end

  def timeCheck()
	  return PERMANENT;
  end
end
