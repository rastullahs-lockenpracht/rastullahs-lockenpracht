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

#include "NaturalLanguageProcessor.h"

namespace MadaBot
{
	template <class S> class AimlBot;
}
using namespace MadaBot;

namespace rl
{
	class Creature;
	class ContextInterpreter;
	// deprecated
	class DialogScriptObject;

	class _RlDialogExport DialogCharacter :
		public NaturalLanguageProcessor // deprecated
	{
	public:
		DialogCharacter();
		DialogCharacter(const CeGuiString& name);
		virtual ~DialogCharacter(void);

		// deprecated
		Creature* getDialogCharacter() const;
		void setDialogCharacter(Creature* dialogCharacter);
		Creature* getDialogPartner() const;
		void setDialogPartner(Creature* dialogPartner);
		DialogScriptObject* getScriptObject() const;
		void setScriptObject(DialogScriptObject* scriptObject);

		void setBot(AimlBot<CeGuiString>* bot);

		Creature* getNonPlayerCharacter() const;
		void setNonPlayerCharacter(Creature* npc);

		Creature* getPlayerCharacter() const;
		void setPlayerCharacter(Creature* pc);

		CeGuiString getVoiceFile() const;
		void setVoiceFile(const CeGuiString& filename);

		/**
		 * DialogScriptObject will be deprecated soon, 
		 * DialogCharacter is responsible for the calculations now
		 * TODO: make those methods abstract again
		 */
		virtual int calcOptionValue(const CeGuiString& optionName){return 0;};
		virtual int calcResponseValue(const CeGuiString& responseName){return 0;}
		virtual int calcSelectionValue(const CeGuiString& selectionName){return 0;};

		DialogResponse* respond(const CeGuiString& input);
		
	private:
		AimlBot<CeGuiString>* mBot;
		ContextInterpreter* mInterpreter;
		//deprecated
		DialogScriptObject* mScriptObject;
		
		Creature* mNonPlayerCharacter;
		Creature* mPlayerCharacter;
		CeGuiString mVoiceFile;

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

	inline DialogScriptObject* DialogCharacter::getScriptObject() const
	{
		return mScriptObject;
	}
}

#endif
