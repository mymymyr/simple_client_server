#include "threadsafefile.hpp"

#include <assert.h>

namespace pbf {

ThreadSafeFile::ThreadSafeFile() {}
ThreadSafeFile::~ThreadSafeFile() {}

bool ThreadSafeFile::Open(const std::string& filename) {
  file_.open(filename, std::ofstream::app);
  return IsOpen();
}

bool ThreadSafeFile::IsOpen() const noexcept {
  return file_.is_open();
}

bool ThreadSafeFile::Append(const std::string& data) {
  std::lock_guard<std::mutex> lock(mutex_);
  try {
    assert(file_.is_open());
    file_ << data;
    file_.flush();
  } catch (...) {
    return false;
  }
  return true;
}

}  // namespace pbf
