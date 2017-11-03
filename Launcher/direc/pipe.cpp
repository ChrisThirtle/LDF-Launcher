//Wrapper for pipe operations

#include "pipe.h"

Pipe::Reader::Reader(void* p){
	pipe = p;
}
Pipe::Writer::Writer(void* p){
	pipe = p;
}

#ifdef __WIN32__
void Pipe::create(Pipe::Reader* r, Pipe::Writer* w){
	CreatePipe(r->pipe,w->pipe,NULL,0);
}

auto Pipe::Reader::readObj(size_t sz) {

}

bool Pipe::Reader::isEmpty(){

}

void Pipe::Writer::writeObj(void* obj) {

}

#elif defined __UNIX__

Pipe::create(Pipe::Reader* r, Pipe::Writer* w){
	pipe(r->pipe,w->pipe);
}

auto Pipe::Reader::readObj(size_t sz) {

}

bool Pipe::Reader::isEmpty(){

}

void Pipe::Writer::writeObj(void* obj) {

}

#endif