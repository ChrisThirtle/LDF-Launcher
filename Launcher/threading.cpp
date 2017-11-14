MutexVector::MutexVector<T>() : std::vector<T>(){
}
void MutexVector::AcquireLock() {
	v_mutex.lock();
}
void MutexVector::ReleaseLock() {
	v_mutex.unlock();
}