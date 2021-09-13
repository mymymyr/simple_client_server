#ifndef THREADSAFEFILE_HPP
#define THREADSAFEFILE_HPP

#include <fstream>
#include <mutex>
#include <string>

namespace pbf {
class ThreadSafeFile {
 public:
  ThreadSafeFile();
  ~ThreadSafeFile();

  bool Open(const std::string& filename);
  bool IsOpen() const noexcept;
  bool Append(const std::string& data);

 private:
  bool is_open_ = false;
  std::ofstream file_;
  mutable std::mutex mutex_;
};
}  // namespace pbf

#endif  // THREADSAFEFILE_HPP
