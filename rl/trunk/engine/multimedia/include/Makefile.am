
noinst_HEADERS = \
	ListenerMovable.h \
	MultimediaPrerequisites.h \
	MultimediaSubsystem.h \
	SoundChannel.h \
	SoundManager.h \
	Sound.h \
	SoundDriver.h \
	SoundResource.h \
	SoundEvents.h \
	SoundUpdateTask.h \
	Video.h \
	VideoEvents.h \
	VorbisFile.h \
	null/NullSoundChannel.h \
	null/NullSound.h \
	null/NullDriver.h

if with_fmod3
noinst_HEADERS += \
	fmod3/Fmod3SoundChannel.h \
	fmod3/Fmod3SoundSample.h \
	fmod3/Fmod3SoundStream.h \
	fmod3/Fmod3Driver.h 
endif
	
if with_oal
noinst_HEADERS += \
	openal/OalSoundChannel.h \
	openal/OalSoundSample.h \
	openal/OalSoundStream.h \
	openal/OalDriver.h
endif
