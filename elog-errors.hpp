// This file was autogenerated.  Do not edit!
// See elog-gen.py for more details
#pragma once

#include <phosphor-logging/elog.hpp>
#include <phosphor-logging/log.hpp>
#include <sdbusplus/exception.hpp>
#include <string>
#include <tuple>
#include <type_traits>

namespace sdbusplus::error::org::open_power::occ::metrics
{
struct Event;
} // namespace sdbusplus::error::org::open_power::occ::metrics

namespace phosphor::logging
{

namespace org::open_power::occ::metrics
{
namespace _Event
{
struct ESEL
{
    /*
     * We can't use -fsanitize=undefined if we declare a
     * 'static constexpr auto str' member, so don't. Instead, open-code the
     * mako template lookups.
     */
    static constexpr auto str_short = "ESEL";
    using type = std::tuple<std::decay_t<decltype("ESEL=%s")>,const char*>;
    explicit constexpr ESEL(const char* a) : _entry(entry("ESEL=%s", a)) {};
    type _entry;
};
} // namespace _Event

struct Event
{
    static constexpr auto L = level::INFO;
    using ESEL = _Event::ESEL;
    using metadata_types = std::tuple<ESEL>;
};

} // namespace org::open_power::occ::metrics

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace org::open_power::OCC::Metrics
{
using Event =
    phosphor::logging::org::open_power::occ::metrics::Event;
}
#endif

namespace details
{

template <>
struct map_exception_type<sdbusplus::error::org::open_power::occ::metrics::Event>
{
    using type =
        phosphor::logging::org::open_power::occ::metrics::Event;
};

} // namespace details
} // namespace phosphor::logging
