#pragma once

// STD
#include <iostream>
#include <queue>

// wxWidgets
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif 

// Boost
#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/nil_generator.hpp"
#include "boost/uuid/random_generator.hpp"
#include "boost/unordered_map.hpp"
#include "boost/signals2.hpp"

// Poco
#include <Poco/LocalDateTime.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>

// netlib
#include "netlib_core.h"
#include "netlib_server.h"

// zlib
#include "zlib.h"

// Protobuf
#include "Viewport.pb.h"