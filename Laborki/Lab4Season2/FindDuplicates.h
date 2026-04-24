#ifndef LAB4SEASON2_FINDDUPLICATES_H
#define LAB4SEASON2_FINDDUPLICATES_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>

class FindDuplicates
{
public:

static void findDuplicates(const std::string& sentence) {
    std::unordered_map<std::string, int> wordCount;
    std::istringstream stream(sentence);
    std::string word;

    while (stream >> word) {
        ++wordCount[word];
    }

    std::for_each(wordCount.begin(), wordCount.end(), [](const auto& pair) {
        if (pair.second > 1) {
            std::cout << "Word: " << pair.first << ", Count: " << pair.second << std::endl;
        }
    });
}
};


#endif //LAB4SEASON2_FINDDUPLICATES_H
