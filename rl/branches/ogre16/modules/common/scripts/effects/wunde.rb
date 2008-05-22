class Wunde < Effect
  def initialize()
    super(0);
    setName("Wunde");
    setDescription("Eine schwere Verletzung");
  end

  def getMod(target, type, tag)
    if (((target == Creature::WERT_MOD_AT) ||
	 (target == Creature::WERT_MOD_PA) ||
	 (target == Creature::WERT_MOD_FK) ||
	 (target == Creature::WERT_INI) ||
	 (target == "GE")) &&
	 (type == MODTYPE_WERTSUM))
        return -2;
    elsif ((target == Creature::WERT_GS) && (type == MODTYPE_SUM))
	return -1;  
    else
        return 0;
    end
  end

  def timeCheck()
    return REMOVE;
  end
end
