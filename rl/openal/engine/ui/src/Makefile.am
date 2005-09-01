INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(CEGUIOGRE_CFLAGS) \
	$(STLPORT_CFLAGS) -I../include -I../../common/include  -I../../core/include \
	-I../../rules/include -I../components/include -I../../dialog/include \
	-I../../startup/include -I../../sound/include -I../../uicomponents/include

lib_LTLIBRARIES = libRlUi.la
libRlUi_la_LDFLAGS = $(RUBY_LIBS) $(LIBS) \
	$(CEGUI_LIBS) $(OGRE_LIBS) $(CEGUIOGRE_LIBS) -lRlUiComponents -L../../uicomponents/src
libRlUi_la_SOURCES = \
	ActionChoiceWindow.cpp \
	CeGuiWindow.cpp \
	CharacterSheetWindow.cpp \
	CharacterStateWindow.cpp \
	CommandExecutor.cpp \
	CommandMapper.cpp \
	CommandMapperWindow.cpp \
	Console.cpp \
	ContainerContentWindow.cpp \
	DebugWindow.cpp \
	DialogWindow.cpp \
	GameController.cpp \
	GameLoggerWindow.cpp \
	InGameMenuWindow.cpp \
	InputManager.cpp \
	InventoryWindow.cpp \
	MainMenuWindow.cpp \
	MessageWindow.cpp \
	PlaylistWindow.cpp \
	TargetSelectionWindow.cpp \
	UiSubsystem.cpp \
	WindowManager.cpp 
