#ifndef __DsaDataLoader_H__
#define __DsaDataLoader_H__

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include <string>

namespace rl {

	class Talent;
	class Person;

	class DsaDataLoader
	{
	public:
		static void loadData(std::string filename);

	private:
		static XERCES_CPP_NAMESPACE::DOMDocument* loadDataFile(std::string filename);
		
		static int getEBeFromString(const std::string& eBeString);

		static void initializeTalente(XERCES_CPP_NAMESPACE::DOMElement*);
		static Talent* processTalent(int id, int gruppe, XERCES_CPP_NAMESPACE::DOMElement* talentXml);

		static void initializePersonen(XERCES_CPP_NAMESPACE::DOMElement*);
		static Person* processPerson(int id, XERCES_CPP_NAMESPACE::DOMElement* talentXml);
			
		static void initializeKampftechniken(XERCES_CPP_NAMESPACE::DOMElement*);

		DsaDataLoader();
	};

}

#endif
