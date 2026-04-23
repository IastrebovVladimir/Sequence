#include <gtest/gtest.h>
#include "../ArraySequence.h"

static bool IsEven(const int& x) {
    return x % 2 == 0;
}

static int Sum(const int& summ, const int& value) {
    return summ + value;
}

static int DoubleValue(int x) {
    return x * 2;
}

TEST(ArraySequenceTest, AppendWorksMutable) {
    MutableArraySequence<int> sequence;
    sequence.Append(10);
    sequence.Append(20);
    sequence.Append(30);
    sequence.Append(40);

    EXPECT_EQ(sequence.GetLength(), 4);
    EXPECT_EQ(sequence.GetFirst(), 10);
    EXPECT_EQ(sequence.Get(1), 20);
    EXPECT_EQ(sequence.Get(2), 30);
    EXPECT_EQ(sequence.GetLast(), 40);
}

TEST(ArraySequenceTest, AppendWorksImmutable) {
    ImmutableArraySequence<int> empty;

    Sequence<int>* sequence1 = empty.Append(10);
    Sequence<int>* sequence2 = sequence1->Append(20);
    Sequence<int>* sequence3 = sequence2->Append(30);
    Sequence<int>* sequence4 = sequence3->Append(240);

    EXPECT_EQ(empty.GetLength(), 0);

    EXPECT_EQ(sequence1->GetLength(), 1);
    EXPECT_EQ(sequence1->GetFirst(), 10);
    EXPECT_EQ(sequence1->GetLast(), 10);

    EXPECT_EQ(sequence2->GetLength(), 2);
    EXPECT_EQ(sequence2->GetFirst(), 10);
    EXPECT_EQ(sequence2->GetLast(), 20);

    EXPECT_EQ(sequence3->GetLength(), 3);
    EXPECT_EQ(sequence3->GetFirst(), 10);
    EXPECT_EQ(sequence3->GetLast(), 30);

    EXPECT_EQ(sequence4->GetLength(), 4);
    EXPECT_EQ(sequence4->GetFirst(), 10);
    EXPECT_EQ(sequence4->GetLast(), 240);

    delete sequence1;
    delete sequence2;
    delete sequence3;
    delete sequence4;
}

TEST(ArraySequenceTest, PrependWorksMutable) {
    MutableArraySequence<int> sequence;
    sequence.Prepend(10);
    sequence.Prepend(5);

    EXPECT_EQ(sequence.GetLength(), 2);
    EXPECT_EQ(sequence.GetFirst(), 5);
    EXPECT_EQ(sequence.GetLast(), 10);
    EXPECT_EQ(sequence.Get(0), 5);
    EXPECT_EQ(sequence.Get(1), 10);
}

TEST(ArraySequenceTest, PrependWorksImmutable) {
    ImmutableArraySequence<int> empty;

    Sequence<int>* sequence1 = empty.Prepend(10);
    Sequence<int>* sequence2 = sequence1->Prepend(20);

    EXPECT_EQ(empty.GetLength(), 0);

    EXPECT_EQ(sequence1->GetLength(), 1);
    EXPECT_EQ(sequence1->GetFirst(), 10);
    EXPECT_EQ(sequence1->GetLast(), 10);

    EXPECT_EQ(sequence2->GetLength(), 2);
    EXPECT_EQ(sequence2->GetFirst(), 20);
    EXPECT_EQ(sequence2->GetLast(), 10);

    delete sequence1;
    delete sequence2;
}

TEST(ArraySequenceTest, InsertAtWorks) {
    MutableArraySequence<int> sequence;
    sequence.Append(10);
    sequence.Append(30);
    sequence.InsertAt(20, 1);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.Get(0), 10);
    EXPECT_EQ(sequence.Get(1), 20);
    EXPECT_EQ(sequence.Get(2), 30);
}

