class DeadEffect < Effect
  def initialize(stufe)
    super(0);
    setName("Tot.");
    setDescription("Das Leben ist unwideruflich vorbei.");
  end

  def getLifeState()
    return LS_DEAD;
  end
    
  def timeCheck()
    return PERMANENT;
  end
end

class IncapacitatedEffect < Effect
  def initialize(stufe)
    super(0);
    setName("Kampfunfähig.");
    setDescription("Die Verletzungen sind zu stark um zu kämpfen.");
  end

  def getLifeState()
    return LS_INCAPACITATED;
  end
    
  def timeCheck()
    return PERMANENT;
  end
end

class UnconsciousEffect < Effect
  def initialize(stufe)
    super(0);
    setName("Bewusstlos.");
    setDescription("Alles um einen herum wird dunkel.");
  end

  def getLifeState()
    return LS_UNCONSCIOUS;
  end
    
  def timeCheck()
    return PERMANENT;
  end
end
