// AISwiftAircraft.h - Derived AIBase class for swift aircraft
//
// Copyright (C) 2020 - swift Project Community / Contributors (http://swift-project.org/)
// Written by Lars Toenning <dev@ltoenning.de> started on April 2020.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#pragma once

#include <string>
#include <string_view>
#include <utility>

#include "AIBaseAircraft.hxx"

struct AircraftTransponder
{
    AircraftTransponder(std::string  callsign, int code, bool modeC, bool ident)
        : callsign(std::move(callsign)), code(code), modeC(modeC), ident(ident)
    {}

    std::string callsign;
    int code;
    bool modeC;
    bool ident;
};

struct AircraftSurfaces
{
    AircraftSurfaces(std::string callsign, double gear, double flaps, double spoilers, double speedBrake, double slats, double wingSweeps,
                     double thrust, double elevator, double rudder, double aileron, bool landingLight, bool taxiLight, bool beaconLight,
                     bool strobeLight, bool navLight, int lightPattern)
        : callsign(std::move(callsign)), gear(gear), flaps(flaps), spoilers(spoilers), speedBrake(speedBrake), slats(slats), wingSweeps(wingSweeps),
        thrust(thrust), elevator(elevator), rudder(rudder), aileron(aileron), landingLight(landingLight), taxiLight(taxiLight), beaconLight(beaconLight),
        strobeLight(strobeLight), navLight(navLight), lightPattern(lightPattern){}

    std::string callsign;
    double gear;
    double flaps;
    double spoilers;
    double speedBrake;
    double slats;
    double wingSweeps;
    double thrust;
    double elevator;
    double rudder;
    double aileron;
    bool landingLight;
    bool taxiLight;
    bool beaconLight;
    bool strobeLight;
    bool navLight;
    int lightPattern;
};

class FGAISwiftAircraft : public FGAIBaseAircraft
{
public:
    FGAISwiftAircraft(const std::string& callsign, const std::string& modelString);
    virtual ~FGAISwiftAircraft() = default;

    string_view getTypeString() const override { return "swift"; }
    void update(double dt) override;

    void updatePosition(const SGGeod &position, const SGVec3d &orientation, double groundspeed, bool initPos);
    double getGroundElevation(const SGGeod& pos) const;
    void initProps();
    void setPlaneSurface(const AircraftSurfaces& surfaces);
    void setPlaneTransponder(const AircraftTransponder& transponder);

private:
    bool m_initPos = false;
    // Property Nodes for transponder and parts
    SGPropertyNode_ptr m_transponderCodeNode;
    SGPropertyNode_ptr m_transponderCModeNode;
    SGPropertyNode_ptr m_transponderIdentNode;
};
