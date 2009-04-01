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
#ifndef __CREATURE_H__
#define __CREATURE_H__

#include "RulesPrerequisites.h"

#include "Date.h"
#include "Effect.h"
#include "Effect.h"
#include "Eigenschaft.h"
#include "GameObject.h"
#include "ZauberStateSet.h"
#include "SonderfertigkeitenStateSet.h"

namespace rl
{
    class Container;
    class Inventory;
    class Item;
    class Weapon;

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
   
    enum DamageStrength {
        DMG_NONE,
        DMG_HALF,
        DMG_NORMAL,
        DMG_DOUBLE
    };

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
             *  List of all talents. Consists of the (german) name of the talent
             *  (e.g. "Athletik") as key and its value (TaW)
             **/
            typedef std::map<const CeGuiString, int> TalentMap;

            static const Ogre::String CLASS_NAME;
            static const Ogre::String PROPERTY_AI;
            static const Ogre::String PROPERTY_AI_BEHAVIOURS;
			static const Ogre::String PROPERTY_INVENTORY_WINDOW_TYPE;
            static const Ogre::String PROPERTY_CURRENT_LE;
            static const Ogre::String PROPERTY_CURRENT_AE;
            static const Ogre::String PROPERTY_CURRENT_AU;
            static const Ogre::String PROPERTY_CURRENT_FATIGUE;
            static const Ogre::String PROPERTY_EFFECTS;
            static const Ogre::String PROPERTY_EIGENSCHAFTEN;
            static const Ogre::String PROPERTY_TALENTE;
            static const Ogre::String PROPERTY_KAMPFTECHNIKEN;
            static const Ogre::String PROPERTY_KAMPFAKTIONEN;
            static const Ogre::String PROPERTY_VORTEILE;
            static const Ogre::String PROPERTY_NACHTEILE;
            static const Ogre::String PROPERTY_SF;
            static const Ogre::String PROPERTY_WERTE;
            static const Ogre::String PROPERTY_AP;
            static const Ogre::String PROPERTY_INVENTORY;
            static const Ogre::String PROPERTY_ANIMATIONS;
            static const Ogre::String PROPERTY_ANIMATIONSPEEDS;

            typedef Ogre::String Wert;

            // some targets
			static const std::string ALL_EIGENSCHAFTEN;
			static const std::string ALL_TALENTE;

            static const Wert WERT_MOD_AE;
            static const Wert WERT_MOD_LE;
            static const Wert WERT_MOD_AU;
            static const Wert WERT_MOD_MR;
            static const Wert WERT_MOD_AT;
            static const Wert WERT_MOD_PA;
            static const Wert WERT_MOD_FK;
            static const Wert WERT_GS;
            static const Wert WERT_SOZIALSTATUS;
            static const Wert WERT_BE;
            static const Wert WERT_RS;
            static const Wert WERT_INI;
            static const Wert WERT_KAMPFUNFAEHIGKEITSSCHWELLE; 
            static const Wert WERT_REGENERATION;
    			
			Creature(const CeGuiString &id);

            /// Der Standarddestruktor.
            virtual ~Creature();

///////////////////////////////////////////////////////////////////////////////
// Enums
            /// Alignment of a Creature towards the player. This determines
            /// whether a combat is entered.
            enum Alignment
            {
                ALIGNMENT_ALLY    = 1, ///< Creature fights with the player.
                ALIGNMENT_NEUTRAL = 2, ///< Creature doesn't fight for either side.
                ALIGNMENT_ENEMY   = 4  ///< Creature fights against the player.
            };

            /// Determines the damage type
            enum DamageTag
            {
                DAMAGETAG_NORMAL = 1,
                DAMAGETAG_PIERCING, ///< Wie Pfeile, Bolzen oder gezielte Stiche, verletzen schon bei KO/2 < SP
                DAMAGETAG_FIRE
            };
            /// The differente states a @ref abbdes "SF" can be in.
             enum SfStatus
            {
                SFSTATUS_IN_TRAINING = 1,
                SFSTATUS_OK,
                SFSTATUS_REQUIREMENTS_NOT_MET /// @todo Is this even needed?
            };

///////////////////////////////////////////////////////////////////////////////
// Werte

            /**
             *  Modifies the current @ref abbdel "LE" of the creature.
             *  @param mod The value to be added to the LE (so to lower the LE, pass a negative 
             *  value).
             *  @param ignoreMax If ture, the LE can be increased beayond the maximum.
             **/
            virtual void modifyLe(int mod, bool ignoreMax = false);
            /**
             *  Returns the current @ref abbdel "LE" of the creature.
             **/
            virtual int getLe() const;
            /**
             *  Returns the maximum @ref abbdel "LE" the creature can have.
             **/
            virtual int getLeMax() const;

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
            virtual int getAe() const;
            /**
             *  Gibt die maximale Anzahl an @ref abbdea "AE" zurueck, die die Kreatur haben kann.
             **/
            virtual int getAeMax() const;

