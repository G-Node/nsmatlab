///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2003 - 2010  Neuroshare Project                                                         
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// A copy of the GNU Lesser General Public License can be obtained by writing to:
//  Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330,
//  Boston, MA  02111-1307
//  USA
//
// Contact information:
//  Hyrum L. Sessions
//  Blackrock Microsystems
//  391 Chipeta Way, Suite G
//  Salt Lake City,  UT  84108
//  USA
//  hsessions@blackrockmicro.com
//
// Website:
//  www.neuroshare.org
//
// All other copyrights on this material are replaced by this license agreeement.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// $Workfile: main.c $
//
// Description   : This is the gateway routine for a MATLAB Math/Graphics Library-based
//                 C MATLAB MEX File.
//
// Structure     : There is a MATLAB m-file for each Neuroshare function. This was done
//                 so function names and arguments could be preserved since mex DLLs
//                 only have one entry/input function.
//
//                 Each MATLAB m-file passes the functions input and output arguments
//                 and a number identifying which function was called to this mex DLL.
//
//                 The main/entry function of this mex DLL checks the number of
//                 input and output arguments and the format of the input arguments 
//                 based on which function was called. It then calls a separate
//                 function for each Neuroshare function. Input arguments and output
//                 pointers are passed to these functions.
//
//                 Each specific function calls the appropriate function in the 
//                 Neuroshare DLL with the input arguments it was given. The returned
//                 data are then converted into MATLAB mxArrays and passed back to the
//                 main function.
//
//                 The main function then converts the result (ns_RESULT) into an
//                 mxArray and returns the output arguments back to MATLAB.
//
// Remarks       : If there is an error along the way, the output arguments are filled
//                 with an empty string, a MATLAB error message is created and the 
//                 program returns back to MATLAB with a general library error.
//
//                 The function 'fOpenFile' currently loads the NEV Neuroshare DLL on
//                 default. In the future, it will select the appropriate Neuroshare
//                 DLL based on the file extension and open it.
//
// Authors       : Almut Branner
//
// $Date: 5/10/04 11:58a $
//
// $History: main.c $
// 
// *****************  Version 22  *****************
// User: Kkorver      Date: 5/10/04    Time: 11:58a
// Updated in $/Neuroshare/Matlab wrapper
// Bug fix: ns_GetLibraryInfo() wasn't giving the correct information
// 
// *****************  Version 21  *****************
// User: Kkorver      Date: 4/26/04    Time: 2:19p
// Updated in $/Neuroshare/Matlab wrapper
// Added MAX macro for compatability with LINUX
// 
// *****************  Version 20  *****************
// User: Kkorver      Date: 4/21/04    Time: 3:09p
// Updated in $/Neuroshare/Matlab wrapper
// Library now will compile in Linux
// 
// *****************  Version 19  *****************
// User: Abranner     Date: 4/01/04    Time: 4:45p
// Updated in $/Neuroshare/Matlab wrapper
// Fixed problem with ns_GetIndexByTime.
// 
// *****************  Version 18  *****************
// User: Kkorver      Date: 2/13/04    Time: 4:57p
// Updated in $/Neuroshare/Matlab wrapper
// ns_GetTimeByIndex can now take a "vector" of Indices as an input
// 
// *****************  Version 17  *****************
// User: Kkorver      Date: 2/13/04    Time: 3:35p
// Updated in $/Neuroshare/Matlab wrapper
// Added support for ns_SetLibrary() from Matlab
// 
// *****************  Version 16  *****************
// User: Abranner     Date: 8/12/03    Time: 11:18a
// Updated in $/Neuroshare/Matlab wrapper
// Changed size of nsLIBRARYINFO structure (was too short).
// 
// *****************  Version 15  *****************
// User: Abranner     Date: 8/12/03    Time: 11:14a
// Updated in $/Neuroshare/Matlab wrapper
// Changed from mxCreateDoubleScalar which only exists in version 6.5 to
// mxCreateScalarDouble which is still supported.
// Fixed error in nsLIBRARYINFO structure.
// 
// *****************  Version 14  *****************
// User: Abranner     Date: 6/24/03    Time: 1:09p
// Updated in $/Neuroshare/Matlab wrapper
// Fixed error in ns_GetSegmentData. Data is now correct when loaded the
// second time.
// 
// *****************  Version 13  *****************
// User: Abranner     Date: 6/23/03    Time: 5:38p
// Updated in $/Neuroshare/Matlab wrapper
// Changed all returned structures to not have C conventions.
// Also changed ns_GetNeuralData to not return only zeros when one of the
// indeces is invalid.
// 
// *****************  Version 12  *****************
// User: Almut        Date: 4/02/03    Time: 1:39p
// Updated in $/Neuroshare/Matlab wrapper
// Fixed Neuroshare Anomaly # 10.
// ns_GetAnalogInfo does not crash anymore
// 
// *****************  Version 11  *****************
// User: Almut        Date: 2/21/03    Time: 2:07p
// Updated in $/Neuroshare/Matlab wrapper
// Program now conforms to Neuroshare API Spec 1.0
// 
// *****************  Version 10  *****************
// User: Almut        Date: 2/21/03    Time: 10:56a
// Updated in $/Neuroshare/Matlab wrapper
// Changed EntityType to numbers instead of a text field to make it easier
// to find certain types.
// 
// *****************  Version 9  *****************
// User: Almut        Date: 2/10/03    Time: 2:38p
// Updated in $/Neuroshare/Matlab wrapper
// Made error messages function specific.
// 
// *****************  Version 8  *****************
// User: Almut        Date: 2/07/03    Time: 5:06p
// Updated in $/Neuroshare/Matlab wrapper
// - Added more checks to allow input of entities and indeces that don't
// exist. It will return empty structure or zeros now and give the user a
// message.
// - Put in the loading of text in geteventdata.
// 
// *****************  Version 7  *****************
// User: Almut        Date: 2/06/03    Time: 5:37p
// Updated in $/Neuroshare/Matlab wrapper
// - transposed arrays and matrices that are returned to make plotting
// simpler.
// 
// *****************  Version 6  *****************
// User: Almut        Date: 2/06/03    Time: 3:37p
// Updated in $/Neuroshare/Matlab wrapper
// - most functions accept arrays for certain parameters like EntityIDs or
// Indeces
// 
// *****************  Version 5  *****************
// User: Almut        Date: 2/04/03    Time: 5:38p
// Updated in $/Neuroshare/Matlab wrapper
// Changed to return/accept file handles to allow opening more than one
// file at a time from within Matlab.
// Code not tested yet.
// 
// *****************  Version 4  *****************
// User: Almut        Date: 2/04/03    Time: 11:32a
// Updated in $/Neuroshare/Matlab wrapper
// Changed program to conform with Coding Standard.
// File is closed and Neuroshare DLL is unloaded when mex DLL shuts down.
// 
// *****************  Version 3  *****************
// User: Almut        Date: 1/22/03    Time: 12:22p
// Updated in $/Neuroshare/Matlab wrapper
// - Changed tabs to spaces
// 
// *****************  Version 2  *****************
// User: Almut        Date: 1/16/03    Time: 3:43p
// Updated in $/Neuroshare/Matlab wrapper
// Included Neuroshare header.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "mex.h"

#include <string.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// Load library for Neuroshare
#include "ns.h"

ns_DLLHANDLE g_nsDllHandle = 0;

typedef int BOOL;
#define FALSE 0
#define TRUE  1

#if defined(WIN32) || defined(_WIN32)

    // Author & Date: Almut Branner, 2/3/2003
    // Purpose: This function is called when the DLL is loaded and unloaded
    //          It also unloads the Neuroshare DLL when this mex DLL is closed
    // Inputs:  Called automatically when the DLL is loaded or closed.
    // Outputs: BOOL - Did function work or not
    BOOL APIENTRY DllMain( HANDLE modhandle, DWORD dllevent, LPVOID reserved )
    {
        switch (dllevent)
        {
            // ignore library loading and thread level events
            case DLL_PROCESS_ATTACH: 
            case DLL_THREAD_ATTACH:
            case DLL_THREAD_DETACH: break;

            // handle library closure 
            // (MEX DLL is unlinked by "clear all", "clear mexprog", or matlab closure)
            case DLL_PROCESS_DETACH: 
                // Unload Neuroshare DLL here
                if (g_nsDllHandle)
                    ns_CloseLibrary(g_nsDllHandle);
                break;
        }

        // DllMain's return value is only processed for DLL_PROCESS_ATTACH.  Since this library
        // does not do any dynamic allocation that may cause the load to fail, always return TRUE.
        return TRUE;
    }

#else

#ifndef __APPLE__
    int _fini(void);
#endif

    int __attribute__ ((destructor)) mexprog_fini (void)
    {
        if (g_nsDllHandle)
            ns_CloseLibrary(g_nsDllHandle);

#ifndef __APPLE__
        // Chain the stdlib _fini to make sure any necessary
        // cleanup is completed properly.
        _fini();
#endif __APPLE__

        return 0;
    }

#endif

////////////////////////////////////////////////////////////////////////////
//
// Specific Neuroshare functions
//
//      Each specific function calls the appropriate function in the 
//      Neuroshare DLL with the input arguments it was given. The returned
//      data are then converted into MATLAB mxArrays and passed back to the
//      main function.
//
////////////////////////////////////////////////////////////////////////////

// Author & Date: Almut Branner, 2/4/2003
// Purpose: Load Neuroshare DLL and open the data file
// Inputs:  szFile - name of the file to be opened
//          ppmxFilehandle - double pointer to the handle/ID number of the 
//                           file that was opened
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxFilehandle is filled

