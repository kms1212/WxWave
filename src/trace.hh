#ifndef __TRACE_HH__
#define __TRACE_HH__

#include <memory>
#include <type_traits>
#include <vector>

#include "time.hh"

#define SIG_X_BOOL false
#define SIG_Z_BOOL false

template <typename T> class Trace {
private:
public:
    virtual ~Trace() = default;

    virtual void setCurrentTime(Time time) = 0;
    virtual Time getCurrentTime() const = 0;
    virtual bool seekNextTransition() = 0;
    virtual T getValue() const = 0;
};

struct LogicValue {
    enum Value { V_0 = 0, V_1, V_X, V_Z };

    Value value;

    explicit LogicValue(Value value)
        : value(value)
    {
    }

    explicit LogicValue(char ch)
    {
        switch (ch) {
        case '0':
            this->value = V_0;
            break;
        case '1':
            this->value = V_1;
            break;
        case 'Z':
            this->value = V_Z;
            break;
        default:
            this->value = V_X;
        }
    }

    bool operator==(const LogicValue rhs) const
    {
        return this->value == rhs.value;
    }

    bool operator==(const Value rhs) const { return this->value == rhs; }

    bool operator!=(const LogicValue rhs) const { return !(*this == rhs); }
    bool operator!=(const Value rhs) const { return !(*this == rhs); }

    explicit operator bool() const
    {
        const bool table[] = { false, true, SIG_X_BOOL, SIG_Z_BOOL };
        return table[this->value];
    }

    explicit operator char() const
    {
        const char table[] = { '0', '1', 'X', 'Z' };
        return table[this->value];
    }
};

#endif // __TRACE_HH__
