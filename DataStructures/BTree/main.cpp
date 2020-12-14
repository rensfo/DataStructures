#include <iostream>

#include "BTree.h"

using namespace std;

int main()
{
    BTree<int> btree;
    cout << "add 1" << std::endl;
    btree.add(1);
    cout << "add 3" << std::endl;
    btree.add(3);
    cout << "add 2" << std::endl;
    btree.add(2);
    cout << "add 4" << std::endl;
    btree.add(4);
    cout << "add 5" << std::endl;
    btree.add(5);

    btree.dump();

    cout << "add 6" << std::endl;
    btree.add(6);
    cout << "add 7" << std::endl;
    btree.add(7);
    btree.dump();

    cout << "add 8" << std::endl;
    btree.add(8);
    cout << "add 9" << std::endl;
    btree.add(9);
    btree.dump();
    cout << "add 10" << std::endl;
    btree.add(10);
    btree.dump();
    cout << "add 12" << std::endl;
    btree.add(12);
    btree.dump();
    cout << "add 13" << std::endl;
    btree.add(13);
    btree.dump();
    cout << "remove 13" << std::endl;
    btree.remove(13);
    btree.dump();
    cout << "remove 12" << std::endl;
    btree.remove(12);
    btree.dump();
    return 0;
}
