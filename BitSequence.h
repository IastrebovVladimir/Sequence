#ifndef BITSEQUENCE_H
#define BITSEQUENCE_H

#include "Bit.h"
#include "Sequence.h"
#include "IEnumerator.h"
#include <stdexcept>

class BitSequence : public Sequence<Bit> {
protected:
    Sequence<Bit>* CreateEmpty() const override {
        return new BitSequence();
    }
private:
    unsigned char* data;
    int bitLength;
    int byteCapacity;

    mutable Bit firstCache;
    mutable Bit lastCache;

    class BitEnumerator : public IEnumerator<Bit> {
        const BitSequence* sequence;
        Bit currentBit;
        int index;
    public:
        explicit BitEnumerator(const BitSequence* sequence)
                : sequence(sequence), currentBit(false), index(-1) {}

        bool MoveNext() override {
            if (index + 1 < sequence->GetLength()) {
                ++index;
                currentBit = sequence->GetBit(index);
                return true;
            }
            return false;
        }

        const Bit& Current() const override {
            if (index < 0 || index >= sequence->GetLength()) {
                throw std::out_of_range("BitEnumerator::Current: invalid position");
            }
            return currentBit;
        }
    };

    void ReSize(int bits) {
        int neededBytes = (bits + 7) / 8;
        if (neededBytes <= byteCapacity) {
            return;
        }

        int newCapacity = (byteCapacity == 0) ? 1 : byteCapacity;
        while (newCapacity < neededBytes) {
            newCapacity *= 2;
        }

        auto* newData = new unsigned char[newCapacity]();

        for (int index = 0; index < byteCapacity; ++index) {
            newData[index] = data[index];
        }

        delete[] data;
        data = newData;
        byteCapacity = newCapacity;
    }

public:
    BitSequence() : data(nullptr), bitLength(0), byteCapacity(0), firstCache(false), lastCache(false) {}

    ~BitSequence() override {
        delete[] data;
    }

    BitSequence(const BitSequence& other)
            : data(nullptr),
              bitLength(other.bitLength),
              byteCapacity(other.byteCapacity),
              firstCache(false),
              lastCache(false) {
        if (byteCapacity > 0) {
            data = new unsigned char[byteCapacity];
            for (int i = 0; i < byteCapacity; i++) {
                data[i] = other.data[i];
            }
        }
    }

    BitSequence& operator=(const BitSequence& other) {
        if (this == &other) {
            return *this;
        }

        unsigned char* newData = nullptr;
        if (other.byteCapacity > 0) {
            newData = new unsigned char[other.byteCapacity];
            for (int i = 0; i < other.byteCapacity; i++) {
                newData[i] = other.data[i];
            }
        }

        delete[] data;
        data = newData;
        bitLength = other.bitLength;
        byteCapacity = other.byteCapacity;
        firstCache = Bit(false);
        lastCache = Bit(false);

        return *this;
    }

    Enumerator<Bit> GetEnumerator() const override {
        return Enumerator<Bit>(new BitEnumerator(this));
    }

    const Bit& GetFirst() const override {
        if (GetLength() == 0) {
            throw std::out_of_range("BitSequence::GetFirst: sequence is empty");
        }
        firstCache = GetBit(0);
        return firstCache;
    }

    const Bit& GetLast() const override {
        if (GetLength() == 0) {
            throw std::out_of_range("BitSequence::GetLast: sequence is empty");
        }
        lastCache = GetBit(bitLength - 1);
        return lastCache;
    }

    Bit GetBit(int index) const {
        if (index < 0 || index >= bitLength) {
            throw std::out_of_range("BitSequence::GetBit: index out of range");
        }

        int byteIndex = index / 8;
        int bitIndex = index % 8;
        bool value = (data[byteIndex] >> bitIndex) & 1u;
        return Bit(value);
    }

    void SetBit(int index, const Bit& value) {
        if (index < 0 || index >= bitLength) {
            throw std::out_of_range("BitSequence::SetBit: index out of range");
        }
        int byteIndex = index / 8;
        int bitIndex = index % 8;
        if (value.GetValue()) {
            data[byteIndex] |= (1u << bitIndex);
        } else {
            data[byteIndex] &= (~(1u << bitIndex));
        }
    }

    int GetLength() const override {
        return bitLength;
    }

    Sequence<Bit>* Append(const Bit& item) override {
        ReSize(bitLength + 1);
        bitLength++;
        SetBit(bitLength - 1, item);
        return this;
    }

    Sequence<Bit>* Prepend(const Bit& item) override {
        return InsertAt(item, 0);
    }

    Sequence<Bit>* InsertAt(const Bit& item, int index) override {
        if (index < 0 || index > bitLength) {
            throw std::out_of_range("BitSequence::InsertAt: index out of range");
        }

        ReSize(bitLength + 1);
        bitLength++;

        for (int i = bitLength - 1; i > index; i--) {
            SetBit(i, GetBit(i - 1));
        }

        SetBit(index, item);
        return this;
    }

    BitSequence And(const BitSequence& other) const {
        int length = std::min(GetLength(), other.GetLength());

        BitSequence result;
        for (int index = 0; index < length; index++) {
            bool value = (GetBit(index) & other.GetBit(index)).GetValue();
            result.Append(Bit(value));
        }
        return result;
    }

    BitSequence Or(const BitSequence& other) const {
        int minLength = std::min(GetLength(), other.GetLength());
        int maxLength = std::max(GetLength(), other.GetLength());

        BitSequence result;
        for (int index = 0; index < minLength; index++) {
            bool value = (GetBit(index) | other.GetBit(index)).GetValue();
            result.Append(Bit(value));
        }

        const BitSequence& longer = (GetLength() >= other.GetLength()) ? *this : other;

        for (int index = minLength; index < maxLength; index++) {
            bool value = longer.GetBit(index).GetValue();
            result.Append(Bit(value));
        }

        return result;
    }

    BitSequence Xor(const BitSequence& other) const {
        int minLength = std::min(GetLength(), other.GetLength());
        int maxLength = std::max(GetLength(), other.GetLength());

        BitSequence result;
        for (int index = 0; index < minLength; index++) {
            bool value = (GetBit(index) ^ other.GetBit(index)).GetValue();
            result.Append(Bit(value));
        }

        const BitSequence& longer = (GetLength() >= other.GetLength()) ? *this : other;

        for (int index = minLength; index < maxLength; index++) {
            result.Append(longer.GetBit(index));
        }

        return result;
    }

    BitSequence Not() const {
        BitSequence result;
        for (int index = 0; index < GetLength(); index++) {
            bool value = (~GetBit(index)).GetValue();
            result.Append(Bit(value));
        }
        return result;
    }

    BitSequence operator&(const BitSequence& other) const {
        return And(other);
    }

    BitSequence operator|(const BitSequence& other) const {
        return Or(other);
    }

    BitSequence operator^(const BitSequence& other) const {
        return Xor(other);
    }

    BitSequence operator~() const {
        return Not();
    }
};

#endif