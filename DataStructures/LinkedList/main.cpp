#include <iostream>

#include "LinkedList.h"

using namespace std;

int main() {
  LinkedList<int> ll;

  std::cout << "add 1" << std::endl;
  ll.addToBack(1);
  cout << "count: " << ll.count() << std::endl;
  cout << ll << std::endl;

  std::cout << "add 10" << std::endl;
  ll.addToBack(10);
  cout << "count: " << ll.count() << std::endl;
  cout << ll << std::endl;

  std::cout << "add 100" << std::endl;
  ll.addToBack(100);
  cout << "count: " << ll.count() << std::endl;
  cout << ll << std::endl;

  std::cout << "remove 10" << std::endl;
  ll.remove(10);
  cout << "count: " << ll.count() << std::endl;
  cout << ll << std::endl;

  std::cout << "remove 99" << std::endl;
  ll.remove(99);
  cout << "count: " << ll.count() << std::endl;
  cout << ll << std::endl;

  return 0;
}
