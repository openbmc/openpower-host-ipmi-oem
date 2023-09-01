// This file was autogenerated.  Do not edit!
// See elog-gen.py for more details
#pragma once

#include <phosphor-logging/elog.hpp>
#include <phosphor-logging/log.hpp>
#include <sdbusplus/exception.hpp>

#include <string>
#include <tuple>
#include <type_traits>

namespace sdbusplus::error::org::open_power::common::callout
{
struct Procedure;
} // namespace sdbusplus::error::org::open_power::common::callout
namespace sdbusplus::error::org::open_power::host
{
struct Event;
} // namespace sdbusplus::error::org::open_power::host
namespace sdbusplus::error::org::open_power::host
{
struct MaintenanceProcedure;
} // namespace sdbusplus::error::org::open_power::host
namespace sdbusplus::error::org::open_power::occ::metrics
{
struct Event;
} // namespace sdbusplus::error::org::open_power::occ::metrics
namespace sdbusplus::error::xyz::openbmc_project::common::callout
{
struct Device;
} // namespace sdbusplus::error::xyz::openbmc_project::common::callout
namespace sdbusplus::error::xyz::openbmc_project::common::callout
{
struct GPIO;
} // namespace sdbusplus::error::xyz::openbmc_project::common::callout
namespace sdbusplus::error::xyz::openbmc_project::common::callout
{
struct IIC;
} // namespace sdbusplus::error::xyz::openbmc_project::common::callout
namespace sdbusplus::error::xyz::openbmc_project::common::callout
{
struct IPMISensor;
} // namespace sdbusplus::error::xyz::openbmc_project::common::callout
namespace sdbusplus::error::xyz::openbmc_project::common::callout
{
struct Inventory;
} // namespace sdbusplus::error::xyz::openbmc_project::common::callout

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
    using type = std::tuple<std::decay_t<decltype("ESEL=%s")>, const char*>;
    explicit constexpr ESEL(const char* a) : _entry(entry("ESEL=%s", a)){};
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
using Event = phosphor::logging::org::open_power::occ::metrics::Event;
}
#endif

namespace details
{

template <>
struct map_exception_type<
    sdbusplus::error::org::open_power::occ::metrics::Event>
{
    using type = phosphor::logging::org::open_power::occ::metrics::Event;
};

} // namespace details
namespace org::open_power::common::callout
{
namespace _Procedure
{
struct PROCEDURE
{
    /*
     * We can't use -fsanitize=undefined if we declare a
     * 'static constexpr auto str' member, so don't. Instead, open-code the
     * mako template lookups.
     */
    static constexpr auto str_short = "PROCEDURE";
    using type = std::tuple<std::decay_t<decltype("PROCEDURE=%u")>, uint32_t>;
    explicit constexpr PROCEDURE(uint32_t a) :
        _entry(entry("PROCEDURE=%u", a)){};
    type _entry;
};
} // namespace _Procedure

struct Procedure
{
    static constexpr auto L = level::ERR;
    using PROCEDURE = _Procedure::PROCEDURE;
    using metadata_types = std::tuple<PROCEDURE>;
};

} // namespace org::open_power::common::callout

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace org::open_power::Common::Callout
{
using Procedure =
    phosphor::logging::org::open_power::common::callout::Procedure;
}
#endif

namespace details
{

template <>
struct map_exception_type<
    sdbusplus::error::org::open_power::common::callout::Procedure>
{
    using type = phosphor::logging::org::open_power::common::callout::Procedure;
};

} // namespace details
namespace xyz::openbmc_project::common::callout
{
namespace _Device
{
struct CALLOUT_ERRNO
{
    /*
     * We can't use -fsanitize=undefined if we declare a
     * 'static constexpr auto str' member, so don't. Instead, open-code the
     * mako template lookups.
     */
    static constexpr auto str_short = "CALLOUT_ERRNO";
    using type =
        std::tuple<std::decay_t<decltype("CALLOUT_ERRNO=%d")>, int32_t>;
    explicit constexpr CALLOUT_ERRNO(int32_t a) :
        _entry(entry("CALLOUT_ERRNO=%d", a)){};
    type _entry;
};
struct CALLOUT_DEVICE_PATH
{
    /*
     * We can't use -fsanitize=undefined if we declare a
     * 'static constexpr auto str' member, so don't. Instead, open-code the
     * mako template lookups.
     */
    static constexpr auto str_short = "CALLOUT_DEVICE_PATH";
    using type = std::tuple<std::decay_t<decltype("CALLOUT_DEVICE_PATH=%s")>,
                            const char*>;
    explicit constexpr CALLOUT_DEVICE_PATH(const char* a) :
        _entry(entry("CALLOUT_DEVICE_PATH=%s", a)){};
    type _entry;
};
} // namespace _Device

struct Device
{
    static constexpr auto L = level::ERR;
    using CALLOUT_ERRNO = _Device::CALLOUT_ERRNO;
    using CALLOUT_DEVICE_PATH = _Device::CALLOUT_DEVICE_PATH;
    using metadata_types = std::tuple<CALLOUT_ERRNO, CALLOUT_DEVICE_PATH>;
};

} // namespace xyz::openbmc_project::common::callout

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace xyz::openbmc_project::Common::Callout
{
using Device = phosphor::logging::xyz::openbmc_project::common::callout::Device;
}
#endif

