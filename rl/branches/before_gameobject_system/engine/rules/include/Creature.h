/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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



#ifndef __CREATURE_H__
#define __CREATURE_H__

#include "RulesPrerequisites.h"

#include "GameObject.h"
#include "Eigenschaft.h"
#include "Inventory.h"
#include "Container.h"
#include "Weapon.h"
#include "EigenschaftenStateSet.h"
#include "TalentStateSet.h"
#include "ZauberStateSet.h"
#include "SonderfertigkeitenStateSet.h"
#include "EffectManager.h"

//#include "CompositeEffect.h"

using namespace std;

namespace rl
{
	class Effect;

///////////////////////////////////////////////////////////////////////////////
// Konstanten

    /**
    *  Der minimale Wert mit dem Proben Spezialtalente durchfuehrt werden
    *  duerfen.
    *  @note Es gibt da verschiedene Angaben zwischen dem Basisregelwerk 
    *   und SuH.
    **/
    static const int TALENT_MIN_TAW_FOR_SPEZIAL = 0;
    /**
    *  Der minimale Wert mit dem Proben Berufstalente durchfuehrt werden
    *  duerfen.
    *  @note Es gibt da verschiedene Angaben zwischen dem Basisregelwerk 
    *   und SuH.
    **/
    static const int TALENT_MIN_TAW_FOR_BERUF = 0;

    /// Eine Doppel-1
	static const int RESULT_AUTOERFOLG = 100;
    /// Eine Dreifach-1
	static const int RESULT_SPEKT_AUTOERFOLG = 1000;
    /// Eine Doppel-20
	static const int RESULT_AUTOMISSERFOLG = -100;
    /// Eine Dreifach-20
	static const int RESULT_SPEKT_AUTOMISSERFOLG = -1000;
    /** 
     *  Eine bestaetigte 1 bei einer @ref abbdea "AT" oder @ref abbdep "PA",
     *  eine einfache 1 bei einer Eigenschaftsprobe.
     **/
	static const int RESULT_GLUECKLICH = 100;
    /**
     *  Eine bestaetigte 20 bei einer @ref abbdea "AT" oder @ref abbdep "PA",
     *  eine einfache 20 bei einer Eigenschaftsprobe.
     **/
	static const int RESULT_PATZER = -100;
    /// Eine normal gelungene Attacke oder Parade
	static const int RESULT_ERFOLG = 1;
    /// Eine normal misslungene Attacke oder Parade
	static const int RESULT_MISSERFOLG = -1;


    /// Basistalente
	/// @todo Passenderen Ort suchen
    static const CeGuiString TALENT_ART_BASIS = "Basis";
    /// Spezialtalente
    static const CeGuiString TALENT_ART_SPEZIAL = "Spezial";
    /// Berufstalente
    static const CeGuiString TALENT_ART_BERUF = "Beruf";

    /**
     *  @defgroup CreatureRubyExports Creature
     *  @ingroup RulesRubyExports
     **/

    /**
    * Basisklasse aller spielrelevanten Objekte in RL. Die Klasse kapselt alle 
    * Wesen, die aktiv im Spiel agieren, sei es computer- oder spielergesteuert. 
    * Klasse enthaelt Methoden und Daten fuer die DSA-Spielwerte (Lebensenergie,
    * Eigenschaften) und Methoden fuer alles was fuer die Interaktion mit der 
    * Spielwelt noetig ist (z.B. Proben).
    */
    class _RlRulesExport Creature : public GameObject
    {
    public:
///////////////////////////////////////////////////////////////////////////////
// Typedefs
		/**
		 *  Liste der Talente. Besteht aus den Namen der Talente (z.B. Athletik)
         *  als Schluessel und ihrem Wert.
		 **/
		typedef map<const CeGuiString, TalentStateSet*> TalentMap;

		///@warning Nur zu Testzwecken da. Wird spaeter entfernt.
        Creature(const CeGuiString name,
                 const CeGuiString description);
		        
        /// Der Standarddestruktor.
        virtual ~Creature();

///////////////////////////////////////////////////////////////////////////////
// Enums
        /// Beschreibt die art des zugefuegten Schadens
        enum DamageTag
        {
            DAMAGETAG_NORMAL = 1,
            DAMAGETAG_PIERCING, ///< Wie Pfeile, Bolzen oder gezielte Stiche, verletzen schon bei KO/2 < SP
            DAMAGETAG_FIRE
        };
        /// Die verschiedenen stati in denen sich eine @ref abbdes "SF" befinden kann.
        enum SfStatus
        {
            SFSTATUS_IN_TRAINING = 1,
            SFSTATUS_OK,
            SFSTATUS_REQUIREMENTS_NOT_MET /// @todo Wird das ueberhaupt gebraucht?
        };
  	    /**
         * Identifiziert welcher Wert angesprochen werden soll.
         * @warning Bei WERT_MOD_* gibt getValue() Unsinn zurueck, da 
         * der Multiplikator dann auf 0 + modifier multipliziert wird. 
         * Stattdessen einzeln auf die Modifikatoren zugreifen!
         * @ingroup CreatureRubyExports
         **/
        enum Wert
        {
            WERT_MOD_AE = 1, ///< Astralenergie
            WERT_MOD_LE, ///< Lebensenergie
            WERT_MOD_AT, ///< Attacke
            WERT_MOD_PA, ///< Parade
            WERT_MOD_FK, ///< Fernkampf
            WERT_MOD_AU, ///< Ausdauer
            WERT_MOD_MR, ///< Magieresistenz
            WERT_MOD_INI, ///< Initiative
            WERT_MOD_REGENERATION_LE, ///< Naechtliche Regeneration. modifier modifiziert den W6, ProbenModifier modifiziert die KO-Probe.
            WERT_MOD_REGENERATION_AE, ///< Astrale Regeneration. modifier modifiziert den W6, ProbenModifier modifiziert die IN-Probe.
            WERT_MOD_ERSCHOEPFUNGSSCHWELLE, ///< Die Modifkitoren von KO bezueglich der Erschoepfungsschwelle.
            WERT_MOD_ALL_EIGENSCHAFTSPROBEN, ///< Modifiziert alle Proben, die mit 1W20 gewuerfelt werden.
            WERT_MOD_ALL_TALENTPROBEN, ///< Modifiziert alle Proben, die mit 3W20 gewuerfelt werden.
            WERT_GS, ///< Geschwindigkeit
            WERT_SOZIALSTATUS, ///< Sozialstatus
            WERT_BE, ///< Behinderung
            WERT_KAMPFUNFAEHIGKEITSSCHWELLE ///< Die Schwelle zur Kampfunfaehigkeit, bei Menschen 5 LE
        };
        
///////////////////////////////////////////////////////////////////////////////
// Werte

