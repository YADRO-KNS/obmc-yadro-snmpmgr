/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2021 YADRO.
 */
#pragma once

#include <sdbusplus/bus.hpp>

namespace phosphor
{
namespace dbus
{

using PropName = std::string;
using PropValue = std::variant<uint16_t, std::string>;
using Properties = std::map<PropName, PropValue>;
using IfaceName = std::string;
using IfaceProperties = std::map<IfaceName, Properties>;
using ManagedObjects =
    std::map<sdbusplus::message::object_path, IfaceProperties>;

using Path = std::string;
using ServiceName = std::string;
using Interfaces = std::vector<IfaceName>;
using Services = std::map<ServiceName, Interfaces>;

/**
 * @brief Obtain a dictionary of serivce -> implemented interface(s) for the
 *        given path.
 *
 * @param path   - The object path for which the result should be fetched.
 * @param ifaces - An array of result set constraining interfaces.
 *
 * @return A dictionary of service -> implemented interface(s).
 */
Services getObject(const Path& path, const Interfaces& ifaces);

/**
 * @brief Obtain a dictionary of object path -> properties mapped by interface
 *
 * @param service - The service provides the properties
 * @param path    - The root object path of the service
 *
 * @return A dictionary of object path -> implemented properties groupped by
 *         interface.
 */
ManagedObjects getManagedObjects(const ServiceName& service, const Path& path);

/**
 * @brief Obtain the service name working in the specified object path and
 *        implemented specified interface.
 *
 * @param path - The object path of service
 * @param iface - The implemented interface
 *
 * @return The service name
 */
ServiceName getService(const Path& path, const IfaceName& iface);

/**
 * @brief The D-Bus connection object.
 */
extern sdbusplus::bus::bus bus;

} // namespace dbus
} // namespace phosphor
