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

#pragma once

#include <time.h>
#include <string>

#include <simgear/compiler.h>
#include <simgear/xml/easyxml.hxx>

#include "runwayprefs.hxx"


class FGRunwayPreferenceXMLLoader : public XMLVisitor {
public:
    FGRunwayPreferenceXMLLoader(FGRunwayPreference* p);

protected:
    virtual void startXML (); 
    virtual void endXML   ();
    virtual void startElement (const char * name, const XMLAttributes &atts);
    virtual void endElement (const char * name);
    virtual void data (const char * s, int len);
    virtual void pi (const char * target, const char * data);
    virtual void warning (const char * message, int line, int column);
    virtual void error (const char * message, int line, int column);

    time_t processTime(const std::string &tme);

private:
    FGRunwayPreference* _pref;

    std::string value;

    std::string scheduleName;
    ScheduleTime currTimes; // Needed for parsing;

    RunwayList  rwyList;
    RunwayGroup rwyGroup;
};
