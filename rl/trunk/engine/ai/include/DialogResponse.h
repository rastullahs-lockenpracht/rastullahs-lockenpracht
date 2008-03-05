/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */
#ifndef __DialogResponse_H__
#define __DialogResponse_H__

#include "AiPrerequisites.h"

#include "DialogElement.h"

namespace rl
{
    class Dialog;
    class DialogImplication;
	class DialogOption;   
    class Creature;

    /**
     * The reponse a DialogCharacter can give in a Dialog
     */
    class _RlAiExport DialogResponse : public DialogElement
    {
    public:
        typedef std::vector<DialogOption*> Options;
        typedef std::vector<DialogImplication*> Implications;

        DialogResponse(int id, int npcId = 0);
        virtual ~DialogResponse();


        void addOption(DialogOption* option);
        void addImplication(DialogImplication* effect);
        virtual const Options& getOptions(Dialog* dialog) const;
        const Options getAvailableOptions(Dialog* dialog) const;

        void applyImplications(Dialog* dialog);

        Creature* getNpc(Dialog* dialog) const;

    private:
        Options mOptions;
        Implications mEffects;
        int mNpcId;
    };
    
    class DialogResponseSelection : public DialogSelection<DialogResponse>
    {
    public:
        DialogResponseSelection(int id);
        virtual const Options& getOptions(Dialog* dialog) const;
    };

}
#endif
