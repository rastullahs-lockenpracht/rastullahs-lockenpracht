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
#ifndef __DialogElement_H__
#define __DialogElement_H__

#include "AiPrerequisites.h"

#include "DialogCondition.h"
#include "DialogVariable.h"


namespace rl
{
    class DialogParagraph;

    class _RlAiExport DialogElement
    {
    public:
        const CeGuiString& getId() const;
        void addParagraph(DialogParagraph* paragraph);
        virtual std::list<DialogParagraph*> getParagraphs(Dialog* dialog);
		virtual bool isSelection() const;

    protected:
        DialogElement(const CeGuiString& id);
        virtual ~DialogElement();

    private:
        CeGuiString mId;
        std::list<DialogParagraph*> mParagraphs;
    };

    template<class DialogElementType>
    class DialogSelection : public DialogElementType
    {
    public:
        DialogSelection(const CeGuiString& id)
            : DialogElementType(id, false), mVariable(NULL)
        {
        }


        ~DialogSelection()
        {
        }

        void setVariable(DialogVariable* variable)
        {
            mVariable = variable;
        }

        virtual bool isConditional()
        {
            return (mVariable != NULL);
        }
            
        virtual const CeGuiString& getConditionVariableType()
        {
            RlAssert(mVariable, CeGuiString("No variable found for option with id: " + DialogElementType::getId()).c_str()); 
            return mVariable->getType();
        }

        virtual std::list<DialogParagraph*> getParagraphs(Dialog* dialog)
        {
            RlAssert(dialog, "No dialog given! Possible reason: DialogSelection has no label set");
            DialogElementType* selectedElem = getSelectedElement(dialog);

            if (selectedElem)
            {
                return selectedElem->getParagraphs(dialog);
            }
            std::list<DialogParagraph*> empty;
            return empty;
        }

        void addElement(DialogCondition* condition, DialogElementType* element)
        {
            mElements.push_back(std::make_pair(condition, element));
        }

        DialogElementType* getSelectedElement(Dialog* dialog) const
        {
			// Recalculate switch variable on start, keep for whole switch evaluation
			if (mVariable)
			{
				mVariable->invalidate();
			}

            for (typename CondElemMap::const_iterator it = mElements.begin(); it != mElements.end(); ++it)
            {
                DialogCondition* condition = it->first;
                if ((mVariable && condition->isFulfilled(mVariable, dialog))
                    || (!mVariable && it->first->isTrue(dialog)))
                {
                    return it->second;
                }
            }

            return NULL;
        }

		virtual bool isSelection() const
		{
			return true;
		}

    private:
        typedef std::vector<std::pair<DialogCondition*, DialogElementType*> > CondElemMap;
        CondElemMap mElements;
        DialogVariable* mVariable;
    };

}

#endif // __DialogElement_H__
