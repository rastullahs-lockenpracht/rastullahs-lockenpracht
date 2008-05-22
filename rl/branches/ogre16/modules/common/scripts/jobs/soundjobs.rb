require 'globals.rb'

# Play a 2d sound
def PlaySound2d(sound, volume=1.0, priority = Sound::PRIO_MUSIC)
    job = PlaySound2dJob.new(sound, volume, priority)
    $JS.addJob(job)
end

# Play a 3d sound at position pos
def PlaySound3d(sound, pos, volume=1.0, priority = Sound::PRIO_SFX_ENVIRONMENT)
    job = PlaySound3dJob.new(sound, pos, volume, priority)
    $JS.addJob(job)
end

def _PlaySound2d(sound, volume=1.0, priority = Sound::PRIO_MUSIC)
    PlaySound2dJob.new(sound, volume, priority)
end

# Play a 3d sound at position pos
def _PlaySound3d(sound, pos, volume=1.0, priority = Sound::PRIO_SFX_ENVIRONMENT)
    PlaySound3dJob.new(sound, pos, volume, priority)
end