// TODO: This function will eventually select a DLL based on the file extension
//       and load that DLL. At this point only NEV files are supported.
ns_RESULT fOpenFile(char *szFile, mxArray **ppmxFilehandle)
{
    UINT32 hFile;
    ns_RESULT nsresult;

    nsresult = ns_OpenFile(g_nsDllHandle, szFile, &hFile);

    if (0 != nsresult) 
    {
        *ppmxFilehandle = mxCreateString("");
        mexPrintf("NEV file could not be loaded!\n");
    }
    else 
    {
        *ppmxFilehandle = mxCreateScalarDouble(hFile);
    }

    return(nsresult);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// Author & Date: Almut Branner, 2/3/2003
// Purpose: Get library information and convert it into Matlab format
// Inputs:  ppmxInfo - double pointer to the mex converted ns_LIBRARYINFO structure
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxInfo is filled
ns_RESULT fLibInfo(mxArray **ppmxInfo)
{
    // Arrays & enumerations with structure element names are created
    enum
    {
        LI_LIB_VERSION_MAJ,
        LI_LIB_VERSION_MIN,
        LI_API_VERSION_MAJ, 
        LI_API_VERSION_MIN, 
        LI_DESCRIPTION,
        LI_CREATOR, 
        LI_TIME_YEAR,
        LI_TIME_MONTH,
        LI_TIME_DAY,
        LI_FLAGS,
        LI_MAX_FILES, 
        LI_FILE_DESC_COUNT,
        LI_FILE_DESC,
        LI_COUNT
    };
    
    const char *aszLibInfoNames[LI_COUNT] = 
    {
        "LibVersionMaj",
        "LibVersionMin",
        "APIVersionMaj", 
        "APIVersionMin", 
        "Description",
        "Creator", 
        "Time_Year",
        "Time_Month",
        "Time_Day",
        "Flags",
        "MaxFiles", 
        "FileDescCount",
        "FileDesc"
    };

    enum
    {
        FD_DESCRIPTION,
        FD_EXTENSION,
        FD_MACCODES,
        FD_MAGICCODE,
        FD_COUNT
    };
    const char *aszFileDescNames[FD_COUNT] = 
    {
        "Description",
        "Extension",
        "MacCodes",
        "MagicCode"
    };


    // Intermediate mxArrays to hold the structures
    mxArray *mxOutput;
    mxArray *mxFileDesc;
    mxArray *mxFieldValue;
    BOOL bPrev = FALSE;
    char szTemp[80];
    ns_LIBRARYINFO nsLibInfo;
    ns_RESULT nsresult;
    unsigned int i;

    nsresult = ns_GetLibraryInfo(g_nsDllHandle, &nsLibInfo, (UINT32)sizeof(nsLibInfo));

    if (0 == nsresult)
    {
        // A structure array to hold the libray information is created
        mxOutput = mxCreateStructMatrix(1, 1, 13, aszLibInfoNames);

        // The library information is converted into Matlab mxArrays
        // and the fields of the structure array are filled
        mxFieldValue = mxCreateScalarDouble(nsLibInfo.dwLibVersionMaj);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_LIB_VERSION_MAJ], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsLibInfo.dwLibVersionMin);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_LIB_VERSION_MIN], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsLibInfo.dwAPIVersionMaj);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_API_VERSION_MAJ], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsLibInfo.dwAPIVersionMin);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_API_VERSION_MIN], mxFieldValue);

        mxFieldValue = mxCreateString(nsLibInfo.szDescription);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_DESCRIPTION], mxFieldValue);

        mxFieldValue = mxCreateString(nsLibInfo.szCreator);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_CREATOR], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsLibInfo.dwTime_Year);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_TIME_YEAR], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsLibInfo.dwTime_Month);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_TIME_MONTH], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsLibInfo.dwTime_Day);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_TIME_DAY], mxFieldValue);

        if (nsLibInfo.dwFlags & ns_LIBRARY_DEBUG)
        {
            strcpy(szTemp, "DEBUG ");
            bPrev = TRUE;
        }
        if (nsLibInfo.dwFlags & ns_LIBRARY_MODIFIED)
        {
            if (bPrev == FALSE) 
            {
                strcpy(szTemp, "MODIFIED ");
                bPrev = TRUE;
            }
            else
                strcat(szTemp, "+ MODIFIED ");
        }
        if (nsLibInfo.dwFlags & ns_LIBRARY_PRERELEASE)
        {
            if (bPrev == FALSE) 
            {
                strcpy(szTemp, "PRERELEASE ");
                bPrev = TRUE;
            }
            else
                strcat(szTemp, "+ PRERELEASE ");
        }
        if (nsLibInfo.dwFlags & ns_LIBRARY_SPECIALBUILD)
        {
            if (bPrev == FALSE) 
            {
                strcpy(szTemp, "SPECIALBUILD ");
                bPrev = TRUE;
            }
            else
                strcat(szTemp, "+ SPECIALBUILD ");
        }
        if (nsLibInfo.dwFlags & ns_LIBRARY_MULTITHREADED)
        {
            if (bPrev == FALSE)
                strcpy(szTemp, "MULTITHREADED");
            else
                strcat(szTemp, "+ MULTITHREADED");
        }
        if (bPrev == FALSE)
            strcpy(szTemp, "NONE");
            

        mxFieldValue = mxCreateString(szTemp);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_FLAGS], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsLibInfo.dwMaxFiles);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_MAX_FILES], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsLibInfo.dwFileDescCount);
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_FILE_DESC_COUNT], mxFieldValue);

        
        // Build the other structure

        mxFileDesc = mxCreateStructMatrix(nsLibInfo.dwFileDescCount, 1, 4, aszFileDescNames);

        for (i = 0; i < nsLibInfo.dwFileDescCount; ++i)
        {
            mxFieldValue = mxCreateString(nsLibInfo.FileDesc[i].szDescription);
            mxSetField(mxFileDesc, i, aszFileDescNames[FD_DESCRIPTION], mxFieldValue);
            
            mxFieldValue = mxCreateString(nsLibInfo.FileDesc[i].szExtension);
            mxSetField(mxFileDesc, i, aszFileDescNames[FD_EXTENSION], mxFieldValue);
            
            mxFieldValue = mxCreateString(nsLibInfo.FileDesc[i].szMacCodes);
            mxSetField(mxFileDesc, i, aszFileDescNames[FD_MACCODES], mxFieldValue);
            
            mxFieldValue = mxCreateString(nsLibInfo.FileDesc[i].szMagicCode);
            mxSetField(mxFileDesc, i, aszFileDescNames[FD_MAGICCODE], mxFieldValue);
        }

        // Now put the structure into the other structure
        mxSetField(mxOutput, 0, aszLibInfoNames[LI_FILE_DESC], mxFileDesc);


        // Assign the structure array to the output argument
        *ppmxInfo = mxOutput;
    }
    else
    {
        mexPrintf("There was an error running ns_GetLibraryInfo!\n");
        *ppmxInfo = mxCreateString("");
    }
    return(nsresult);
}

// Author & Date: Almut Branner, 2/6/2003
// Purpose: Get file information and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ppmxInfo - double pointer to the mex converted ns_FILEINFO structure
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxInfo is filled.
ns_RESULT fFileInfo(UINT32 hFile, mxArray **ppmxInfo)
{
    const char *aszFileInfoNames[] = {"FileType","EntityCount","TimeStampResolution",
                                "TimeSpan","AppName","Time_Year","Time_Month","Time_Day",
                                "Time_Hour","Time_Min","Time_Sec","Time_MilliSec",
                                "FileComment"};
    mxArray *mxOutput;
    mxArray *mxFieldValue;
    ns_FILEINFO nsFileInfo;
    ns_RESULT nsresult;

    nsresult = ns_GetFileInfo(g_nsDllHandle, hFile, &nsFileInfo, (UINT32)sizeof(nsFileInfo));

    if (0 == nsresult)
    {
        mxOutput = mxCreateStructMatrix(1, 1, 13, aszFileInfoNames);

        mxFieldValue = mxCreateString(nsFileInfo.szFileType);
        mxSetField(mxOutput, 0, aszFileInfoNames[0], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsFileInfo.dwEntityCount);
        mxSetField(mxOutput, 0, aszFileInfoNames[1], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsFileInfo.dTimeStampResolution);
        mxSetField(mxOutput, 0, aszFileInfoNames[2], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsFileInfo.dTimeSpan);
        mxSetField(mxOutput, 0, aszFileInfoNames[3], mxFieldValue);

        mxFieldValue = mxCreateString(nsFileInfo.szAppName);
        mxSetField(mxOutput, 0, aszFileInfoNames[4], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsFileInfo.dwTime_Year);
        mxSetField(mxOutput, 0, aszFileInfoNames[5], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsFileInfo.dwTime_Month);
        mxSetField(mxOutput, 0, aszFileInfoNames[6], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsFileInfo.dwTime_Day);
        mxSetField(mxOutput, 0, aszFileInfoNames[7], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsFileInfo.dwTime_Hour);
        mxSetField(mxOutput, 0, aszFileInfoNames[8], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsFileInfo.dwTime_Min);
        mxSetField(mxOutput, 0, aszFileInfoNames[9], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsFileInfo.dwTime_Sec);
        mxSetField(mxOutput, 0, aszFileInfoNames[10], mxFieldValue);

        mxFieldValue = mxCreateScalarDouble(nsFileInfo.dwTime_MilliSec);
        mxSetField(mxOutput, 0, aszFileInfoNames[11], mxFieldValue);

        mxFieldValue = mxCreateString(nsFileInfo.szFileComment);
        mxSetField(mxOutput, 0, aszFileInfoNames[12], mxFieldValue);

        *ppmxInfo = mxOutput;
    }
    else
    {
        mexPrintf("There was an error running ns_GetFileInfo!\n");
        *ppmxInfo = mxCreateString("");
    }

    return(nsresult);
}

