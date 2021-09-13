#ifndef KERNEL_HPP
#define KERNEL_HPP
#include <memory>

namespace pbf {
class Server;
class Kernel {
 public:
  Kernel();
  ~Kernel();
  bool start(int argc, char* argv[]);
  void sleep();

 private:
  std::unique_ptr<Server> server_;
};
}  // namespace pbf

#endif  // KERNEL_HPP
