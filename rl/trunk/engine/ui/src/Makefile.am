INCLUDES = $(SDL_CFLAGS) $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(STLPORT_CFLAGS) -I../include -I../../common/include  -I../../core/include -I../../rules/include  

lib_LTLIBRARIES = libRlUi.la
libRlUi_la_LDFLAGS = $(SWIG_RUBY_LIBS) $(RUBY_LIBS) $(SDL_LIBS) $(LIBS) $(CEGUI_LIBS) #-Wl,--defsym -Wl,Init_libRlUi=Init_RlUi
libRlUi_la_SOURCES = \
	ActionChoiceWindow.cpp \
	CeConsole.cpp \
	CeGuiWindow.cpp \
	CharacterSheetWindow.cpp \
	Console.cpp \
	DebugWindow.cpp \
	DialogWindow.cpp \
	FlyingCameraController.cpp \
	FPSOverlay.cpp \
	GameLoggerWindow.cpp \
	InputManager.cpp \
	InventoryWindow.cpp \
	RubyInterpreter.cpp \
	ScriptObject.cpp \
	ThirdPersonGameController.cpp \
	UiSubsystem.cpp
	
