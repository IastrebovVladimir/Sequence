#ifndef BIT_H
#define BIT_H

struct Bit {
private:
    bool value;

public:
    Bit() : value(false) {}

    Bit(bool value) : value(value) {}

    bool GetValue() const {
        return value;
    }

    Bit operator~() const {
        return Bit(!value);
    }

    Bit operator&(const Bit& other) const {
        return Bit(value && other.value);
    }

    Bit operator|(const Bit& other) const {
        return Bit(value || other.value);
    }

    Bit operator^(const Bit& other) const {
        return Bit(value != other.value);
    }

    bool operator==(const Bit& other) const {
        return value == other.value;
    }

    bool operator!=(const Bit& other) const {
        return value != other.value;
    }
};

#endif