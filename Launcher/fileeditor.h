#ifndef FILEEDIT
#define FILEEDIT

#define LDFTHREADED 0

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <future>
#include <thread>
#include <chrono>

#include "direc/direc.h"
#include "xmlparser.h"
#include "switch.h"
#include "threading.h"

class EditorManager {
	private:
	std::future<bool> endManager;		  //Promise from the GUI, when broken, empty Edits queue, wait for all threads to finish, then exit
	
	static std::vector<std::string> GetFiles();
	static std::vector<FullSwitch> GetFullSwitches(MutexVector<HalfSwitch> *halfsw);
	static void ModifyFiles(std::vector<std::string>& files, std::vector<FullSwitch>& diffs);
	public:
	static int AsyncManager(std::future<bool> f, MutexVector<HalfSwitch> *editsource);
	EditorManager(std::future<bool> f, MutexVector<HalfSwitch> *editsource);
};
#endif