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

#include "MAPLoader.h"

CMAPLoader::CMAPLoader()
{
}

bool CMAPLoader::LoadMAPFile(String *sFile, CWorld *World, CTextureManager *TextureManager)
{
    iLine = 0;
    StreamReader *MAPFile;

    LogManager::getSingletonPtr()->Log(String::Concat(S"Loading ", sFile, S"..."), Color::Green);
    
    try
    {
        MAPFile = File::OpenText(sFile);
    }
    catch(Exception *e)
    {
        LogManager::getSingletonPtr()->Log(String::Concat(S"Error loading ", sFile, S": ", e->Message, S"\n"), Color::Red);
        return false;
    }

    try
    {
        String *sLine = NULL;
        CEntity *Entity;

        while((sLine = MAPFile->ReadLine()) != NULL)
        {
            switch(ProcessLine(&sLine))
            {
                case LineTypeEmpty:
                    continue;
                case LineTypeOpenBrace:
                    Entity = new CEntity();
                    if(ReadEntity(MAPFile, Entity, TextureManager))
                    {
                        World->AddEntity(Entity);
                    }
                    else
                    {
                        LogManager::getSingletonPtr()->Log(String::Concat(S"Error loading ", sFile, S" on line ", iLine.ToString(), S".", S"\n"), Color::Red);
                        MAPFile->Close();
                        return false;
                    }
                    break;
                default:
                    LogManager::getSingletonPtr()->Log(String::Concat(S"Error loading ", sFile, S" on line ", iLine.ToString(), S".", S"\n"), Color::Red);
                    MAPFile->Close();
                    return false;
            };
        }
    }
    catch(Exception *e)
    {
        LogManager::getSingletonPtr()->Log(String::Concat(S"Error loading ", sFile, S": ", e->Message, S"\n"), Color::Red);
    }

    MAPFile->Close();

    LogManager::getSingletonPtr()->Log(String::Concat(S"Loading ", sFile, S" finished."), Color::Green);

    return World->GetEntities()->Count != 0;
}

bool CMAPLoader::ReadEntity(StreamReader *MAPFile, CEntity *Entity, CTextureManager *TextureManager)
{
    String *sLine = NULL;
    CBrush *Brush;
    while((sLine = MAPFile->ReadLine()) != NULL)
    {
        switch(ProcessLine(&sLine))
        {
            case LineTypeEmpty:
                continue;
            case LineTypeArgVal:
                if(!sLine->StartsWith("\"") || ! sLine->EndsWith("\""))
                    return false;

                sLine = sLine->Remove(0, 1);
                sLine = sLine->Remove(sLine->Length - 1, 1);

                int iPos;
                String *sArg, *sVal;

                iPos = sLine->IndexOf("\" \"");
                if(iPos == -1)
                    return false;

                sArg = sLine->Substring(0, iPos);
                sVal = sLine->Substring(iPos + 3);
                Entity->AddArgVal(new CArgVal(sArg, sVal));
                break;
            case LineTypeOpenBrace:
                Brush = new CBrush();
                if(ReadBrush(MAPFile, Brush, TextureManager))
                {
                    Entity->AddBrush(Brush);
                }
                else
                {
                    return false;
                }
                break;
            case LineTypeCloseBrace:
                return true;
            default:
                return false;
        };
    }

    return false;
}

