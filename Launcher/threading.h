#ifndef LDFTHREADSAFETY
#define LDFTHREADSAFETY

#include <vector>
#include <mutex>

template <class T> class MutexVector : public std::vector<T> {
	private:
	std::mutex v_mutex;
	public:
	MutexVector<T>();
	void AcquireLock();
	void ReleaseLock();
};

#endif