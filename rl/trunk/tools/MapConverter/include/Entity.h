/*
Half-Life MAP to Quake3 Map Conversion Utility
Copyright (C) 2004  Henrik Hinrichs

based on the
Half-Life MAP viewing utility.
Copyright (C) 2003  Ryan Samuel Gregg

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#pragma once

#include "ArgVal.h"
#include "Brush.h"
#include "TextureManager.h"

__gc class CEntity
{
private:
	ArrayList *ArgVals;
	ArrayList *Brushes;

public:
	CEntity()
	{
		Brushes = new ArrayList();
		ArgVals = new ArrayList();
	}

	void AddBrush(CBrush *Brush)
	{
		Brushes->Add(Brush);
	}

	void AddArgVal(CArgVal *ArgVal)
	{
		ArgVals->Add(ArgVal);
	}

	ArrayList *GetArgVals()
	{
		return ArgVals;
	}

	ArrayList *GetBrushes()
	{
		return Brushes;
	}

	String *GetName()
	{
		CArgVal* ArgVal;
		for(int i = 0; i < ArgVals->Count; i++)
		{
			ArgVal = static_cast<CArgVal*>(ArgVals->get_Item(0));
			if(ArgVal->GetArg()->ToLower()->Equals("classname"))
			{
				return ArgVal->GetVal();
			}
		}

		return S"unknowen";
	}

	int GetBrushCount()
	{
		return Brushes->Count;
	}

	void UpdateTexCoords(CTextureManager *TextureManager)
	{
		for(int i = 0; i < Brushes->Count; i++)
		{
			static_cast<CBrush*>(Brushes->get_Item(i))->UpdateTedCoords(TextureManager);
		}
	}
};