        /**
         *  Veraendert die aktuelle @ref abbdel "LE" der Kreatur.
         *  @param mod Der Wert um den die LE erhoeht (erniedrigt bei 
         *   negativem Wert) werden soll
         *  @param ignoreMax Wenn true kann die LE auch ueber das Maximum
         *   hinaus erhoeht werden.
         *  @ingroup CreatureRubyExports
         **/
        virtual void modifyLe(int mod, bool ignoreMax = false);
        /**
         *  Gibt die aktuelle @ref abbdel "LE" der Kreatur zurueck.
         *  @ingroup CreatureRubyExports
         **/
        virtual int getLe();
        /**
         *  Gibt die maximale Anzahl an @ref abbdel "LE" zurueck, die die Kreatur haben kann.
         **/
        virtual int getLeMax();

        /**
         *  Veraendert die aktuelle @ref abbdea "AE" der Kreatur.
         *  @param mod Der Wert um den die AE erhoeht (erniedrigt bei 
         *   negativem Wert) werden soll
         *  @param ignoreMax Wenn true kann die AE auch ueber das Maximum
         *   hinaus erhoeht werden.
         *  @ingroup CreatureRubyExports
         **/
		virtual void modifyAe(int mod,  bool ignoreMax = false);
        /**
         *  Gibt die aktuelle @ref abbdea "AE" der Kreatur zurueck.
         **/
        virtual int getAe();
        /**
         *  Gibt die maximale Anzahl an @ref abbdea "AE" zurueck, die die Kreatur haben kann.
         **/
        virtual int getAeMax();

        /**
         *  Veraendert die aktuelle @ref abbdea "AU" der Kreatur.
         *  @param mod Der Wert um den die AU erhoeht (erniedrigt bei 
         *   negativem Wert) werden soll
         *  @param ignoreMax Wenn true kann die AU auch ueber das Maximum
         *   hinaus erhoeht werden.
         *  @ingroup CreatureRubyExports
         **/
		virtual void modifyAu(int mod,  bool ignoreMax = false);
        /**
         *  Gibt die aktuelle @ref abbdea "AU" der Kreatur zurueck.
         **/
        virtual int getAu();
        /**
         *  Gibt die maximale Anzahl an @ref abbdea "AE" zurueck, die die Kreatur haben kann.
         **/
        virtual int getAuMax();

        /**
         *  Berechnet den @ref abbdea "AT"-Basiswert der Kreatur.
         **/
		virtual int getAttackeBasis();
        /**
         *  Berechnet den @ref abbdep "PA"-Basiswert der Kreatur.
         **/
	    virtual int getParadeBasis();
        /**
         *  Berechnet den @ref abbdef "FK"-Basiswert der Kreatur.
         **/
		virtual int getFernkampfBasis();
        /**
         *  Berechnet den @ref abbdei "INI"-Basiswert der Kreatur.
         **/
		virtual int getInitiativeBasis();

        /**
         *  Setzt den Basiswert \a wertId auf \a value.
         *  @param wertId Bezeichnet welcher Basiswert veraendert werden soll.
         *  @param value Der neue wert des Basiswerts.
         *  @see Wert
         *  @ingroup CreatureRubyExports
         **/
		virtual void setWert(Wert wertId, int value);
		/** 
         *  Liefert den Basiswert \a wertId zurueck.
		 *  @param wertId Bezeichnet welcher Basiswert zurueckgeliefert werden soll.
         *  @param getUnmodified Wenn true wird der unmodifizierte Wert 
         *   zurueckgegeben.
		 *  @return Der Wert des Basiswerts.
		 *  @throws InvalidArgumentException wertId konnte in mWerte nicht
		 *    gefunden werden.
		 **/
		virtual int getWert(Wert wertId, bool getUnmodified = false);
        /**
         *  Gibt einen Zeiger auf das ganze Stateset des Basiswerts \a wertId zurueck.
         *  @param wertID Bezeichnet den Basiswert, von dem das StateSet zurueckgegeben
         *   werden soll.
         *  @ingroup CreatureRubyExports
         **/
		virtual StateSet* getWertStateSet(Wert wertId);
        /**
         *  Gibt der Kreatur @ref abbddea "AP".
         *  @param modifier Kann auch negativ sein (z.B. Borbarad-Moskitos).
         **/
        void modifyAp(int modifier);
        /**
         *  Liefert die Gesamtzahl der @ref abbdea "AP" zurueck.
         **/
        int getAp();
        /**
         *  Ver�ndert die verbrauchten @ref abbdea "AP", wird beim Steigern
         *  aufgerufen.
         *  @param modifier Kann auch negativ sein (z.B. Borbarad-Moskitos).
         **/
        void modifyUsedAp(int modifier);
        /**
         *  Liefert die verbrauchten @refabbdea "AP" zurueck.
         **/
        int getUsedAp();


///////////////////////////////////////////////////////////////////////////////
// Eigenschaften