// Author & Date: Almut Branner, 2/6/2003
// Purpose: Get entity information and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ncols - number of elements in the array of entities (pdEntityID)
//          pdEntityID - pointer to the array of entities to get info for
//          ppmxInfo - double pointer to the mex converted ns_ENTITYINFO structure
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxInfo is filled.
ns_RESULT fEntityInfo(UINT32 hFile, size_t ncols, double *pdEntityID, mxArray **ppmxInfo)
{
    const char *aszEntityNames[] = {"EntityLabel","EntityType","ItemCount"};
    UINT32 i;
    mxArray *mxOutput = 0;
    mxArray *mxFieldValue = 0;
    ns_ENTITYINFO nsEntityInfo;
    ns_RESULT nsresult = 0;
    BOOL bEntity = TRUE;

    for (i = 0; i < ncols; ++i)
    {
        nsresult = ns_GetEntityInfo(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], &nsEntityInfo, 
                                    (UINT32) sizeof(nsEntityInfo));
        if (0 == i)
            mxOutput = mxCreateStructMatrix(ncols, 1, 3, aszEntityNames);

        if (0 == nsresult)
        {
            mxFieldValue = mxCreateString(nsEntityInfo.szEntityLabel);
            mxSetField(mxOutput, i, aszEntityNames[0], mxFieldValue);

            switch (nsEntityInfo.dwEntityType)
            {
            case 0:     // ns_ENTITY_UNKNOWN
                mxFieldValue = mxCreateScalarDouble(0);
                break;
            case 1:     // ns_ENTITY_EVENT
                mxFieldValue = mxCreateScalarDouble(1);
                break;
            case 2:     // ns_ENTITY_ANALOG
                mxFieldValue = mxCreateScalarDouble(2);
                break;
            case 3:     // ns_ENTITY_SEGMENT
                mxFieldValue = mxCreateScalarDouble(3);
                break;
            case 4:     // ns_ENTITY_NEURALEVENT
                mxFieldValue = mxCreateScalarDouble(4);
                break;
            }
            mxSetField(mxOutput, i, aszEntityNames[1], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsEntityInfo.dwItemCount);
            mxSetField(mxOutput, i, aszEntityNames[2], mxFieldValue);
        }
        else if (-5 == nsresult)
        {
            if (TRUE == bEntity)
                mexPrintf("Some entities do not exist (ns_GetEntityInfo).\n");
            bEntity = FALSE;
            nsresult = 0;
        }
        else
        {
            // If any element fails, the function returns an error and an empty structure
            mexPrintf("There was an error running ns_GetEntityInfo!\n");
            *ppmxInfo = mxCreateString("");
            break;
        }
    }
    if (0 == nsresult)
        *ppmxInfo = mxOutput;
    return(nsresult);
}

// Author & Date: Almut Branner, 2/6/2003
// Purpose: Get event information and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ncols - number of elements in the array of entities (pdEntityID)
//          pdEntityID - pointer to the array of entities to get info for
//          ppmxInfo - double pointer to the mex converted ns_EVENTINFO structure
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxInfo is filled.
ns_RESULT fEventInfo(UINT32 hFile, size_t ncols, double *pdEntityID, mxArray **ppmxInfo)
{
    const char *aszEventNames[] = {"EventType","MinDataLength","MaxDataLength",
                                    "CSVDesc"};
    UINT32 i;
    mxArray *mxOutput = 0;
    mxArray *mxFieldValue = 0;
    ns_EVENTINFO nsEventInfo;
    ns_RESULT nsresult = 0;
    BOOL bEntity = TRUE;

    for (i = 0; i < ncols; ++i)
    {
        nsresult = ns_GetEventInfo(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], &nsEventInfo, 
                                   (UINT32) sizeof(nsEventInfo));
        if (0 == i)
            mxOutput = mxCreateStructMatrix(ncols, 1, 4, aszEventNames);

        if (0 == nsresult)
        {
            switch (nsEventInfo.dwEventType)
            {
            case 0:
                mxFieldValue = mxCreateString("ns_EVENT_TEXT");
                break;
            case 1:
                mxFieldValue = mxCreateString("ns_EVENT_CSV");
                break;
            case 2:
                mxFieldValue = mxCreateString("ns_EVENT_BYTE");
                break;
            case 3:
                mxFieldValue = mxCreateString("ns_EVENT_WORD");
                break;
            case 4:
                mxFieldValue = mxCreateString("ns_EVENT_DWORD");
                break;
            }
            mxSetField(mxOutput, i, aszEventNames[0], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsEventInfo.dwMinDataLength);
            mxSetField(mxOutput, i, aszEventNames[1], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsEventInfo.dwMaxDataLength);
            mxSetField(mxOutput, i, aszEventNames[2], mxFieldValue);

            mxFieldValue = mxCreateString(nsEventInfo.szCSVDesc);
            mxSetField(mxOutput, i, aszEventNames[3], mxFieldValue);
        }
        else if (-5 == nsresult)
        {
            if (TRUE == bEntity)
                mexPrintf("Some entities do not exist (ns_GetEventInfo).\n");
            bEntity = FALSE;
            nsresult = 0;
        }
        else
        {
            mexPrintf("There was an error running ns_GetEventInfo!\n");
            *ppmxInfo = mxCreateString("");
        }
    }
    if (0 == nsresult)
        *ppmxInfo = mxOutput;
    return(nsresult);
}

// Author & Date: Almut Branner, 2/6/2003
// Purpose: Get analog information and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ncols - number of elements in the array of entities (pdEntityID)
//          pdEntityID - pointer to the array of entities to get info for
//          ppmxInfo - double pointer to the mex converted ns_ANALOGINFO structure
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxInfo is filled.
ns_RESULT fAnalogInfo(UINT32 hFile, size_t ncols, double *pdEntityID, mxArray **ppmxInfo)
{
    const char *aszAnalogNames[] = {"SampleRate","MinVal","MaxVal","Units","Resolution",
                                "LocationX","LocationY","LocationZ","LocationUser",
                                "HighFreqCorner","HighFreqOrder","HighFilterType",
                                "LowFreqCorner","LowFreqOrder","LowFilterType","ProbeInfo"};
    UINT32 i;
    mxArray *mxOutput = 0;
    mxArray *mxFieldValue = 0;
    ns_ANALOGINFO nsAnalogInfo;
    ns_RESULT nsresult = 0;
    BOOL bEntity = TRUE;

    for (i = 0; i < ncols; ++i)
    {
        nsresult = ns_GetAnalogInfo(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], &nsAnalogInfo, 
                                    (UINT32) sizeof(nsAnalogInfo));
        if (0 == i)
            mxOutput = mxCreateStructMatrix(ncols, 1, 16, aszAnalogNames);

        if (nsresult == 0)
        {
            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dSampleRate);
            mxSetField(mxOutput, i, aszAnalogNames[0], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dMinVal);
            mxSetField(mxOutput, i, aszAnalogNames[1], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dMaxVal);
            mxSetField(mxOutput, i, aszAnalogNames[2], mxFieldValue);

            mxFieldValue = mxCreateString(nsAnalogInfo.szUnits);
            mxSetField(mxOutput, i, aszAnalogNames[3], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dResolution);
            mxSetField(mxOutput, i, aszAnalogNames[4], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dLocationX);
            mxSetField(mxOutput, i, aszAnalogNames[5], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dLocationY);
            mxSetField(mxOutput, i, aszAnalogNames[6], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dLocationZ);
            mxSetField(mxOutput, i, aszAnalogNames[7], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dLocationUser);
            mxSetField(mxOutput, i, aszAnalogNames[8], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dHighFreqCorner);
            mxSetField(mxOutput, i, aszAnalogNames[9], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dwHighFreqOrder);
            mxSetField(mxOutput, i, aszAnalogNames[10], mxFieldValue);

            mxFieldValue = mxCreateString(nsAnalogInfo.szHighFilterType);
            mxSetField(mxOutput, i, aszAnalogNames[11], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dLowFreqCorner);
            mxSetField(mxOutput, i, aszAnalogNames[12], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsAnalogInfo.dwLowFreqOrder);
            mxSetField(mxOutput, i, aszAnalogNames[13], mxFieldValue);

            mxFieldValue = mxCreateString(nsAnalogInfo.szLowFilterType);
            mxSetField(mxOutput, i, aszAnalogNames[14], mxFieldValue);

            mxFieldValue = mxCreateString(nsAnalogInfo.szProbeInfo);
            mxSetField(mxOutput, i, aszAnalogNames[15], mxFieldValue);
        }
        else if (-5 == nsresult)
        {
            if (TRUE == bEntity)
                mexPrintf("Some entities do not exist (ns_GetAnalogInfo).\n");
            bEntity = FALSE;
            nsresult = 0;
        }
        else
        {
            mexPrintf("There was an error running ns_GetAnalogInfo!\n");
            *ppmxInfo = mxCreateString("");
        }
    }
    if (0 == nsresult)
        *ppmxInfo = mxOutput;
    return(nsresult);
}

// Author & Date: Almut Branner, 2/7/2003
// Purpose: Get segment information and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ncols - number of elements in the array of entities (pdEntityID)
//          pdEntityID - pointer to the array of entities to get info for
//          ppmxInfo - double pointer to the mex converted ns_SEGMENTINFO structure
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxInfo is filled.
ns_RESULT fSegmentInfo(UINT32 hFile, size_t ncols, double *pdEntityID, mxArray **ppmxInfo)
{
    const char *aszSegmentNames[] = {"SourceCount","MinSampleCount","MaxSampleCount",
                                     "SampleRate"};
    UINT32 i;
    mxArray *mxOutput = 0;
    mxArray *mxFieldValue = 0;
    ns_SEGMENTINFO nsSegmentInfo;
    ns_RESULT nsresult =0;
    BOOL bEntity = TRUE;

    for (i = 0; i < ncols; ++i)
    {
        nsresult = ns_GetSegmentInfo(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], &nsSegmentInfo, 
                                     (UINT32) sizeof(nsSegmentInfo));
        if (0 == i)
            mxOutput = mxCreateStructMatrix(ncols, 1, 4, aszSegmentNames);

        if (nsresult == 0)
        {
            mxFieldValue = mxCreateScalarDouble(nsSegmentInfo.dwSourceCount);
            mxSetField(mxOutput, i, aszSegmentNames[0], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsSegmentInfo.dwMinSampleCount);
            mxSetField(mxOutput, i, aszSegmentNames[1], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsSegmentInfo.dwMaxSampleCount);
            mxSetField(mxOutput, i, aszSegmentNames[2], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsSegmentInfo.dSampleRate);
            mxSetField(mxOutput, i, aszSegmentNames[3], mxFieldValue);
        }
        else if (-5 == nsresult)
        {
            if (TRUE == bEntity)
                mexPrintf("Some entities do not exist (ns_GetSegmentInfo).\n");
            bEntity = FALSE;
            nsresult = 0;
        }
        else
        {
            mexPrintf("There was an error running ns_GetSegmentInfo!\n");
            *ppmxInfo = mxCreateString("");
            break;
        }
    }
    if (0 == nsresult)
        *ppmxInfo = mxOutput;
    return(nsresult);
}

