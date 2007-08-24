class Ruestung < Effect
  def initialize(stufe)
    super(stufe);
    setName("Ruestung");
    setDescription("Ruestungsschutz verringert den erlittenen Schaden.");
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
      print @_prop_g_be;
      @_prop_g_be
    elsif ((target == Creature::WERT_RS) &&
        (type == MODTYPE_SUM))
      print @_prop_g_be;
      @_prop_g_rs
    end
    return 0;
  end

  def check()
	  return PERMANENT;
  end
end
