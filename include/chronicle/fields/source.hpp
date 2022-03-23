// This file is part of chronicle library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#ifdef CHRONICLE_USE_SYSTEM_UFORMAT

#include <uformat/texter.hpp>

#else

#include "../bundled/uformat/texter.hpp"

#endif


#include "../message.hpp"


namespace chronicle::fields {

  class source {
  public:
  
    template<class S, typename D, class TimePoint>
    void print(message<D, TimePoint> const& m, uformat::texter<S>& texter) {
        texter << '[' << m.source << ']';
    }
    
  }; // source

} // chronicle::fields