// Author & Date: Almut Branner, 2/6/2003
// Purpose: Get segment source information and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ncolsEntity - number of elements in the array of entities (pdEntityID)
//          pdEntityID - pointer to the array of entities to get info for
//          ncolsSource - number of elements in the array of sources (pdSourceID)
//          pdSourceID - pointer to the array of sources to get info for
//          ppmxInfo - double pointer to the mex converted ns_SEGMENTSOURCEINFO structure
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxInfo is filled.
ns_RESULT fSegmentSourceInfo(UINT32 hFile, size_t ncolsEntity, double *pdEntityID, 
                             size_t ncolsSource, double *pdSourceID, mxArray **ppmxInfo)
{
    const char *aszSegSourceNames[] = {"MinVal","MaxVal","Resolution","SubSampleShift",
                                "LocationX","LocationY","LocationZ","LocationUser",
                                "HighFreqCorner","HighFreqOrder","HighFilterType",
                                "LowFreqCorner","LowFreqOrder","LowFilterType","ProbeInfo"};
    UINT32 i;
    UINT32 j;
    mxArray *mxOutput = 0;
    mxArray *mxFieldValue = 0;
    ns_SEGSOURCEINFO nsSegmentSourceInfo;
    ns_RESULT nsresult = 0;
    BOOL bEntity = TRUE;
    BOOL bSource = TRUE;

    for (i = 0; i < ncolsEntity; ++i)
    {
        for (j = 0; j < ncolsSource; ++j)
        {
            nsresult = ns_GetSegmentSourceInfo(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], (UINT32) pdSourceID[j],
                                       &nsSegmentSourceInfo, (UINT32) sizeof(nsSegmentSourceInfo));
            if ((0 == i) && (0 == j))
                mxOutput = mxCreateStructMatrix(ncolsSource, ncolsEntity, 15, aszSegSourceNames);

            if (0 == nsresult)
            {
                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dMinVal);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[0], mxFieldValue);

                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dMaxVal);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[1], mxFieldValue);

                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dResolution);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[2], mxFieldValue);

                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dSubSampleShift);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[3], mxFieldValue);

                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dLocationX);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[4], mxFieldValue);

                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dLocationY);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[5], mxFieldValue);

                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dLocationZ);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[6], mxFieldValue);

                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dLocationUser);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[7], mxFieldValue);

                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dHighFreqCorner);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[8], mxFieldValue);

                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dwHighFreqOrder);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[9], mxFieldValue);

                mxFieldValue = mxCreateString(nsSegmentSourceInfo.szHighFilterType);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[10], mxFieldValue);

                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dLowFreqCorner);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[11], mxFieldValue);

                mxFieldValue = mxCreateScalarDouble(nsSegmentSourceInfo.dwLowFreqOrder);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[12], mxFieldValue);

                mxFieldValue = mxCreateString(nsSegmentSourceInfo.szLowFilterType);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[13], mxFieldValue);

                mxFieldValue = mxCreateString(nsSegmentSourceInfo.szProbeInfo);
                mxSetField(mxOutput, (i * ncolsSource) + j, aszSegSourceNames[14], mxFieldValue);
            }
            else if (-5 == nsresult)
            {
                if (TRUE == bEntity)
                    mexPrintf("Some entities do not exist (ns_GetSegmentSourceInfo).\n");
                bEntity = FALSE;
            }
            else if (-6 == nsresult)
            {
                if (TRUE == bSource)
                    mexPrintf("Some sources do not exist (ns_GetSegmentSourceInfo).\n");
                bSource = FALSE;
                break;
            }
            else
            {
                mexPrintf("There was an error running ns_GetSegmentSourceInfo!\n");
                *ppmxInfo = mxCreateString("");
                return(nsresult);
            }
        }
    }

    *ppmxInfo = mxOutput;
    return(nsresult);
}

// Author & Date: Almut Branner, 2/7/2003
// Purpose: Get neural information and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ncols - number of elements in the array of entities (pdEntityID)
//          pdEntityID - pointer to the array of entities to get info for
//          ppmxInfo - double pointer to the mex converted ns_NEURALINFO structure
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxInfo is filled.
ns_RESULT fNeuralInfo(UINT32 hFile, size_t ncols, double *pdEntityID, mxArray **ppmxInfo)
{
    const char *aszNeuralNames[] = {"SourceEntityID","SourceUnitID","ProbeInfo"};
    UINT32 i;
    mxArray *mxOutput = 0;
    mxArray *mxFieldValue = 0;
    ns_NEURALINFO nsNeuralInfo;
    ns_RESULT nsresult = 0;
    BOOL bEntity = TRUE;

    for (i = 0; i < ncols; ++i)
    {
        nsresult = ns_GetNeuralInfo(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], &nsNeuralInfo, 
                                    (UINT32) sizeof(nsNeuralInfo));
        if (0 == i)
            mxOutput = mxCreateStructMatrix(ncols, 1, 3, aszNeuralNames);

        if (nsresult == 0)
        {
            mxFieldValue = mxCreateScalarDouble(nsNeuralInfo.dwSourceEntityID);
            mxSetField(mxOutput, i, aszNeuralNames[0], mxFieldValue);

            mxFieldValue = mxCreateScalarDouble(nsNeuralInfo.dwSourceUnitID);
            mxSetField(mxOutput, i, aszNeuralNames[1], mxFieldValue);

            mxFieldValue = mxCreateString(nsNeuralInfo.szProbeInfo);
            mxSetField(mxOutput, i, aszNeuralNames[2], mxFieldValue);
        }
        else if (-5 == nsresult)
        {
            if (TRUE == bEntity)
                mexPrintf("Some entities do not exist (ns_GetNeuralInfo).\n");
            bEntity = FALSE;
        }
        else
        {
            mexPrintf("There was an error running ns_GetNeuralInfo!\n");
            *ppmxInfo = mxCreateString("");
        }
    }

    if (0 == nsresult)
        *ppmxInfo = mxOutput;
    return(nsresult);
}

// Author & Date: Almut Branner, 2/21/2003
// Purpose: Get event data and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ncolsEntity - number of elements in the array of entities (pdEntityID)
//          pdEntityID - pointer to the array of entities to get info for
//          ncolsIndex - number of elements in the array of indeces (pdIndex)
//          pdIndex - pointer to the array of indeces to get info for
//          ppmxTimeStamp - double pointer to the mex converted timestamp
//          ppmxData - double pointer to the mex converted data structure
//          ppmxDataSize - how big is the data structure that was returned
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxTimeStamp is filled.
//          ppmxData is filled.
//          ppmxDataSize is filled.
ns_RESULT fEventData(UINT32 hFile, size_t ncolsEntity, double *pdEntityID, size_t ncolsIndex, 
                     double *pdIndex, mxArray **ppmxTimeStamp, mxArray **ppmxData, 
                     mxArray **ppmxDataSize)
{
    UINT32 i;
    UINT32 j;
    UINT32 dwTempType;
    double dTimeStamp;
    void *pvData;
    double *pdOutputTimeStamp = 0;
    double *pdOutputData = 0;
    double *pdOutputDataSize = 0;
    UINT32 dwDataSize;
    ns_EVENTINFO nsEventInfo;
    ns_RESULT nsresult;
    BOOL bIndex = TRUE;
    
    // Compare the types of all requested entities. Error when not the same.
    // Also checks whether loading the information works for all entities.
    nsresult = ns_GetEventInfo(g_nsDllHandle, hFile, (UINT32) pdEntityID[0], &nsEventInfo, sizeof(nsEventInfo));
    if (0 == nsresult)
    {
        dwTempType = nsEventInfo.dwEventType;
        // Allocate mxArray for the data depending on the type
        if ((ns_EVENT_BYTE == dwTempType) || (ns_EVENT_WORD == dwTempType) ||
            (ns_EVENT_DWORD == dwTempType))
        {
            *ppmxTimeStamp = mxCreateDoubleMatrix(ncolsIndex, ncolsEntity, mxREAL);
            *ppmxData = mxCreateDoubleMatrix(ncolsIndex, ncolsEntity, mxREAL);
            *ppmxDataSize = mxCreateDoubleMatrix(ncolsIndex, ncolsEntity, mxREAL);
            pdOutputTimeStamp = mxGetPr(*ppmxTimeStamp);
            pdOutputData = mxGetPr(*ppmxData);
            pdOutputDataSize = mxGetPr(*ppmxDataSize);
        }
        else if (ns_EVENT_TEXT == dwTempType)
        {
            *ppmxTimeStamp = mxCreateDoubleMatrix(ncolsIndex, ncolsEntity, mxREAL);
            *ppmxData = mxCreateCellMatrix(ncolsIndex, ncolsEntity);
            *ppmxDataSize = mxCreateDoubleMatrix(ncolsIndex, ncolsEntity, mxREAL);
            pdOutputTimeStamp = mxGetPr(*ppmxTimeStamp);
            pdOutputDataSize = mxGetPr(*ppmxDataSize);
        }
        else if (ns_EVENT_CSV == dwTempType)
        {
            *ppmxTimeStamp = mxCreateDoubleMatrix(ncolsIndex, ncolsEntity, mxREAL);
            *ppmxData = mxCreateCellMatrix(ncolsIndex, ncolsEntity);
            *ppmxDataSize = mxCreateDoubleMatrix(ncolsIndex, ncolsEntity, mxREAL);
            pdOutputTimeStamp = mxGetPr(*ppmxTimeStamp);
            pdOutputDataSize = mxGetPr(*ppmxDataSize);
        }
    }
    else
    {
        mexPrintf("All requested Entities have to be of the same event type\nor there was an error running ns_GetEventInfo!\n(Was required for ns_GetEventData)\n");
        *ppmxTimeStamp = mxCreateString("");
        *ppmxDataSize = mxCreateString("");
        *ppmxData = mxCreateString("");
        return(ns_LIBERROR);
    }

    for (i = 1; i < ncolsEntity; ++i)
    {
        nsresult = ns_GetEventInfo(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], &nsEventInfo, sizeof(nsEventInfo));
        
        if ((0 == nsresult) && (dwTempType == nsEventInfo.dwEventType))
        {
            dwTempType = nsEventInfo.dwEventType;
        }
        else
        {
            mexPrintf("All requested Entities have to be of the same event type\nor there was an error running ns_GetEventInfo!\n(Was required for ns_GetEventData)\n");
            *ppmxTimeStamp = mxCreateString("");
            *ppmxDataSize = mxCreateString("");
            *ppmxData = mxCreateString("");
            return(ns_LIBERROR);
        }
    }

    // Actually load the data
    for (i = 0; i < ncolsEntity; ++i)
    {
        for (j = 0; j < ncolsIndex; ++j)
        {
            // Call GetEventInfo first to get the type of data that is loaded
            nsresult = ns_GetEventInfo(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], &nsEventInfo, 
                                       sizeof(nsEventInfo));
            pvData = malloc(nsEventInfo.dwMaxDataLength);

            nsresult = ns_GetEventData(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], (UINT32) pdIndex[j], 
                                       &dTimeStamp, pvData, nsEventInfo.dwMaxDataLength, 
                                       &dwDataSize);
            if (0 == nsresult)
            {
                switch (nsEventInfo.dwEventType)
                {
                case 0: // ns_EVENT_TEXT
                    mxSetCell(*ppmxData, i * ncolsIndex + j, mxCreateString((char*)pvData));
                    break;
                case 1: // ns_EVENT_CSV
                    mxSetCell(*ppmxData, i * ncolsIndex + j, mxCreateString("Not supported"));
                    break;
                case 2: // ns_EVENT_BYTE
                    *(pdOutputData + (i * ncolsIndex) + j) = *((UINT8*)pvData);
                    break;
                case 3: // ns_EVENT_WORD
                    *(pdOutputData + (i * ncolsIndex) + j) = *((UINT16*)pvData);
                    break;
                case 4: // ns_EVENT_DWORD
                    *(pdOutputData + (i * ncolsIndex) + j) = *((UINT32*)pvData);
                    break;
                }
                *(pdOutputTimeStamp + (i * ncolsIndex) + j) = dTimeStamp;
                *(pdOutputDataSize + (i * ncolsIndex) + j) = dwDataSize;
            }
            else if (-7 == nsresult)
            {
                if (TRUE == bIndex)
                    mexPrintf("Some indeces do not exist (ns_GetEventData).\n");
                bIndex = FALSE;
                break;
            }
            else
            {
                mexPrintf("There was an error running ns_GetEventData!\n");
                *ppmxTimeStamp = mxCreateString("");
                *ppmxDataSize = mxCreateString("");
                *ppmxData = mxCreateString("");
                free(pvData);
                return(nsresult);
            }
            free(pvData);
        }
    }
    return(nsresult);
}

