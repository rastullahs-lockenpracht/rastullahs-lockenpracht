require 'globals.rb'

class ActorUpdateAnimationListener < AnimationListener
	def animationFinished(anEvent)
		$CORE.log("animationFinished.");
		anim = anEvent.getAnimation();
		mo = anim.getMeshObject();
		actor = mo.getActor();
		pt = actor.getPhysicalThing();
		pt.updateCollisionHull();
	end
end

