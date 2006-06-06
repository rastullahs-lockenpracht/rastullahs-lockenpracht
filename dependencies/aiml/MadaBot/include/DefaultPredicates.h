/**
 * Copyright (c) 2006 Philipp Walser
 * 
 * Permission is hereby granted, free of charge, to any person obtaining 
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE 
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef DEFAULT_PREDICATES_H
#define DEFAULT_PREDICATES_H

#include "Predicates.h"

namespace MadaBot
{
	/**
	 * Attributes/Properties/Abilities and other variables of a bot
	 */
	template <class S> class DefaultPredicates : public Predicates<S>
	{
	public:
		DefaultPredicates() {}
		~DefaultPredicates(){}
		/**
		 * Getter, should be templatized, but then it couldn't be virtual
		 * maybe return string and use lexical cast from boot in ConditionProcessor
		 */
		S getPredicate(const S& pName) const;

		void setPredicate(const S& pName, const S& pValue);
		
		S getType()const;
	protected:
		std::map<S,S> mValues;
	};

	template <class S> S DefaultPredicates<S>::getPredicate(const S& pName) const
	{
		std::map<S,S>::const_iterator itr = mValues.find(pName);
		if(itr != mValues.end())
		{
			return itr->second;
		}
		//throw exception
		return "";
	}

	template <class S> void DefaultPredicates<S>::setPredicate(const S& pName, const S& pValue)
	{
		mValues[pName] = pValue;
	}

	template <class S> S DefaultPredicates<S>::getType() const
	{
		return "default";
	}
}
#endif
