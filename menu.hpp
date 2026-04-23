#include <iostream>
#include "types.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "Sequence.h"

template <typename T>
void run_menu_for_type(const TypeInfo* t) {
    Sequence<T>* sequence = nullptr;

    std::cout << "\n1. Create Sequence\n";
    std::cout << "2. Delete Sequence\n";
    std::cout << "3. Append element\n";
    std::cout << "4. Prepend element\n";
    std::cout << "5. Insert at index (InsertAt)\n";
    std::cout << "6. Get element by index (Get)\n";
    std::cout << "7. Get Subsequence (GetSubsequence)\n";
    std::cout << "8. Slice\n";
    std::cout << "9. Print Sequence\n";
    std::cout << "0. Exit\n";

    while (true) {
        int command = 0;
        std::cout << "Choose: ";
        while (!get_int_t()->scan(&command)) {
            std::cout << "Enter an int value between 0 and 9: ";
        }

        switch (command) {
            case 1:
                case_create_sequence(&sequence);
                break;
            case 2:
                case_delete_sequence(&sequence);
                break;
            case 3:
                case_append(&sequence, t);
                break;
            case 4:
                case_prepend(&sequence, t);
                break;
            case 5:
                case_insert_at(&sequence, t);
                break;
            case 6:
                case_get_by_index(sequence, t);
                break;
            case 7:
                case_get_subsequence(&sequence);
                break;
            case 8:
                case_slice(sequence, t);
                break;
            case 9:
                case_print(sequence);
                break;
            case 0:
                delete sequence;
                std::cout << "Exit.\n";
                return;
            default:
                std::cout << "Unknown command.\n";
        }
    }
}

template <typename T>
void case_create_sequence(Sequence<T>** sequence) {
    if (*sequence != nullptr) {
        std::cout << "Sequence is already created\n";
        return;
    }

    int typeChoice = 0;

    std::cout << "Choose sequence type:\n";
    std::cout << "1. ArraySequence\n";
    std::cout << "2. ListSequence\n";

    while (!get_int_t()->scan(&typeChoice) || typeChoice < 1 || typeChoice > 2) {
        std::cout << "Error! Enter a digit from 1 to 2: ";
    }

    switch (typeChoice) {
        case 1:
            *sequence = new MutableArraySequence<T>();
            std::cout << "Created ArraySequence\n";
            break;
        case 2:
            *sequence = new MutableListSequence<T>();
            std::cout << "Created ListSequence\n";
            break;
    }
}

template <typename T>
void case_delete_sequence(Sequence<T>** sequence) {
    if (*sequence == nullptr) {
        std::cout << "Sequence is not created\n";
        return;
    }

    delete *sequence;
    *sequence = nullptr;

    std::cout << "Sequence has been deleted\n";
}

template <typename T>
void case_append(Sequence<T>** sequence, const TypeInfo* t) {
    if (*sequence == nullptr) {
        std::cout << "Sequence is not created\n";
        return;
    }

    T value;
    std::cout << "Enter element to append: ";
    while (!t->scan(&value)) {
        std::cout << "Error! Enter value of correct type: ";
    }

    *sequence = (*sequence)->Append(value);

    std::cout << "Element appended\n";
}

template <typename T>
void case_prepend(Sequence<T>** sequence, const TypeInfo* t) {
    if (*sequence == nullptr) {
        std::cout << "Sequence is not created\n";
        return;
    }

    T value;
    std::cout << "Enter element to prepend: ";
    while (!t->scan(&value)) {
        std::cout << "Error! Enter value of correct type: ";
    }

    *sequence = (*sequence)->Prepend(value);

    std::cout << "Element prepended\n";
}

