#pragma once

// wxWidgets
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif 

#include <wx/mstream.h>

// Poco
#include <Poco/LocalDateTime.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>

// netlib
#include "netlib.h"

// Protobuf
#include "Viewport.pb.h"