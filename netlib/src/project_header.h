#pragma once

// stl
#include <thread>
#include <functional>
#include <vector>

// netlib
#include "netlib_log.h"

// boost
#include "boost/shared_ptr.hpp"
#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/asio/strand.hpp"
#include "boost/bind.hpp"

// protobuf
#include "Protocol.pb.h"
