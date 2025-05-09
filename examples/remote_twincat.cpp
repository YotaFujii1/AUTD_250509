#include "autd3.hpp"
#include "autd3/link/twincat.hpp"
#include "runner.hpp"
#include "util.hpp"

int main() try {
  auto autd = autd3::ControllerBuilder(std::vector{autd3::AUTD3(autd3::Vector3::Zero())})
                  .open(autd3::link::RemoteTwinCAT::builder("your TwinCATAUTDServer AMS net id (e.g. 172.16.99.2.1.1)"));
  run(autd);
  return 0;
} catch (std::exception& e) {
  print_err(e);
  return -1;
}
