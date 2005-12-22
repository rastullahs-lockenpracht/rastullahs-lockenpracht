
noinst_HEADERS = \
	ListenerMovable.h \
	MultimediaSubsystem.h \
	SoundChannel.h \
	SoundManager.h \
	SoundPrerequisites.h \
    Sound.h \
    SoundDriver.h \
    SoundResource.h \
    SoundEvents.h \
	SoundUpdateTask.h \
	Video.h \
	VideoEvents.h \
	null/NullSoundChannel.h \
	null/NullSoundSample.h \
	null/NullSoundStream.h \
	null/NullDriver.h

if with_fmod
noinst_HEADERS += \
	fmod/FmodSoundChannel.h \
	fmod/FmodSoundSample.h \
	fmod/FmodSoundStream.h \
	fmod/FmodDriver.h 
endif
	
if with_oal
noinst_HEADERS += \
	openal/OalSoundChannel.h \
	openal/OalSoundSample.h \
	openal/OalSoundStream.h \
	openal/OalDriver.h
endif
