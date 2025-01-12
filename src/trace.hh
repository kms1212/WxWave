#ifndef __TRACE_HH__
#define __TRACE_HH__

#include <memory>
#include <type_traits>
#include <vector>

#include "time.hh"

class Trace {
private:
public:
    enum AppearanceType {
        AT_LOW = 0,
        AT_HIGH,
        AT_MID,
        AT_BOTH,
        AT_UNKNOWN
    };

    virtual ~Trace() = default;

    virtual void setCurrentTime(Time time) = 0;
    virtual Time getCurrentTime() const = 0;
    virtual bool seekNextTransition() = 0;

    virtual AppearanceType getAppearanceType() const = 0;
    virtual char getAppearanceChar() const = 0;
};

#endif // __TRACE_HH__
