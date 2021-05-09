#pragma once

// STD
#include <iostream>

// wxWidgets
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif 

// Boost
#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"

// Poco
#include <Poco/LocalDateTime.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>

// netlist
#include "netlib_core.h"
#include "netlib_server.h"

// zlib
#include "zlib.h"

// Protobuf
#include "Boss.pb.h"