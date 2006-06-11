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
#ifndef GRAPH_PATH_H
#define GRAPH_PATH_H

#include <queue>
#include "AimlUtility.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/tokenizer.hpp>
#include "StringTokenizer.h"

namespace MadaBot
{
	template <class S> class AimlNode;

	/**
	 * Represents a SearchPath in the Graph
	 * TODO: remove the unnecessary context from path
	 * TODO: far in the feature, check if something like this could be done with XPath
	 */
	template <class S> class GraphPath
	{
	public:
		GraphPath()
			: mContext(AimlUtility::ASTERISK), 
			  mPattern(AimlUtility::ASTERISK), 
			  mThat(AimlUtility::ASTERISK), 
			  mTopic(AimlUtility::ASTERISK),
			  mSeperator(&(*S(" ").begin())), // seperator needs to be initialized as value_type of S
			  mTemplate(NULL)
		{}
		~GraphPath(){ if(mTemplate!= NULL) delete mTemplate; }

		S& getContext() { return mContext; }
		void setContext(const S& pContext)
		{ 
			mContext = pContext;
			toUpper(mContext);
		}
		S& getPattern(){ return mPattern; }
		void setPattern(const S& pPattern)
		{ 
			mPattern = pPattern;
			toUpper(mPattern);
		}
		S& getThat(){ return mThat; }
		void setThat(const S& pThat)
		{ 
			mThat = pThat;
			toUpper(mThat);
		}
		S& getTopic(){ return mTopic; }
		void setTopic(const S& pTopic)
		{ 
			mTopic = pTopic;
			toUpper(mTopic);
		}

		S getPath() const;

		std::queue<S> getPathQueue() const;

		AimlNode<S>* getTemplateValue() const { return mTemplate; }
		void setTemplateValue(AimlNode<S>* pValue) { mTemplate = pValue; }

		void reset();
	private:
		/**
		 * Helpermethod for converting the string input to upper case
		 * maybe this should be a static helper method in AimlUtility
		 * @param  pData string input data
		 */
		void toUpper(S& pData);
		std::queue<S> tokenize(const S& pData) const;

		S mContext;
		S mPattern;
		S mThat;
		S mTopic;
		

		// tokenizer stuff for boost
		typedef boost::char_separator<typename S::value_type> Seperator;
		typedef typename boost::tokenizer<Seperator, typename S::const_iterator, S > Tokenizer;
		Seperator mSeperator;

		AimlNode<S>* mTemplate;
	};

	template <class S> S GraphPath<S>::getPath() const
	{
		return mContext + AimlUtility::sPathSeperators[AimlUtility::PATTERN]
				+ mPattern + AimlUtility::sPathSeperators[AimlUtility::THAT] 
				+ mThat + AimlUtility::sPathSeperators[AimlUtility::TOPIC] 
				+ mTopic;
	}

	template <class S> std::queue<S> GraphPath<S>::getPathQueue() const
	{
		return tokenize(getPath());
	}

	template <class S> void GraphPath<S>::reset()
	{
		mContext = AimlUtility::ASTERISK;
		mPattern = AimlUtility::ASTERISK;
		mThat = AimlUtility::ASTERISK;
		mTopic = AimlUtility::ASTERISK;
		mTemplate = NULL;
	}

	template <class S> std::queue<S> GraphPath<S>::tokenize(const S& pData) const
	{
		std::queue<S> returnValue;
#ifdef _USE_BOOST
		Tokenizer tokens(pData, mSeperator);
		for (typename Tokenizer::iterator itr = tokens.begin();
				itr != tokens.end(); ++itr)
		{
			returnValue.push( *itr );
		}

#else
		StringTokenizer<S> tokens(pData, " ");
		while(tokens.hasMoreTokens())
		{
			returnValue.push( tokens.getNextToken() );
		}
#endif
		return returnValue;
	}

	template <class S> void GraphPath<S>::toUpper(S& pData)
	{
#ifdef _USE_BOOST
		boost::to_upper(mPattern, AimlUtility::sLocale);		
#else
	//  some string implementations does not support the boot::to_upper(), 
	//  in this case, we use the equivalent c-funtion
		for ( register unsigned int ix = 0; ix < pData.length(); ++ix )
		{
			pData[ix] = toupper(pData[ix]);
		}
#endif
	}
}
#endif