            /**
             *  Veraendert die aktuelle @ref abbdea "AU" der Kreatur.
             *  @param mod Der Wert um den die AU erhoeht (erniedrigt bei 
             *   negativem Wert) werden soll
             *  @param ignoreMax Wenn true kann die AU auch ueber das Maximum
             *   hinaus erhoeht werden.
             *  @ingroup CreatureRubyExports
             **/
            virtual void modifyAu(float mod,  bool ignoreMax = false);
            /**
             *  Gibt die aktuelle @ref abbdea "AU" der Kreatur zurueck.
             **/
            virtual float getAu() const;
            /**
             *  Gibt die maximale Anzahl an @ref abbdea "AE" zurueck, die die Kreatur haben kann.
             **/
            virtual int getAuMax() const;

            /**
             *  Berechnet den @ref abbdea "AT"-Basiswert der Kreatur.
             **/
            virtual int getAttackeBasis() const;
            /**
             *  Berechnet den @ref abbdep "PA"-Basiswert der Kreatur.
             **/
            virtual int getParadeBasis() const;
            /**
             *  Berechnet den @ref abbdef "FK"-Basiswert der Kreatur.
             **/
            virtual int getFernkampfBasis() const;
            /**
             *  Berechnet den @ref abbdei "INI"-Basiswert der Kreatur.
             **/
            virtual int getInitiativeBasis() const;

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
            virtual int getWert(Wert wertId, bool getUnmodified = false) const;

            /**
             *  Gibt der Kreatur @ref abbddea "AP".
             *  @param modifier Kann auch negativ sein (z.B. Borbarad-Moskitos).
             **/
            void modifyAp(int modifier);
            /**
             *  Liefert die Gesamtzahl der @ref abbdea "AP" zurueck.
             **/
            int getAp() const;
            /**
             *  Veraendert die verbrauchten @ref abbdea "AP", wird beim Steigern
             *  aufgerufen.
             *  @param modifier Kann auch negativ sein (z.B. Borbarad-Moskitos).
             **/
            void modifyUsedAp(int modifier);
            /**
             *  Liefert die verbrauchten @refabbdea "AP" zurueck.
             **/
            int getUsedAp() const;


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
			virtual int getEigenschaft(const CeGuiString eigenschaftName, Effect::ModTag tag = Effect::MODTAG_NONE) const;
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

            /**
                  * Gibt true zurueck, falls das Talent \a talentName \a gefunden wurde
              * @param talentName Bezeichnet das Talent
              * @param checkAusweichTalente Gibt an, ob ebenfalls nach Ausweichtalenten gesucht werden soll
            */
            bool hasTalent(const CeGuiString talentName, bool checkAusweichTalente = true) const;

            virtual int getTalent(const CeGuiString talentName) const;
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
            void addKampftechnik(const CeGuiString kampftechnikName, const std::pair<int, int>& value = std::make_pair(0, 0));
            /**
                   *  Liefert die @ref abbdea "AT" und @ref abbdep "PA" Werte in einer 
                   *  bestimmten Kampftechnik zurueck.
             *  @param kampftechnikName Beszeichnet die Kampftechnik.
             *  @return Ein std::pair<AT, PA>.
             *  @throws InvalidArgumentException \a kampftechnikId konnte nicht in 
             *   \c mKampftechniken gefunden werden.
             **/
            virtual std::pair<int, int> getKampftechnik(const CeGuiString kampftechnikName) const;
            /**
                   *  Setzt die @ref abbdea "AT" und @ref abbdep "PA" Werte in einer bestimmten 
                   *  Kampftechnik.
             *  @param kampftechnikName Bestimmt die zu setzende Kampftechnik.
             *  @param value Die neuen AT/PA Werte.
             *  @throws InvalidArgumentException Die Kampftechnik \a kampftechnikId
             *    konnte nicht in \c mKampftechniken gefunden werden.
             **/
            virtual void setKampftechnik(const CeGuiString kampftechnikName, const std::pair<int, int>& value);

///////////////////////////////////////////////////////////////////////////////
// Kampfaktionen (Attacke, Parade, etc..)

