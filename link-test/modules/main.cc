#include <iostream>

#include "a.hpp"
import base;
import b;

int main() {
  {
    using namespace a;
    std::cout << f1test() << "\t";
    std::cout << f1() << "\n";
    std::cout << f2test() << "\t";
    std::cout << f2() << "\n";
    std::cout << f3test() << "\t";
    std::cout << f3() << "\n";
  }
  std::cout << "---------\n";
  {
    using namespace b;
    std::cout << f1test() << "\t";
    std::cout << f1() << "\n";
    std::cout << f3test() << "\t";
    std::cout << f3() << "\n";
  }
}