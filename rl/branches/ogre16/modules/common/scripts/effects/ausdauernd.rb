class Ausdauernd < Effect
  def initialize(stufe)
    super(stufe);
    setName("Ausdauernd");
    setDescription("Man überanstrengt sich nicht so schnell.");
  end

  def getMod(target, type, tag)
      if ((target == Creature::WERT_MOD_AU) &&
	  (type == MODTYPE_SUM))
	      return getStufe();
      end
      if ((target == "KO") &&
	  (type == MODTYPE_SUM) &&
	  (tag == MODTAG_ERSCHOEPFUNGSSCHWELLE) &&
	  (stufe >= 3))
	      return +1;
      end
      return 0;
  end

  def timeCheck()
	  return PERMANENT;
  end
end