            void addKampfaktion(const CeGuiString& kampfaktionName);
            bool hasKampfaktion(const CeGuiString& kampfaktionName) const;
			const std::set<CeGuiString>& getAllKampfaktionen() const;

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
            virtual bool hasVorteil(const CeGuiString vorteilName) const;
 
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
            virtual bool hasNachteil(const CeGuiString nachteilName) const;
            /**
             *  Liefert das StateSet des Nachteils zurueck.
             *  Gedacht um die erforderlichen Daten von Schlechten Eigenschaften
             *  abzufragen.
             *  @param nachteilName Der Name der Schlechten Eigenschaft.
             *  @return Ein Zeiger auf das StateSet der Schlechten Eigenschaft.
             *  @throws InvalidArgumentException Der Nachteil \a nachteilName konnte
             *   nicht gefunden werden.
             **/
            virtual int getSchlechteEigenschaft(const CeGuiString nachteilName) const;

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
            virtual int getSf(const CeGuiString sfName) const;
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
            virtual SonderfertigkeitenStateSet* getSonderfertigkeitenStateSet(const CeGuiString sfName) const;

///////////////////////////////////////////////////////////////////////////////
// Status
            /**
             * Gets the status of a creature.
             **/
			Effect::LifeState getLifeState() const;


            bool isMagic() const;


///////////////////////////////////////////////////////////////////////////////
// Inventory


            /**
             *  Gibt das Inventar der Kreatur zurueck.
             *  @return Liefert einen Zeiger auf Inventar der Kreatur
             **/
            Inventory* getInventory() const;

        const Ogre::String& getInventoryWindowType() const;

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
		virtual int doAlternativeTalentprobe(const CeGuiString talentName, Effect::ModTag  spezialisierung,
                                                 int modifier, CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name,
                                                 CeGuiString eigenschaft3Name);
            virtual int doTalentprobe(const CeGuiString talentName, int modifier);

