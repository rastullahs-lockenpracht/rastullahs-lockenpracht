/**
 *
 */
#ifndef SIMPLE_XML_NODE_H
#define SIMPLE_XML_NODE_H

#include <map>
#include "XmlMapper/XmlNode.h"

namespace XmlMapper
{

	/**
	 * Needs to be a template class that parametrize the string class
	 * TODO: add prefix- and namespace-members for validation
	 */
	template <class S> class SimpleXmlNode : public XmlNode<S>
	{
	public:
		SimpleXmlNode(){}
		SimpleXmlNode(const S& pNodeName, const S& pNodeValue)
			: mNodeName(pNodeName), mNodeValue(pNodeValue)
		{
		}

		S getNodeName() { return mNodeName; }
		S getNodeValue(){ return mNodeValue; }

		void setNodeValue(const S& pValue){ mNodeValue = pValue; }
		
		bool hasAttributes()
		{
			mAttributes.size();
			return (!mAttributes.empty());
		}

		S getAttribute(const S& pName)
		{
			typename Attributes::const_iterator itr = mAttributes.find(pName);
			if(itr != mAttributes.end())
			{
				return itr->second;
			}
			return "";
		}

	protected:
		S mNodeName;
		S mNodeValue;
		typedef std::map<S, S> Attributes;
		Attributes mAttributes;
	};
}
#endif
