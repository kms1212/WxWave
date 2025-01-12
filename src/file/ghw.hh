#ifndef __GHW_HH__
#define __GHW_HH__

#include <map>
#include <string>
#include <vector>

#include <ghdl/libghw.h>

#include "wavefile.hh"
#include "trace.hh"

struct GhwTraceData {
    Time time;
    Trace::AppearanceType type;
    char ch;

    GhwTraceData(Time time, Trace::AppearanceType type, char ch)
        : time(time)
        , type(type)
        , ch(ch)
    {
    }
};

class GhwFile;
class GhwHierarchy;

class GhwTraceHandle : public Trace {
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
    
    virtual Trace::AppearanceType getAppearanceType() const override;
    virtual char getAppearanceChar() const override;
};

class GhwHierarchy : public WaveFileHierarchy {
private:
    const GhwFile& file;
    struct ghw_handler* h;
    struct ghw_hie* hie;

    std::string kind;
    std::string name;
    std::string subtype;
    std::vector<std::string> packages;
    std::map<std::string, std::string> properties;

    std::vector<std::shared_ptr<const GhwHierarchy>> children;
    const GhwHierarchy* parent;

    int sig_index_start = -1, sig_index_end = -1;
    const std::vector<GhwTraceData>* trace_data;

public:
    GhwHierarchy(const GhwHierarchy* parent, const GhwFile& file,
        struct ghw_handler* h, struct ghw_hie* hie);

    virtual std::string getName() const override;
    virtual std::string getPath() const override;
    virtual std::string getSubtype() const override;
    virtual std::map<std::string, std::string> getProperties() const override;

    virtual std::vector<const WaveFileHierarchy*> getChildren() const override;
    virtual const GhwHierarchy* getParent() const override;

    bool getSignalIndexRange(unsigned int* start, unsigned int* end) const;
};

class GhwFile : public WaveFile {
private:
    struct ghw_handler handler;
    std::shared_ptr<const GhwHierarchy> top;

    std::vector<std::vector<GhwTraceData>> trace_data;

    Time end_time;

    int openHandle(const std::string& path);
    void closeHandle();

    void loadData();

public:
    GhwFile(const std::string& path);
    ~GhwFile();

    virtual const GhwHierarchy* getTop() const override;
    virtual Time getEndTime() const override;

    const std::vector<GhwTraceData>& getTraceData(unsigned int signal) const;
};

#endif // __GHW_HH__
