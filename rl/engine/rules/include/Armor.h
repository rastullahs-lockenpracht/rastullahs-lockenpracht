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

#ifndef __ARMOR_H__
#define __ARMOR_H__

#include "Item.h"

namespace rl
{
    class Effect;

	/// Objekte die sich mit Kampffertigkeiten einsetzen lassen
	class _RlRulesExport Armor : public Item
	{

	public:
        static const Ogre::String CLASS_NAME;
        
        static const Ogre::String PROPERTY_G_BE;
        static const Ogre::String PROPERTY_G_RS;

        // Konstruktor und Destruktor
		Armor (unsigned int id);
		virtual ~Armor();
		 

		/**
		* @override
		*/

		const CeGuiString getDescription() const;

		// Getter 
		int getKo() const;
		int getBr() const;
		int getRue() const;
		int getBa() const;
		int getLA() const;
		int getRA() const;
		int getLB() const;
		int getRB() const;
		int getGes() const;
		int getGRS() const;
		int getGBE() const;
		// und Setter
		void setKo(int ko);
		void setBr(int br);
		void setRue(int rue);
		void setBa(int ba);
		void setLA(int la);
		void setRA(int ra);
		void setLB(int lb);
		void setRB(int rb);
		void setGes(int ges);
		void setGRS(int grs);
		void setGBE(int gbe);

        virtual const Property getProperty(const CeGuiString& key) const;
        virtual void setProperty(const CeGuiString& key, const Property& value);
        virtual PropertyKeys getAllPropertyKeys() const;

        virtual void onStateChange(GameObjectState oldState, GameObjectState newState);

	private:
		// Ruestungsschutz
		// Kopf
		int mKo;
		// Brust
		int mBr;
		// Ruecken
		int mRue;
		// Bauch und Unterleib
		int mBa;
		// Linker Arm
		int mLA;
		// Rechter Arm
		int mRA;
		// Linkes Bein
		int mLB;
		// Rechtes Bein
		int mRB;
		// Gesamt nach Zonenmodell
		int mGes;

		// Gesamtruestung nach einfachem Modell
		int mGRS;
		// Gesamtbehinderung nach einfachem Modell
		int mGBE;

        Effect* mRsEffect;
	};
}
#endif //__ARMOR_H__
