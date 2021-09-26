#include <math.h>

#include <iostream>
int main(int argv, char* argc[]) {
    int size = atoi(argc[1]);
    int num = 0;
    srand(time(NULL));
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= size; j++) {
            int num = (rand() % 10) >= 8;
            std::cout << num;
            if (j < size)
                std::cout << ", ";
            else
                std::cout << "\n";
            num++;
        }
    }
}
