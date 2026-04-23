#include <iostream>
#include "menu.h"
#include "types.h"

int main() {
    int choice = 0;
    std::cout << "Choose element type for Sequence:\n";
    std::cout << "1. int\n";
    std::cout << "2. double\n";
    std::cout << "Enter choice: ";

    while (!get_int_t()->scan(&choice) || choice < 1 || choice > 2)  {
        std::cout << "Error! Enter an integer from 1 to 2:";
    }

    switch (choice) {
        case 1:
            run_menu_for_type<int>(get_int_t());
            break;
        case 2:
            run_menu_for_type<double>(get_double_t());
            break;
    }

    return 0;
}
