INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(CEGUIOGRE_CFLAGS) \
    $(STLPORT_CFLAGS) $(OIS_CFLAGS) $(BOOST_CPPFLAGS) -I../include -I../../common/include  -I../../core/include \
    -I../../rules/include -I../components/include -I../../dialog/include \
    -I../../startup/include  -I../../ai/include

lib_LTLIBRARIES = libRlUi.la
libRlUi_la_LIBADD = \
    $(RUBY_LIBS) $(LIBS) $(CEGUI_LIBS) $(OGRE_LIBS) $(CEGUIOGRE_LIBS) $(OIS_LIBS)

libRlUi_la_SOURCES = \
    AboutWindow.cpp \
    ActionChoiceWindow.cpp \
    AbstractWindow.cpp \
    CharacterSheetWindow.cpp \
    CharacterStateWindow.cpp \
    CloseConfirmationWindow.cpp \
    CombatControlState.cpp \
    CombatWindow.cpp \
    CommandMapper.cpp \
    ConfigComponent.cpp \
    ContainerContentWindow.cpp \
    ControlState.cpp \
    Console.cpp \
    CutsceneControlState.cpp \
    DebugWindow.cpp \
    DialogControlState.cpp \
    DialogWindow.cpp \
    FreeflightControlState.cpp \
    GameLoggerWindow.cpp \
    GameObjectInfoWindow.cpp \
    GameSettings.cpp \
    InfoPopup.cpp \
    InGameMenuWindow.cpp \
    InputManager.cpp \
    InventoryWindow.cpp \
    ItemDragContainer.cpp \
    ItemDescriptionDragContainer.cpp \
    ItemIconDragContainer.cpp \
    JournalWindow.cpp \
    ListboxWrappedTextItem.cpp \
    LogWindow.cpp \
    MainMenuWindow.cpp \
    MainMenuEngineWindow.cpp \
    MessageWindow.cpp \
    MovementControlState.cpp \
    ObjectDescriptionWindow.cpp \
    PlaylistWindow.cpp \
    ProgressWindow.cpp \
    PropertiesWindow.cpp \
    RenderSystemConfigComponent.cpp \
    SaveLoadWindow.cpp \
    SoundConfig.cpp \
    SoundDriverConfigComponent.cpp \
    SubtitleWindow.cpp \
    UiSubsystem.cpp \
    WindowFactory.cpp \
    WindowFadeJob.cpp \
    WindowManager.cpp
