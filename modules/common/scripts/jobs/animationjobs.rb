require 'globals.rb'

def PlayAnimation(actor, anim, doloop=false, replace=true)
    job = PlayAnimationJob.new(actor, anim, doloop, replace)
    $JS.addJob(job)
end

def _PlayAnimation(actor, anim, doloop=false, replace=true)
    PlayAnimationJob.new(actor, anim, doloop, replace)
end

