// tide.cxx -- interface for tidal movement
//
// Written by Erik Hofman, Octover 2020
//
// Copyright (C) 2020  Erik Hofman <erik@ehofman.com>
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

#include <simgear/constants.h>
#include <simgear/timing/sg_time.hxx>
#include <simgear/structure/SGExpression.hxx>
#include <Main/globals.hxx>

#include "tide.hxx"
#include "light.hxx"
#include "bodysolver.hxx"

void FGTide::reinit() {
    _prev_moon_rotation = -9999.0;
}

void FGTide::bind()
{
    SGPropertyNode *prop = globals->get_props();

    _tideAnimation = prop->getNode("/environment/sea/surface/delta-T-tide", true);

    _tideLevelNorm = prop->getNode("/sim/time/tide-level-norm", true);
    _tideLevelNorm->setDoubleValue(_tide_level);
}

void FGTide::unbind()
{
    _tideLevelNorm.reset();
    _tideAnimation.reset();
}

void FGTide::update(double dt)
{
    FGLight *l = static_cast<FGLight*>(globals->get_subsystem("lighting"));
    double moon_rotation = l->get_moon_rotation();
    if (fabs(_prev_moon_rotation - moon_rotation) > (SGD_PI/180.0))
    {
//      double sun_rotation = _sunAngleRad->getDoubleValue();
        _prev_moon_rotation = moon_rotation;

        // Angles range from 0.0 (straight up) to pi (straight down).
        // The sun and moon rotate once every day (approximately) but tides
        // happen twice a day.
        _tide_level = cos(2.0*moon_rotation);

        _tideLevelNorm->setDoubleValue(_tide_level);
        _tideAnimation->setDoubleValue(0.5 + 0.5*_tide_level);
    }
}

// Register the subsystem.
SGSubsystemMgr::Registrant<FGTide> registrantFGTide;