		/**
		 *  Liefert den Wert der Eigenschaft \a eigenschaftName zurueck.
		 *  @param eigenschaftName Der Name als Abkuerzung (z.B. @ref abbdem "MU", 
         *   @abbdef "FF", etc.).
		 *  @return Der Wert der Eigenschaft.
		 *  @throws InvalidArgumentException Die Eigenschaft konnte nicht
		 *   gefunden werden (Name ausgeschrieben statt abgekuerzt? 
		 *   Gross/Kleinschreibung beachtet?).
         *  @ingroup CreatureRubyExports
		 **/
        virtual int getEigenschaft(const CeGuiString eigenschaftName);
		/**
		 *  Setzt den Wert der Eigenschaft \a eigenschaftName auf \a value.
		 *  @param eigenschaftName Der Name als Abkuerzung (z.B. @ref abbdem "MU", 
         *   @ref abbdef "FF", etc.).
		 *  @param value Der Wert auf den die Eigenschaft gesetzt werden soll.
		 *  @throws InvalidArgumentException Die Eigenschaft konnte nicht
		 *   gefunden werden (Name ausgeschrieben statt abgekuerzt? 
		 *   Gross/Kleinschreibung beachtet?).
         *  @ingroup CreatureRubyExports
		 **/
        virtual void setEigenschaft(const CeGuiString eigenschaftName, int value);
		/** Addiert \a mod auf den Wert der Eigenschaft \a eigenschaftName.
		 *  @param eigenschaftName Der Name als Abkuerzung (z.B. @ref abbdem "MU", 
         *   @ref abbdef "FF", etc.).
		 *  @param mod Wird auf den Wert addiert (kann auch negativ sein).
		 *  @throws InvalidArgumentException Die Eigenschaft konnte nicht
		 *   gefunden werden (Name ausgeschrieben statt abgekuerzt? 
		 *   Gross/Kleinschreibung beachtet?).
         *  @ingroup CreatureRubyExports
		 **/
        virtual void modifyEigenschaft(const CeGuiString eigenschaftName, int mod);

        /**
         *  Liefert einen Zeiger auf das StateSet von \a eigenschaftName zurueck.
		 *  @param eigenschaftName Der Name als Abkuerzung (z.B. @ref abbdem "MU", 
         *   @ref abbdef "FF", etc.).
		 *  @throws InvalidArgumentException Die Eigenschaft konnte nicht
		 *   gefunden werden (Name ausgeschrieben statt abgekuerzt? 
		 *   Gross/Kleinschreibung beachtet?).
         *  @ingroup CreatureRubyExports
		 **/
		virtual EigenschaftenStateSet* getEigenschaftenStateSet(const CeGuiString eigenschaftName);

///////////////////////////////////////////////////////////////////////////////
// Talente

		/** 
         *  Fuegt das Talent \a talentName zu \c mTalente hinzu.
		 *  Das neue Talent wird mit @ref abbdet "TaW" 0 initialisiert.
		 *  @param talentName Bezeichnet das Talent.
         *  @param value Startwert des Talents.
		 *  @throws InvalidArgumentException Das Talent konnte nicht
		 *    gefunden werden.
         *  @ingroup CreatureRubyExports
		 */
        void addTalent(const CeGuiString talentName, int value = 0);
		/** 
         *  Liefert den Wert des Talents \a talentName zurueck.
		 *  @param talentName Bezeichnet das Talent.
		 *  @return @ref abbdet "TaW"
         *  @ingroup CreatureRubyExports
		 *  @throws InvalidArgumentException Das Talent konnte in \c mTalente
		 *   nicht gefunden werden.
		 */
		virtual int getTalent(const CeGuiString talentName);
		/** 
         *  Setzt den Wert des Talents \a talentName.
		 *  @param talentName Bezeichnet das zu veraendernde Talent.
		 *  @param value Der neue @ref abbdet TaW.
		 *  @throws InvalidArgumentException Das Talent konnte in \c mTalente
		 *   nicht gefunden werden.
         *  @ingroup CreatureRubyExports
		 */
        virtual void setTalent(const CeGuiString talentName, int value);
		/** 
         *  Erhoeht das Talent \a talentName um \a mod.
		 *  @param talentName Bezeichnet das zu steigernde Talent.
		 *  @param mod Der Wert um den das Talent gesteigert werden soll.
		 *  @throws InvalidArgumentException Das Talent konnte in \c mTalente
		 *   nicht gefunden werden.
         *  @ingroup CreatureRubyExports
		 */
        virtual void modifyTalent(const CeGuiString talentName, int mod);
        /**
         * Liefert eine Liste mit allen Talenten und @ref abbdet "TaW"s zurueck.
         * Dies wird auch der komplette Talentspiegel genannt.
         **/
		virtual const Creature::TalentMap& getAllTalents() const;
		/** 
         *  Markiert ein Talent mit einer @ref abbdes "SE". Siehe 
         *  Spezielle Erfahrungen, MFF 47.
		 *  @param talentName Bezeichnet das Talent in dem die SE erhalten wurde.
         *  @ingroup CreatureRubyExports
		 */
		virtual void addSe(const CeGuiString talentName);
        /**
         *  Liefert einen Zeiger auf das StateSet des Talents \a talentName zurueck.
         *  @param talentName Bezeichnet das Talent von dem das StateSet zurueckgegeben
         *   werden soll.
         *  @throws IllegalArgumentException Talent nicht gefunden.
         *  @ingroup CreatureRubyExports
         **/
		virtual TalentStateSet* getTalentStateSet(const CeGuiString talentName);

///////////////////////////////////////////////////////////////////////////////
// Kampftechniken

