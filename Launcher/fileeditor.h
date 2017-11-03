#ifndef FILEEDIT
#define FILEEDIT

#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <mutex>
#include <future>
#include <queue>
#include <deque>

#include "direc/direc.h"

class EditorManager {
	private:
	std::future endManager;		  //Promise from the GUI, when broken, empty Edits queue, wait for all threads to finish, then exit
	MutexQueue* edits; //List of edits from the GUI process to make
	
	//2 vectors for each subprocess (Editors)
	//1. Futures from the threads; when they return 0, a thread is done with all work and has closed, until then, they are not *ready*
	//2. MutexQueues for each thread; this end writes to them, the threads receive them
	std::vector<MutexQueue> queueVector;
	std::vector<std::future> editorVector;
	
	
	
	//Promise made to all threads; when broken, indicates threads should close after emptying mutex queue.
	std::promise EndThreads;
	
	void addEditorFromDir(std::string relPath);
	void spawnEditors();
	void manageEditors();
	void closeEditors();
	
	public:
	EditorManager(std::future f, MutexQueue* queue);
	int MainLoop(std::future f, MutexQueue* queue);
}


namespace FileEditor {
	public:
	int MainLoop(std::string fname, //Name of the file for this thread to control
				 std::future f,		//Shared future from the manager, used to signal an end for the loop
				 MutexQueue* queue);//Queue used to receive continuous input from the manager.

	int EditFile(std::string fname, //Name of the file for this thread/function to control
				 EditPacket edits);	//List of edits to perform on file
}







class EditPacket {
	private:
	std::queue<EditPacket::Switch> q;
	
	public:
	class Switch {
		private:
		std::regex regstr;
		std::string repstr;
		std::string filePrefix;
		public:
		Switch(std::regex regin, std::string repin, std::string prefin);
		std::regex getRegex();
		std::string getReplacement();
		std::string getPrefix();
	}
	
	void push(EditPacket::Switch swch);
	EditPacket::Switch pop();
	bool isEmpty();
}


class MutexQueue {
	private:
	std::queue<EditPacket> q;
	std::mutex q_mutex;
	
	public:
	void push(std::string input);
	EditPacket pop();
	bool isEmpty();
}


#endif