namespace details
{

template <>
struct map_exception_type<
    sdbusplus::error::xyz::openbmc_project::common::callout::Device>
{
    using type =
        phosphor::logging::xyz::openbmc_project::common::callout::Device;
};

} // namespace details
namespace xyz::openbmc_project::common::callout
{
namespace _GPIO
{
struct CALLOUT_GPIO_NUM
{
    /*
     * We can't use -fsanitize=undefined if we declare a
     * 'static constexpr auto str' member, so don't. Instead, open-code the
     * mako template lookups.
     */
    static constexpr auto str_short = "CALLOUT_GPIO_NUM";
    using type =
        std::tuple<std::decay_t<decltype("CALLOUT_GPIO_NUM=%u")>, uint32_t>;
    explicit constexpr CALLOUT_GPIO_NUM(uint32_t a) :
        _entry(entry("CALLOUT_GPIO_NUM=%u", a)){};
    type _entry;
};
} // namespace _GPIO

struct GPIO
{
    static constexpr auto L = level::ERR;
    using CALLOUT_GPIO_NUM = _GPIO::CALLOUT_GPIO_NUM;
    using CALLOUT_ERRNO = phosphor::logging::xyz::openbmc_project::common::
        callout::Device::CALLOUT_ERRNO;
    using CALLOUT_DEVICE_PATH = phosphor::logging::xyz::openbmc_project::
        common::callout::Device::CALLOUT_DEVICE_PATH;
    using metadata_types =
        std::tuple<CALLOUT_GPIO_NUM, CALLOUT_ERRNO, CALLOUT_DEVICE_PATH>;
};

} // namespace xyz::openbmc_project::common::callout

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace xyz::openbmc_project::Common::Callout
{
using GPIO = phosphor::logging::xyz::openbmc_project::common::callout::GPIO;
}
#endif

namespace details
{

template <>
struct map_exception_type<
    sdbusplus::error::xyz::openbmc_project::common::callout::GPIO>
{
    using type = phosphor::logging::xyz::openbmc_project::common::callout::GPIO;
};

} // namespace details
namespace xyz::openbmc_project::common::callout
{
namespace _IIC
{
struct CALLOUT_IIC_BUS
{
    /*
     * We can't use -fsanitize=undefined if we declare a
     * 'static constexpr auto str' member, so don't. Instead, open-code the
     * mako template lookups.
     */
    static constexpr auto str_short = "CALLOUT_IIC_BUS";
    using type =
        std::tuple<std::decay_t<decltype("CALLOUT_IIC_BUS=%s")>, const char*>;
    explicit constexpr CALLOUT_IIC_BUS(const char* a) :
        _entry(entry("CALLOUT_IIC_BUS=%s", a)){};
    type _entry;
};
struct CALLOUT_IIC_ADDR
{
    /*
     * We can't use -fsanitize=undefined if we declare a
     * 'static constexpr auto str' member, so don't. Instead, open-code the
     * mako template lookups.
     */
    static constexpr auto str_short = "CALLOUT_IIC_ADDR";
    using type =
        std::tuple<std::decay_t<decltype("CALLOUT_IIC_ADDR=0x%hx")>, uint16_t>;
    explicit constexpr CALLOUT_IIC_ADDR(uint16_t a) :
        _entry(entry("CALLOUT_IIC_ADDR=0x%hx", a)){};
    type _entry;
};
} // namespace _IIC

struct IIC
{
    static constexpr auto L = level::ERR;
    using CALLOUT_IIC_BUS = _IIC::CALLOUT_IIC_BUS;
    using CALLOUT_IIC_ADDR = _IIC::CALLOUT_IIC_ADDR;
    using CALLOUT_ERRNO = phosphor::logging::xyz::openbmc_project::common::
        callout::Device::CALLOUT_ERRNO;
    using CALLOUT_DEVICE_PATH = phosphor::logging::xyz::openbmc_project::
        common::callout::Device::CALLOUT_DEVICE_PATH;
    using metadata_types = std::tuple<CALLOUT_IIC_BUS, CALLOUT_IIC_ADDR,
                                      CALLOUT_ERRNO, CALLOUT_DEVICE_PATH>;
};

} // namespace xyz::openbmc_project::common::callout

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace xyz::openbmc_project::Common::Callout
{
using IIC = phosphor::logging::xyz::openbmc_project::common::callout::IIC;
}
#endif

