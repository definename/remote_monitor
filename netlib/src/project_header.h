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
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/nil_generator.hpp"
#include "boost/uuid/random_generator.hpp"
#include "boost/uuid/uuid_io.hpp"

// protobuf
#include "Protocol.pb.h"
