require 'globals.rb'

def PlayAnimation(actor, anim, loops = 1, replace=true, holdOnEnd=false)
    job = PlayAnimationJob.new(actor, anim, loops, replace)
    job.setHoldOnEnd(holdOnEnd)
    $JS.addJob(job)
end

def PlayAnimationTime(actor, anim, duration, replace=true, holdOnEnd=false)
    job = PlayAnimationJob.new(actor, anim, duration, replace)
    job.setHoldOnEnd(holdOnEnd)
    $JS.addJob(job)
end

def _PlayAnimation(actor, anim, loops=1, replace=true)
    PlayAnimationJob.new(actor, anim, loops, replace)
end

def _PlayAnimationTime(actor, anim, duration, replace=true)
    PlayAnimationJob.new(actor, anim, duration, replace)
end

def StartAnimation(actor, anim, doloop=false, replace=true)
    job = StartAnimationJob.new(actor, anim, doloop, replace)
    $JS.addJob(job)
end

def _StartAnimation(actor, anim, doloop=false, replace=true)
    StartAnimationJob.new(actor, anim, doloop, replace)
end

