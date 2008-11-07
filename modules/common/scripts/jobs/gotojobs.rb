require 'globals.rb'

def GoTo(acting_creature, targetpos)
    job = GoToJob.new(acting_creature, targetpos, 0.5, 1000000)
    $JS.addJob(job)
end

def _GoTo(acting_creature, targetpos)
    return GoToJob.new(acting_creature, targetpos, 0.5, 1000000)
end