namespace details
{

template <>
struct map_exception_type<
    sdbusplus::error::xyz::openbmc_project::common::callout::IIC>
{
    using type = phosphor::logging::xyz::openbmc_project::common::callout::IIC;
};

} // namespace details
namespace xyz::openbmc_project::common::callout
{
namespace _Inventory
{
struct CALLOUT_INVENTORY_PATH
{
    /*
     * We can't use -fsanitize=undefined if we declare a
     * 'static constexpr auto str' member, so don't. Instead, open-code the
     * mako template lookups.
     */
    static constexpr auto str_short = "CALLOUT_INVENTORY_PATH";
    using type = std::tuple<std::decay_t<decltype("CALLOUT_INVENTORY_PATH=%s")>,
                            const char*>;
    explicit constexpr CALLOUT_INVENTORY_PATH(const char* a) :
        _entry(entry("CALLOUT_INVENTORY_PATH=%s", a)){};
    type _entry;
};
} // namespace _Inventory

struct Inventory
{
    static constexpr auto L = level::ERR;
    using CALLOUT_INVENTORY_PATH = _Inventory::CALLOUT_INVENTORY_PATH;
    using metadata_types = std::tuple<CALLOUT_INVENTORY_PATH>;
};

} // namespace xyz::openbmc_project::common::callout

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace xyz::openbmc_project::Common::Callout
{
using Inventory =
    phosphor::logging::xyz::openbmc_project::common::callout::Inventory;
}
#endif

namespace details
{

template <>
struct map_exception_type<
    sdbusplus::error::xyz::openbmc_project::common::callout::Inventory>
{
    using type =
        phosphor::logging::xyz::openbmc_project::common::callout::Inventory;
};

} // namespace details
namespace xyz::openbmc_project::common::callout
{
namespace _IPMISensor
{
struct CALLOUT_IPMI_SENSOR_NUM
{
    /*
     * We can't use -fsanitize=undefined if we declare a
     * 'static constexpr auto str' member, so don't. Instead, open-code the
     * mako template lookups.
     */
    static constexpr auto str_short = "CALLOUT_IPMI_SENSOR_NUM";
    using type =
        std::tuple<std::decay_t<decltype("CALLOUT_IPMI_SENSOR_NUM=%u")>,
                   uint32_t>;
    explicit constexpr CALLOUT_IPMI_SENSOR_NUM(uint32_t a) :
        _entry(entry("CALLOUT_IPMI_SENSOR_NUM=%u", a)){};
    type _entry;
};
} // namespace _IPMISensor

struct IPMISensor
{
    static constexpr auto L = level::ERR;
    using CALLOUT_IPMI_SENSOR_NUM = _IPMISensor::CALLOUT_IPMI_SENSOR_NUM;
    using metadata_types = std::tuple<CALLOUT_IPMI_SENSOR_NUM>;
};

} // namespace xyz::openbmc_project::common::callout

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace xyz::openbmc_project::Common::Callout
{
using IPMISensor =
    phosphor::logging::xyz::openbmc_project::common::callout::IPMISensor;
}
#endif

namespace details
{

template <>
struct map_exception_type<
    sdbusplus::error::xyz::openbmc_project::common::callout::IPMISensor>
{
    using type =
        phosphor::logging::xyz::openbmc_project::common::callout::IPMISensor;
};

} // namespace details
namespace org::open_power::host
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
    using type = std::tuple<std::decay_t<decltype("ESEL=%s")>, const char*>;
    explicit constexpr ESEL(const char* a) : _entry(entry("ESEL=%s", a)){};
    type _entry;
};
} // namespace _Event

struct Event
{
    static constexpr auto L = level::ERR;
    using ESEL = _Event::ESEL;
    using CALLOUT_INVENTORY_PATH = phosphor::logging::xyz::openbmc_project::
        common::callout::Inventory::CALLOUT_INVENTORY_PATH;
    using metadata_types = std::tuple<ESEL, CALLOUT_INVENTORY_PATH>;
};

} // namespace org::open_power::host

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace org::open_power::Host
{
using Event = phosphor::logging::org::open_power::host::Event;
}
#endif

namespace details
{

template <>
struct map_exception_type<sdbusplus::error::org::open_power::host::Event>
{
    using type = phosphor::logging::org::open_power::host::Event;
};

} // namespace details
namespace org::open_power::host
{
namespace _MaintenanceProcedure
{
struct ESEL
{
    /*
     * We can't use -fsanitize=undefined if we declare a
     * 'static constexpr auto str' member, so don't. Instead, open-code the
     * mako template lookups.
     */
    static constexpr auto str_short = "ESEL";
    using type = std::tuple<std::decay_t<decltype("ESEL=%s")>, const char*>;
    explicit constexpr ESEL(const char* a) : _entry(entry("ESEL=%s", a)){};
    type _entry;
};
} // namespace _MaintenanceProcedure

struct MaintenanceProcedure
{
    static constexpr auto L = level::ERR;
    using ESEL = _MaintenanceProcedure::ESEL;
    using PROCEDURE = phosphor::logging::org::open_power::common::callout::
        Procedure::PROCEDURE;
    using metadata_types = std::tuple<ESEL, PROCEDURE>;
};

} // namespace org::open_power::host

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace org::open_power::Host
{
using MaintenanceProcedure =
    phosphor::logging::org::open_power::host::MaintenanceProcedure;
}
#endif

namespace details
{

template <>
struct map_exception_type<
    sdbusplus::error::org::open_power::host::MaintenanceProcedure>
{
    using type = phosphor::logging::org::open_power::host::MaintenanceProcedure;
};

} // namespace details
} // namespace phosphor::logging
