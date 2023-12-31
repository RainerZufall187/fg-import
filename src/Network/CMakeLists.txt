include(FlightGearComponent)

set(SOURCES
	ATC-Inputs.cxx
	ATC-Main.cxx
	ATC-Outputs.cxx
	AV400.cxx
	AV400Sim.cxx
	AV400WSim.cxx
	atlas.cxx
	garmin.cxx
	generic.cxx
	HTTPClient.cxx
	DNSClient.cxx
	flarm.cxx
	igc.cxx
	joyclient.cxx
	jsclient.cxx
	lfsglass.cxx
	native.cxx
        native_structs.cxx
	native_ctrls.cxx
	native_fdm.cxx
	native_gui.cxx
	nmea.cxx
	opengc.cxx
	props.cxx
	protocol.cxx
	pve.cxx
	ray.cxx
	rul.cxx
	)

set(HEADERS
	ATC-Inputs.hxx
	ATC-Main.hxx
	ATC-Outputs.hxx
	AV400.hxx
	AV400Sim.hxx
	AV400WSim.hxx
	atlas.hxx
	garmin.hxx
	generic.hxx
	HTTPClient.hxx
	DNSClient.hxx
	flarm.hxx
	igc.hxx
	joyclient.hxx
	jsclient.hxx
	lfsglass.hxx
	native.hxx
        native_ctrls.hxx
	native_ctrls.hxx
	native_fdm.hxx
	native_gui.hxx
	nmea.hxx
	opengc.hxx
	props.hxx
	protocol.hxx
	pve.hxx
	ray.hxx
	rul.hxx
	)

if (CycloneDDS_FOUND)
  list(APPEND SOURCES
        dds_props.cxx
        )

  list(APPEND HEADERS
        dds_props.hxx
        )
endif()

if(ENABLE_IAX)
    list(APPEND SOURCES fgcom.cxx)
    list(APPEND HEADERS fgcom.hxx)
endif()

flightgear_component(Network "${SOURCES}" "${HEADERS}")

if (CycloneDDS_FOUND)
  add_subdirectory(DDS)
endif()

if(RTI_FOUND)
  add_subdirectory(HLA)
endif()

add_subdirectory(http)

if(ENABLE_SWIFT)
	add_subdirectory(Swift)
endif()
