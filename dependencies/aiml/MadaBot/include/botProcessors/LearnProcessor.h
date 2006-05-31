/**
 * 
 */
#ifndef LEARN_PROCESSOR_H
#define LEARN_PRCOESSOR_H

#include "XmlMapper/XmlNodeProcessor.h"
using namespace XmlMapper;

namespace MadaBot
{
	template <class T> class GraphBuilder;
	template <class S> class AimlGraphMaster;

	/**
	 * TODO: implement this class correctly as BotProcessor AND AimlProcessor
	 * learn-tag is supported by both
	 */
	template <class S> class LearnProcessor
		: public XmlNodeProcessor<AimlBot, AimlCore, S>
	{
	public:
		/**
		 * Constructor
		 */
		LearnProcessor()
			: XmlNodeProcessor<AimlBot, AimlCore, S>("learn")
		{
			initialize();
		}

		void preprocessStep()
		{
			std::string src = child->getAttribute("src").c_str();
			if(src.empty())
			{
				src = mCurrentNode->getNodeValue();
			}
			if(!src.empty())
			{
				AimlGraphMaster<S>* gm = pProcessHelper->loadAiml(src.c_str());
				if(gm != NULL)
				{
					pProcessHelper->getCurrentBot()->addGraphMaster(gm);
				}
			}
		}

		void processChildStep(XmlNode<S>* pChild)
		{
			// Seems not to be necessary
		}

		void postprocessStep()
		{
			// Seems not to be necessary
		}

	protected:

		void initialize()
		{
			// No Subprocessors
		}
	};
}
#endif
