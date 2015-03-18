// FatLootInstaller.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string CONFIG_FILE = "Config.txt";
string COPY_DATA_FOLDER = "\\Copy";
string FATLOOT_UDK_INSTALLER = "InstallData\\UDKInstall-FatLoot.exe";
string DEFAULT_INSTALL_LOC = "C:\\UDK\\Fat Loot\\";
string LOG_FILE = "Log\\installLog.txt";
string DLL_COPY_BAT_FILE = "InstallData\\dllCopy.bat";

bool bUseDefaultInstallLoc = false;
string customInstallLoc;

FILE *pLogFile;
char temp[10];
char copyCommand[250];

void initLogFile();
void closeLogFile();
void readConfigFile();
void runSystemCommand(const char *command);
void copyDLLFiles();
void writeToLogFile(const char *line);


int _tmain(int argc, _TCHAR* argv[])
{
	printf("Started Fat Loot installer \n");
	initLogFile();	
	readConfigFile();
	runSystemCommand(FATLOOT_UDK_INSTALLER.c_str());
	copyDLLFiles();
	closeLogFile();
	return 0;
}

void initLogFile()
{
	printf("Trying to create log file \n");
	pLogFile = fopen(LOG_FILE.c_str(), "w");
	if(pLogFile == NULL)
	{
		printf("Unable to create log file! \n");
	}
	else
	{
		printf("Log file opened: %s \n", LOG_FILE.c_str());
		writeToLogFile("Log initialized");
	}
}

void readConfigFile()
{
	writeToLogFile("Trying to read config file");
	ifstream configFile (CONFIG_FILE);
	if (configFile.is_open())
	{
		getline (configFile, customInstallLoc);
		configFile.close();
		writeToLogFile("Install location specified in config:");
		writeToLogFile( customInstallLoc.c_str() );
	}
	else 
	{
		bUseDefaultInstallLoc = true;
		writeToLogFile("Unable to open config file. Assuming default install loc:");
		writeToLogFile(DEFAULT_INSTALL_LOC.c_str());		
	}
}

void runSystemCommand(const char *command)
{
	writeToLogFile("Running command:");
	writeToLogFile(command);
	int status = system(command);	
	if(status == 0)
	{
		writeToLogFile("Ran command successfully");
	}
	else
	{
		writeToLogFile("Error running command. Error code: ");
		memset(temp, 0, 10);
		writeToLogFile(itoa(status, temp, 10));
	}
}

void copyDLLFiles()
{
	FILE *pBatFile;

	writeToLogFile("Trying to create dll copy bat file:");
	writeToLogFile(DLL_COPY_BAT_FILE.c_str());
	pBatFile = fopen(DLL_COPY_BAT_FILE.c_str(), "w");
	if(pBatFile == NULL)
	{
		writeToLogFile("Unable to create bat file! \n");
	}
	else
	{
		writeToLogFile("Bat file created");		
	}

	writeToLogFile("Generating copy command");

	strcpy(copyCommand, "XCOPY \"%%~dp0\\");
	strcat(copyCommand, COPY_DATA_FOLDER.c_str());
	strcat(copyCommand, "\" \"");
	if(bUseDefaultInstallLoc)
	{
		strcat(copyCommand, DEFAULT_INSTALL_LOC.c_str());		
	}
	else
	{
		strcat(copyCommand, customInstallLoc.c_str());
	}	
	strcat(copyCommand, "\" /S /V /C /Y /I");

	fprintf(pBatFile, copyCommand);
	fclose(pBatFile);
	runSystemCommand(DLL_COPY_BAT_FILE.c_str());
}

void writeToLogFile(const char *line)
{
	if(pLogFile != NULL)
	{
		fprintf(pLogFile, "%s \n", line);
	}
}

void closeLogFile()
{
	if(pLogFile != NULL)
	{
		fclose(pLogFile);
	}
}
