#ifndef __GHW_HH__
#define __GHW_HH__

#include <map>
#include <string>
#include <vector>

#include <ghdl/libghw.h>

#include "trace.hh"

struct GhwTraceData {
    Time time;
    LogicValue value;

    GhwTraceData(Time time, LogicValue value)
        : time(time)
        , value(value)
    {
    }
};

class GhwFile;
class GhwHierarchy;

class GhwTraceHandle : public Trace<LogicValue> {
private:
    const std::vector<GhwTraceData>& trace_data;
    int current_data_index;
    Time current_time;

public:
    GhwTraceHandle(const GhwFile& file, unsigned int sig_index);
    ~GhwTraceHandle();

    virtual void setCurrentTime(Time time) override;
    virtual Time getCurrentTime() const override;
    virtual bool seekNextTransition() override;
    virtual LogicValue getValue() const override;
};

class GhwHierarchy {
private:
    const GhwFile& file;
    struct ghw_handler* h;
    struct ghw_hie* hie;

    std::string kind;
    std::string name;
    std::string subtype;
    std::vector<std::string> packages;
    std::map<std::string, std::string> properties;

    std::vector<GhwHierarchy> children;

    int sig_index_start = -1, sig_index_end = -1;
    const std::vector<GhwTraceData>* trace_data;

public:
    GhwHierarchy(
        const GhwFile& file, struct ghw_handler* h, struct ghw_hie* hie);

    const std::string& getKind() const;
    const std::string& getName() const;
    const std::string& getSubtype() const;
    const std::vector<std::string>& getPackages() const;
    const std::map<std::string, std::string>& getProperties() const;

    const std::vector<GhwHierarchy>& getChildren() const;

    bool getSignalIndexRange(unsigned int* start, unsigned int* end) const;
};

class GhwFile {
private:
    struct ghw_handler handler;
    std::vector<std::string> string_table;
    std::vector<std::string> type_table;
    std::unique_ptr<GhwHierarchy> top;

    std::vector<std::vector<GhwTraceData>> trace_data;

    Time end_time;

    int openHandle(const std::string& path);
    void closeHandle();

    void loadData();

public:
    GhwFile(const std::string& path);
    ~GhwFile();

    const std::vector<std::string>& getStringTable() const;
    const std::vector<std::string>& getTypeTable() const;

    const GhwHierarchy& getTop() const;

    const std::vector<GhwTraceData>& getTraceData(unsigned int signal) const;
    Time getEndTime() const;
};

#endif // __GHW_HH__