// Author & Date: Almut Branner, 2/7/2003
// Purpose: Get analog data and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ncols - number of elements in the array of entities (pdEntityID)
//          pdEntityID - pointer to the array of entities to get info for
//          dwIndex - index in the particular entity
//          dwIndexCount - How many indeces are loaded
//          ppmxContCount - double pointer to the mex converted number of how
//                          many continuous indeces were loaded
//          ppmxData - double pointer to the mex converted data structure
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxContCount is filled.
//          ppmxData is filled.
ns_RESULT fAnalogData(UINT32 hFile, size_t ncols, double *pdEntityID, UINT32 dwIndex, 
                      UINT32 dwIndexCount, mxArray **ppmxContCount, mxArray **ppmxData)
{
    UINT32 i;
    UINT32 j;
    double *pdData;
    double *pdTempData = 0;
    double *pdTempContCount = 0;
    UINT32 dwContCount;
    ns_RESULT nsresult = 0;
    BOOL bEntity = TRUE;
    BOOL bIndex = TRUE;
    
    pdData = calloc(dwIndexCount, 8);

    for (i = 0; i < ncols; ++i)
    {
        nsresult = ns_GetAnalogData(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], dwIndex, dwIndexCount, 
                                    &dwContCount, pdData);
        if (0 == i)
        {
            *ppmxContCount = mxCreateDoubleMatrix(ncols, 1, mxREAL);
            pdTempContCount = mxGetPr(*ppmxContCount);
            *ppmxData = mxCreateDoubleMatrix(dwIndexCount, ncols, mxREAL);
            pdTempData = mxGetPr(*ppmxData);
        }
        if (0 == nsresult)
        {
            *(pdTempContCount + i) = dwContCount;
            for (j = 0; j < dwIndexCount; ++j)
            {
                *(pdTempData + (i * dwIndexCount) + j) = *(pdData + j);
            }
        }
        else if (-5 == nsresult)
        {
            if (TRUE == bEntity)
                mexPrintf("Some entities do not exist (ns_GetAnalogData).\n");
            bEntity = FALSE;
        }
        else if (-7 == nsresult)
        {
            if (TRUE == bIndex)
                mexPrintf("Some indeces do not exist (ns_GetAnalogData).\n");
            bIndex = FALSE;
        }
        else
        {
            mexPrintf("There was an error running ns_GetAnalogData!\n");
            *ppmxContCount = mxCreateString("");
            *ppmxData = mxCreateString("");
            break;
        }
    }

    free(pdData);
    return(nsresult);
}

// Author & Date: Almut Branner, 2/21/2003
// Purpose: Get segment data and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ncolsEntity - number of elements in the array of entities (pdEntityID)
//          pdEntityID - pointer to the array of entities to get data for
//          ncolsIndex - number of elements in the array of entities (pdEntityID)
//          pdIndex - pointer to the array of indeces to get data for
//          ppmxTimeStamp - double pointer to the mex converted time stamp
//          ppmxData - double pointer to the mex converted data structure
//          ppmxSampleCount - double pointer to the mex converted count of the
//                            samples loaded
//          ppmxUnitID - double pointer to the mex converted unit classification
//                       code
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxTimeStamp is filled.
//          ppmxData is filled.
//          ppmxSampleCount is filled.
//          ppmxUnitID is filled.
ns_RESULT fSegmentData(UINT32 hFile, size_t ncolsEntity, double *pdEntityID, size_t ncolsIndex, 
                       double *pdIndex, mxArray **ppmxTimeStamp, mxArray **ppmxData, 
                       mxArray **ppmxSampleCount, mxArray **ppmxUnitID)
{
    UINT32 i;
    UINT32 j;
    UINT32 k;
    double dTimeStamp;
    double *pdData;
    double *pdTempData;
    double *pdTempTimeStamp;
    double *pdTempSampleCount;
    double *pdTempUnitID;
    UINT32 dwSampleCount;
    UINT32 dwUnitID;
    size_t dwMaxSampleCount = 0;
    ns_SEGMENTINFO nsSegmentInfo;
    ns_RESULT nsresult = 0;
    BOOL bEntity = TRUE;
    BOOL bIndex = TRUE;

    // Determine the maximum data buffer necessary in case segment have different length
    for (i = 0; i < ncolsEntity; ++i)
    {
        nsresult = ns_GetSegmentInfo(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], &nsSegmentInfo, 
                                     sizeof(nsSegmentInfo));
        if (0 == nsresult)
        {
            if (nsSegmentInfo.dwMaxSampleCount > dwMaxSampleCount)
                dwMaxSampleCount = nsSegmentInfo.dwMaxSampleCount;
        }
        else if (-5 == nsresult)
        {
            if (TRUE == bEntity)
                mexPrintf("Some entities do not exist (ns_GetSegmentInfo).\n");
            bEntity = FALSE;
        }
        else
        {
            mexPrintf("There was an error running ns_GetSegmentInfo!\n(Required for ns_GetSegmentData)\n");
            *ppmxTimeStamp = mxCreateString("");
            *ppmxSampleCount = mxCreateString("");
            *ppmxUnitID = mxCreateString("");
            *ppmxData = mxCreateString("");
            return(ns_LIBERROR);
        }
    }

    // Check whether there actually is data or whether the size of it is defined
    if (0 < dwMaxSampleCount)
    {
        const int dims[] = {dwMaxSampleCount, ncolsIndex, ncolsEntity};
        pdData = calloc(dwMaxSampleCount, 8);
        *ppmxData = mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
        *ppmxTimeStamp = mxCreateDoubleMatrix(ncolsIndex, ncolsEntity, mxREAL);
        *ppmxSampleCount = mxCreateDoubleMatrix(ncolsIndex, ncolsEntity, mxREAL);
        *ppmxUnitID = mxCreateDoubleMatrix(ncolsIndex, ncolsEntity, mxREAL);
        pdTempData = mxGetPr(*ppmxData);
        pdTempTimeStamp = mxGetPr(*ppmxTimeStamp);
        pdTempSampleCount = mxGetPr(*ppmxSampleCount);
        pdTempUnitID = mxGetPr(*ppmxUnitID);
    }
    else
    {
        mexPrintf("ns_GetSegmentInfo returned a ZERO sample count for the data!\n(Required for ns_GetSegmentData)\n");
        *ppmxTimeStamp = mxCreateString("");
        *ppmxSampleCount = mxCreateString("");
        *ppmxUnitID = mxCreateString("");
        *ppmxData = mxCreateString("");
        return(ns_LIBERROR);
    }

    // Load the data and fill the matrix
    for (i = 0; i < ncolsEntity; ++i)
    {
        for (j = 0; j < ncolsIndex; ++j)
        {
            nsresult = ns_GetSegmentData(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], (UINT32) pdIndex[j], 
                                         &dTimeStamp, pdData, 8 * (UINT32) dwMaxSampleCount, &dwSampleCount, 
                                         &dwUnitID);
            if (0 == nsresult)
            {
                for (k = 0; k < dwSampleCount; ++k)
                {
                    // totalrow * totalcol * 3rd + col * totalrow + row
                    *(pdTempData + dwSampleCount * ncolsIndex * i + dwSampleCount * j + k)
                         = *(pdData + k);
                }

                *(pdTempTimeStamp + i * ncolsIndex + j) = dTimeStamp;
                *(pdTempSampleCount + i * ncolsIndex + j) = dwSampleCount;
                *(pdTempUnitID + i * ncolsIndex + j) = dwUnitID;
            }
            else if (-5 == nsresult)
            {
                break;
            }
            else if (-7 == nsresult)
            {
                if (TRUE == bIndex)
                    mexPrintf("Some indeces do not exist (ns_GetSegmentData).\n");
                bIndex = FALSE;
                break;
            }
            else
            {
                mexPrintf("There was an error running ns_GetSegmentData!\n");
                *ppmxTimeStamp = mxCreateString("");
                *ppmxSampleCount = mxCreateString("");
                *ppmxUnitID = mxCreateString("");
                *ppmxData = mxCreateString("");
                free(pdData);
                return(nsresult);
            }
        }
    }

    free(pdData);
    return(nsresult);
}

