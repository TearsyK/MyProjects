#ifndef LAB3SEASON2_PASCAL_H
#define LAB3SEASON2_PASCAL_H

#include <iostream>
#include <vector>

class Pascal
{
public:
static void trojkatPascala(int n) {
    std::vector<std::vector<int>> pascal(n);

    for (int i = 0; i < n; ++i) {
        pascal[i].resize(i + 1);
        pascal[i][0] = pascal[i][i] = 1;

        for (int j = 1; j < i; ++j) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }

    // Wyświetlanie trójkąta
    for (const auto& row : pascal) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

};


#endif //LAB3SEASON2_PASCAL_H
