if !INSTALL_ERROR_YAML
liboemhandlerdir = ${libdir}/ipmid-providers
liboemhandler_LTLIBRARIES = liboemhandler.la

liboemhandler_la_SOURCES = oemhandler.cpp \
                           host-interface.cpp \
                           org/open_power/OCC/Metrics/error.cpp

BUILT_SOURCES = org/open_power/OCC/Metrics/error.hpp \
                org/open_power/OCC/Metrics/error.cpp

if GEN_ERRORS
nobase_nodist_include_HEADERS = \
                 phosphor-logging/elog-errors.hpp
BUILT_SOURCES += phosphor-logging/elog-errors.hpp
endif
CLEANFILES = ${BUILT_SOURCES}

liboemhandler_la_LDFLAGS = $(SYSTEMD_LIBS)\
                           $(SDBUSPLUS_LIBS) \
                           $(OPENPOWER_DBUS_INTERFACES_LIBS) \
                           $(PHOSPHOR_LOGGING_LIBS) \
                           -version-info 0:0:0 -shared

liboemhandler_la_CXXFLAGS = $(SYSTEMD_CFLAGS) \
                            $(SDBUSPLUS_CFLAGS) \
                            $(OPENPOWER_DBUS_INTERFACES_CFLAGS) \
                            $(PHOSPHOR_LOGGING_CFLAGS) \
                            $(AM_CXXFLAGS)

org/open_power/OCC/Metrics/error.hpp: ${top_srcdir}/org/open_power/OCC/Metrics.errors.yaml
	@mkdir -p `dirname $@`
	$(SDBUSPLUSPLUS) -r $(top_srcdir) error exception-header org.open_power.OCC.Metrics > $@

org/open_power/OCC/Metrics/error.cpp: ${top_srcdir}/org/open_power/OCC/Metrics.errors.yaml
	@mkdir -p `dirname $@`
	$(SDBUSPLUSPLUS) -r $(top_srcdir) error exception-cpp org.open_power.OCC.Metrics > $@
endif
# Export error YAML to shared location
yamldir = ${datadir}/phosphor-dbus-yaml/yaml
nobase_yaml_DATA = \
	org/open_power/OCC/Metrics.errors.yaml \
	org/open_power/OCC/Metrics.metadata.yaml

# Generate phosphor-logging/elog-errors.hpp
if GEN_ERRORS
ELOG_MAKO ?= elog-gen-template.mako.hpp
ELOG_DIR ?= ${OECORE_NATIVE_SYSROOT}${datadir}/phosphor-logging/elog
ELOG_GEN_DIR ?= ${ELOG_DIR}/tools/
ELOG_MAKO_DIR ?= ${ELOG_DIR}/tools/phosphor-logging/templates/
YAML_DIR ?= ${OECORE_NATIVE_SYSROOT}${datadir}/phosphor-dbus-yaml/yaml
phosphor-logging/elog-errors.hpp:
	@mkdir -p ${YAML_DIR}/org/open_power/OCC/
	@cp ${top_srcdir}/org/open_power/OCC/Metrics.errors.yaml ${YAML_DIR}/org/open_power/OCC/Metrics.errors.yaml
	@cp ${top_srcdir}/org/open_power/OCC/Metrics.metadata.yaml ${YAML_DIR}/org/open_power/OCC/Metrics.metadata.yaml
	@mkdir -p `dirname $@`
	@chmod 777 $(ELOG_GEN_DIR)/elog-gen.py
	$(AM_V_at)$(PYTHON) $(ELOG_GEN_DIR)/elog-gen.py -y ${YAML_DIR} -t ${ELOG_MAKO_DIR} -m ${ELOG_MAKO} -o $@
endif

