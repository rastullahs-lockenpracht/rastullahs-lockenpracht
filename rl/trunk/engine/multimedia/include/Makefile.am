
noinst_HEADERS = \
	ListenerMovable.h \
	MultimediaPrerequisites.h \
	SoundChannel.h \
	Sound.h \
	SoundDriver.h \
	SoundEvents.h \
	SoundResource.h \
	Video.h \
	VideoEvents.h \
	VideoManager.h \
	VorbisFile.h \
	null/NullSoundChannel.h \
	null/NullSound.h \
	null/NullDriver.h \
	null/NullListener.h

if with_fmod3
noinst_HEADERS += \
	fmod3/Fmod3SoundChannel.h \
	fmod3/Fmod3SoundSample.h \
	fmod3/Fmod3SoundStream.h \
	fmod3/Fmod3Driver.h \
	fmod3/Fmod3Listener.h
endif
	
if with_oal
noinst_HEADERS += \
	openal/OalSoundChannel.h \
	openal/OalSoundSample.h \
	openal/OalSoundStream.h \
	openal/OalDriver.h \
	openal/OalListener.h
endif
