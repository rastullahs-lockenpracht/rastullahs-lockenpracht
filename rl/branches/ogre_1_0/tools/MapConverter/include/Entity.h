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
		return GetArgVal( S"classname" );
	}

    String *GetArgVal(String* argval)
	{
		CArgVal* ArgVal;
		for(int i = 0; i < ArgVals->Count; i++)
		{
			ArgVal = static_cast<CArgVal*>(ArgVals->get_Item(i));

			if(ArgVal->GetArg()->ToLower()->Equals(argval))
			{
				return ArgVal->GetVal();
			}
		}

		return S"unknown";
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

    void WriteToStream( System::IO::StringWriter* sw, CTextureManager* textureManager )
    {
        sw->WriteLine(S"{");
        
        CArgVal* ArgVal;
		for(int i = 0; i < ArgVals->Count; i++)
		{
			ArgVal = static_cast<CArgVal*>(ArgVals->get_Item(i));

            sw->WriteLine(S"\"{0}\" \"{1}\"",ArgVal->GetArg(), ArgVal->GetVal());
		}

        CBrush* brush;
		for(int i = 0; i < Brushes->Count; i++)
		{
            sw->WriteLine(S"{");


			brush = static_cast<CBrush*>(Brushes->get_Item(i));
            
            CFace* face;
            ArrayList* Faces = brush->GetFaces();

            for(int j = 0; j < Faces->Count; j++)
		    {
			    face = static_cast<CFace*>(Faces->get_Item(j));

                for( int k = 0; k < 3; k++ )
                {
                    Vector* v = face->GetV(k);
                    sw->Write( S"( " );
                    sw->Write( v->X );
                    sw->Write( S" " );
                    sw->Write( v->Y );
                    sw->Write( S" " );
                    sw->Write( v->Z );
                    sw->Write( S" ) " );
                }


                sw->Write( face->GetTexture(textureManager) );
                sw->Write( S" " );
                sw->Write( face->GetUShift().ToString() );
                sw->Write( S" " );
                sw->Write( face->GetVShift().ToString() );
                sw->Write( S" " );
                sw->Write( face->GetRotation().ToString() );
                sw->Write( S" " );
                sw->Write( face->GetUScale().ToString() );
                sw->Write( S" " );
                sw->Write( face->GetVScale().ToString() );
                sw->WriteLine( S" " );
		    }

            sw->WriteLine(S"}");
		}

        sw->WriteLine(S"}");
        sw->Flush();
    }
};