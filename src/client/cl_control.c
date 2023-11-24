/*
RTCWPro - req SS functions.
Source: Nate (rtcwMP)
*/

#include "client.h"
#include "../qcommon/http.h"
#include <time.h>

/*
===============
RTCWPro - this is taken from bg,
but it should be somewhere in engine
like qc or qs or some sh*t coz it's just lame to
copy it around from place to place
===============
*/
int CL_CleanName(const char* pszIn, char* pszOut, unsigned int dwMaxLength, qboolean fCRLF) {
	const char* pInCopy = pszIn;
	const char* pszOutStart = pszOut;

	while (*pInCopy && (pszOut - pszOutStart < dwMaxLength - 1))
	{
		if (*pInCopy == '^')
		{
			pInCopy += ((pInCopy[1] == 0) ? 1 : 2);
		}
		else if ((*pInCopy < 32 && (!fCRLF || *pInCopy != '\n')) || (*pInCopy > 126) || (*pInCopy == '/'))
		{
			pInCopy++;
		}
		else
		{
			*pszOut++ = *pInCopy++;
		}
	}

	*pszOut = 0;
	return(pszOut - pszOutStart);
}

/*
================
Takes ScreenShot
================
*/
void CL_TakeSS(char* name) {

	Cbuf_ExecuteText(EXEC_NOW, va("screenshotJPEG reqss %s\n", name));
}

/*
===============
RTCWPro - lazy way
to get path to ss
CL_GetFilePath
===============
*/
char* CL_GetFilePath(char* filename) {
	char* basepath = Cvar_VariableString("fs_basepath");
	char* fs = Cvar_VariableString("fs_game");

	return va("%s/%s/screenshots/%s.jpg", basepath, fs, filename);
}

/*
================
ScreenShot request from server
================
*/
void CL_GenerateSS(char* address, char* hookid, char* hooktoken, char* waittime, char* datetime) {
	char* filename;
	char* filepath;
	char* clientName, cleanName[16];
	char* guid;
	unsigned int n;
	SS_info_t* SS_info = (SS_info_t*)malloc(sizeof(SS_info_t));

	if (clc.demoplaying || cls.state == CA_CINEMATIC || cls.state < CA_ACTIVE)
	{
		return;
	}

	// don't ss if console is down
	if (cls.keyCatchers & KEYCATCH_CONSOLE)
	{
		return;
	}

	clientName = Cvar_VariableString("name");
	CL_CleanName(clientName, cleanName, 16, qfalse);
	guid = Cvar_VariableString("cl_guid");

	srand(time(NULL));
	n = rand() % 99;
	filename = va("%i", n);
	CL_TakeSS(filename);

	filepath = CL_GetFilePath(filename);

	//Com_Printf("CL_GenerateSS:\n address: %s\n hookid: %s\n hooktoken: %s\n waittime: %s\n cleanName: %s\n guid: %s\n filepath: %s\n filename: %s\n",
		//address, hookid, hooktoken, waittime, cleanName, guid, filepath, filename);

	if (SS_info) {
		SS_info->address = address;
		SS_info->hookid = va("ID: %s", hookid);
		SS_info->hooktoken = va("TOK: %s", hooktoken);
		SS_info->waittime = va("WAIT: %s", waittime);
		SS_info->datetime = va("TIME: %s", datetime);
		SS_info->name = va("NAME: %s", cleanName);
		SS_info->guid = va("GUID: %s", guid);
		SS_info->filepath = filepath;
		SS_info->filename = filename;

		Threads_Create(CL_HTTP_SSUpload, SS_info);
	}
}
/*
* RTCWPro cvar add 
*/
void CL_Cvar_Add(char* cvarName, char* cvarAdd) {
	extern cvar_t* cvar_vars;
	cvar_t* var;
	for (var = cvar_vars; var; var = var->next) {
		if (_strcmpi(var->name, cvarName) == 0) {
			if (strchr(cvarAdd, '.')) {
				float value = var->value;
				value += atof(cvarAdd);
				Cvar_SetLatched(var->name, va("%f", value));
			}
			else 
			{
				int value = var->integer;
				value += atoi(cvarAdd);
				Cvar_SetLatched(var->name, va("%d", value));
			}
			break;
		}
	}
}
/*
* RTCWPro cvar multiply
*/
void CL_Cvar_Mult(char* cvarName, char* cvarMult) {
	extern cvar_t* cvar_vars;
	cvar_t* var;
	for (var = cvar_vars; var; var = var->next) {
		if (_strcmpi(var->name, cvarName) == 0) {
			if (strchr(cvarMult, '.')) {
				float value = var->value;
				value *= atof(cvarMult);
				Cvar_SetLatched(var->name, va("%f", value));
			}
			else
			{
				int value = var->integer;
				value *= atoi(cvarMult);
				Cvar_SetLatched(var->name, va("%d", value));
			}
			break;
		}
	}
}

/*
* RTCWPro cvar toggle
*/
void CL_Cvar_Toggle(char* cvarName) {
	extern cvar_t* cvar_vars;
	cvar_t* var;
	for (var = cvar_vars; var; var = var->next) {
		if (_strcmpi(var->name, cvarName) == 0) {
				int value = var->integer;
				value != var->integer;
				Cvar_SetLatched(var->name, va("%d", value));
		}
		break;
		
	}
}