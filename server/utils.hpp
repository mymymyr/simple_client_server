#ifndef UTILS_HPP
#define UTILS_HPP
#include <memory>

namespace pbf {

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}  // namespace pbf

#endif // UTILS_HPP
