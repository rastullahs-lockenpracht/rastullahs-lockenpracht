class Ausdauernd < Effect
  def initialize(stufe)
    super(stufe);
    setName("Ausdauernd");
    setDescription("Man überanstrengt sich nicht so schnell.");
  end

  def apply()
	  getOwner().getWertStateSet(Creature::WERT_MOD_ERSCHOEPFUNGSSCHWELLE).modifyModifier(+1);
  end

  def remove()
	  getOwner().getWertStateSet(Creature::WERT_MOD_ERSCHOEPFUNGSSCHWELLE).modifyModifier(-1);
  end
end
