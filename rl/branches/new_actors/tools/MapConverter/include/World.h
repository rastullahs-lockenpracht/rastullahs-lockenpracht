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
#include "Brush.h"
#include "Entity.h"
#include "TextureManager.h"

__sealed __gc class CWorld
{
private:
	int iBrushCount;
	ArrayList *Entities;

public:
	CWorld()
	{
		Entities = new ArrayList();
	}

	void AddEntity(CEntity *Entity)
	{
		Entities->Add(Entity);
	}

	ArrayList *GetEntities()
	{
		return Entities;
	}

	int GetBrushCount()
	{
		return iBrushCount;
	}

	bool GetWADString(String **sWADString)
	{
		if(Entities->Count == 0)
			return false;

		ArrayList *ArgVals;
		CArgVal *ArgVal;
		ArgVals = static_cast<CEntity*>(Entities->get_Item(0))->GetArgVals();

		for(int i = 0; i < ArgVals->Count; i++)
		{
			ArgVal = static_cast<CArgVal*>(ArgVals->get_Item(i));
			if(ArgVal->GetArg()->ToLower()->Equals("wad"))
			{
				*sWADString = ArgVal->GetVal();
				return true;
			}
		}

		return false;
	}

	void UpdateBrushCount()
	{
		iBrushCount = 0;

		for(int i = 0; i < Entities->Count; i++)
		{
			iBrushCount += static_cast<CEntity*>(Entities->get_Item(i))->GetBrushCount();
		}
	}

	void UpdateTexCoords(CTextureManager *TextureManager)
	{
		for(int i = 0; i < Entities->Count; i++)
		{
			static_cast<CEntity*>(Entities->get_Item(i))->UpdateTexCoords(TextureManager);
		}
	}
};