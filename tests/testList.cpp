#include <gtest/gtest.h>
#include "../ListSequence.h"

static bool IsEven(const int& x) {
    return x % 2 == 0;
}

static int DoubleValue(int x) {
    return x * 2;
}

static int Sum(const int &summ, const int &value) {
    return summ + value;
}

TEST(ListSequenceTest, AppendWorks) {
    MutableListSequence<int> sequence;
    sequence.Append(10);
    sequence.Append(20);

    EXPECT_EQ(sequence.GetLength(), 2);
    EXPECT_EQ(sequence.GetFirst(), 10);
    EXPECT_EQ(sequence.GetLast(), 20);
}

TEST(ListSequenceTest, AppendWorksImmutable) {
    ImmutableListSequence<int> empty;

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

TEST(ListSequenceTest, GetSubsequenceWorks) {
    MutableListSequence<int> sequence;
    sequence.Append(10);
    sequence.Append(20);
    sequence.Append(30);
    sequence.Append(40);

    Sequence<int>* subsequence = sequence.GetSubsequence(1, 2);

    EXPECT_EQ(subsequence->GetLength(), 2);
    EXPECT_EQ(subsequence->GetFirst(), 20);
    EXPECT_EQ(subsequence->GetLast(), 30);

    Enumerator<int> iterator = subsequence->GetEnumerator();
    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 20);

    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 30);

    EXPECT_FALSE(iterator.MoveNext());

    delete subsequence;
}

TEST(ListSequenceTest, WhereWorks) {
    MutableListSequence<int> sequence;
    sequence.Append(1);
    sequence.Append(2);
    sequence.Append(3);
    sequence.Append(4);
    sequence.Append(5);
    sequence.Append(6);

    Sequence<int>* filtered = sequence.Where(IsEven);

    EXPECT_EQ(filtered->GetLength(), 3);
    EXPECT_EQ(filtered->GetFirst(), 2);
    EXPECT_EQ(filtered->GetLast(), 6);

    Enumerator<int> iterator = filtered->GetEnumerator();
    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 2);

    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 4);

    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 6);

    EXPECT_FALSE(iterator.MoveNext());

    delete filtered;
}

TEST(ListSequenceTest, ReduceWorks) {
    MutableListSequence<int> sequence;
    sequence.Append(1);
    sequence.Append(2);
    sequence.Append(3);
    sequence.Append(4);

    int result = sequence.Reduce(Sum, 0);

    EXPECT_EQ(result, 10);
}

TEST(ListSequenceTest, MapWorks) {
    MutableListSequence<int> sequence;
    sequence.Append(1);
    sequence.Append(2);
    sequence.Append(3);

    Sequence<int>* mapped = sequence.Map(DoubleValue);

    EXPECT_EQ(mapped->GetLength(), 3);
    EXPECT_EQ(mapped->GetFirst(), 2);
    EXPECT_EQ(mapped->GetLast(), 6);

    Enumerator<int> iterator = mapped->GetEnumerator();
    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 2);

    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 4);

    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 6);

    EXPECT_FALSE(iterator.MoveNext());

    delete mapped;
}

TEST(ListSequenceTest, SliceWorks) {
    MutableListSequence<int> sequence;
    sequence.Append(10);
    sequence.Append(20);
    sequence.Append(30);
    sequence.Append(40);

    MutableListSequence<int> replacement;
    replacement.Append(99);
    replacement.Append(100);

    Sequence<int>* sliced = sequence.Slice(1, 2, replacement);

    EXPECT_EQ(sliced->GetLength(), 4);
    EXPECT_EQ(sliced->GetFirst(), 10);
    EXPECT_EQ(sliced->GetLast(), 40);

    Enumerator<int> iterator = sliced->GetEnumerator();
    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 10);

    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 99);

    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 100);

    ASSERT_TRUE(iterator.MoveNext());
    EXPECT_EQ(iterator.Current(), 40);

    EXPECT_FALSE(iterator.MoveNext());

    delete sliced;
}

TEST(ListSequenceTest, LargeMatrixWorks) {
    const int N = 5000;
    MutableListSequence<MutableListSequence<int>*> matrix;

    for (int index = 0; index < N; index++) {
        auto* row = new MutableListSequence<int>();
        for (int j = 0; j < N; j++) {
            row->Append(index * N + j);
        }
        matrix.Append(row);
    }

    ASSERT_EQ(matrix.GetLength(), N);

    Enumerator<MutableListSequence<int>*> rowsIterator = matrix.GetEnumerator();
    for (int index = 0; index < N; index++) {
        ASSERT_TRUE(rowsIterator.MoveNext());
        MutableListSequence<int>* row = rowsIterator.Current();
        ASSERT_NE(row, nullptr);
        ASSERT_EQ(row->GetLength(), N);

        Enumerator<int> intIterator = row->GetEnumerator();
        for (int j = 0; j < N; j++) {
            ASSERT_TRUE(intIterator.MoveNext());
            int value = intIterator.Current();
            EXPECT_EQ(value, index * N + j);
        }
        EXPECT_FALSE(intIterator.MoveNext());
    }

    Enumerator<MutableListSequence<int>*> deleteIterator = matrix.GetEnumerator();
    while (deleteIterator.MoveNext()) {
        delete deleteIterator.Current();
    }
}