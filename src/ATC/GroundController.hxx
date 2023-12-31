// GroundController.hxx - forked from groundnetwork.cxx
//
// Written by Durk Talsma, started June 2005.
//
// Copyright (C) 2004 Durk Talsma.
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
//
// $Id$

#ifndef ATC_GROUND_CONTROLLER_HXX
#define ATC_GROUND_CONTROLLER_HXX

#include <simgear/compiler.h>

#include <string>

#include <ATC/trafficcontrol.hxx>
#include <ATC/TowerController.hxx>

class FGAirportDynamics;


/**************************************************************************************
 * class FGGroundController
 *************************************************************************************/
class FGGroundController : public FGATCController
{
private:

    bool hasNetwork;
    bool networkInitialized;
    int count;
    int version;

    FGTowerController *towerController;
    /**Returns the frequency to be used. */
    int getFrequency();


    void checkSpeedAdjustment(int id, double lat, double lon,
                              double heading, double speed, double alt);
    void checkHoldPosition(int id, double lat, double lon,
                           double heading, double speed, double alt);


    void updateStartupTraffic(TrafficVectorIterator i, int& priority, time_t now);
    bool updateActiveTraffic(TrafficVectorIterator i, int& priority, time_t now);
public:
    FGGroundController(FGAirportDynamics *par);
    ~FGGroundController();

    void setVersion (int v) { version = v;};
    int getVersion() { return version; };

    bool exists() {
        return hasNetwork;
    };
    void setTowerController(FGTowerController *twrCtrlr) {
        towerController = twrCtrlr;
    };


    virtual void announcePosition(int id, FGAIFlightPlan *intendedRoute, int currentRoute,
                                  double lat, double lon, double hdg, double spd, double alt,
                                  double radius, int leg, FGAIAircraft *aircraft);
    virtual void updateAircraftInformation(int id, SGGeod geod, double heading, double speed, double alt, double dt);

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            bool checkForCircularWaits(int id);
    virtual void render(bool);
    virtual std::string getName();
    virtual void update(double dt);

    void addVersion(int v) {version = v; };
};


#endif
