#ifndef PIPEH
#define PIPEH

#ifdef __WIN32__
#include <Windows.h>

#elif defined __UNIX__
#include <dirent.h>

#endif

namespace Pipe {
//Provides a wrapper for pipes for interprocess communication

	void create(Pipe::Reader* r, Pipe::Writer* w);

	class Reader {
		private:
		void* pipe;
		Reader(void* p);
		
		public:
		Reader();
		friend void create(Pipe::Reader* r, Pipe::Writer* w);
		auto readObj(size_t sz);
		bool isEmpty();
	}

	class Writer {
		private:
		void* pipe;
		Writer(void* p);
		
		public:
		Writer();
		friend void create(Pipe::Reader* r, Pipe::Writer* w);
		void writeObj(void* obj);
	}
}

#endif