TEST(ArraySequenceTest, GetSubsequenceWorks) {
    MutableArraySequence<int> sequence;
    sequence.Append(10);
    sequence.Append(20);
    sequence.Append(30);
    sequence.Append(40);

    Sequence<int>* subsequence = sequence.GetSubsequence(1, 2);
    auto* arraySubsequence = dynamic_cast<ArraySequence<int>*>(subsequence);

    EXPECT_EQ(arraySubsequence->GetLength(), 2);
    EXPECT_EQ(arraySubsequence->Get(0), 20);
    EXPECT_EQ(arraySubsequence->Get(1), 30);
    EXPECT_EQ(arraySubsequence->GetFirst(), 20);
    EXPECT_EQ(arraySubsequence->GetLast(), 30);

    delete subsequence;
}

TEST(ArraySequenceTest, WhereWorks) {
    MutableArraySequence<int> sequence;
    sequence.Append(1);
    sequence.Append(2);
    sequence.Append(3);
    sequence.Append(4);
    sequence.Append(5);
    sequence.Append(6);

    Sequence<int>* filtered = sequence.Where(IsEven);
    auto* arrayFiltered = dynamic_cast<ArraySequence<int>*>(filtered);

    EXPECT_EQ(arrayFiltered->GetLength(), 3);
    EXPECT_EQ(arrayFiltered->Get(0), 2);
    EXPECT_EQ(arrayFiltered->Get(1), 4);
    EXPECT_EQ(arrayFiltered->Get(2), 6);

    delete filtered;
}

TEST(ArraySequenceTest, ReduceWorks) {
    MutableArraySequence<int> sequence;
    sequence.Append(1);
    sequence.Append(2);
    sequence.Append(3);
    sequence.Append(4);

    int result = sequence.Reduce(Sum, 0);

    EXPECT_EQ(result, 10);
}

TEST(ArraySequenceTest, SliceWorks) {
    MutableArraySequence<int> sequence;
    sequence.Append(10);
    sequence.Append(20);
    sequence.Append(30);
    sequence.Append(40);

    MutableArraySequence<int> replacement;
    replacement.Append(99);
    replacement.Append(100);

    Sequence<int>* sliced = sequence.Slice(1, 2, replacement);
    auto* arraySliced = dynamic_cast<ArraySequence<int>*>(sliced);

    EXPECT_EQ(arraySliced->GetLength(), 4);
    EXPECT_EQ(arraySliced->Get(0), 10);
    EXPECT_EQ(arraySliced->Get(1), 99);
    EXPECT_EQ(arraySliced->Get(2), 100);
    EXPECT_EQ(arraySliced->Get(3), 40);

    delete sliced;
}

TEST(ArraySequenceTest, MapWorks) {
    MutableArraySequence<int> sequence;
    sequence.Append(1);
    sequence.Append(2);
    sequence.Append(3);

    Sequence<int>* mapped = sequence.Map(DoubleValue);
    auto* arrayMapped = dynamic_cast<ArraySequence<int>*>(mapped);

    EXPECT_EQ(arrayMapped->GetLength(), 3);
    EXPECT_EQ(arrayMapped->Get(0), 2);
    EXPECT_EQ(arrayMapped->Get(1), 4);
    EXPECT_EQ(arrayMapped->Get(2), 6);

    delete mapped;
}

TEST(MatrixSequenceTest, LargeMatrixWorks) {
    const int N = 5000;
    MutableArraySequence<MutableArraySequence<int>*> matrix;

    for (int index = 0; index < N; index++) {
        auto* row = new MutableArraySequence<int>();
        for (int j = 0; j < N; ++j) {
            row->Append(index * N + j);
        }
        matrix.Append(row);
    }

    ASSERT_EQ(matrix.GetLength(), N);

    for (int index = 0; index < N; index++) {
        for (int j = 0; j < N; j++) {
            EXPECT_EQ(matrix.Get(index)->Get(j), index * N + j);
        }
    }

    for (int index = 0; index < N; index++) {
        delete matrix.Get(index);
    }
}