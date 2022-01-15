// FGAIStorm - AIBase derived class creates an AI thunderstorm
//
// Written by David Culp, started Feb 2004.
//
// Copyright (C) 2004  David P. Culp - davidculp2@comcast.net
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

#include "AIManager.hxx"
#include "AIBase.hxx"


class FGAIStorm : public FGAIBase {

public:
        FGAIStorm();
        virtual ~FGAIStorm() = default;

        const char* getTypeString(void) const override { return "thunderstorm"; }
        void readFromScenario(SGPropertyNode* scFileNode) override;
        void update(double dt) override;

        inline void setStrengthNorm( double s ) { strength_norm = s; };
        inline void setDiameter( double d ) { diameter = d; };
        inline void setHeight( double h ) { height = h; };
        inline double getStrengthNorm() const { return strength_norm; };
        inline double getDiameter() const { return diameter; };
        inline double getHeight() const { return height; };

private:
        double diameter = 0.0;          // diameter of turbulence zone, in nm
        double height = 0.0;            // top of turbulence zone, in feet MSL
        double strength_norm = 0.0;     // strength of turbulence

        void Run(double dt);

        // lightning stuff
        double delay;   // average time (sec) between lightning flashes
        int subflashes; // number of subflashes per flash
        double random_delay;  // delay +/- random number
        double timer;
        SGPropertyNode_ptr flash_node;
        int flashed;    // number of subflashes already done this flash
        bool flashing;  // true if currently flashing;
        int subflash_array[8];
        int subflash_index;

        // turbulence stuff
        SGPropertyNode_ptr turb_mag_node;
        SGPropertyNode_ptr turb_rate_node;
};
