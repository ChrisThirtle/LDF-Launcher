#ifndef DIRECH
#define DIRECH

#include <string>

#ifdef __WIN32__
#include <Windows.h>

#elif defined __UNIX__
#include <dirent.h>

#endif

namespace Directory {
//Provides a basic wrapper for directory manipulation.

	class Iterator {
		private:
			#ifdef __WIN32__
			HANDLE pos = INVALID_HANDLE_VALUE;
			#elif defined __UNIX__
			DIR* dp;
			#endif
			std::string homedir;
		public:
			Iterator();								//Creates directory lister in current directory
			
			~Iterator();							//Closes any dir items and returns to the original directory
			
			std::string getNextFilename();			//Gets the name of the next file in the directory
													//Throws std::out_of_range exception at end of file.
			
			std::string getDirPath();				//Returns the full path to the current directory
			
			void resetPos();						//Resets the pointer to the next file to the first file in the directory
			
			void changeDirectory(char* newDir);		//Changes to a new directory pointed to by newDir
			void changeDirectory(std::string newDir);
			
			void home();							//Returns to the original directory. Directory remains open.
	};
};

#endif