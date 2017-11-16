#ifndef LDFTHREADSAFETY
#define LDFTHREADSAFETY

#include <vector>
#include <mutex>

template<class T>
class MutexVector {
	private:
	std::vector<T> v;
	
	public:
	typename std::vector<T>::reference operator[](std::size_t pos) {return v[pos];}
	typename std::vector<T>::reference at(std::size_t pos) {return v.at(pos);}
	typename std::vector<T>::iterator begin() {return v.begin();}
	typename std::vector<T>::iterator end() {return v.end();}
	typename std::vector<T>::size_type size() {return v.size();}
	void reserve(typename std::vector<T>::size_type size) {return v.reserve(size);}
	void push_back( T&& value ) {return v.push_back(value);}
	
	template< class... Args >
	void emplace_back( Args&&... args ) {return v.emplace_back(args...);}
	
	std::mutex v_mutex;
	void AcquireLock() {v_mutex.lock();}
	void ReleaseLock() {v_mutex.unlock();}
};

#endif