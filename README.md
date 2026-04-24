# Sequence

Учебный проект по реализациям последовательностей на C++.

## Описание

Проект реализует аазные варианты последовательностей Sequence (массив, список, битовая последовательность) и набор операций с ними.
Программа `Lab_2_Sequence` предоставляет интерфейс для взаимодействия с последовательностями.
Корректность реализаций проверяется модульными тестами на Google Test.

## Установка

```bash
cd ~
git clone https://github.com/IastrebovVladimir/Sequence.git
cd Sequence
git clone https://github.com/google/googletest.git
```

## Запуск тестов

```bash
g++ -g -O0 -std=c++20 tests/testMain.cpp tests/testArray.cpp tests/testList.cpp tests/testBit.cpp -I. -isystem googletest/googletest/include -Igoogletest/googletest googletest/googletest/src/gtest-all.cc -pthread -o SequenceTests
./SequenceTests
```

## Запуск интерфейса

```bash
g++ -g -O0 -std=c++20 main.cpp types.cpp -o Lab_2_Sequence
./Lab_2_Sequence
```

## Структура проекта

```text
Sequence/
├─ ArraySequence.h      # Реализация последовательности на динамическом массиве
├─ Bit.h                # Вспомогательный тип/обёртка для одного бита
├─ BitSequence.h        # Битовая последовательность
├─ CMakeLists.txt       # Конфигурация сборки CMake
├─ DynamicArray.h       # Реализация динамического массива
├─ IEnumerator.h        # Интерфейс для перечислителя элементов
├─ LinkedList.h         # Односвязныйй список
├─ ListSequence.h       # Последовательность на базе связного списка
├─ Option.h             # Опциональное значение
├─ Sequence.h           # Базовый интерфейс последовательности
├─ main.cpp             # Консольный интерфейс (Lab_2_Sequence)
├─ menu.h               # Объявления меню интерфейса
├─ menu.hpp             # Реализация функций меню
├─ types.cpp            # Реализации вспомогательных типов
├─ types.h              # Объявления вспомогательных типов
└─ tests/
   ├─ testMain.cpp      # Запуск тестов
   ├─ testArray.cpp     # Тесты ArraySequence
   ├─ testList.cpp      # Тесты ListSequence
   └─ testBit.cpp       # Тесты BitSequence

```
