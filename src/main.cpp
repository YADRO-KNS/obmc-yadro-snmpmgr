/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2021 YADRO.
 */

#include "dbus.hpp"

#include <CLI/CLI.hpp>

constexpr auto SNMPManagerPath = "/xyz/openbmc_project/network/snmp/manager";
constexpr auto SNMPManagerIface = "xyz.openbmc_project.Network.Client.Create";
constexpr auto SNMPReceiverIface = "xyz.openbmc_project.Network.Client";
constexpr auto SNMPMgrPathLen = std::char_traits<char>::length(SNMPManagerPath);

/**
 * @brief Show all configured SNMP trap receivers`
 */
void listReceivers()
{
    auto service =
        phosphor::dbus::getService(SNMPManagerPath, SNMPManagerIface);
    auto objects = phosphor::dbus::getManagedObjects(service, SNMPManagerPath);

    printf("List configured SNMP trap receivers\n");
    for (const auto& [path, ifaces] : objects)
    {
        auto it = ifaces.find(SNMPReceiverIface);
        if (it != ifaces.end())
        {
            const auto& addr = std::get<std::string>(it->second.at("Address"));
            const auto& port = std::get<uint16_t>(it->second.at("Port"));

            printf("%5s %15s %d\n", path.str.c_str() + SNMPMgrPathLen + 1,
                   addr.c_str(), port);
        }
    }

    if (objects.empty())
    {
        printf("     (empty)\n");
    }
}

/**
 * @brief Add new SNMP trap receiver
 *
 * @param address - Receiver address
 * @param port - Receiver UDP port
 */
void addReceiver(const std::string& address, uint16_t port)
{
    auto service =
        phosphor::dbus::getService(SNMPManagerPath, SNMPManagerIface);
    auto method = phosphor::dbus::bus.new_method_call(
        service.c_str(), SNMPManagerPath, SNMPManagerIface, "Client");
    method.append(address, port);

    phosphor::dbus::Path path;
    phosphor::dbus::bus.call(method).read(path);

    printf("Receiver #%s added: address=%s, port=%d\n",
           path.c_str() + SNMPMgrPathLen + 1, address.c_str(), port);
}

/**
 * @brief Remove configured SNMP trap receiver
 *
 * @param index - Receiver index in the list
 */
void dropReceiver(size_t index)
{
    phosphor::dbus::Path path = SNMPManagerPath;
    path += "/";
    path += std::to_string(index);

    auto service = phosphor::dbus::getService(path, SNMPReceiverIface);
    auto method = phosphor::dbus::bus.new_method_call(
        service.c_str(), path.c_str(), "xyz.openbmc_project.Object.Delete",
        "Delete");
    phosphor::dbus::bus.call_noreply(method);

    printf("SNMP trap receiver #%zu removed!\n", index);
}

/**
 * @brief Application entry point
 *
 * @param argc   - Number of command line arguments
 * @param argv[] - Array of command line arguments
 *
 * @return - Return code
 */
int main(int argc, char* argv[])
{
    CLI::App app("SNMP trap receivers manager");

#if REMOTE_HOST
    app.add_option_function<std::string>(
        "--host",
        [](const std::string& host) {
            if (!host.empty())
            {
                printf("Open D-Bus session on %s\n", host.c_str());
                sd_bus* b = nullptr;
                sd_bus_open_system_remote(&b, host.c_str());
                phosphor::dbus::bus = sdbusplus::bus::bus(b, std::false_type());
            }
        },
        "Operate on remote host");
#endif

    CLI::App* list = app.add_subcommand("list", "List configured receivers");
    list->callback(listReceivers);

    constexpr auto DefaultSNMPTrapPort = 162;

    std::string addr;
    uint16_t port{DefaultSNMPTrapPort};
    CLI::App* add = app.add_subcommand("add", "Add a new receiver");
    add->add_option("address", addr, "Receiver address")
        ->check(CLI::ValidIPV4)
        ->required();
    add->add_option("port", port, "Receiver port", true);
    add->callback([&addr, &port]() { addReceiver(addr, port); });

    size_t index{0};
    CLI::App* drop = app.add_subcommand("drop", "Remove specified receiver");
    drop->add_option("index", index, "Receiver index in the list")->required();
    drop->callback([&index]() { dropReceiver(index); });

    app.require_subcommand();

    try
    {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError& ex)
    {
        return app.exit(ex);
    }

    return EXIT_SUCCESS;
}
