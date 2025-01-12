#ifndef __WAVEFILE_HH__
#define __WAVEFILE_HH__

#include <map>
#include <string>
#include <vector>

#include "trace.hh"

class WaveFileHierarchy {
public:
    virtual ~WaveFileHierarchy() = default;

    virtual std::string getName() const = 0;
    virtual std::string getPath() const = 0;
    virtual std::string getSubtype() const = 0;
    virtual std::map<std::string, std::string> getProperties() const = 0;

    virtual std::vector<const WaveFileHierarchy*> getChildren() const = 0;
    virtual const WaveFileHierarchy* getParent() const = 0;
};

class WaveFile {
public:
    virtual ~WaveFile() = default;

    virtual const WaveFileHierarchy* getTop() const = 0;
    virtual Time getEndTime() const = 0;
};

#endif // __WAVEFILE_HH__
