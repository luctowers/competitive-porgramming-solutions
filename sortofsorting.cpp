// https://open.kattis.com/problems/sortofsorting

#include <iostream>
#include <map>

int main() {
  unsigned short name_count;
  std::string name;
  std::map<unsigned,std::string> sorted_index;

  while (true) {

    std::cin >> name_count;
    if (!name_count)
      break;

    for (unsigned short i = 0; i < name_count; i++) {
      std::cin >> name;
      // compose an index where the first two bytes of the name are the most significant
      // and the insertion order is the least significant
      unsigned index = (name[0] << 24) | (name[1] << 16) | i;
      sorted_index.insert({index,name});
    }

    // output the names
    for (auto pair : sorted_index) {
      std::cout << pair.second << std::endl;
    }
    std::cout << std::endl;

    sorted_index.clear();
  }

  return 0;
}