		/**
		 *  Fuegt eine neue Kampftechnik zu \c mKampftechniken hinzu.
		 *  @param kampftechnikName Bezeichnet die Kampftechnik.
		 *  @param value Initialisiert die Kampftechnik mit value. Standard ist
		 *  (0,0).
		 *  @throws InvalidArgumentExeption Die Kampftechnik ist unbekannt.
         *  @ingroup CreatureRubyExports
		 **/
		void addKampftechnik(const CeGuiString kampftechnikName, const pair<int,int>& value = make_pair(0,0));
		/** 
         *  Liefert die @ref abbdea "AT" und @ref abbdep "PA" Werte in einer 
         *  bestimmten Kampftechnik zurueck.
		 *  @param kampftechnikName Beszeichnet die Kampftechnik.
		 *  @return Ein pair<AT, PA>.
		 *  @throws InvalidArgumentException \a kampftechnikId konnte nicht in 
		 *   \c mKampftechniken gefunden werden.
		 **/
        virtual pair<int, int> getKampftechnik(const CeGuiString kampftechnikName) const;
		/** 
         *  Setzt die @ref abbdea "AT" und @ref abbdep "PA" Werte in einer bestimmten 
         *  Kampftechnik.
		 *  @param kampftechnikName Bestimmt die zu setzende Kampftechnik.
		 *  @param value Die neuen AT/PA Werte.
		 *  @throws InvalidArgumentException Die Kampftechnik \a kampftechnikId
		 *    konnte nicht in \c mKampftechniken gefunden werden.
		 **/
        virtual void setKampftechnik(const CeGuiString kampftechnikName, const pair<int, int>& value);

///////////////////////////////////////////////////////////////////////////////
// Vorteile
        /**
         *  Fuegt einen Vorteil hinzu.
         *  @param vorteilName Der Name des Vorteils.
         *  @param value Der Wert einer eventuellen Gabe oder die Stufe des
         *   Vorteils (z.B. Astrale Regeneration 3).
         *  @throws InvalidArgumentException Vorteil schon in \c mVorteile enthalten.
         **/
        virtual void addVorteil(const CeGuiString vorteilName, int value = 0);
        /**
         *  Ueberprueft ob die Kreatur einen bestimmten Vorteil hat.
         *  @param vorteilName Der Name des zu ueberpruefenden Vorteils.
         **/
        virtual bool hasVorteil(const CeGuiString vorteilName);
        /**
         *  Liefert das StateSet des Vorteils zurueck.
         *  Gedacht um die erforderlichen Daten von Gaben abzufragen.
         *  @param vorteilName Der Name der Gabe.
         *  @return Ein Zeiger auf das StateSet der Gabe.
         *  @throws InvalidArgumentException Der Vorteil \a vorteilName konnte
         *   nicht gefunden werden.
         **/
        virtual TalentStateSet* getVorteilStateSet(const CeGuiString vorteilName);

///////////////////////////////////////////////////////////////////////////////
// Nachteile
        /**
         *  Fuegt einen Nachteil hinzu.
         *  @param nachteilName Der Name des Nachteils.
         *  @param value Der Wert einer eventuellen Schlechten Eigenschaft oder
         *   die Stufe des Nachteils.
         *  @throws InvalidArgumentException Nachteil schon in \c mNachteile enthalten.
         **/
        virtual void addNachteil(const CeGuiString nachteilName, int value = 0);
        /**
         *  Ueberprueft ob die Kreatur einen bestimmten Nachteil hat.
         *  @param nachteilName Der Name des zu ueberpruefenden Nachteils.
         **/
        virtual bool hasNachteil(const CeGuiString nachteilName);
        /**
         *  Liefert das StateSet des Nachteils zurueck.
         *  Gedacht um die erforderlichen Daten von Schlechten Eigenschaften
         *  abzufragen.
         *  @param nachteilName Der Name der Schlechten Eigenschaft.
         *  @return Ein Zeiger auf das StateSet der Schlechten Eigenschaft.
         *  @throws InvalidArgumentException Der Nachteil \a nachteilName konnte
         *   nicht gefunden werden.
         **/
        virtual EigenschaftenStateSet* getNachteilStateSet(const CeGuiString nachteilName);

///////////////////////////////////////////////////////////////////////////////
// Sonderfertigkeiten

		/** 
         *  Fuegt der Kreatur eine @ref abbdes "SF" hinzu.
		 *  @param sfName Bezeichnet die SF.
         *  @param value Der Status der SF. Standardmaessig SFSTATUS_IN_TRAINING.
		 *  @throws InvalidArgumentException \a sfName kann nicht gefunden
		 *    werden.
         *  @see SfStatus
		 */
		virtual void addSf(const CeGuiString sfName, SfStatus value = SFSTATUS_IN_TRAINING);
		/** 
         *  Liefert den Wert der @ref abbdes "SF" zurueck.
		 *  @param sfName Bezeichnet die SF
		 *  @throws InvalidArgumentException \a sfName kann nicht in 
		 *   \c mSonderfertigkeiten gefunden werden.
		 *  @see SonderfertigkeitMap
		 */
		virtual int getSf(const CeGuiString sfName);
		/** 
         *  Setzt den Wert der @ref abbdes "SF".
		 *  @param sfName Bezeichnet die Sonderfertigkeit deren Wert gesetzt
		 *   werden soll.
		 *  @param value Der Wert auf den die Sonderfertigkeit gesetzt werden 
		 *   soll.
		 *  @throws InvalidArgumentException \a sfName kann nicht in 
		 *   \c mSonderfertigkeiten gefunden werden.
		 *  @see SonderfertigkeitMap
		 */
		virtual void setSf(const CeGuiString sfName, SfStatus value);

