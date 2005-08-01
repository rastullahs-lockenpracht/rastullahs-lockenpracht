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


#include "WADLoader.h"

bool CWADLoader::MapFile(LPCTSTR szFileName, LPVOID *pView, LPDWORD pdwFileSize)
{
    HANDLE hFile = NULL;
    HANDLE hFileMapping = NULL;
    LPVOID lpView = NULL;
    DWORD dwFileSize = 0;

    hFile = CreateFile( 
        szFileName,                 // Name of file
        GENERIC_READ,               // Desired access
        FILE_SHARE_READ,            // Share mode
        NULL,                       // Security attributes
        OPEN_EXISTING,              // Creation disposition
        FILE_FLAG_SEQUENTIAL_SCAN,  // Attributes and flags
        NULL);                      // Template file    

    if(hFile == INVALID_HANDLE_VALUE)
        return false;

    // Store this away for now...
    dwFileSize = GetFileSize(hFile, NULL);

    if(dwFileSize == -1)
        return false;
    
    hFileMapping = CreateFileMapping( 
        hFile,                      // Handle to file
        NULL,                       // Security attributes
        PAGE_READONLY,              // Protection
        0,                          // Max size high
        0,                          // Max size low
        NULL);                      // Name of mapping object   
    
    if(hFileMapping == NULL)
        return false;

    // We don't need this anymore
    CloseHandle(hFile);

    // Map to the entire file
    lpView = MapViewOfFile(
        hFileMapping,               // Handle to the mapping
        FILE_MAP_READ,              // Desired access
        0,                          // Offset high
        0,                          // Offset low
        0);                         // Number of bytes

    if(lpView == NULL)
        return false;

    // We don't need this anymore
    CloseHandle(hFileMapping);

    if(pView)
    {
        *pView = lpView;
    }
    
    if(pdwFileSize)
    {
        *pdwFileSize = dwFileSize;
    }

    return true;
}

bool CWADLoader::LoadWADFile(String *sFile, ArrayList *Textures, int *iTexturesLoaded)
{
    LPVOID          lpView = NULL;
    LPWAD3_HEADER   lpHeader = NULL;
    LPWAD3_LUMP     lpLump = NULL;
    LPWAD3_MIP      lpMip = NULL;

    DWORD           dwNumLumps = 0;
    DWORD           dwTableOffset = 0;
    DWORD           dwFileSize = 0;
    DWORD           dwFilePos = 0;
    DWORD           dwPaletteOffset = 0;
    WORD            wPaletteSize = 0;
    String          *sTextureName;
    DWORD           dwWidth = 0;
    DWORD           dwHeight = 0;
    LPBYTE          lpImageData;
    LPBYTE          lpPalette;
    LPBYTE          lpImage;

    unsigned int    iID;
    bool            bLoadTex;
    Texture         *Tex;
    unsigned char cTextureName __gc[] = new unsigned char __gc[16];
    LPCTSTR         szFileName = NULL;

    *iTexturesLoaded = 0;
    
    System::Text::ASCIIEncoding *ASCII = new System::Text::ASCIIEncoding();

    szFileName = (LPCTSTR)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(sFile).ToPointer());

    if(MapFile(szFileName, &lpView, &dwFileSize) == false)
    {
        System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr((void*)szFileName)));
        return false;
    }

    System::Runtime::InteropServices::Marshal::FreeHGlobal((IntPtr((void*)szFileName)));

    if(dwFileSize < sizeof(WAD3_HEADER))
        return false;

    lpHeader = (LPWAD3_HEADER)lpView;

    if(lpHeader->identification != WAD3_ID)
        return false;

    dwNumLumps = lpHeader->numlumps;
    dwTableOffset = lpHeader->infotableofs;

    if(((dwNumLumps * sizeof(WAD3_LUMP)) + dwTableOffset) > dwFileSize)
        return false;

    lpLump = (LPWAD3_LUMP)((LPBYTE)lpView + dwTableOffset);

    for(DWORD i = 0; i < dwNumLumps; i++, lpLump++)
    {       
        if(lpLump->type != WAD3_TYPE_MIP)
            continue;

        // Find out where the MIP actually is.
        dwFilePos = lpLump->filepos;

        if(dwFilePos >= dwFileSize)
            return false;

        lpMip = (LPWAD3_MIP)((LPBYTE)lpView + dwFilePos);

        for(DWORD j = 0; j < 16; j++)
        {
            /*if(lpMip->name[j] == '\0')
            {
                for(;j < 16; j++)
                {
                    cTextureName[j] = '\0';
                }
            }
            else
            {
                cTextureName[j] = (unsigned char)lpMip->name[j];
            }*/
            cTextureName[j] = (unsigned char)lpMip->name[j];
        }

        bLoadTex = false;
        sTextureName = ASCII->GetString(cTextureName, 0, strlen(lpMip->name))->ToLower();

        for(INT j = 0; j < Textures->Count; j++)
        {
            Tex = static_cast<Texture*>(Textures->get_Item(j));

            if(Tex->Loaded)
                continue;

            if(String::Compare(sTextureName, Tex->TextureName) == 0)
            {
                bLoadTex = true;
                break;
            }
        }

        if(!bLoadTex)
            continue;

        dwWidth = lpMip->width;
        dwHeight = lpMip->height;

        dwPaletteOffset = GET_MIP_DATA_SIZE(dwWidth, dwHeight);
        wPaletteSize = *(WORD *)((LPBYTE)lpMip + dwPaletteOffset);
        dwPaletteOffset += sizeof(WORD);

        lpPalette = ((LPBYTE)lpMip + dwPaletteOffset);

        if((dwFilePos + lpMip->offsets[0] + dwWidth * dwHeight) > dwFileSize)
            return false;

        lpImageData = ((LPBYTE)lpMip + lpMip->offsets[0]);

        lpImage = NULL;
        lpImage = new BYTE[dwWidth * dwHeight * 3];

        if(lpImage == NULL)
            return false;

        for(DWORD j = 0, k = 0; j < dwWidth * dwHeight; j++)
        {
            lpImage[k++] = lpPalette[lpImageData[j] * 3];
            lpImage[k++] = lpPalette[lpImageData[j] * 3 + 1];
            lpImage[k++] = lpPalette[lpImageData[j] * 3 + 2];
        }

        Tex->Loaded = true;
        Tex->ID = 0;
        Tex->Width = dwWidth;
        Tex->Height = dwHeight;

        (*iTexturesLoaded)++;

        delete[] lpImage;
    }

    UnmapViewOfFile(lpView);

    return true;
}