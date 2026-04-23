#include <gtest/gtest.h>
#include "../BitSequence.h"

static BitSequence MakeSequence(std::initializer_list<int> bits) {
    BitSequence sequence;
    for (int bit : bits) {
        sequence.Append(Bit(bit != 0));
    }
    return sequence;
}

static void ExpectBitsEqual(const BitSequence& sequence, std::initializer_list<int> expected) {
    ASSERT_EQ(sequence.GetLength(), static_cast<int>(expected.size()));

    int index = 0;
    for (int bit : expected) {
        EXPECT_EQ(sequence.GetBit(index).GetValue(), bit != 0);
        index++;
    }
}

TEST(BitSequenceTests, AppendAndGetBitWork) {
    BitSequence sequence;
    sequence.Append(Bit(true));
    sequence.Append(Bit(false));
    sequence.Append(Bit(true));

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_TRUE(sequence.GetBit(0).GetValue());
    EXPECT_FALSE(sequence.GetBit(1).GetValue());
    EXPECT_TRUE(sequence.GetBit(2).GetValue());
}

TEST(BitSequenceTests, GetFirstAndGetLastWork) {
    BitSequence sequence = MakeSequence({1, 0, 1, 1, 0});

    EXPECT_TRUE(sequence.GetFirst().GetValue());
    EXPECT_FALSE(sequence.GetLast().GetValue());
}

TEST(BitSequenceTests, AndWorks) {
    BitSequence sequence1 = MakeSequence({1, 0, 1, 0, 1, 0, 1, 0});
    BitSequence sequence2 = MakeSequence({1, 1, 0, 0});

    BitSequence result = sequence1.And(sequence2);

    ExpectBitsEqual(result, {1, 0, 0, 0});
}

TEST(BitSequenceTests, OrWorks) {
    BitSequence sequence1 = MakeSequence({1, 0, 1, 0, 1, 0, 1, 0});
    BitSequence sequence2 = MakeSequence({1, 1, 0, 1, 0});

    BitSequence result = sequence1.Or(sequence2);

    ExpectBitsEqual(result, {1, 1, 1, 1, 1, 0, 1, 0});
}

TEST(BitSequenceTests, XorWorks) {
    BitSequence sequence1 = MakeSequence({1, 0, 1, 0, 1, 0, 1, 0});
    BitSequence sequence2 = MakeSequence({1, 1, 0, 0, 1, 1, 0, 0});

    BitSequence result = sequence1.Xor(sequence2);

    ExpectBitsEqual(result, {0, 1, 1, 0, 0, 1, 1, 0});
}

TEST(BitSequenceTests, NotWorks) {
    BitSequence sequence = MakeSequence({1, 0, 1, 0, 0, 1});

    BitSequence result = sequence.Not();

    ExpectBitsEqual(result, {0, 1, 0, 1, 1, 0});
}

TEST(BitSequenceTests, PrependWorks) {
    BitSequence sequence = MakeSequence({0, 1, 1});
    sequence.Prepend(Bit(true));

    ExpectBitsEqual(sequence, {1, 0, 1, 1});
}

TEST(BitSequenceTests, InsertAtWorks) {
    BitSequence sequence = MakeSequence({1, 0, 1});
    sequence.InsertAt(Bit(true), 1);

    ExpectBitsEqual(sequence, {1, 1, 0, 1});
}

TEST(BitSequenceTests, OperatorsMatchMethods) {
    BitSequence sequence1 = MakeSequence({1, 0, 1, 0, 1, 0, 1, 0});
    BitSequence sequence2 = MakeSequence({1, 1, 0, 0, 1, 1, 0, 0});

    BitSequence resultAndOp = sequence1 & sequence2;
    BitSequence resultAndMethod = sequence1.And(sequence2);

    BitSequence resultOrOp = sequence1 | sequence2;
    BitSequence resultOrMethod = sequence1.Or(sequence2);

    BitSequence resultXorOp = sequence1 ^ sequence2;
    BitSequence resultXorMethod = sequence1.Xor(sequence2);

    BitSequence resultNotOp = ~sequence1;
    BitSequence resultNotMethod = sequence1.Not();

    ASSERT_EQ(resultAndOp.GetLength(), resultAndMethod.GetLength());
    ASSERT_EQ(resultOrOp.GetLength(), resultOrMethod.GetLength());
    ASSERT_EQ(resultXorOp.GetLength(), resultXorMethod.GetLength());
    ASSERT_EQ(resultNotOp.GetLength(), resultNotMethod.GetLength());

    for (int index = 0; index < resultAndOp.GetLength(); index++) {
        EXPECT_EQ(resultAndOp.GetBit(index).GetValue(), resultAndMethod.GetBit(index).GetValue());
    }

    for (int index = 0; index < resultOrOp.GetLength(); index++) {
        EXPECT_EQ(resultOrOp.GetBit(index).GetValue(), resultOrMethod.GetBit(index).GetValue());
    }

    for (int index = 0; index < resultXorOp.GetLength(); index++) {
        EXPECT_EQ(resultXorOp.GetBit(index).GetValue(), resultXorMethod.GetBit(index).GetValue());
    }

    for (int index = 0; index < resultNotOp.GetLength(); index++) {
        EXPECT_EQ(resultNotOp.GetBit(index).GetValue(), resultNotMethod.GetBit(index).GetValue());
    }
}