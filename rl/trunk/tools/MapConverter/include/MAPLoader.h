/*
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

#include "MapConverterPrerequisites.h"

using namespace System::IO;

__gc class CMainForm;

#include "World.h"
#include "Entity.h"
#include "Brush.h"
#include "Face.h"
#include "TextureManager.h"

#define LineTypeEmpty 0
#define LineTypeOpenBrace 1
#define LineTypeCloseBrace 2
#define LineTypeArgVal 3
#define LineTypeFace 4

__gc class CMAPLoader
{
private:
    int iLine;

public:
    CMAPLoader();
    bool LoadMAPFile(String *sFile, CWorld *World, CTextureManager *TextureManager);

private:
    bool ReadEntity(StreamReader *MAPFile, CEntity *Entity, CTextureManager *TextureManager);
    bool ReadBrush(StreamReader *MAPFile, CBrush *Brush, CTextureManager *TextureManager);
    int ProcessLine(String **sLine);

private:
    typedef __gc struct CMAPFace
    {
    public:
        CMAPFace(Vector *V1, Vector *V2, Vector *V3, String *Texture, Vector *AxisU, Vector *AxisV, Vertex2f Shift, Vertex2f Scale, float Rotation)
        {
            this->V1 = V1;
            this->V2 = V2;
            this->V3 = V3;

            this->Texture = Texture;

            this->AxisU = AxisU;
            this->AxisV = AxisV;
            this->Shift.X = Shift.X;
            this->Shift.Y = Shift.Y;
            this->Scale.X = Scale.X;
            this->Scale.Y = Scale.Y;
            this->Rotation = Rotation;
        }

        Vector *V1;
        Vector *V2;
        Vector *V3;

        String *Texture;

        Vector *AxisU;
        Vector *AxisV;
        Vertex2f Shift;
        Vertex2f Scale;
        float Rotation;
    } CMAPFace;

    typedef __gc struct CPlane
    {
    public:
        CPlane(Vector *Normal, float Distance)
        {
            this->Normal = Normal;
            this->Distance = Distance;
        }

        Vector *Normal;
        float Distance;

        /*                        -1
        |x|     | n1.x n1.y n1.z |     |k1|
        |y|  =  | n2.x n2.y n2.z |  X  |k2|
        |z|     | n3.x n3.y n3.z |     |k3|
        */
        static bool GetIntersection(CPlane *P1, CPlane *P2, CPlane *P3, Vector *V)
        {
            //float fDenom = P1->Normal->Dot(P2->Normal->Cross(P3->Normal));

            //if(fDenom == 0.0f)
                //return false;

            float fDet;
            float MN[9] = { P1->Normal->X, P1->Normal->Y, P1->Normal->Z, P2->Normal->X, P2->Normal->Y, P2->Normal->Z, P3->Normal->X, P3->Normal->Y, P3->Normal->Z };
            float IMN[9] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
            float MD[3] = { P1->Distance, P2->Distance , P3->Distance };

            //fDet = MN[0] * (MN[4] * MN[8] - MN[5] * MN[7]);
            //fDet += -MN[1] * (MN[3] * MN[8] - MN[5] * MN[6]);
            //fDet += MN[2] * (MN[3] * MN[7] - MN[4] * MN[6]);

            /*fDet = MN[0] * MN[4] * MN[8]
                 + MN[1] * MN[5] * MN[6]
                 + MN[2] * MN[3] * MN[7] 
                 - MN[2] * MN[4] * MN[6]
                 - MN[0] * MN[5] * MN[7]
                 - MN[1] * MN[3] * MN[8];*/

            IMN[0] = MN[4] * MN[8] - MN[5] * MN[7];
            IMN[3] = -(MN[3] * MN[8] - MN[5] * MN[6]);
            IMN[6] = MN[3] * MN[7] - MN[4] * MN[6];

            fDet = MN[0] * IMN[0] + MN[1] * IMN[3] + MN[2] *IMN[6];

            if(fDet == 0.0f)
                return false;

            IMN[1] = -(MN[1] * MN[8] - MN[2] * MN[7]);
            IMN[4] = MN[0] * MN[8] - MN[2] * MN[6];
            IMN[7] = -(MN[0] * MN[7] - MN[1] * MN[6]);
            IMN[2] = MN[1] * MN[5] - MN[2] * MN[4];
            IMN[5] = -(MN[0] * MN[5] - MN[2] * MN[3]);
            IMN[8] = MN[0] * MN[4] - MN[1] * MN[3];

            fDet = 1.0f / fDet;

            IMN[0] *= fDet;
            IMN[1] *= fDet;
            IMN[2] *= fDet;
            IMN[3] *= fDet;
            IMN[4] *= fDet;
            IMN[5] *= fDet;
            IMN[6] *= fDet;
            IMN[7] *= fDet;
            IMN[8] *= fDet;

            V->X = IMN[0] * MD[0] + IMN[1] * MD[1] + IMN[2] * MD[2];
            V->Y = IMN[3] * MD[0] + IMN[4] * MD[1] + IMN[5] * MD[2];
            V->Z = IMN[6] * MD[0] + IMN[7] * MD[1] + IMN[8] * MD[2];

            return true;
        }
    } CPlane;

    __gc class CMAPFaceManager
    {
    private:
        ArrayList *MAPFaces;

    public:
        CMAPFaceManager()
        {
            MAPFaces = new ArrayList();
        }

        void AddMAPFace(CMAPFace *MAPFace)
        {
            MAPFaces->Add(MAPFace);
        }

        ArrayList *GetMAPFaces()
        {
            return MAPFaces;
        }
    };

private:
    void BuildBrushFaces(CMAPFaceManager *MAPFaceManager, CBrush *Brush, CTextureManager *TextureManager);
};