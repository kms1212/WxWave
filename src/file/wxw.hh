#ifndef __WXW_HH__
#define __WXW_HH__

#include <map>
#include <string>
#include <vector>

#include <ghdl/libghw.h>
#include <libxml/parser.h>

#include "trace.hh"

class WhwFile {
public:
    WhwFile(const std::string& path);
    ~WhwFile();
};

#endif // __WXW_HH__
