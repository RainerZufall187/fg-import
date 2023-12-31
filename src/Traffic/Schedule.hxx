/* -*- Mode: C++ -*- *****************************************************
 * Schedule.hxx
 * Written by Durk Talsma. Started May 5, 2004
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 **************************************************************************/

/**************************************************************************
 * This file contains the definition of the class Schedule.
 *
 * A schedule is basically a number of scheduled flights, which can be
 * assigned to an AI aircraft.
 **************************************************************************/

#pragma once

constexpr double TRAFFIC_TO_AI_DIST_TO_START = 150.0;
constexpr double TRAFFIC_TO_AI_DIST_TO_DIE = 200.0;

// forward decls
class FGAIAircraft;
class FGScheduledFlight;

typedef std::vector<FGScheduledFlight*> FGScheduledFlightVec;


class FGAISchedule
{
private:
    std::string modelPath;
    std::string homePort;
    std::string livery;
    std::string registration;
    std::string airline;
    std::string acType;
    std::string m_class;
    std::string flightType;
    std::string flightIdentifier;
    std::string currentDestination;
    bool heavy;
    FGScheduledFlightVec flights;
    SGGeod position;
    double radius;
    double groundOffset;
    double distanceToUser;
    double score;
    unsigned int runCount;
    unsigned int hits;
    unsigned int lastRun;
    bool firstRun;
    double courseToDest;
    bool initialized;
    bool valid;
    bool scheduleComplete;

    bool scheduleFlights(time_t now);
    int groundTimeFromRadius();

    /**
   * Transition this schedule from distant mode to AI mode;
   * create the AIAircraft (and flight plan) and register with the AIManager
   */
    bool createAIAircraft(FGScheduledFlight* flight, double speedKnots, time_t deptime, time_t remainingTime);

    // the aiAircraft associated with us
    SGSharedPtr<FGAIAircraft> aiAircraft;

public:
    FGAISchedule(); // constructor
    FGAISchedule(const std::string& model,
                 const std::string& livery,
                 const std::string& homePort,
                 const std::string& registration,
                 const std::string& flightId,
                 bool heavy,
                 const std::string& acType,
                 const std::string& airline,
                 const std::string& m_class,
                 const std::string& flight_type,
                 double radius,
                 double offset);             // construct & init
    FGAISchedule(const FGAISchedule& other); // copy constructor

    ~FGAISchedule(); //destructor

    static bool validModelPath(const std::string& model);
    static SGPath resolveModelPath(const std::string& model);

    bool update(time_t now, const SGVec3d& userCart);
    bool init();

    double getSpeed();
    //void setClosestDistanceToUser();
    bool next(); // forces the schedule to move on to the next flight.

    // TODO: rework these four functions
    time_t getDepartureTime();
    FGAirport* getDepartureAirport();
    FGAirport* getArrivalAirport();
    int getCruiseAlt();
    double getRadius() { return radius; };
    double getGroundOffset() { return groundOffset; };
    const std::string& getFlightType() { return flightType; };
    const std::string& getAirline() { return airline; };
    const std::string& getAircraft() { return acType; };
    std::string getCallSign();
    const std::string& getRegistration() { return registration; };
    std::string getFlightRules();
    bool getHeavy() { return heavy; };
    double getCourse() { return courseToDest; };
    unsigned int getRunCount() { return runCount; };
    unsigned int getHits() { return hits; };

    void setrunCount(unsigned int count) { runCount = count; };
    void setHits(unsigned int count) { hits = count; };
    void setScore();
    double getScore() { return score; };
    /**Create an initial heading for user controlled aircraft.*/
    void setHeading();
    void assign(FGScheduledFlight* ref);
    void clearAllFlights();
    void setFlightType(const std::string& val) { flightType = val; };
    FGScheduledFlight* findAvailableFlight(const std::string& currentDestination, const std::string& req, time_t min = 0, time_t max = 0);
    // used to sort in descending order of score: I've probably found a better way to
    // descending order sorting, but still need to test that.
    bool operator<(const FGAISchedule& other) const;
    int getLastUsed() { return lastRun; };
    void setLastUsed(unsigned int val) { lastRun = val; };
    //void * getAiRef                 () { return AIManagerRef; };
    //FGAISchedule* getAddress        () { return this;};

    static bool compareSchedules(const FGAISchedule* a, const FGAISchedule* b);
};

typedef std::vector<FGAISchedule*> ScheduleVector;
typedef std::vector<FGAISchedule*>::iterator ScheduleVectorIterator;
