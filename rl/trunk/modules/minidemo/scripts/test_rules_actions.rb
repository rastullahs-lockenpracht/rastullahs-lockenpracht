include RlScript

class RubyAction < Action
	def getClassName
		return self.class
	end
end

class HeiltrankWirkung < RubyAction
	def initialize(name, description, quali)
		super(name, description)
		@mQuali = quali		
	end
	
	def getTargetClass
		return 0
	end
	
	def doAction(object, actor, target)
		
		
		if @mQuali == "M"
		  # TODO: Qualität M
		end
		
		dice = 0
		const = 0
		
		if @mQuali == "A"
			dice = 1
			# TODO: KL, GE -1 für 1 Stunde
		elsif @mQuali == "B"
		  dice = 1
		  const = 2
		elsif @mQuali == "C"
			dice = 2
			const = 4
		elsif @mQuali == "D"
		  dice = 3
		  const = 6
		elsif @mQuali == "E"
		  const = 999999  # Alle LeP
		elsif @mQuali == "F"
			dice = 3
		  const = 25
		end
		
		if @mQuali == "F"
			target.modifyLe(lep, true);
		else
		  target.modifyLe(lep);
		end
	end
end


class UseTalent < RubyAction
	def initialize(name, desc, talentName, probeMod, action)
		super(name, desc)
		@mTalentName = talentName
		@mProbeMod = probeMod
		@mAction = action
	end
	
	def getTargetClass
		return 0
	end
	
	def doAction(object, actor, target)
		talent = DsaManager.getSingleton.getTalent(@mTalentName)
		twS = actor.doEigenschaftsprobe(talent.getId, @mProbeMod)
		
		#object.doAction(mActionClass, mActionName, actor, target) if twS >= 0
	end
end

class ChangeDescriptionAction < RubyAction
	def initialize(name, newdesc)
		super(name, "")
		@mNewDescription = newdesc
	end
	
	def getTargetClass
		return 0
	end
	
	def doAction(object, actor, target)
		object.setDescription(@mNewDescription)
	end
end