// Author & Date: Almut Branner, 2/7/2003
// Purpose: Get neural data and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ncols - number of elements in the array of entities (pdEntityID)
//          pdEntityID - pointer to the array of entities to get info for
//          dwIndex - index in the particular entity
//          dwIndexCount - how many indeces are loaded
//          ppmxData - double pointer to the mex converted data structure
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxData is filled.
ns_RESULT fNeuralData(UINT32 hFile, size_t ncols, double *pdEntityID, UINT32 dwIndex, 
                      UINT32 dwIndexCount, mxArray **ppmxData)
{
    UINT32 i;
    UINT32 j;
    double *pdData = 0;
    double *pdTempData = 0;
    ns_RESULT nsresult = 0;
    BOOL bEntity = TRUE;
    BOOL bIndex = TRUE;

    pdData = calloc(dwIndexCount, 8);

    for (i = 0; i < ncols; ++i)
    {
        nsresult = ns_GetNeuralData(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], dwIndex, dwIndexCount, pdData);

        if (0 == i)
        {
            *ppmxData = mxCreateDoubleMatrix(dwIndexCount, ncols, mxREAL);
            pdTempData = mxGetPr(*ppmxData);
        }

        if (nsresult == 0)
        {
            for (j = 0; j < dwIndexCount; ++j)
            {
                *(pdTempData + (i * dwIndexCount) + j) = *(pdData + j);
            }
        }
        else if (-5 == nsresult)
        {
            if (TRUE == bEntity)
                mexPrintf("Some entities do not exist (ns_GetNeuralData).\n");
            bEntity = FALSE;
        }
        else if (-7 == nsresult)
        {
            ns_ENTITYINFO nsEntityInfo;

            if (TRUE == bIndex)
                mexPrintf("Some indeces do not exist (ns_GetNeuralData).\n");

            if (ns_GetEntityInfo(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], &nsEntityInfo, 
                sizeof(ns_ENTITYINFO)) == 0)
            {
                if (ns_GetNeuralData(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], dwIndex, 
                    nsEntityInfo.dwItemCount, pdData) == 0)
                {
                    for (j = 0; j < dwIndexCount; ++j)
                    {
                        *(pdTempData + (i * dwIndexCount) + j) = *(pdData + j);
                    }
                }
            }

            bIndex = FALSE;
        }
        else
        {
            mexPrintf("There was an error running ns_GetNeuralData!\n");
            *ppmxData = mxCreateString("");
            break;
        }
    }

    free(pdData);
    return(nsresult);
}

// Author & Date: Almut Branner, 2/7/2003
// Purpose: Find index and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          ncols - number of elements in the array of entities (pdEntityID)
//          pdEntityID - pointer to the array of entities to get info for
//          dTime - time of the data to search for
//          nFlag - flag specifying whether index to be retrieved belongs to data item
//                 occuring before (-1), closest (0) or after (1) specified time
//          ppmxIndex - double pointer to the mex converted index
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxIndex is filled.
ns_RESULT fIndexByTime(UINT32 hFile, size_t ncols, double *pdEntityID, double dTime, INT32 nFlag, 
                       mxArray **ppmxIndex)
{
    UINT32 i;
    UINT32 dwIndex;
    double *pdIndex = 0;
    ns_RESULT nsresult = 0;
    BOOL bEntity = TRUE;

    for (i = 0; i < ncols; ++i)
    {
        nsresult = ns_GetIndexByTime(g_nsDllHandle, hFile, (UINT32) pdEntityID[i], dTime, nFlag, &dwIndex);

        if (0 == i)
        {
            *ppmxIndex = mxCreateDoubleMatrix(ncols, 1, mxREAL);
            pdIndex = mxGetPr(*ppmxIndex);
        }

        if (0 == nsresult)
        {
            *(pdIndex + i) = dwIndex;
        }
        else if (-5 == nsresult)
        {
            if (TRUE == bEntity)
                mexPrintf("Some entities do not exist (ns_GetIndexByTime).\n");
            bEntity = FALSE;
        }
        else 
        {
            mexPrintf("There was an error running ns_GetIndexByTime!\n");
            *ppmxIndex = mxCreateString("");
            break;
        }
    }

    return(nsresult);
}

// Author & Date: Almut Branner, 2/6/2003
// Purpose: Find time and convert it into Matlab format
// Inputs:  hFile - handle/ID number of the file
//          dwEntityID - get info for this entity
//          dwIndexCount - the number of indices we care about
//          pdIndex - array of indices we want to know about
//          ppmxTime - double pointer to the mex converted time
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxTime is filled.
ns_RESULT fTimeByIndex(UINT32 hFile, UINT32 dwEntityID, UINT32 dwIndexCount, double * pdIndex, mxArray **ppmxTime)
{
    double * adTime = 0;
    ns_RESULT nsresult = ns_BADINDEX;
    UINT32 i;

    // Let's get our space
    *ppmxTime = mxCreateDoubleMatrix(dwIndexCount, 1, mxREAL);
    adTime = mxGetPr(*ppmxTime);

    // Now do them all
    for (i = 0; i < dwIndexCount; ++i)
    {
        UINT32 dwIndex = (UINT32)pdIndex[i];
        
        nsresult = ns_GetTimeByIndex(g_nsDllHandle, hFile, dwEntityID, dwIndex, &adTime[i]);
        if (nsresult != ns_OK)
        {
            mexPrintf("There was an error running ns_GetTimeByIndex!\n");
            *ppmxTime = mxCreateString("");
            return nsresult;
        }
    }
    return nsresult;
}


// Author & Date: Almut Branner, 2/21/2003
// Purpose: Get the extended error message to last error
// Inputs:  ppmxErrorMsg - double pointer to the mex converted error message
// Outputs: ns_RESULT - what error was returned by the function (should be 0)
//          ppmxErrorMsg is filled.
ns_RESULT fLastErrorMsg(mxArray **ppmxErrorMsg)
{
    char szMsgBuffer[256];
    ns_RESULT nsresult;

    nsresult = ns_GetLastErrorMsg(g_nsDllHandle, szMsgBuffer, sizeof(szMsgBuffer));

    if (nsresult == 0) 
        *ppmxErrorMsg = mxCreateString(szMsgBuffer);
    else 
    {
        mexPrintf("There was an error running ns_GetTimeByIndex!\n");
        *ppmxErrorMsg = mxCreateString("");
    }

    return(nsresult);
}

// Author & Date:       Kirk Korver     17 Dec 2003
// Purpose: Unload the previous library if it was loaded, then load this DLL
// Inputs:
//  szName - the name of the library to load
// Outputs:
//  ns_OK if life is good; ns_LIBERROR, if the DLL couldn't be loaded
ns_RESULT fSetLibrary(const char * szName)
{
    if (g_nsDllHandle)
        ns_CloseLibrary(g_nsDllHandle);

    g_nsDllHandle = ns_LoadLibrary(szName);

    return g_nsDllHandle ? ns_OK : ns_LIBERROR;
}




////////////////////////////////////////////////////////////////////////////
//
// Error handling functions
//
//      If an error occurs, the output arguments are filled with an empty
//      string, a MATLAB error message is created and the program returns
//      back to MATLAB with a general library error.
//
////////////////////////////////////////////////////////////////////////////

// Author & Date: Almut Branner, 2/6/2003
// Purpose: Check whether the right amount of arguments were given by Matlab
// Inputs:  pplhs - double pointer to the return arguments of the function
//          nRight - number of right hand arguments
//          nLeft - number of left hand arguments
//          nExpRight - number of expected right hand arguments
//          nExpLeft - number of expected left hand arguments
// Outputs: BOOL - is expected number of arguments the same as actual
//          If there is an error the left hand arguments are filled in with blanks
BOOL fCheckNumArguments(mxArray **pplhs, int nRight, int nLeft, int nExpRight, int nExpLeft)
{
    int i;

    // Check for proper number of input and output arguments.
    if ((nRight != nExpRight) || (nLeft != nExpLeft))
    {
        mexPrintf("Check number of input arguments.\n");
        
        *pplhs = mxCreateScalarDouble(ns_LIBERROR);
        for (i = 1; i < nExpLeft; ++i)
        {
            *(pplhs + i) = mxCreateString("");
        }
        return(FALSE);
    }
    return(TRUE);
}


