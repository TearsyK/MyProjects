#ifndef LAB2SEASON2_PASCAL_H
#define LAB2SEASON2_PASCAL_H
#include <iostream>
#include <vector>
#include <algorithm>

class Pascal
{
public:

static int pascal() {
    // Liczba wierszy trójkąta Pascala
    const int numRows = 5;

    // Kontener na cały trójkąt Pascala
    std::vector<std::vector<int>> pascalTriangle(numRows);

    // Generowanie trójkąta Pascala
    std::generate(pascalTriangle.begin(), pascalTriangle.end(), [row = 0, &pascalTriangle]() mutable {
        std::vector<int> currentRow(row + 1, 1); // Wszystkie elementy wiersza inicjalizowane na 1
        if (row > 0) {
            // Uzupełnianie środkowych wartości wiersza
            std::transform(pascalTriangle[row - 1].begin(), pascalTriangle[row - 1].end() - 1,
                           pascalTriangle[row - 1].begin() + 1,
                           currentRow.begin() + 1,
                           std::plus<>());
        }
        ++row;
        return currentRow;
    });

    // Wyświetlanie trójkąta Pascala
    for (const auto& row : pascalTriangle) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
};


#endif //LAB2SEASON2_PASCAL_H
