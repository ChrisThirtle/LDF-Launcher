//Wrapper for directory manipulations

#include "direc.h"
#include <string>

#include <iostream>

#ifdef __WIN32__
#include <Windows.h>
#include <stdexcept>
//Do windows API directory controls

Directory::Iterator::Iterator() {
	homedir = getDirPath();
}
Directory::Iterator::~Iterator() {
	home();
}

std::string Directory::Iterator::getNextFilename() {
	WIN32_FIND_DATA* currFile = new WIN32_FIND_DATA();
	std::string outStr;
	do {
		if (pos == INVALID_HANDLE_VALUE) {
			pos = FindFirstFile("*",currFile);
		}
		else {
			if(!FindNextFile(pos,currFile))
				throw (std::out_of_range("End of directory"));
		}
		outStr.assign(currFile->cFileName);
	} while(outStr == "." || outStr == ".."); //Don't return . or .. as filenames
	return outStr;
}

std::string Directory::Iterator::getDirPath() {
	int sz = GetCurrentDirectory(0,NULL);
	char* charbuff = new char[sz];
	std::string outStr;
	
	GetCurrentDirectory(sz, charbuff);
	outStr.assign(charbuff);
	delete charbuff;
	return outStr;
}

void Directory::Iterator::changeDirectory(char* newDir) {
	resetPos();
	std::string dirStr = newDir;
	if(dirStr.data()[dirStr.size()-1] != '\\')
		dirStr.push_back('\\'); //Ensure the new name ends with a backslash
	SetCurrentDirectory(dirStr.c_str());
}

void Directory::Iterator::changeDirectory(std::string newDir) {
	std::string dirStr = newDir;
	if(dirStr.data()[dirStr.size()-1] != '\\')
		dirStr.push_back('\\'); //Ensure the new name ends with a backslash
	SetCurrentDirectory(dirStr.c_str());
}

void Directory::Iterator::resetPos() {
	//Returns to the beginning by reseting pos's value
	pos = INVALID_HANDLE_VALUE;
}

#elif defined __UNIX__
//Do unix API directory controls

Directory::Iterator::Iterator() {
	homedir = getDirPath();
	dp = opendir(".");
}
Directory::Iterator::~Iterator() {
	home();
	closedir(dp);
}

std::string Directory::Iterator::getNextFilename() {
	struct dirent* currFile;
	std::string outStr;
	do {
		if((currFile = readdir(dp)) == NULL)
			throw std::out_of_range("End of directory");
		outStr.assign(currFile.d_name);
	} while(outStr == "." || outStr == ".."); //Don't return . or .. as filenames
	return outStr;
}

std::string Directory::Iterator::getDirPath() {
	std::string outStr;
	outStr.assign(getchwd(NULL,0));
	return outStr;
}

void Directory::Iterator::changeDirectory(char* newDir) {
	closedir(dp);
	chdir(newDir);
	dp = opendir(".");
}

void Directory::Iterator::changeDirectory(std::string newDir) {
	closedir(dp);
	chdir(newDir.c_str());
	dp = opendir(".");
}

void Directory::Iterator::resetPos() {
	//Magically gets back to the first entry in the directory by closing and reopening
	closedir(dp);
	dp = opendir(".");
}

#endif

void Directory::Iterator::home() {
	changeDirectory(homedir);
}