        /**
         *  Liefert einen Zeiger auf das StateSet der @ref abbdes "SF" zurueck.
         *  @param sfName Bezeichnet die SF deren StateSet zurueckgelifert werden soll.
		 *  @throws InvalidArgumentException \a sfName kann nicht in 
		 *   \c mSonderfertigkeiten gefunden werden.
         *  @ingroup CreatureRubyExports
         **/
		virtual SonderfertigkeitenStateSet* getSonderfertigkeitenStateSet(const CeGuiString sfName);

///////////////////////////////////////////////////////////////////////////////
// Status
        /**
         *  @todo Weitere denkbare Zustaende: unconcious (wie sleeping, nur 
         *   schwieriger zu wecken), petrified (paralyzed eigentlich = gelaehmt,
         *   petrified = versteinert
         **/

        /// Checks if the creature is currently blind.
        bool isBlind();
        /**
         *  Set the creature to blind / not blind. This status works with reference
         *  counting.
         *  @see setStatus for a detailed description of the reference system.
         **/
        void setBlind(bool value);
        /// Checks if the creature is dead.
        bool isDead();
        /**
         *  Set the creature to dead / alive. This status works with reference
         *  counting.
         *  @see setStatus for a detailed description of the reference system.
         **/
        void setDead(bool value);
        /// Checks if the creature is currently deaf.
        bool isDeaf();
        /**
         *  Set the creature to deaf / not deaf. This status works with reference
         *  counting.
         *  @see setStatus for a detailed description of the reference system.
         **/
        void setDeaf(bool value);
        /**
         *  Checks if the creature is currently incapacitated.
         *  This happens usually if the creature's @ref abbenv "VI" (de: 
         *  @ref abbdel "LE") falls below the value of 
         *  WERT_KAMPFUNFAEHIGKEITSSCHWELLE.
         *  @see Wert
         **/
        bool isIncapacitated();
        /**
         *  Set the creature to incapacitated / not incapacitated. This status 
         *  works with reference counting.
         *  @see setStatus for a detailed description of the reference system.
         **/
        void setIncapacitated(bool value);
        /// Checks if the creature is currently invisible.
        bool isInvisible();
        /**
         *  Set the creature to invisible / visible. This status works with reference
         *  counting.
         *  @see setStatus for a detailed description of the reference system.
         **/
        void setInvisible(bool value);
        /// Checks if the creature is currently invulnerable.
        bool isInvulnerable();
        /**
         *  Set the creature to invulnerable / vulnerable. This status works with 
         *  reference counting.
         *  @see setStatus for a detailed description of the reference system.
         **/
        void setInvulnerable(bool value);
        /// Checks if the creature is currently parallyzed.
        bool isParalyzed();
        /**
         *  Set the creature to paralyzed / not paralyzed. This status works with 
         *  reference counting.
         *  @see setStatus for a detailed description of the reference system.
         **/
        void setParalyzed(bool value);
        /// Checks if the creature is currently silenced.
        bool isSilenced();
        /**
         *  Set the creature to silenced / not silenced. This status works with 
         *  reference counting.
         *  @see setStatus for a detailed description of the reference system.
         **/
        void setSilenced(bool value);
        /// Checks if the creature is currently sleeping.
        bool isSleeping();
        /**
         *  Set the creature to sleeping / awake. This status works with reference
         *  counting.
         *  @see setStatus for a detailed description of the reference system.
         **/
        void setSleeping(bool value);
        /**
         *  Checks if the creature is unconcious. This status works with reference
         *  counting.
         **/
        bool isUnconscious();
        /**
         *  Set the creature to unconcious/concious. This status works with reference
         *  counting.
         *  @see setStatus for a detailed description of the reference system.
         **/
        void setUnconscious(bool value);
        /**
         *  Returns true if the creature cannot move. This function does not 
         *  correspond to a certain status variable but checks several other 
         *  stati such as isDead, isParalyzed and isSleeping.
         **/
        bool isImmovable();
        /**
         *  Checks if the creature is actively magic. This function does not 
         *  correspond to a certain status variable but returns true if the 
         *  modified AE is greater than zero.
         **/
		bool isMagic();


///////////////////////////////////////////////////////////////////////////////
// Inventory


		/**
		 *  Gibt das Inventar der Kreatur zurueck.
		 *  @return Liefert einen Zeiger auf Inventar der Kreatur
		 **/
		Inventory* getInventory();


		/**
		 *  Gibt der Spielfigur eine Waffe in die Hand. Wird gesteuert durch 
         *  das Inventar.
		 *  @param weapon Waffe, die in die Hand soll
		 */
		void attachWeapon(Weapon* weapon);

		/**
		 *  Entfernt der Kreatur die Waffe aus der Hand.
		 **/
		void detachWeapon();

// Das Inventar der Kreatur wird durch das Inventarobjekt verwaltet.
		
		/**
		 *  Heftet Container an die Kreatur.
		 *  @param container Ein Zeiger auf den hinzuzufuegenden Container.
		 *  @throws InvalidArgumentException Nullpointer uebergeben.
		 *  @throws InvalidArgumentException Container bereits in
		 *   \c mContainer enthalten.
         *  @ingroup CreatureRubyExports
		 **/
		void addContainer(Container* container);
		/**
		 *  Liefert den Container \a containerName zurueck.
		 *  @param containerName Bezeichnet den Container in mContainer.
		 *  @return Liefert einen Zeiger auf den Container.
		 *  @throws InvalidArgumentException Der Container wurde nicht
		 *  in \c mContainer gefunden.
         *  @ingroup CreatureRubyExports
		 **/
		Container* getContainer(const CeGuiString containerName) const;
		/**
		 *  Entfernt den Container containerName. Der Container wird aus 
         *  \c mContainer geloescht und ein Zeiger darauf zurueckgegeben.
		 *  @param containerName Bezeichnet den Container in \c mContainer.
		 *  @throws InvalidArgumentException Container nicht in
		 *   \c mContainer gefunden.
         *  @ingroup CreatureRubyExports
		 **/
		Container* removeContainer(const CeGuiString containerName);

