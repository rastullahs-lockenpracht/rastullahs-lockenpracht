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

#include "TextureManager.h"

CTextureManager::CTextureManager()
{
    Textures = new ArrayList();
}

int CTextureManager::AddTexture(String *TextureName)
{
    TextureName = TextureName->ToLower();

    for(int i = 0; i < Textures->Count; i++)
    {
        if(static_cast<Texture*>(Textures->get_Item(i))->TextureName->Equals(TextureName))
        {
            return i;
        }
    }

    Textures->Add(new Texture(TextureName));
    return Textures->Count - 1;
}

ArrayList *CTextureManager::GetTextures()
{
    return Textures;
}

Texture *CTextureManager::GetTexture(int iIndex)
{
    return static_cast<Texture*>(Textures->get_Item(iIndex));
}

unsigned int CTextureManager::GetID(int iIndex)
{
    return static_cast<Texture*>(Textures->get_Item(iIndex))->ID;
}

unsigned long CTextureManager::GetWidth(int iIndex)
{
    return static_cast<Texture*>(Textures->get_Item(iIndex))->Width;
}

unsigned long CTextureManager::GetHeight(int iIndex)
{
    return static_cast<Texture*>(Textures->get_Item(iIndex))->Height;
}

String *CTextureManager::GetName(int iIndex)
{
    return static_cast<Texture*>(Textures->get_Item(iIndex))->TextureName;
}

void CTextureManager::LoadTextures(String *WADString, String *WADPath )
{
    int iTexturesLoaded = 0, iTotalTexturesLoaded = 0;
    WADString = WADString->Replace("../", "");
    WADString = WADString->Replace('/', '\\');
    String *WAD, *WADs[] = WADString->Split(S";"->ToCharArray());

    bool bWADFound;
    LPCTSTR szDrive= NULL;
    String *Drives[] = System::IO::Directory::GetLogicalDrives();

    for(int i = 0; i < Drives->Length; i++)
    {
        szDrive = (LPCTSTR)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(Drives[i]).ToPointer());

        if(GetDriveType(szDrive) != DRIVE_FIXED)
            Drives[i] = "";

        System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr((void*)szDrive)));
    }

    String* pathName = WADPath;

    for(int i = 0; i < WADs->Count; i++)
    {
        bWADFound = false;
        String* wadChanged = WADs[i]->Replace( "\\\\", "\\");
        String* justWAD = S"";

        

        int iPos = wadChanged->LastIndexOf("\\");

        if( iPos != -1 )
            justWAD = wadChanged->Substring( iPos );

        WAD = String::Concat( pathName, justWAD ) ;
        bWADFound = System::IO::File::Exists(WAD);

        if(!bWADFound)
        {
            for(int j = 0; j < Drives->Length; j++)
            {
                if(Drives[j]->Length == 0)
                    continue;

                WAD = String::Concat( Drives[j], wadChanged ) ;
                bWADFound = System::IO::File::Exists(WAD);

                if(bWADFound)
                    break;
            }
        }

        if(bWADFound)
        {
            LogManager::getSingletonPtr()->Log(String::Concat(S"Loading ", WAD, S"..."), Color::Gray);
            CWADLoader::LoadWADFile(WAD, Textures, &iTexturesLoaded);
            LogManager::getSingletonPtr()->Log(String::Concat(iTexturesLoaded.ToString(), S" textures loaded.", S"\n"), Color::Gray);

            iTotalTexturesLoaded += iTexturesLoaded;
        }
        else
        {
           LogManager::getSingletonPtr()->Log(String::Concat(justWAD, S" was nowhere to be found.", S"\n"), Color::Red);
        }
    }
    
    LogManager::getSingletonPtr()->Log(String::Concat(iTotalTexturesLoaded.ToString(), S" out of ", Textures->Count.ToString(), S" textures loaded.", S"\n"), Color::Green);
}

void CTextureManager::DestroyTextures()
{
    Textures->Clear();
}