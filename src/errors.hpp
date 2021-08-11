/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2021 YADRO.
 */
#pragma once

#include "strfmt.hpp"

#include <stdexcept>

class SNMPMgrError : public std::runtime_error
{
  public:
    using std::runtime_error::runtime_error;

    template <typename... Args>
    SNMPMgrError(const char* fmt, Args&&... args) :
        std::runtime_error::runtime_error(
            strfmt(fmt, std::forward<Args>(args)...))
    {}
};

class ObjectMapperNotFound : public SNMPMgrError
{
  public:
    ObjectMapperNotFound() :
        SNMPMgrError("Object mapper service not found!\n"
                     "Are you running this tool on OpenBMC?")
    {}
};

class ServiceNotFound : public SNMPMgrError
{
  public:
    ServiceNotFound() : SNMPMgrError("SNMP config manager service not found!")
    {}
};
