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

#include "TextureManager.h"

__gc class CFace
{
private:	
	int iIndex;
	unsigned int iID;
	Vector *vAxisU;
	Vector *vAxisV;
	Vertex2f vShift;
	Vertex2f vScale;
	float fRotation;
	
	int iNumVertices;

    Vector *V[];

	Vertex2f *TexCoords;
	Vertex3f *Vertices;
	Vertex3f vNormal;

public:
	CFace(int iNumVertices)
	{
		this->iNumVertices = iNumVertices;

		TexCoords = new Vertex2f[iNumVertices];
		Vertices = new Vertex3f[iNumVertices];
	}

	~CFace()
	{
		delete []TexCoords;
		delete []Vertices;
	}
	
	void SetVertex(int iIndex, Vertex3f Vertex)
	{
		Vertices[iIndex] = Vertex;
	}

    void SetV( Vector *V1, Vector *V2, Vector *V3 )
    {
        V = new Vector*[3];
        this->V[0] = V1;
        this->V[1] = V2;
        this->V[2] = V3;
    }

    Vector* GetV( int id )
    {
        return V[id];
    }

	void SetNormal(Vertex3f vNormal)
	{
		this->vNormal.X = vNormal.X;
		this->vNormal.Y = vNormal.Y;
		this->vNormal.Z = vNormal.Z;
	}

	void SetTextureInfo(int iIndex, Vector *vAxisU, Vector *vAxisV, Vertex2f vShift, Vertex2f vScale, float fRotation)
	{
		this->iIndex = iIndex;
		this->vAxisU = vAxisU;
		this->vAxisV = vAxisV;
		this->vShift.X = vShift.X;
		this->vShift.Y = vShift.Y;
		this->vScale.X = vScale.X;
		this->vScale.Y = vScale.Y;
		this->fRotation = fRotation;
	}

	void SetTextureID(unsigned int iID)
	{
		this->iID = iID;
	}

	Vertex3f *GetVertices()
	{
		return Vertices;
	}

	Vertex3f GetVertex(int iIndex)
	{
		return Vertices[iIndex];
	}
	
	int GetVertexCount()
	{
		return iNumVertices;
	}

	String *GetTexture(CTextureManager *TextureManager)
	{
		return TextureManager->GetName(iIndex);
	}

	float GetUShift()
	{
		return vShift.X;
	}

	float GetVShift()
	{
		return vShift.Y;
	}

	float GetUScale()
	{
		return vScale.X;
	}

	float GetVScale()
	{
		return vScale.Y;
	}

	float GetRotation()
	{
		return fRotation;
	}

	void UpdateTexCoords(CTextureManager *TextureManager)
	{
		iID = TextureManager->GetID(iIndex);

		float W, H, SX, SY;
		W = 1.0f / (float)TextureManager->GetWidth(iIndex);
		H = 1.0f / (float)TextureManager->GetHeight(iIndex);
		SX = 1.0f / vScale.X;
		SY = 1.0f / vScale.Y;

		Vector *V;
		for(int i = 0; i < iNumVertices; i++)
		{
			V = new Vector(Vertices[i]);

			TexCoords[i].X = (V->Dot(vAxisU) * W * SX) + (vShift.X * W);
			TexCoords[i].Y = (V->Dot(vAxisV) * H * SY) + (vShift.Y * H);
		}
	}
};