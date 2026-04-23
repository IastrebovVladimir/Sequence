#include "types.h"

#include <iostream>
#include <string>
#include <cstring>
#include <limits>

void sumInt(const void* arg1, const void* arg2, void* result) {
    *static_cast<int*>(result) =
            *static_cast<const int*>(arg1) + *static_cast<const int*>(arg2);
}

void sumDouble(const void* arg1, const void* arg2, void* result) {
    *static_cast<double*>(result) =
            *static_cast<const double*>(arg1) + *static_cast<const double*>(arg2);
}

void subInt(const void* arg1, const void* arg2, void* result) {
    *static_cast<int*>(result) =
            *static_cast<const int*>(arg1) - *static_cast<const int*>(arg2);
}

void subDouble(const void* arg1, const void* arg2, void* result) {
    *static_cast<double*>(result) =
            *static_cast<const double*>(arg1) - *static_cast<const double*>(arg2);
}

void mulInt(const void* arg1, const void* arg2, void* result) {
    *static_cast<int*>(result) =
            *static_cast<const int*>(arg1) * *static_cast<const int*>(arg2);
}

void mulDouble(const void* arg1, const void* arg2, void* result) {
    *static_cast<double*>(result) =
            *static_cast<const double*>(arg1) * *static_cast<const double*>(arg2);
}

void negInt(void* arg) {
    *static_cast<int*>(arg) = -*static_cast<int*>(arg);
}

void negDouble(void* arg) {
    *static_cast<double*>(arg) = -*static_cast<double*>(arg);
}

void zeroInt(void* arg) {
    *static_cast<int*>(arg) = 0;
}

void zeroDouble(void* arg) {
    *static_cast<double*>(arg) = 0.0;
}

void printInt(const void* arg) {
    std::cout << *static_cast<const int*>(arg);
}

void printDouble(const void* arg) {
    std::cout << *static_cast<const double*>(arg);
}

int scanInt(void* arg) {
    int value;

    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 0;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    *static_cast<int*>(arg) = value;
    return 1;
}

int scanDouble(void* arg) {
    double value;

    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 0;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    *static_cast<double*>(arg) = value;
    return 1;
}

const char* int_to_string(const void* arg) {
    std::string s = std::to_string(*static_cast<const int*>(arg));
    char* result = new char[s.size() + 1];
    std::strcpy(result, s.c_str());
    return result;
}

const char* double_to_string(const void* arg) {
    std::string s = std::to_string(*static_cast<const double*>(arg));
    char* result = new char[s.size() + 1];
    std::strcpy(result, s.c_str());
    return result;
}

const TypeInfo *get_int_t(void) {
    static TypeInfo int_t;
    static int inited = 0;

    if (!inited) {
        int_t.size = sizeof(int);
        int_t.sum = sumInt;
        int_t.sub = subInt;
        int_t.neg = negInt;
        int_t.mul = mulInt;
        int_t.zero = zeroInt;
        int_t.print = printInt;
        int_t.scan = scanInt;
        int_t.to_string = int_to_string;
        inited = 1;
    }
    return &int_t;
}

const TypeInfo *get_double_t(void) {
    static TypeInfo double_t;
    static int inited = 0;

    if (!inited) {
        double_t.size = sizeof(double);
        double_t.sum = sumDouble;
        double_t.sub = subDouble;
        double_t.neg = negDouble;
        double_t.mul = mulDouble;
        double_t.zero = zeroDouble;
        double_t.print = printDouble;
        double_t.scan = scanDouble;
        double_t.to_string = double_to_string;
        inited = 1;
    }
    return &double_t;
}