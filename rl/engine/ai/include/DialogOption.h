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
#ifndef __DialogOption_H__
#define __DialogOption_H__

#include "AiPrerequisites.h"

#include "DialogElement.h"

namespace rl
{
    class Dialog;
    class DialogCondition;
    class DialogResponse;
    class DialogElement;

    /**
     * The selectable options a player can choose from in a Dialog
     */
    class _RlAiExport DialogOption : public DialogElement
    {
    public:
        DialogOption(const CeGuiString& id, const CeGuiString& person, bool autoSelected = false);
        ~DialogOption();

        const CeGuiString& getLabel() const;
        void setLabel(const CeGuiString& label);

        DialogResponse* getResponse() const;
        void setResponse(DialogResponse* response);

        void setPrecondition(DialogCondition* precondition);
        virtual const CeGuiString& getConditionVariableType();
        virtual bool isConditional();

        void setAutoSelected(bool autoSelected);
        bool isAutoSelected() const;
        bool isAvailable(Dialog* dialog) const;

    protected:
        virtual const CeGuiString& getDefaultPerson() const;

    private:
        DialogResponse* mResponse;
        DialogCondition* mPrecondition;
        CeGuiString mLabel;
        bool mAutoSelected;
    };

    typedef DialogSelection<DialogOption> DialogOptionSelection;
}

#endif // __DialogOption_H__
