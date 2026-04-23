#ifndef MENU_H
#define MENU_H

#include "ArraySequence.h"
#include "ListSequence.h"
#include "Sequence.h"
#include "types.h"

template <typename T>
void run_menu_for_type(const TypeInfo* t);

template <typename T>
void case_create_sequence(Sequence<T>** sequence);

template <typename T>
void case_delete_sequence(Sequence<T>** sequence);

template <typename T>
void case_append(Sequence<T>** sequence, const TypeInfo* t);

template <typename T>
void case_prepend(Sequence<T>** sequence, const TypeInfo* t);

template <typename T>
void case_get_by_index(Sequence<T>* sequence, const TypeInfo* t);

template <typename T>
void case_insert_at(Sequence<T>** sequence, const TypeInfo* t);

template <typename T>
void case_get_subsequence(Sequence<T>** sequence);

template <typename T>
void case_slice(Sequence<T>*& sequence, const TypeInfo* t);

template <typename T>
void case_print(Sequence<T>* sequence);

#include "menu.hpp"

#endif
