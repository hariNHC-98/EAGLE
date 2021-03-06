#pragma once

#include <ANSIColors.hpp>
#include <cstdint>
#include <iostream>
#include <vector>

enum ODEResultCodes : uint8_t {
    SUCCESS                     = 0,
    MINIMUM_STEP_SIZE_REACHED   = 1 << 0,
    MAXIMUM_ITERATIONS_EXCEEDED = 1 << 1,
};

struct ODEResultCode {
    ODEResultCode(ODEResultCodes code = ODEResultCodes::SUCCESS) : code(code) {}
    uint8_t code = SUCCESS;

    ODEResultCode operator|(const ODEResultCode &rhs) const {
        ODEResultCode result = *this;
        result |= rhs;
        return result;
    }
    ODEResultCode &operator|=(const ODEResultCode &rhs) {
        this->code |= rhs.code;
        return *this;
    }

    ODEResultCode operator&(const ODEResultCode &rhs) const {
        ODEResultCode result = *this;
        result &= rhs;
        return result;
    }
    ODEResultCode &operator&=(const ODEResultCode &rhs) {
        this->code &= rhs.code;
        return *this;
    }

    ODEResultCode operator^(const ODEResultCode &rhs) const {
        ODEResultCode result = *this;
        result ^= rhs;
        return result;
    }
    ODEResultCode &operator^=(const ODEResultCode &rhs) {
        this->code ^= rhs.code;
        return *this;
    }

    ODEResultCode &operator~() {
        this->code = ~this->code;
        return *this;
    }

    bool operator==(const ODEResultCode &rhs) const {
        return this->code == rhs.code;
    }

    explicit operator uint8_t() const { return this->code; }
    explicit operator bool() const { return this->code != 0; }

    void verbose() const {
        if (code & ODEResultCodes::MAXIMUM_ITERATIONS_EXCEEDED)
            std::cerr << ANSIColors::redb
                      << "Error: maximum number of iterations exceeded"
                      << ANSIColors::reset << std::endl;
        if (code & ODEResultCodes::MINIMUM_STEP_SIZE_REACHED)
            std::cerr << ANSIColors::yellow
                      << "Warning: minimum step size reached"
                      << ANSIColors::reset << std::endl;
    }
};

template <class V>
struct ODEResultX {
    std::vector<double> time;
    std::vector<V> solution;
    ODEResultCode resultCode = {};
    size_t iterations;
};