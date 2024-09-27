#ifndef __TIME_HH__
#define __TIME_HH__

#include <cmath>
#include <stdint.h>

#define STEP_UNIT FS

struct Time {
public:
    enum timeunit { FS = 0, PS, NS, US, MS, S };

    int64_t steps = 0;

    explicit Time(int64_t steps)
        : steps(steps)
    {
    }

    Time(double time, enum timeunit unit)
        : steps(ConvertTimeIntoSteps(time, unit))
    {
    }

    int getSteps() const { return this->steps; }
    void setSteps(int64_t steps) { this->steps = steps; }

    double getTime(enum timeunit unit) const
    {
        return ConvertStepsIntoTime(this->steps, unit);
    }
    void setTime(double time, enum timeunit unit)
    {
        this->steps = ConvertTimeIntoSteps(time, unit);
    }

    const Time operator+(const Time rhs) const
    {
        return Time(this->steps + rhs.steps);
    }

    const Time operator-(const Time rhs) const
    {
        return Time(this->steps - rhs.steps);
    }

    const Time operator*(const int rhs) const
    {
        return Time(this->steps * rhs);
    }

    const Time operator*(const double rhs) const
    {
        return Time((int)((double)this->steps * rhs));
    }

    const Time operator/(const int rhs) const
    {
        return Time(this->steps / rhs);
    }

    const Time operator/(const double rhs) const
    {
        return Time((int)((double)this->steps / rhs));
    }

    bool operator==(const Time rhs) const { return this->steps == rhs.steps; }
    bool operator!=(const Time rhs) const { return !(*this == rhs); }
    bool operator<(const Time rhs) const { return this->steps < rhs.steps; }
    bool operator<=(const Time rhs) const { return this->steps <= rhs.steps; }
    bool operator>(const Time rhs) const { return this->steps > rhs.steps; }
    bool operator>=(const Time rhs) const { return this->steps >= rhs.steps; }

    static double ConvertStepsIntoTime(int64_t steps, enum timeunit unit)
    {
        double time = steps;
        if (unit < STEP_UNIT) {
            for (int i = unit; i < STEP_UNIT; i++) {
                time *= 1000;
            }
        } else {
            for (int i = unit; i > STEP_UNIT; i--) {
                time /= 1000;
            }
        }

        return std::round(time);
    }

    static int ConvertTimeIntoSteps(double time, enum timeunit unit)
    {
        if (unit < STEP_UNIT) {
            // when the given time is shorter than step unit
            for (int i = unit; i < STEP_UNIT; i++) {
                time /= 1000;
            }
        } else {
            // when the give ntime is longer than step unit
            for (int i = unit; i > STEP_UNIT; i--) {
                time *= 1000;
            }
        }

        return std::round(time);
    }
};

#endif // __TIME_HH__