bool CMAPLoader::ReadBrush(StreamReader *MAPFile, CBrush *Brush, CTextureManager *TextureManager)
{
    CMAPFaceManager *MAPFaceManager = new CMAPFaceManager();
    String *sLine = NULL;
    String *sTokens[];

    Vector *Normal, *UAxis, *VAxis, *V1, *V2, *V3;
    Vertex2f Shift, Scale;
    float Rotation;


    while((sLine = MAPFile->ReadLine()) != NULL)
    {
        switch(ProcessLine(&sLine))
        {
            case LineTypeEmpty:
                continue;
            case LineTypeFace:
                sLine = sLine->Replace(S"  ", S" ");
                sTokens = sLine->Split(S" "->ToCharArray());

                if(sTokens->Count == 31)
                {
                    if(!sTokens[0]->Equals("("))
                        return false;
                    if(!sTokens[4]->Equals(")"))
                        return false;
                    if(!sTokens[5]->Equals("("))
                        return false;
                    if(!sTokens[9]->Equals(")"))
                        return false;
                    if(!sTokens[10]->Equals("("))
                        return false;
                    if(!sTokens[14]->Equals(")"))
                        return false;

                    if(!sTokens[16]->Equals("["))
                        return false;
                    if(!sTokens[21]->Equals("]"))
                        return false;
                    if(!sTokens[22]->Equals("["))
                        return false;
                    if(!sTokens[27]->Equals("]"))
                        return false;

                    try
                    {
                        V1 = new Vector(Convert::ToSingle(sTokens[ 1]), Convert::ToSingle(sTokens[ 2]), Convert::ToSingle(sTokens[ 3]));
                        V2 = new Vector(Convert::ToSingle(sTokens[ 6]), Convert::ToSingle(sTokens[ 7]), Convert::ToSingle(sTokens[ 8]));
                        V3 = new Vector(Convert::ToSingle(sTokens[11]), Convert::ToSingle(sTokens[12]), Convert::ToSingle(sTokens[13]));
                        UAxis = new Vector(Convert::ToSingle(sTokens[17]), Convert::ToSingle(sTokens[18]), Convert::ToSingle(sTokens[19]));
                        VAxis = new Vector(Convert::ToSingle(sTokens[23]), Convert::ToSingle(sTokens[24]), Convert::ToSingle(sTokens[25]));

                        Shift.X = Convert::ToSingle(sTokens[20]);
                        Shift.Y = Convert::ToSingle(sTokens[26]);
                        Scale.X = Convert::ToSingle(sTokens[29]);
                        Scale.Y = Convert::ToSingle(sTokens[30]);

                        Rotation = Convert::ToSingle(sTokens[28]);

                        MAPFaceManager->AddMAPFace(new CMAPFace(V1, V2, V3, sTokens[15]->ToLower(), UAxis, VAxis, Shift, Scale, Rotation));
                    }
                    catch(Exception *e)
                    {
                        return false;
                    }
                }
                else if(sTokens->Count >= 21)
                {
                    if(!sTokens[0]->Equals("("))
                        return false;
                    if(!sTokens[4]->Equals(")"))
                        return false;
                    if(!sTokens[5]->Equals("("))
                        return false;
                    if(!sTokens[9]->Equals(")"))
                        return false;
                    if(!sTokens[10]->Equals("("))
                        return false;
                    if(!sTokens[14]->Equals(")"))
                        return false;

                    try
                    {
                        V1 = new Vector(Convert::ToSingle(sTokens[ 1]), Convert::ToSingle(sTokens[ 2]), Convert::ToSingle(sTokens[ 3]));
                        V2 = new Vector(Convert::ToSingle(sTokens[ 6]), Convert::ToSingle(sTokens[ 7]), Convert::ToSingle(sTokens[ 8]));
                        V3 = new Vector(Convert::ToSingle(sTokens[11]), Convert::ToSingle(sTokens[12]), Convert::ToSingle(sTokens[13]));

                        Normal = VectorMath::CalculateNormal(V1, V2, V3);

                        // To do: Axis needs to be rotated by 'Rotation'.
                        VectorMath::BestAxis(Normal, &UAxis, &VAxis);

                        Shift.X = Convert::ToSingle(sTokens[17]);
                        Shift.Y = Convert::ToSingle(sTokens[18]);
                        Scale.X = Convert::ToSingle(sTokens[19]);
                        Scale.Y = Convert::ToSingle(sTokens[20]);

                        Rotation = Convert::ToSingle(sTokens[16]);

                        MAPFaceManager->AddMAPFace(new CMAPFace(V1, V2, V3, sTokens[15]->ToLower(), UAxis, VAxis, Shift, Scale, Rotation));
                    }
                    catch(Exception *e)
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
                break;
            case LineTypeCloseBrace:
                BuildBrushFaces(MAPFaceManager, Brush, TextureManager);
                return true;
                break;
            default:
                return false;
        };
    }

    return false;
}

void CMAPLoader::BuildBrushFaces(CMAPFaceManager *MAPFaceManager, CBrush *Brush, CTextureManager *TextureManager)
{
    CMAPFace *MAPFace;
    Vector *Normal;
    ArrayList *Planes = new ArrayList();
    for(int i = 0; i < MAPFaceManager->GetMAPFaces()->Count; i++)
    {
        MAPFace = static_cast<CMAPFace*>(MAPFaceManager->GetMAPFaces()->get_Item(i));
        Normal = VectorMath::CalculateNormal(MAPFace->V1, MAPFace->V2, MAPFace->V3);
        Planes->Add(new CPlane(Normal, Normal->Dot(MAPFace->V1)));
    }

    ArrayList *Faces = new ArrayList();
    for(int i = 0; i < Planes->Count; i++)
    {
        Faces->Add(new ArrayList());
    }

    for(int i = 0; i < Planes->Count - 2; i++)
    {
        for(int j = 0; j < Planes->Count - 1; j++)
        {
            for(int k = 0; k < Planes->Count; k++)
            {
                if(i != j && i != k && j != k)
                {
                    Vector *vIntersection = new Vector();
                    if(CPlane::GetIntersection(static_cast<CPlane*>(Planes->get_Item(i)), static_cast<CPlane*>(Planes->get_Item(j)), static_cast<CPlane*>(Planes->get_Item(k)), vIntersection) && VectorMath::PointInWorld(vIntersection))
                    {
                        CPlane *Plane;
                        bool bLegal = true;

                        for(int l = 0; l < Planes->Count; l++)
                        {
                            if(l != i && l != j && l != k)
                            {
                                Plane = static_cast<CPlane*>(Planes->get_Item(l));
                                if((Plane->Normal->Dot(vIntersection) - Plane->Distance) < EPSILON)
                                {
                                    bLegal = false;
                                    break;
                                }
                            }
                        }

                        if(bLegal)
                        {
                            if(!VectorMath::PointInVectors(static_cast<ArrayList*>(Faces->get_Item(i)), vIntersection))
                                static_cast<ArrayList*>(Faces->get_Item(i))->Add(vIntersection);

                            if(!VectorMath::PointInVectors(static_cast<ArrayList*>(Faces->get_Item(j)), vIntersection))
                                static_cast<ArrayList*>(Faces->get_Item(j))->Add(vIntersection);

                            if(!VectorMath::PointInVectors(static_cast<ArrayList*>(Faces->get_Item(k)), vIntersection))
                                static_cast<ArrayList*>(Faces->get_Item(k))->Add(vIntersection);
                        }
                    }
                }
            }
        }
    }

    float fCount = 0.0f;
    ArrayList *Face;
    Vector *NewVector;

    Vector *Origin= new Vector();
    for(int i = 0; i < Planes->Count; i++)
    {
        Face = static_cast<ArrayList*>(Faces->get_Item(i));
        for(int j = 0; j < Face->Count; j++)
        {
            fCount++;
            NewVector = static_cast<Vector*>(Face->get_Item(j));

            Origin->X += NewVector->X;
            Origin->Y += NewVector->Y;
            Origin->Z += NewVector->Z;
        }
    }

    fCount = 1.0f / fCount;
    Origin->X *= fCount;
    Origin->Y *= fCount;
    Origin->Z *= fCount;

    CFace *BrushFace;
    Vertex3f NewVertex;
    for(int i = 0; i < Planes->Count; i++)
    {
        Face = static_cast<ArrayList*>(Faces->get_Item(i));
        MAPFace = static_cast<CMAPFace*>(MAPFaceManager->GetMAPFaces()->get_Item(i));

        VectorMath::SortVectors(Face, VectorMath::CalculateNormal(MAPFace->V1, MAPFace->V2, MAPFace->V3));
        VectorMath::ArrangeCCW(Origin, Face);

        BrushFace = new CFace( Face->Count);
        
        BrushFace->SetV( MAPFace->V1, MAPFace->V2, MAPFace->V3 );
        NewVector = VectorMath::CalculateNormal(MAPFace->V1, MAPFace->V2, MAPFace->V3);
        NewVertex.X = NewVector->X;
        NewVertex.Y = NewVector->Y;
        NewVertex.Z = NewVector->Z;

        BrushFace->SetNormal(NewVertex);

        for(int j = 0; j < Face->Count; j++)
        {
            NewVector = static_cast<Vector*>(Face->get_Item(j));
            NewVertex.X = NewVector->X;
            NewVertex.Y = NewVector->Y;
            NewVertex.Z = NewVector->Z;

            BrushFace->SetVertex(j, NewVertex);
        }
        
        Vertex2f Shift, Scale;
        float Rotation;
        Shift.X = MAPFace->Shift.X;
        Shift.Y = MAPFace->Shift.Y;
        Scale.X = MAPFace->Scale.X;
        Scale.Y = MAPFace->Scale.Y;
        Rotation = MAPFace->Rotation;
        BrushFace->SetTextureInfo(TextureManager->AddTexture(MAPFace->Texture), MAPFace->AxisU, MAPFace->AxisV, Shift, Scale, Rotation);

        Brush->AddFace(BrushFace);
    }
}

int CMAPLoader::ProcessLine(String **sLine)
{
    iLine++;

    int iPos;

    iPos = (*sLine)->IndexOf("//");
    if(iPos != -1)
        *sLine = (*sLine)->Remove(iPos, (*sLine)->Length - iPos);

    iPos = (*sLine)->IndexOf(";");
    if(iPos != -1)
        *sLine = (*sLine)->Remove(iPos, (*sLine)->Length - iPos);

    *sLine = (*sLine)->Trim();

    if((*sLine)->Length == 0)
    {
        return LineTypeEmpty;
    }
    else if((*sLine)->Equals("{"))
    {
        return LineTypeOpenBrace;
    }
    else if((*sLine)->Equals("}"))
    {
        return LineTypeCloseBrace;
    }
    else if((*sLine)->StartsWith("("))
    {
        return LineTypeFace;
    }
    else
    {
        return LineTypeArgVal;
    }
}