			virtual int doTalentprobe(const CeGuiString talentName, Effect::ModTag spezialisierung,
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
			virtual int doEigenschaftsprobe(const CeGuiString eigenschaftName, int modifier = 0, Effect::ModTag tag = Effect::MODTAG_NONE);

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

			int doTrefferpunkteWurf(Weapon* weapon, DamageStrength damage) const;

            /**
             *  Fuegt der Kreatur \a tp @ref abbdet "Trefferpunkte" zu.
             *  @param tp die Trefferpunkte
                   *  @param damageType Typ des Schadens, um Immunitaeten, Verletzlichkeiten
                   *   etc. korrekt zu beruecksichtigen.
             **/
            void damageLe(int tp, int damageType = LEDAMAGE_NORMAL);
            static const int LEDAMAGE_NORMAL = 0;
            static const int LEDAMAGE_SP = 1;
            static const int LEDAMAGE_FIRE = 2;
            static const int LEDAMAGE_WATER = 4;
            static const int LEDAMAGE_DEMONIC = 8;
            static const int LEDAMAGE_TP_A = 16;

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
            void damageAu(float aup, int damageType = AUDAMAGE_NORMAL);
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
             *  einer mind. 5 minuetigen Ruhephase. Andernfalls kann die Funktion mit dem
             *  Parameter time aufgerufen werden.
             *  @param modifier Ein situationsabhaengiger Modifikator, sollte eher
             *   selten eingesetzt werden.
             *  @param time Sollte nur benutzt werden, falls es nicht m�lich ist
             *   die Funktion wirklich nur alle 5 Minuten aufzurufen
             **/
            void regenerateAu(int modifier = 0, float factor = 1, float time = Date::ONE_SPIELRUNDE);


            virtual const Property getProperty(const CeGuiString& key) const;
            virtual void setProperty(const CeGuiString& key, const Property& value);
            virtual PropertyKeys getAllPropertyKeys() const;

            void setAlignment(Alignment);
            Alignment getAlignment() const;



            /**
             * Gibt an, ob die Creature mit den Händen an den Gegenstand herankommt,
             * ihn also berühren kann, mit ihm interagieren etc
            **/
            virtual bool canReachItem(const Item* item) const;

            /**
             * Animations-Name und relative/absolute Abspielgeschwindigkeit fuer
             * den CreatureController
            **/
            typedef std::pair<Ogre::String, Ogre::Real> AnimationSpeedPair;
            /**
             * Gibt die Animation aus der AnimationsMap für den CreatureController zurück
             * @return Wenn kein spezieller Wert gefunden wurde, wird als Animationsname
             * der Schlüsse name und eine Geschwindigkeit von 1 zurueckgegeben
            **/
            AnimationSpeedPair getAnimation(const CeGuiString& name) const;

        protected:
           /**
             *  Berechnet den Basiswert von der @ref abbdea "Astralenergie".
             **/
            virtual int getAeBasis() const;
            /**
             *  Berechnet den Basiswert von der @ref abbdea "Ausdauer".
             **/
            virtual int getAuBasis() const;
            /**
             *  Berechnet den Basiswert von der @ref abbdel "Lebensenergie".
             **/
            virtual int getLeBasis() const;
            /**
             *  Berechnet den Basiswert von der @ref abbdem "Magieresistenz".
             **/
            virtual int getMrBasis() const;
            /**
             *  Liefert die derzeitge @ref abbdeb "BE" der Kreatur zurueck.
             *  Eventuelle Ruestungsgewoehnung schon beruecksichtigt
             **/
            virtual int getCurrentBe() const;

            /// inherited from GameObject
            virtual void doPlaceIntoScene();

            /// inherited from GameObject
            virtual void doRemoveFromScene();




        private:
///////////////////////////////////////////////////////////////////////////////
// Typedefs
            /**
             *  Liste der Werte.
             *  @see Wert
             **/
            typedef std::map<const Wert, int> WertMap;
            /**
                   *  Liste der guten Eigenschaften. Besteht aus dem Abkuerzung der 
                   *  Eigenschaft (z.B. @ref abbdem "MU", @ref abbdek "KL") als Schluessel 
                   *  und einem Zeiger auf ihr StateSet.
             **/
            typedef std::map<const CeGuiString, int> EigenschaftMap;
            /**
             *  Liste der Kampftechniken und ihrer @ref abbdea "AT"/@ref abbdep "PA2 Werte.
             *  Eine Kampftechnik in diesem Sinne ist so was wie Hiebwaffen
             *  oder Raufen, bei Tieren sowas wie Biss oder Prankenhieb. Der erste 
                   *  Wert ist der Name der Kampftechnik, das pair den AT und PA Werten.
             *  Die Werte werden auf den AT/PA Basiswert addiert bevor sie die fertige
             *  AT/PA Werte ergeben. Die Summe des pairs muss also dem TaW in dem
             *  Kampftalent entsprechen.
             **/
            typedef std::map<const CeGuiString, std::pair<int, int> > KampftechnikMap;
            /**
             *  Eine Liste der Vorteile der Kreatur. Gaben gehoeren ebenfalls zu 
             *  den Vorteilen, verhalten sich aber wie Talente.
             **/
            typedef std::map<const CeGuiString, int> VorteilMap;
            /**
             *  Eine Liste der Nachteile der Kreatur. Schlechte Eigenschaften gehoeren 
             *  ebenfalls zu den Nachteilen, verhalten sich aber wie Eigenschaften.
             **/
            typedef std::map<const CeGuiString, int> NachteilMap;
            /**
                   *  Die Sonderfertigkeiten der Kreatur. Besteht aus dem Namen der 
                   *  Sonderfertigkeit als Schluessel und ihrem Status.
             */
            typedef std::map<const CeGuiString , SonderfertigkeitenStateSet*> SonderfertigkeitMap;
            /**
                    * Die Animationen, die der CreatureController bei einer Bewegung der Kreatur
                    * abspielt. Der Schlüssel ist Bezeichner der von einer Bewegungsart des
                    * CreatureControllers verwendet wird, der Wert gibt den echten Namen der Animation
                    * an.
            **/
            typedef std::map<const CeGuiString, CeGuiString> AnimationsMap;
            /**
                    * Die (relative oder absolute) Geschwindigkeit einer Animationen, die der
                    * CreatureController bei einer Bewegung der Kreatur
                    * abspielt. Der Schlüssel ist Bezeichner, der von einer Bewegungsart des
                    * CreatureControllers verwendet wird.
            **/
            typedef std::map<const CeGuiString, Ogre::Real> AnimationSpeedsMap;
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
            float mCurrentAu;
            /// Die aktuelle Erschoepfung der Kreatur.
            int mErschoepfung;
            /// Zuletzt zugewiesene Bewegungsart
            int mMovementType;

            /// Zeigt auf die gerade aktive Waffe.
            EigenschaftMap mEigenschaften;
            TalentMap mTalente;
            KampftechnikMap mKampftechniken;
			std::set<CeGuiString> mKampfaktionen;
            VorteilMap mVorteile;
            NachteilMap mNachteile;
            SonderfertigkeitMap mSonderfertigkeiten;
            WertMap mWerte;
            AnimationsMap mAnimations;
            AnimationSpeedsMap mAnimationSpeeds;
            Ap mAp;
            /// Zeigt auf das Inventar der Kreatur.
            Inventory* mInventory;
            Ogre::String mInventoryWindowType;

            Property mAiProperties;

            Alignment mAlignment;

            /// this variable is only used in regenerateAu
            float mTimeSinceLastRegeneration;
            /// this variable is only used in regenerateAu (must be initialized with 3W6)
            int mLastCalculatedAuToRegenerate;

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
    
    typedef std::list<Creature*> CreatureList;
}
#endif //__CREATURE_H__