// Author & Date: Almut Branner, 2/6/2003
// Purpose: Check whether the library is loaded already
// Inputs:  pplhs - double pointer to the return of the function
//          nLeft - number of left hand arguments
// Outputs: BOOL - FALSE if no file or library were loaded yet
//          If there is an error the left hand arguments are filled in with blanks
BOOL fCheckLoad(mxArray **pplhs, int nLeft)
{
    int i;

    // Check whether the DLL was loaded.
    if (!g_nsDllHandle)
    {
        mexPrintf("Call ns_SetLibrary first! Process interrupted!\n");
        
        *pplhs = mxCreateScalarDouble(ns_LIBERROR);
        for (i = 1; i < nLeft; ++i)
        {
            *(pplhs + i) = mxCreateString("");
        }
        return(FALSE);
    }
    return(TRUE);
}

// Author & Date: Almut Branner, 2/6/2003
// Purpose: Main/entry function for the mex DLL
//      The main function receives input and output arguments from the
//      the MATLAB m-files for each Neuroshare function. An additional
//      number identifies which Neuroshare function is called.
//
//      The number of input and output arguments and the format of the
//      input arguments are checked based on which function was called.
//      This function then calls a separate function for each Neuroshare
//      function. Input arguments and mxArray output pointers are passed
//      to these functions and filled there.
//
//      After returning from the specific functions, the main function then
//      converts the result (ns_RESULT) into an mxArray and returns with all
//      output arguments back to MATLAB.
// Inputs:  nlhs - number of left hand side arguments
//          plhs - pointer to array of left hand side arguments
//          nrhs - number of right hand side arguments
//          prhs - pointer to array of right hand side arguments
// Outputs: plhs is filled.
void mexFunction(
    int nlhs,              // Number of left hand side (output) arguments
    mxArray *plhs[],       // Array of left hand side arguments
    int nrhs,              // Number of right hand side (input) arguments
    const mxArray *prhs[]  // Array of right hand side arguments
)
{
    double dFunc;
    size_t mrows, ncols;

    // Check for proper number of input arguments.
    if (nrhs < 1)
    {
        mexErrMsgTxt("At least one input required.");
    }
    
    // The first input must be a noncomplex scalar double.
    mrows = mxGetM(prhs[0]);
    ncols = mxGetN(prhs[0]);
    if (!mxIsDouble(prhs[0]) || mxIsComplex(prhs[0]) || !(mrows == 1 && ncols == 1))
    {
        mexErrMsgTxt("Input must be a noncomplex scalar double.");
    }

    // Assign pointers to each input and output.
    dFunc = mxGetScalar(prhs[0]);

    switch ((int) dFunc)
    {
    case 1:     // function ns_OpenFile
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 2, 2)) 
                return;

            // Input must be a string and a row vector.
            if ((mxIsChar(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1))
            {
                mexPrintf("Input must be a string and a vector.\n");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }
    
            // Get the length of the input string and allocate memory for it.
            {
                char *szFile;
                size_t cbBuffer;
                int status;
                ns_RESULT fresult;

                cbBuffer = (mxGetM(prhs[1]) * mxGetN(prhs[1])) + 1;
                szFile = mxCalloc(cbBuffer, sizeof(char));

                // Copy the string data from prhs[0] into a C string szFile.
                status = mxGetString(prhs[1], szFile, cbBuffer);
                if (status != 0) 
                    mexWarnMsgTxt("Not enough space. String is truncated.\n");

                // Check whether a DLL and a data file were loaded.
                if (!fCheckLoad(&plhs[0], nlhs)) 
                    return;
                
                fresult = fOpenFile(szFile, &plhs[1]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 2:     // function ns_GetLibraryInfo
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 1, 2)) 
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs)) 
                return;

            {
                ns_RESULT fresult;

                fresult = fLibInfo(&plhs[1]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 3:     // function ns_GetFileInfo
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 2, 2)) 
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs)) 
                return;

            // Input must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("Input must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);

                fresult = fFileInfo(hFile, &plhs[1]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 4:     // function ns_GetEntityInfo
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 3, 2)) 
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs)) 
                return;

            // hFile input must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("hFile input must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID input must be a scalar or a vector.
            if ((mxIsDouble(prhs[2]) != 1) || !((mxGetM(prhs[2]) == 1) || (mxGetN(prhs[2]) == 1)))
            {
                mexPrintf("EntityID input must be a double scalar or vector.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                double *pdEntityID;
                size_t ncols = 0;
                ns_RESULT fresult;
                
                hFile = (UINT32) mxGetScalar(prhs[1]);
                pdEntityID = mxGetPr(prhs[2]);
                if (mxGetM(prhs[2]) == 1)
                    ncols = (UINT32)mxGetN(prhs[2]);
                if (mxGetN(prhs[2]) == 1)
                    ncols = (UINT32)mxGetM(prhs[2]);

                fresult = fEntityInfo(hFile, ncols, pdEntityID, &plhs[1]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 5:     // function ns_GetEventInfo
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 3, 2)) 
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs))
                return;

            // hFile input must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("hFile input must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID input must be a scalar or a vector.
            if ((mxIsDouble(prhs[2]) != 1) || !((mxGetM(prhs[2]) == 1) || (mxGetN(prhs[2]) == 1)))
            {
                mexPrintf("EntityID input must be a double scalar or row vector.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                double *pdEntityID;
                size_t ncols = 0;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                pdEntityID = mxGetPr(prhs[2]);
                if (mxGetM(prhs[2]) == 1)
                    ncols = mxGetN(prhs[2]);
                if (mxGetN(prhs[2]) == 1)
                    ncols = mxGetM(prhs[2]);

                fresult = fEventInfo(hFile, ncols, pdEntityID, &plhs[1]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 6:     // function ns_GetEventData
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 4, 4)) 
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs)) 
                return;

            // Input arguments must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("hFile input arguments must be a double scalar.\n");
                plhs[3] = mxCreateString("");
                plhs[2] = mxCreateString("");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID and Index input must be a scalar or a row vector.
            if ((mxIsDouble(prhs[2]) != 1) || !((mxGetM(prhs[2]) == 1) || (mxGetN(prhs[2]) == 1)) ||
                (mxIsDouble(prhs[3]) != 1) || !((mxGetM(prhs[3]) == 1) || (mxGetN(prhs[3]) == 1)))
            {
                mexPrintf("EntityID and Index input arguments must be a double scalar or vector.\n");
                plhs[3] = mxCreateString("");
                plhs[2] = mxCreateString("");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                double *pdEntityID;
                double *pdIndex;
                size_t ncolsEntity = 0;
                size_t ncolsIndex = 0;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                pdEntityID = mxGetPr(prhs[2]);
                if (mxGetM(prhs[2]) == 1)
                    ncolsEntity = mxGetN(prhs[2]);
                if (mxGetN(prhs[2]) == 1)
                    ncolsEntity = mxGetM(prhs[2]);
                pdIndex = mxGetPr(prhs[3]);
                if (mxGetM(prhs[3]) == 1)
                    ncolsIndex = mxGetN(prhs[3]);
                if (mxGetN(prhs[3]) == 1)
                    ncolsIndex = mxGetM(prhs[3]);

                fresult = fEventData(hFile, ncolsEntity, pdEntityID, ncolsIndex, pdIndex, &plhs[1], 
                                     &plhs[2], &plhs[3]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 7:     // function ns_GetAnalogInfo
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 3, 2)) 
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs)) 
                return;

            // hFile input must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("hFile input must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID input must be a scalar or a vector.
            if ((mxIsDouble(prhs[2]) != 1) || !((mxGetM(prhs[2]) == 1) || (mxGetN(prhs[2]) == 1)))
            {
                mexPrintf("EntityID input must be a double scalar or vector.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                double *pdEntityID;
                size_t ncols = 0;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                pdEntityID = mxGetPr(prhs[2]);
                if (mxGetM(prhs[2]) == 1)
                    ncols = mxGetN(prhs[2]);
                if (mxGetN(prhs[2]) == 1)
                    ncols = mxGetM(prhs[2]);

                fresult = fAnalogInfo(hFile, ncols, pdEntityID, &plhs[1]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 8:     // function ns_GetAnalogData
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 5, 3)) 
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs)) 
                return;

            // Input arguments except EntityID must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1) ||
                (mxIsDouble(prhs[3]) != 1) || (mxGetM(prhs[3]) != 1) || (mxGetN(prhs[3]) != 1) ||
                (mxIsDouble(prhs[4]) != 1) || (mxGetM(prhs[4]) != 1) || (mxGetN(prhs[4]) != 1))
            {
                mexPrintf("Input arguments except EntityID must be a double scalar.\n");
                plhs[2] = mxCreateString("");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID input must be a scalar or a vector.
            if ((mxIsDouble(prhs[2]) != 1) || !((mxGetM(prhs[2]) == 1) || (mxGetN(prhs[2]) == 1)))
            {
                mexPrintf("EntityID input must be a double scalar or vector.\n");
                plhs[2] = mxCreateString("");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                double *pdEntityID;
                size_t ncols = 0;
                UINT32 dwIndex;
                UINT32 dwIndexCount;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                pdEntityID = mxGetPr(prhs[2]);
                if (mxGetM(prhs[2]) == 1)
                    ncols = mxGetN(prhs[2]);
                if (mxGetN(prhs[2]) == 1)
                    ncols = mxGetM(prhs[2]);
                dwIndex = (UINT32) mxGetScalar(prhs[3]);
                dwIndexCount = (UINT32) mxGetScalar(prhs[4]);

                fresult = fAnalogData(hFile, ncols, pdEntityID, dwIndex, dwIndexCount, &plhs[1], 
                                      &plhs[2]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 9:     // function ns_GetSegmentInfo
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 3, 2))
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs))
                return;

            // hFile input arguments must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("hFile input must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID input must be a scalar or a vector.
            if ((mxIsDouble(prhs[2]) != 1) || !((mxGetM(prhs[2]) == 1) || (mxGetN(prhs[2]) == 1)))
            {
                mexPrintf("EntityID input must be a double scalar or vector.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                double *pdEntityID;
                size_t ncols = 0;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                pdEntityID = mxGetPr(prhs[2]);
                pdEntityID = mxGetPr(prhs[2]);
                if (mxGetM(prhs[2]) == 1)
                    ncols = mxGetN(prhs[2]);
                if (mxGetN(prhs[2]) == 1)
                    ncols = mxGetM(prhs[2]);

                fresult = fSegmentInfo(hFile, ncols, pdEntityID, &plhs[1]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 10:    // function ns_GetSegmentSourceInfo
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 4, 2))
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs))
                return;

            // hFile nput arguments must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("hFile input arguments must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID and SourceID input must be a scalar or a vector.
            if ((mxIsDouble(prhs[2]) != 1) || !((mxGetM(prhs[2]) == 1) || (mxGetN(prhs[2]) == 1)) ||
                (mxIsDouble(prhs[3]) != 1) || !((mxGetM(prhs[3]) == 1) || (mxGetN(prhs[3]) == 1)))
            {
                mexPrintf("EntityID and Source ID inputs must be a double scalar or vector.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                double *pdEntityID;
                size_t ncolsEntity = 0;
                double *pdSourceID;
                size_t ncolsSource = 0;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                pdEntityID = mxGetPr(prhs[2]);
                if (mxGetM(prhs[2]) == 1)
                    ncolsEntity = mxGetN(prhs[2]);
                if (mxGetN(prhs[2]) == 1)
                    ncolsEntity = mxGetM(prhs[2]);
                pdSourceID = mxGetPr(prhs[3]);
                if (mxGetM(prhs[3]) == 1)
                    ncolsSource = mxGetN(prhs[3]);
                if (mxGetN(prhs[3]) == 1)
                    ncolsSource = mxGetM(prhs[3]);

                fresult = fSegmentSourceInfo(hFile, ncolsEntity, pdEntityID, ncolsSource,
                                             pdSourceID, &plhs[1]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 11:    // function ns_GetSegmentData
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 4, 5))
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs))
                return;

            // hFile input must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("hFile input must be a double scalar.\n");
                plhs[4] = mxCreateString("");
                plhs[3] = mxCreateString("");
                plhs[2] = mxCreateString("");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID and Index input must be a scalar or a row vector.
            if ((mxIsDouble(prhs[2]) != 1) || !((mxGetM(prhs[2]) == 1) || (mxGetN(prhs[2]) == 1)) ||
                (mxIsDouble(prhs[3]) != 1) || !((mxGetM(prhs[3]) == 1) || (mxGetN(prhs[3]) == 1)))
            {
                mexPrintf("EntityID and Index inputs must be a double scalar or vector.\n");
                plhs[4] = mxCreateString("");
                plhs[3] = mxCreateString("");
                plhs[2] = mxCreateString("");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                double *pdEntityID;
                size_t ncolsEntity = 0;
                double *pdIndex;
                size_t ncolsIndex = 0;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                pdEntityID = mxGetPr(prhs[2]);
                if (mxGetM(prhs[2]) == 1)
                    ncolsEntity = mxGetN(prhs[2]);
                if (mxGetN(prhs[2]) == 1)
                    ncolsEntity = mxGetM(prhs[2]);
                pdIndex = mxGetPr(prhs[3]);
                if (mxGetM(prhs[3]) == 1)
                    ncolsIndex = mxGetN(prhs[3]);
                if (mxGetN(prhs[3]) == 1)
                    ncolsIndex = mxGetM(prhs[3]);

                fresult = fSegmentData(hFile, ncolsEntity, pdEntityID, ncolsIndex, pdIndex, &plhs[1],
                                       &plhs[2], &plhs[3], &plhs[4]);

                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 12:    // function ns_GetNeuralInfo
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 3, 2))
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs))
                return;

            // hFile input arguments must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("hFile input arguments must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID input must be a scalar or a vector.
            if ((mxIsDouble(prhs[2]) != 1) || !((mxGetM(prhs[2]) == 1) || (mxGetN(prhs[2]) == 1)))
            {
                mexPrintf("EntityID input must be a double scalar or vector.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                double *pdEntityID;
                size_t ncols = 0;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                pdEntityID = mxGetPr(prhs[2]);
                if (mxGetM(prhs[2]) == 1)
                    ncols = mxGetN(prhs[2]);
                if (mxGetN(prhs[2]) == 1)
                    ncols = mxGetM(prhs[2]);

                fresult = fNeuralInfo(hFile, ncols, pdEntityID, &plhs[1]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 13:    // function ns_GetNeuralData
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 5, 2))
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs))
                return;

            // Input arguments except EntityID must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1) ||
                (mxIsDouble(prhs[3]) != 1) || (mxGetM(prhs[3]) != 1) || (mxGetN(prhs[3]) != 1) ||
                (mxIsDouble(prhs[4]) != 1) || (mxGetM(prhs[4]) != 1) || (mxGetN(prhs[4]) != 1))
            {
                mexPrintf("Input arguments except EntityID must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID input arguments must be a scalar.
            if ((mxIsDouble(prhs[2]) != 1) || !((mxGetM(prhs[2]) == 1) || (mxGetN(prhs[2]) == 1)))
            {
                mexPrintf("EntityID input must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                double *pdEntityID;
                size_t ncols = 0;
                UINT32 dwIndex;
                UINT32 dwIndexCount;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                pdEntityID = mxGetPr(prhs[2]);
                if (mxGetM(prhs[2]) == 1)
                    ncols = mxGetN(prhs[2]);
                if (mxGetN(prhs[2]) == 1)
                    ncols = mxGetM(prhs[2]);
                dwIndex = (UINT32) mxGetScalar(prhs[3]);
                dwIndexCount = (UINT32) mxGetScalar(prhs[4]);

                fresult = fNeuralData(hFile, ncols, pdEntityID, dwIndex, dwIndexCount, &plhs[1]);

                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 14:    // function ns_CloseFile
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 2, 1))
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs))
                return;

            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("Input arguments must be a double scalar.\n");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                fresult = ns_CloseFile(g_nsDllHandle, hFile);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 15:    // function ns_GetIndexByTime
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 5, 2))
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs))
                return;

            // Input arguments except Entity ID must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1) ||
                (mxIsDouble(prhs[3]) != 1) || (mxGetM(prhs[3]) != 1) || (mxGetN(prhs[3]) != 1) ||
                (mxIsDouble(prhs[4]) != 1) || (mxGetM(prhs[4]) != 1) || (mxGetN(prhs[4]) != 1))
            {
                mexPrintf("Input arguments except EntityID must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID input arguments must be a scalar or vector.
            if ((mxIsDouble(prhs[2]) != 1) || !((mxGetM(prhs[2]) == 1) || (mxGetN(prhs[2]) == 1)))
            {
                mexPrintf("EntityID input must be a double scalar or vector.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                double *pdEntityID;
                size_t ncols = 0;
                double dTime;
                INT32 nFlag;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                pdEntityID = mxGetPr(prhs[2]);
                if (mxGetM(prhs[2]) == 1)
                    ncols = mxGetN(prhs[2]);
                if (mxGetN(prhs[2]) == 1)
                    ncols = mxGetM(prhs[2]);
                dTime = (double) mxGetScalar(prhs[3]);
                nFlag = (INT32) mxGetScalar(prhs[4]);

                fresult = fIndexByTime(hFile, ncols, pdEntityID, dTime, nFlag, &plhs[1]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 16:    // function ns_GetTimeByIndex
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 4, 2))
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs))
                return;

            // hFile input must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("hFile input must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // EntityID input must be a scalar.
            if ((mxIsDouble(prhs[1]) != 1) || (mxGetM(prhs[1]) != 1) || (mxGetN(prhs[1]) != 1))
            {
                mexPrintf("EntityID input must be a double scalar.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            // Index can be a vector or a scalar
            if ((mxIsDouble(prhs[3]) != 1) || !((mxGetM(prhs[3]) == 1) || (mxGetN(prhs[3]) == 1)))
            {
                mexPrintf("Index must be a double scalar or vector.\n");
                plhs[1] = mxCreateString("");
                plhs[0] = mxCreateScalarDouble(ns_LIBERROR);
                return;
            }

            {
                UINT32 hFile;
                UINT32 dwEntityID;
                double * pdIndex = 0;
                size_t dwIndexCount;
                ns_RESULT fresult;

                hFile = (UINT32) mxGetScalar(prhs[1]);
                dwEntityID = (UINT32) mxGetScalar(prhs[2]);

                dwIndexCount = MAX(mxGetM(prhs[3]), mxGetN(prhs[3]));
                pdIndex = mxGetPr(prhs[3]);


                fresult = fTimeByIndex(hFile, dwEntityID, (UINT32) dwIndexCount, pdIndex, &plhs[1]);

                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 17:    // function ns_GetLastErrorMsg
        {
            // Check for proper number of input and output arguments.
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 1, 2))
                return;

            // Check whether a DLL and a data file were loaded.
            if (!fCheckLoad(&plhs[0], nlhs))
                return;

            {
                ns_RESULT fresult;

                fresult = fLastErrorMsg(&plhs[1]);
                plhs[0] = mxCreateScalarDouble(fresult);
            }
        }
        break;
    case 18:    // function ns_SetLibrary
        {
            // Check for proper number of input and output arguments.
            // RHS args:
            //  18 - code to mean ns_SetLibrary
            //  szName - the name of the DLL to load
            if (!fCheckNumArguments(&plhs[0], nrhs, nlhs, 2, 1))
                return;

            {
                char * szFile;
                size_t cbBuffer;
                int status;
                ns_RESULT fresult;

                cbBuffer = (mxGetM(prhs[1]) * mxGetN(prhs[1])) + 1;
                szFile = mxCalloc(cbBuffer, sizeof(char));

                // Copy the string data from rhs into a C string.
                status = mxGetString(prhs[1], szFile, cbBuffer);
                if (status != 0) 
                    mexWarnMsgTxt("Not enough space. String is truncated.\n");

                // Ok...now we've got the name, let's rock'n roll
                fresult = fSetLibrary(szFile);
                plhs[0] = mxCreateScalarDouble(fresult);
                mxFree(szFile);

                if (fresult == ns_LIBERROR)
                {
                    mexWarnMsgTxt("Unable to load this DLL");
                }
            }
        }
        break;
    }
}
