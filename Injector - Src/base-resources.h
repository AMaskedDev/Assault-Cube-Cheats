#pragma once
#include <Windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>

void GetProcId(const char* windowName, DWORD& pID)
{
	GetWindowThreadProcessId(FindWindow(0, windowName), &pID);
}

void LogError(const char* errorTitle, const char* errorMessage)
{
	MessageBox(0, errorMessage, errorTitle, MB_OK | MB_ICONERROR);
	exit(-1);
}

bool FileExists(std::string fileName)
{
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}