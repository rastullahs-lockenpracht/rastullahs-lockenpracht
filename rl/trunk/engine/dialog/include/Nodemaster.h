#ifndef __Rl_DialogNodemaster_H__
#define __Rl_DialogNodemaster_H__

//--	Nodemaster.h
#include <xercesc/dom/DOM.hpp>
#include "DialogPrerequisites.h"

#include <map>
#include <string>


XERCES_CPP_NAMESPACE_USE
using namespace std;

namespace rl
{
	class _RlDialogExport Nodemaster
	{
	public:
		Nodemaster();
		Nodemaster(const string &templateValue);
		~Nodemaster();

		void deleteAllNodes();
		DOMNode* getTemplateNode() const;
		string getTemplate();
		void setTemplate(const string &data);

		Nodemaster *getChild(const string &key);
		void addChild(const string &key, Nodemaster *child);

	private:
		map<string, Nodemaster *> mChildren;
		static Nodemaster* cached;
		string mTemplate;
	};
}
#endif