template <typename T>
void case_get_by_index(Sequence<T>* sequence, const TypeInfo* t) {
    if (sequence == nullptr) {
        std::cout << "Sequence is not created\n";
        return;
    }

    const TypeInfo* intType = get_int_t();
    int index;
    std::cout << "Enter index: ";
    while (!intType->scan(&index)) {
        std::cout << "Error! Enter an integer: ";
    }

    int length = sequence->GetLength();
    if (index < 0 || index >= length) {
        std::cout << "Index out of range\n";
        return;
    }

    Enumerator<T> iterator = sequence->GetEnumerator();
    int current = 0;
    while (iterator.MoveNext() and current < index) {
        current++;
    }

    const T& value = iterator.Current();
    std::cout << "Element: ";
    t->print(&value);
    std::cout << '\n';
}

template <typename T>
void case_insert_at(Sequence<T>** sequence, const TypeInfo* t) {
    if (*sequence == nullptr) {
        std::cout << "Sequence is not created\n";
        return;
    }

    T item;
    int index;
    const TypeInfo* intType = get_int_t();

    std::cout << "Enter item: ";
    while (!t->scan(&item)) {
        std::cout << "Error! Enter value of correct type: ";
    }

    std::cout << "Enter index: ";
    while (!intType->scan(&index)) {
        std::cout << "Error! Enter an integer: ";
    }

    int length = (*sequence)->GetLength();
    if (index < 0 || index > length) {
        std::cout << "Index out of range\n";
        return;
    }

    Sequence<T>* newSequence = (*sequence)->InsertAt(item, index);

    if (newSequence != *sequence) {
        delete *sequence;
        *sequence = newSequence;
    }

    std::cout << "Element inserted\n";
}

template <typename T>
void case_get_subsequence(Sequence<T>** sequence) {
    if (*sequence == nullptr) {
        std::cout << "Sequence is not created\n";
        return;
    }

    int startIndex, endIndex;

    std::cout << "Enter startIndex: ";
    while (!get_int_t()->scan(&startIndex)) {
        std::cout << "Error! Enter an integer: ";
    }

    std::cout << "Enter endIndex: ";
    while (!get_int_t()->scan(&endIndex)) {
        std::cout << "Error! Enter an integer: ";
    }

    int length = (*sequence)->GetLength();
    if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= length) {
        std::cout << "Index out of range\n";
        return;
    }

    Sequence<T>* subsequence = (*sequence)->GetSubsequence(startIndex, endIndex);
    delete *sequence;
    *sequence = subsequence;

    std::cout << "Subsequence has been created\n";
}

template <typename T>
void case_slice(Sequence<T>*& sequence, const TypeInfo* t) {
    if (sequence == nullptr) {
        std::cout << "Sequence is not created\n";
        return;
    }

    const TypeInfo* intType = get_int_t();

    int startIndex;
    int count;

    std::cout << "Enter start index: ";
    while (!intType->scan(&startIndex) || startIndex < 0 || startIndex > sequence->GetLength()) {
        std::cout << "Error! Enter start index from 0 to " << sequence->GetLength() << ": ";
    }

    std::cout << "Enter count of elements to replace: ";
    while (!intType->scan(&count) || count < 0 || startIndex + count > sequence->GetLength()) {
        std::cout << "Error! Enter correct count: ";
    }

    T* data = new T[count];
    for (int index = 0; index < count; index++) {
        std::cout << "Enter replacement element " << index << ": ";
        while (!t->scan(&data[index])) {
            std::cout << "Error! Enter value of correct type: ";
        }
    }

    Sequence<T>* replacement = new MutableArraySequence<T>(data, count);
    delete[] data;

    Sequence<T>* result = sequence->Slice(startIndex, count, *replacement);
    delete sequence;
    sequence = result;

    delete replacement;
    std::cout << "Slice completed\n";
}

template <typename T>
void case_print(Sequence<T>* sequence) {
    if (sequence == nullptr) {
        std::cout << "Sequence is not created\n";
        return;
    }

    Enumerator<T> iterator = sequence->GetEnumerator();
    std::cout << "Sequence elements: ";
    while (iterator.MoveNext()) {
        std::cout << iterator.Current() << " ";
    }

    std::cout << "\n";
}