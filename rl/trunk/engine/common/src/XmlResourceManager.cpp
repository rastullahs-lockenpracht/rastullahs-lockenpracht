#include "XmlResourceManager.h"

template<> rl::XmlResourceManager* Singleton<rl::XmlResourceManager>::ms_Singleton = 0;

namespace rl {
// Gibt das Singleton zurueck.
XmlResourceManager& XmlResourceManager::getSingleton()
{
	return Ogre::Singleton<XmlResourceManager>::getSingleton();
}

// Gibt einen Zeiger auf das Singleton zurueck.
XmlResourceManager* XmlResourceManager::getSingletonPtr()
{
	return Ogre::Singleton<XmlResourceManager>::getSingletonPtr();
}

// Eine Resource erzeugen
XmlResource* XmlResourceManager::create(const String& resName)
{
	XmlResource* res = new XmlResource(resName);
	add(res);
	return res;
}

}
