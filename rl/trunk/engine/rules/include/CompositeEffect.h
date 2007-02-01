/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __COMPOSITEEFFECT_H__
#define __COMPOSITEEFFECT_H__

#include "Effect.h"
#include <vector>

namespace rl
{

	class _RlRulesExport CompositeEffect : public Effect
	{
	public:
		CompositeEffect();
		virtual ~CompositeEffect();

		/// Fügt einen Effekt zur Liste hinzu.
		void addEffect(Effect* effect);
		/// Überprüft die Lebendigkeit des Effekts.
		virtual void check();
	protected:
		/// Bringt die Aenderungen ein.
		void apply();
		/// Entfernt die Aenderungen.
		void remove();
		typedef std::set<Effect*> Effects;
		Effects mEffects;
	};
}

#endif //__COMPOSITEEFFECT_H__
