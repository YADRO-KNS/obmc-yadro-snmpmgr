/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2021 YADRO.
 */

#include "dbus.hpp"
#include <sdbusplus/exception.hpp>

namespace phosphor
{
namespace dbus
{

sdbusplus::bus::bus bus = sdbusplus::bus::new_default();

constexpr auto ObjectMapperService = "xyz.openbmc_project.ObjectMapper";
constexpr auto ObjectMapperPath = "/xyz/openbmc_project/object_mapper";
constexpr auto ObjectMapperIface = "xyz.openbmc_project.ObjectMapper";

Services getObject(const Path& path, const Interfaces& ifaces)
{
    auto method = bus.new_method_call(ObjectMapperService, ObjectMapperPath,
                                      ObjectMapperIface, "GetObject");
    method.append(path, ifaces);

    Services services;
    try
    {
        bus.call(method).read(services);
    }
    catch (const sdbusplus::exception::SdBusError& ex)
    {
        fprintf(stderr, "ERROR: getObject() failed, %s\n", ex.what());
        throw;
    }
    return services;
}

ManagedObjects getManagedObjects(const ServiceName& service, const Path& path)
{
    auto method = bus.new_method_call(service.c_str(), path.c_str(),
                                      "org.freedesktop.DBus.ObjectManager",
                                      "GetManagedObjects");

    ManagedObjects objects;
    try
    {
        bus.call(method).read(objects);
    }
    catch (const sdbusplus::exception::SdBusError& ex)
    {
        fprintf(stderr, "ERROR: getManagedObjects() failed, %s\n", ex.what());
        throw;
    }
    return objects;
}

ServiceName getService(const Path& path, const IfaceName& iface)
{
    auto services = getObject(path, {iface});
    if (!services.empty())
    {
        return std::move(services.begin()->first);
    }
    return {};
}

} // namespace dbus
} // namespace phosphor
