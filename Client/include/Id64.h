#pragma once

class Id64 {
public:
    Id64(uint64_t value) {
        m_Value = value;
    }
    Id64(uint32_t index, uint32_t generation) {
        m_Value = (uint64_t(generation) << 32) | uint64_t(index);
    }
    uint32_t Index() const {
        return uint32_t(value);
    }

    uint32_t Generation() const {
        return uint32_t(value >> 32);
    }
    uint64_t m_Value;
};