		/**
		 *  Fuegt eine Waffer der Liste der derzeit einsetzbaren Waffen
		 *  hinzu.
		 *  @param weapon Ein Zeiger auf die hinzuzufuegende Waffe.
		 *  @throws InvalidArgumentException weapon darf nicht \c NULL sein.
         *  @ingroup CreatureRubyExports
		 **/
		void addWeapon(Weapon* weapon);
		/**
		 *  Liefert einen Zeiger auf eine Waffe aus der Liste der derzeit
		 *  verfuegbaren Waffen zurueck.
		 *  @param weaponId Bezeichnet zurueckzugebende die Waffe.
		 *  @return Ein Zeiger auf die Waffe \a weaponId.
		 *  @throws InvalidArgumentException weaponId kann nicht in \c mWeapons
		 *   gefunden werden.
         *  @ingroup CreatureRubyExports
		 **/
		Weapon* getWeapon(int weaponId) const;
		/** 
		 *  Entfernt die Waffe \a weaponId aus der Liste der derzeit 
		 *  verfuegbaren Waffen.
		 *  @param weaponId Bezeichnet die zu entfernende Waffe.
		 *  @return Ein Zeiger auf die Entfernte Waffe.
		 *  @throws InvalidArgumentException weaponId kann nicht in \a mWeapons
		 *   gefunden werden.
         *  @ingroup CreatureRubyExports
		 **/
		Weapon* removeWeapon(int weaponId);

        /**
         *  Macht die Waffe \a weaponId zur aktiven Waffe.
         *  @param weaponId Bezeichnet die Waffe.
         *  @ingroup CreatureRubyExports
         **/
		void switchToWeapon(int weaponId);
        /**
         *  Liefert die gerade aktive Waffe zurueck.
         *  @return Ein Zeiger auf die aktive Waffe.
         *  @ingroup CreatureRubyExports
         **/
		Weapon* getActiveWeapon();

///////////////////////////////////////////////////////////////////////////////
// Aktionen
        /** 
        *  Durchfuehren einer Talentprobe mit alternativen Eigenschaften.
		*  Siehe dazu auch MFF S.14. Wird nur der Korrektheit halber angeboten,
		*  sollte eher selten eingesetzt werden. Diese Talentprobe erlaubt auch
        *  die Angabe einer Spezialisierung.
        *  @param talentName Bezeichnet das Talent
		*  @param spezialisierungId Bezeichnet die Spezialisierung
        *  @param modifier Modifikator der Probe, dabei
        *     ist ein positiver Wert eine Erschwernis,
        *     ein negativer Wert eine Erleichterung.
		*  @param eigenschaft1Name Bezeichnet die erste Eigenschaft auf die 
		*     gewuerfelt werden soll
		*  @param eigenschaft2Name Bezeichnet die zweite Eigenschaft
		*  @param eigenschaft3Name Bezeichnet die dritte Eigenschaft
        *  @return uebrig gebliebene Talentpunkte, ein
        *     negativer Wert bedeutet die Probe ist nicht bestanden.
        *  @retval   RESULT_AUTOERFOLG bedeutet 2*1 gewuerfelt.
        *  @retval   RESULT_SPEKT_AUTOERFOLG bedeutet 3*1 gewuerfelt.
        *  @retval   RESULT_AUTOMISSERFOLG bedeutet 2*20 gewuerfelt.
        *  @retval   RESULT_SPEKT_AUTOMISSERFOLG bedeutet 3*20 gewuerfelt.
        */
        /**
         *  @overload doTalentprobe(const CeGuiString,int,int)
         *  This is the function that should be used in most cases.
         **/
        /**
         *  @overload doTalentprobe(const CeGuiString,int);
         *  This is the function you should usually use, if there is no fitting
         *  specialization for the task.
         *  @ingroup CreatureRubyExports
         **/
        /**
         *  @overload doAlternativeTalentprobe(const CeGuiString,int,CeGuiString,CeGuiString,CeGuiString)
         **/
        virtual int doAlternativeTalentprobe(const CeGuiString talentName, int spezialisierungId, 
			int modifier, CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name, 
			CeGuiString eigenschaft3Name);
        virtual int doTalentprobe(const CeGuiString talentName, int modifier);

        virtual int doTalentprobe(const CeGuiString talentName, int spezialisierungId,
			int modifier);
        virtual int doAlternativeTalentprobe(const CeGuiString talentName, int modifier, 
			CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name, CeGuiString eigenschaft3Name);

        /** 
        *  Durchfuehren einer Eigenschaftsprobe.
        *  @param eigenschaftName Bezeichnet die Eigenschaft
        *  @param modifier Modifikator der Probe, dabei
        *     ist ein positiver Wert eine Erschwernis,
        *     ein negativer Wert eine Erleichterung.
        *  @return Uebrig gebliebene Eigenschaftspunkte, ein
        *     negativer Wert bedeutet die Probe ist nicht bestanden.
        *  @retval RESULT_GLUECKLICH bedeutet 1 gewuerfelt.
        *  @retval RESULT_PATZER bedeutet 20 gewuerfelt.
        *  @ingroup CreatureRubyExports
        */
        virtual int doEigenschaftsprobe(const CeGuiString eigenschaftName, int modifier);

