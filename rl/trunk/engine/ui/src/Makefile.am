INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include  -I../../core/include -I../../rules/include

lib_LTLIBRARIES = libRlUi.la
libRlUi_la_LDFLAGS = $(SWIG_RUBY_LIBS) $(RUBY_LIBS) $(SDL_LIBS) $(LIBS) $(CEGUI_LIBS) #-Wl,--defsym -Wl,Init_libRlUi=Init_RlUi
libRlUi_la_SOURCES = \
	CeConsole.cpp \
	Console.cpp \
	FlyingCameraController.cpp \
	FPSOverlay.cpp \
	InputManager.cpp \
	RubyInterpreter.cpp \
	ScriptObject.cpp \
	UiSubsystem.cpp \
	DebugWindow.cpp \
	ThirdPersonGameController.cpp \
	DialogWindow.cpp \
	CeGuiWindow.cpp \
	ActionChoiceWindow.cpp \
	CharacterSheetWindow.cpp \
	InventoryWindow.cpp
