#!/bin/sh

set -eu

FRAME_ID=$1
FRAME_ID_CAPITALIZED=$(echo "$1" | tr '[:lower:]' '[:upper:]')

HEADER_FILE="src/ui/$FRAME_ID.hh"
SOURCE_FILE="src/ui/$FRAME_ID.cc"

cat <<EOF > "$HEADER_FILE"
#ifndef __${FRAME_ID_CAPITALIZED}__
#define __${FRAME_ID_CAPITALIZED}__

#include "UIBase.h"

class $FRAME_ID : public ${FRAME_ID}UI {
    DECLARE_CLASS($FRAME_ID)
    DECLARE_EVENT_TABLE();

private:

public:
    $FRAME_ID(wxWindow* present, wxWindowID id = wxID_ANY);
    ~$FRAME_ID();
};

#endif  // __${FRAME_ID_CAPITALIZED}__
EOF

cat <<EOF > "$SOURCE_FILE"
#include "$FRAME_ID.hh"

$FRAME_ID::$FRAME_ID(wxWindow* parent, wxWindowID id) : ${FRAME_ID}UI(parent, id)
{

}

$FRAME_ID::~$FRAME_ID()
{

}

IMPLEMENT_CLASS($FRAME_ID, wxFrame)

BEGIN_EVENT_TABLE($FRAME_ID, wxFrame)
END_EVENT_TABLE()
EOF