		/** 
		*  Fuehrt eine Attacke aus. Die Funktion wird von einem Angriffsmanoever 
        *  aufgerufen.
		*  @param kampftechnikName Bezeichnet die Kampftechnik in dem die 
		*   Attacke ausgefuehrt wird.
		*  @param modifier Erschwert oder erleichtert die Attacke.
		*  @retval RESULT_ERFOLG Eine erfolgreiche Attacke.
		*  @retval RESULT_MISSERFOLG Eine fehlgeschlagene Attacke.
		*  @retval RESULT_GLUECKLICH Eine gute Attacke.
		*  @retval RESULT_PATZER Ein bestaetigter Attackepatzer.
		*  @throws InvalidArgumentException Kampftechnik nicht in 
		*   mKampftechniken gefunden.
		**/
		int doAttacke(const CeGuiString kampftechnikName, int modifier);
		/** 
		*  Fuehrt eine Parade aus. Die Funktion wird von einem Parademanoever 
        *  aufgerufen.
		*  @param kampftechnikName Bezeichnet die Kampftechnik in dem die 
		*   Parade ausgefuehrt wird.
		*  @param modifier Erschwert oder erleichtert die Parade.
		*  @param guteParade Versucht eine gute Parade (bei einem gluecklichen Treffer)
		*  @retval RESULT_ERFOLG Eine erfolgreiche Parade.
		*  @retval RESULT_MISSERFOLG Eine fehlgeschlagene Parade.
		*  @retval RESULT_GLUECKLICH Eine gute Parade.
		*  @retval RESULT_PATZER Ein bestaetigter Paradepatzer.
		*  @throws InvalidArgumentException Kampftechnik nicht in 
		*   \c mKampftechniken gefunden.
		**/
		int doParade(const CeGuiString kampftechnikName, int modifier, bool guteParade = false);

		/**
		 *  Fuehrt einen Initiativewurf durch.
		 *  @param getMaxInitiative Wenn true wird ein Wert zurueckgeliefert,
		 *    als ob eine 6 gewuerfelt worden waere, z.B. fuer das Manoever
		 *    'Orientieren'.
		 **/
		int doInitiativeWurf(bool getMaxInitiative = false);

		/**
		 *  Fuegt der Kreatur \a tp @ref abbdet "Trefferpunkte" zu.
		 *  @param tp die Trefferpunkte
         *  @param damageType Typ des Schadens, um Immunitaeten, Verletzlichkeiten
         *   etc. korrekt zu beruecksichtigen.
		 **/
		void damageLe(int tp, int damageType = LEDAMAGE_NORMAL);
        static const int LEDAMAGE_NORMAL = 0;
        static const int LEDAMAGE_FIRE = 1;
        static const int LEDAMAGE_WATER = 2;
        static const int LEDAMAGE_DEMONIC = 4;
        static const int LEDAMAGE_TP_A = 8;

		/**
		 *  Zieht der Kreatur \a asp @ref abbdea "Astralpunkte" ab.
		 *  @param aup die Trefferpunkte
		 **/
        void damageAe(int asp);

        /**
		 *  Zieht der Kreatur \a aup @ref abbdea "Ausdauerpunkte" ab.
		 *  @param aup die Trefferpunkte
         *  @param damageType Typ des Schadens, um Resistenzen
         *   etc. korrekt zu beruecksichtigen.
		 **/
        void damageAu(int aup, int damageType = AUDAMAGE_NORMAL);
        static const int AUDAMAGE_NORMAL = 0;
        static const int AUDAMAGE_HEAT = 1;
        static const int AUDAMAGE_COLD = 2;
        static const int AUDAMAGE_CHOKE = 4;
        static const int AUDAMAGE_DROWN = 8;

        /**
         *  Regeneriert die @reg appdel "LE" einer Kreatur, ueblicherweise nach
         *  einer mind. 6 stuendigen Ruhephase.
         *  @param modifier Ein situationsabhaengiger Modifikator, vor allem
         *   abhaengig von der Qualitaet der Unterbringung (1-A Hotel vs. draussen
         *   bei Sturm und Hagel...)
         **/
        void regenerateLe(int modifier = 0);
        /**
         *  Regeneriert die @reg appdea "AE" einer Kreatur, ueblicherweise nach
         *  einer mind. 6 stuendigen Ruhephase.
         *  @param modifier Ein situationsabhaengiger Modifikator, vor allem
         *   abhaengig von der Qualitaet der Unterbringung (1-A Hotel vs. draussen
         *   bei Sturm und Hagel...)
         **/
        void regenerateAe(int modifier = 0);
        /**
         *  Regeneriert die @reg appdea "AU" einer Kreatur, ueblicherweise nach
         *  einer mind. 5 minuetigen Ruhephase.
         *  @param modifier Ein situationsabhaengiger Modifikator, sollte eher
         *   selten eingesetzt werden.
         **/
        void regenerateAu(int modifier = 0);

		/**
		 * Laesst einen Effekt auf der Kreatur wirken.
		 * @param effect Zeiger auf den Effekt.
         * @ingroup CreatureRubyExports
		 **/
		void addEffect(Effect* effect);

    protected:
        /**
         * Liefert den Eigenschaftswert \a eigenschaftName fuer eine 
         * Basiswertberechnung zurueck. Manche Effekte veraendern zwar den
         * Eigenschaftswert, nicht jedoch die Basiswerte.
         **/
        virtual int getEigenschaftForBasiswertCalculation(const CeGuiString eigenschaftName);
        /**
         *  Berechnet den Basiswert von der @ref abbdea "Astralenergie".
         **/
		virtual int getAeBasis();
        /**
         *  Berechnet den Basiswert von der @ref abbdea "Ausdauer".
         **/
		virtual int getAuBasis();
        /**
         *  Berechnet den Basiswert von der @ref abbdel "Lebensenergie".
         **/
        virtual int getLeBasis();
        /**
         *  Berechnet den Basiswert von der @ref abbdem "Magieresistenz".
         **/
		virtual int getMrBasis();
		/**
		 *  Liefert die derzeitge @ref abbdeb "BE" der Kreatur zurueck.
		 *  Eventuelle Ruestungsgewoehnung schon beruecksichtigt
		 **/
		virtual int getCurrentBe();


