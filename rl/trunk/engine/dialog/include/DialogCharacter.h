/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#ifndef __Rl_DialogCharacter_H__
#define __Rl_DialogCharacter_H__

#include "DialogPrerequisites.h"
//#include "DialogListener.h"

namespace MadaBot
{
	template <class S> class AimlBot;
	template <class S> class Predicates;
}
using namespace MadaBot;

namespace rl
{
	class Creature;
	class DialogResponse;

    /**
     * Representation of a Character that is able to respond in a Dialog
     * @todo Art der Erstellung ändern, das ist zu kompliziert.
     */
	class _RlDialogExport DialogCharacter 
	{
	public:
		DialogCharacter();
	//	DialogCharacter(const CeGuiString& name);
		virtual ~DialogCharacter(void);

		const CeGuiString getName() const;

		Predicates<CeGuiString>* getPredicates(const CeGuiString& pType);

		// deprecated
		Creature* getDialogCharacter() const;
		void setDialogCharacter(Creature* dialogCharacter);
		Creature* getDialogPartner() const;
		void setDialogPartner(Creature* dialogPartner);

		void initialize();
		/**
		 * SHOULD NOT BE CALLED IN PUBLIC
		 * set the dialogbot, this is done in DialogSubsystem/AimlCore
		 */
		void setBot(AimlBot<CeGuiString>* bot);
        
        bool hasExitRequest() const;
/*		void addDialogListener(DialogListener* listener);
		void removeDialogListener(DialogListener* listener);
		void updateDialogListeners(DialogListener::DialogEvent e);
*/
		Creature* getNonPlayerCharacter() const;
		void setNonPlayerCharacter(Creature* npc);

		Creature* getPlayerCharacter() const;
		void setPlayerCharacter(Creature* pc);

		// not used in script, thereby it can return a reference
		const CeGuiString& getVoiceFile() const;
		void setVoiceFile(const CeGuiString& filename);

		/**
		 * calculation of special condition values
		 * should have a second parameter "patternValue"
		 */
		virtual int calcOptionValue(const CeGuiString& optionName);
		virtual int calcResponseValue(const CeGuiString& responseName);
		virtual int calcSelectionValue(const CeGuiString& selectionName);

		DialogResponse* createResponse(const CeGuiString& input);
		bool isActive(){ return mActive; }
		
	private:
		bool mActive;
		AimlBot<CeGuiString>* mBot;

		Creature* mNonPlayerCharacter;
		Creature* mPlayerCharacter;

//		typedef std::list<DialogListener*> Listeners;
//		Listeners mDialogListener;

	};

	inline void DialogCharacter::setBot(AimlBot<CeGuiString>* bot)
	{
		mBot = bot;
	}

	inline Creature* DialogCharacter::getDialogCharacter() const
	{
		return mPlayerCharacter;
	}
	
	inline void DialogCharacter::setDialogCharacter(Creature* dialogCharacter)
	{
		mPlayerCharacter = dialogCharacter;
	}

	inline Creature* DialogCharacter::getPlayerCharacter() const
	{
		return mPlayerCharacter;
	}
	
	inline void DialogCharacter::setPlayerCharacter(Creature* pc)
	{
		mPlayerCharacter = pc;
	}

	inline Creature* DialogCharacter::getDialogPartner() const
	{
		return mNonPlayerCharacter;
	}

	inline void DialogCharacter::setDialogPartner(Creature* dialogPartner)
	{
		mNonPlayerCharacter = dialogPartner;
	}

	inline Creature* DialogCharacter::getNonPlayerCharacter() const
	{
		return mNonPlayerCharacter;
	}

	inline void DialogCharacter::setNonPlayerCharacter(Creature* npc)
	{
		mNonPlayerCharacter = npc;
	}

    /// @todo Magic Number.
    inline int DialogCharacter::calcOptionValue(const CeGuiString& optionName)
    {
        return 4;
    }

	inline int DialogCharacter::calcResponseValue(const CeGuiString& responseName)
    {
        return 4;
    }

	inline int DialogCharacter::calcSelectionValue(const CeGuiString& selectionName)
    {
        return 4;
    }
}

#endif
