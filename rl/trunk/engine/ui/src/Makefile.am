INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(CEGUIOGRE_CFLAGS) \
    $(STLPORT_CFLAGS) $(OIS_CFLAGS) -I../include -I../../common/include  -I../../core/include \
    -I../../rules/include -I../components/include -I../../dialog/include \
    -I../../startup/include 

lib_LTLIBRARIES = libRlUi.la
libRlUi_la_LIBADD = \
    $(RUBY_LIBS) $(LIBS) $(CEGUI_LIBS) $(OGRE_LIBS) $(CEGUIOGRE_LIBS) $(OIS_LIBS)

libRlUi_la_SOURCES = \
    AboutWindow.cpp \
    ActionChoiceWindow.cpp \
    AbstractWindow.cpp \
    CharacterController.cpp \
    CharacterSheetWindow.cpp \
    CharacterStateWindow.cpp \
    CloseConfirmationWindow.cpp \
    CombatCharacterController.cpp \
    CommandMapper.cpp \
    ConfigComponent.cpp \
    ContainerContentWindow.cpp \
    Console.cpp \
    CutsceneCharacterController.cpp \
    DebugWindow.cpp \
    DialogCharacterController.cpp \
    DialogWindow.cpp \
    FreeFlightCharacterController.cpp \
    GameLoggerWindow.cpp \
    GameObjectInfoWindow.cpp \
    GameObjectPropsWindow.cpp \
    SaveLoadWindow.cpp \
    GameSettings.cpp \
    InfoPopup.cpp \
    InGameMenuWindow.cpp \
    InputManager.cpp \
    InventoryWindow.cpp \
    ItemDragContainer.cpp \
    JournalWindow.cpp \
    ListboxWrappedTextItem.cpp \
    LogWindow.cpp \
    MainMenuWindow.cpp \
    MainMenuEngineWindow.cpp \
    MessageWindow.cpp \
    MovementCharacterController.cpp \
    ObjectDescriptionWindow.cpp \
    PlaylistWindow.cpp \
    ProgressWindow.cpp \
    RenderSystemConfigComponent.cpp \
    SoundDriverConfigComponent.cpp \
    SubtitleWindow.cpp \
    UiSubsystem.cpp \
    WindowFactory.cpp \
    WindowFadeJob.cpp \
    WindowManager.cpp
