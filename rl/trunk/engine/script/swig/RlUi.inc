namespace rl {

class UiSubsystem 
{
public:
    static UiSubsystem& getSingleton(void);
    
    void showActionChoice(rl::GameObject* obj);
    Person* getActiveCharacter();
};

class CommandMapper
{
public:
	static CommandMapper& getSingleton();
	//void loadCommandMap(const char* mapfile);

	void setExecutor(rl::CommandExecutor* executor);
};

/*%feature("director") CommandExecutor;   
class CommandExecutor
{
public:
	virtual ~CommandExecutor();

    virtual rl::CeGuiStringVector getCommandsInBattle();
	virtual rl::CeGuiStringVector getCommandsOffBattle();
	virtual void executeCommand(CeGuiString command);
}; */

}