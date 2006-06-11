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
#ifndef AIML_GRAPHMASTER_H
#define AIML_GRAPHMASTER_H

#include <queue>
#include "AimlUtility.h"
#include "GraphPath.h"
#include "NodeMapper.h"
#include "Match.h"

namespace MadaBot
{
	/**
	 * Representation of an AIML file in memory
	 * Provides methods for searching in the AIML graph
	 */
	template <class S> class AimlGraphMaster
	{
	public:
		/**
		 * Constructors / Destructors
		 */
		AimlGraphMaster(const S& pName)
			: mName(pName), mRoot(new NodeMapper<S>(""))
		{
			// insert standard wildcards. they will be added in reverse order
			mRoot->addChildNode(new NodeMapper<S>(AimlUtility::ASTERISK));
			mRoot->addChildNode(new NodeMapper<S>(AimlUtility::UNDERSCORE));
		}
		~AimlGraphMaster(){ delete mRoot;}

		/**
		 * Add an entry to the GraphMaster with the given GraphPath
		 * @param pPath  A path that specifies the position of the new entry in the graph
		 */
		void add(const GraphPath<S>& pPath);

		/**
		 * Entrypoint for searching for a match of the given user input
		 *
		 * @param  pPath SearchPath, composed from user input
		 *
		 * @see match( NodeMapper<S>* pNode, NodeMapper<S>* pParent, 
							std::queue<S> pPath, const S& pStar, 
							AimlUtility::PathSeperators pState
										=AimlUtility::CONTEXT);
		 */
		Match<S>* match(const GraphPath<S>& pPath);

	private:
		/**
		 * This methods searches for a match of the user input
		 *
		 * This method can be very computation intensive with large graphs.
		 * If this becomes a problem, this method could and should be optimized.
		 * Some ideas on how to do that can be found in the RebeccaAIML sourcecode
		 *
		 * @param  pNode The node that should be searched for a match
		 * @param  pParent ParentNode (needed for some computations)
		 * @param  pPath The search path of the input
		 * @param  pStar Parts of the InputPath that where substituted by Wildcards
		 * @param  pState Current position in the SearchPath (pattern/topic/that)
		 * @return A Match if a TemplateNode for the given InputPath was found, or NULL
		 */
		Match<S>* match( NodeMapper<S>* pNode, NodeMapper<S>* pParent, 
							std::queue<S> pPath, const S& pStar, 
							AimlUtility::PathSeperators pState
										=AimlUtility::CONTEXT);
		S mName;
		NodeMapper<S>* mRoot;
	};

	template <class S> void AimlGraphMaster<S>::add(const GraphPath<S>& pPath)
	{
	//  Get the path as queue of words
		std::queue<S> path = pPath.getPathQueue();
		
		NodeMapper<S>* node = mRoot, *child = NULL;
	//  traverse the node tree along the path
		for(; !path.empty(); node = child, path.pop())
		{
		//  if there is no child with the given path, add it to the tree
			child = node->getChildNode(path.front());
			if(child == NULL)
			{
				child = new NodeMapper<S>(path.front());
				node->addChildNode(child);
			}
		}
	//  add the AIML template to the current tree path
		child->setTemplate(pPath.getTemplateValue());
	}

	template <class S> Match<S>* AimlGraphMaster<S>::match(const GraphPath<S>& pPath)
	{
		return match(mRoot, mRoot, pPath.getPathQueue(), "");
	}

	template <class S> Match<S>* AimlGraphMaster<S>::match(NodeMapper<S>* pNode, 
															  NodeMapper<S>* pParent, 
															  std::queue<S> pPath, 
															  const S& pStar, 
															  AimlUtility::PathSeperators pState)
	{
		Match<S>* m = NULL;
		NodeMapper<S>* n = NULL;

		if(pPath.empty())
		{
		//  no further input, get the TemplateNode if there is one 
		//  and return the match
			if(!pNode->hasTemplate())
			{
			//  no template, no match
				return NULL;
			}
			m = new Match<S>();
			m->setNode(pNode->getTemplate());
			m->addStar(pStar, pState);
			return m;
		}
		else
		{
		//  traverse the graph for the given search path
		//  go one step further in the path
			S word = pPath.front();
			word.c_str();
			pPath.pop();

		//  check if the current step represents a PathState (pattern/that/topic)
			S searchStr = " ";
			searchStr += word + " ";// + " ");
			typename AimlUtility::PathSeperatorMap::iterator itr = 
				AimlUtility::sPathSeperators.begin();
			for(; itr != AimlUtility::sPathSeperators.end(); ++itr)
			{
				if(searchStr == itr->second)
				{
				//  we're moving to next path component 
					pState = itr->first;
					if ( (n = pNode->getChildNode(word)) ) 
					{
						m = match(n, pNode, pPath, "", pState);
						if ( m != NULL ) 
						{
							m->addStar(pStar, pState);
						//	m->setPattern(path, which); // deprecated
						}
					}
					return m;
				}
			}

		//  search for a match in the "_" wildcard domain
			if ( (n = pNode->getChildNode(AimlUtility::UNDERSCORE)) ) 
			{
				m = match(n, pNode, pPath, word, pState);
				if ( m != NULL ) 
				{
					m->addStar(pStar, pState);
					return m;
				}
			}
		//  search for a match in the domain of the current pathStep/word
			if ( (n = pNode->getChildNode(word)) ) 
			{
				m = match(n, pNode, pPath, pStar, pState);
				if ( m != NULL ) 
				{
					return m;
				}
			}
		//  search for a match in the "*" wildcard domain
			if ( (n = pNode->getChildNode(AimlUtility::ASTERISK)) ) 
			{
				m = match(n, pNode, pPath, word, pState);
				if ( m != NULL ) 
				{
					m->addStar(pStar, pState);
					return m;
				}
			}
		//  search for a match in the next wildcard domain, 
		//  the substituted wildcard value in pStar increases
			if ( pNode == pParent->getChildNode(AimlUtility::ASTERISK) 
				|| pNode == pParent->getChildNode(AimlUtility::UNDERSCORE) ) 
			{
				return match(pNode, pParent, pPath, pStar + " " + word, pState);
			}
			return NULL;
		}
	}
}
#endif