		/**
		 * Ueberprueft die wirkenden Effekte auf Lebendigkeit
		 **/
		void checkEffects();

	private:
///////////////////////////////////////////////////////////////////////////////
// Typedefs
        /**
         *  Liste der Werte.
         *  @see Wert
         **/
		typedef map<const Wert, StateSet*> WertMap;
		/** 
         *  Liste der guten Eigenschaften. Besteht aus dem Abkuerzung der 
         *  Eigenschaft (z.B. @ref abbdem "MU", @ref abbdek "KL") als Schluessel 
         *  und einem Zeiger auf ihr StateSet.
		 **/
		typedef map<const CeGuiString, EigenschaftenStateSet*> EigenschaftMap;
		/**
		 *  Liste der Kampftechniken und ihrer @ref abbdea "AT"/@ref abbdep "PA2 Werte.
		 *  Eine Kampftechnik in diesem Sinne ist so was wie Hiebwaffen
		 *  oder Raufen, bei Tieren sowas wie Biss oder Prankenhieb. Der erste 
         *  Wert ist der Name der Kampftechnik, das pair den AT und PA Werten.
		 *  Die Werte werden auf den AT/PA Basiswert addiert bevor sie die fertige
		 *  AT/PA Werte ergeben. Die Summe des pairs muss also dem TaW in dem
		 *  Kampftalent entsprechen.
		 **/
        typedef map<const CeGuiString, pair<int, int> > KampftechnikMap;
        /**
         *  Eine Liste der Vorteile der Kreatur. Gaben gehoeren ebenfalls zu 
         *  den Vorteilen, verhalten sich aber wie Talente.
         **/
        typedef map<const CeGuiString, TalentStateSet*> VorteilMap;
        /**
         *  Eine Liste der Nachteile der Kreatur. Schlechte Eigenschaften gehoeren 
         *  ebenfalls zu den Nachteilen, verhalten sich aber wie Eigenschaften.
         **/
        typedef map<const CeGuiString, EigenschaftenStateSet*> NachteilMap;
		/** 
         *  Die Sonderfertigkeiten der Kreatur. Besteht aus dem Namen der 
         *  Sonderfertigkeit als Schluessel und ihrem Status.
		 */
		typedef map<const CeGuiString ,SonderfertigkeitenStateSet*> SonderfertigkeitMap;
		/** 
         *  Die Container einer Kreatur. Diese Container sollen dann alles beinhalten, 
         *  was direkt am Koerper  getragen wird (Kleidung, Rucksaecke, Ringe etc.), 
         *  sowie angeborene Waffen (Faeuste, Klauen, Zaehne...).
		 **/
		typedef map<const CeGuiString, Container*> ContainerMap;

        struct Ap
        {
            int total;
            int used;
        };

///////////////////////////////////////////////////////////////////////////////
// Membervariablen
        /// Die aktuelle @ref abbdel "Lebensenergie" der Kreatur.
        int mCurrentLe;
        /// Die aktuelle @ref abbdea "Astralenergie" der Kreatur.
		int mCurrentAe;
        /// Die aktuelle @ref abbdea "Ausdauer" der Kreatur.
		int mCurrentAu;
        /// Die aktuelle Erschoepfung der Kreatur.
        int mErschoepfung;
        /// Wenn > 0 ist die Kreatur blind.
        int mBlind;
        /// Wenn > 0 ist die Kreatur tot.
        int mDead;
        /// Wenn > 0 ist die Kreatur taub.
        int mDeaf;
        /// Wenn > 0 ist die Kreatur kampfunfaehig.
        int mIncapacitated;
        /// Wenn > 0 ist die Kreatur unverwundbar.
        int mInvulnerable;
        /// Wenn > 0 ist die Kreatur unsichtbar.
        int mInvisible;
        /// Wenn > 0 ist die Kreatur gelaehmt.
        int mParalyzed;
        /// Wenn > 0 ist liegt ein Silentium auf der Kreatur.
        int mSilenced;
        /// Wenn > 0 schlaeft die Kreatur.
        int mSleeping;
        /// Wenn > 0 ist die Kreatur bewusstlos.
        int mUnconscious;

        /// Verwaltet die Effekte die auf die Kreatur wirken.
		EffectManager mEffectManager;
        /// Zeigt auf die gerade aktive Waffe.
		Weapon* mActiveWeapon;
        EigenschaftMap mEigenschaften;
        TalentMap mTalente;
        KampftechnikMap mKampftechniken;
        VorteilMap mVorteile;
        NachteilMap mNachteile;
		SonderfertigkeitMap mSonderfertigkeiten;
		WertMap mWerte;
		ContainerMap mContainer;
        Ap mAp;

        /// Zeigt auf das Inventar der Kreatur.
		Inventory* mInventory;

        /**
         *  Sets a the status \a statusVariable. It uses a reference counting
         *  System to do this, so if for example severeal effects put the creature
         *  to sleep, and one effect ends the creature doesn't awake until all
         *  the other effects end too. 
         *  @param statusVariable is a reference to the status to be set, such as
         *   \c mBlind.
         *  @param value If true, the reference is increased, if false it is decreased.
         *  @param errorMessage If \c setStatus is called with \a value = \c false more often
         *   than it was with \c true (i.e. the status is removed more often than
         *   it was applied), \c setStatus throws an exception with \a errorMessage as
         *   text.
         *  @throw InvalidArgumentException see \a errorMessage.
         **/
        void setStatus(int& statusVariable, bool value, const Ogre::String& errorMessage);
    };
}
#endif //__CREATURE_H__