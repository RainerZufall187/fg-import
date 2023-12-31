/*
Provide Data for the ATIS Encoder from metarproperties
Copyright (C) 2014 Torsten Dreyer

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "MetarPropertiesATISInformationProvider.hxx"
#include <Main/globals.hxx>
#include <simgear/constants.h>

using std::string;

MetarPropertiesATISInformationProvider::MetarPropertiesATISInformationProvider( SGPropertyNode_ptr metar ) :
  _metar( metar )
{
}

MetarPropertiesATISInformationProvider::~MetarPropertiesATISInformationProvider()
{
}

bool MetarPropertiesATISInformationProvider::isValid() const
{
  return _metar->getBoolValue( "valid", false );
}

string MetarPropertiesATISInformationProvider::airportId() const
{
  return _metar->getStringValue( "station-id", "xxxx" );
}

long MetarPropertiesATISInformationProvider::getTime() const
{
  return makeAtisTime( 0,
    _metar->getIntValue( "hour" ) % 24, 
    _metar->getIntValue( "minute" ) % 60 );
}

int MetarPropertiesATISInformationProvider::getWindDeg() const
{
   return _metar->getIntValue( "base-wind-dir-deg" );
}

int MetarPropertiesATISInformationProvider::getWindMinDeg() const
{
   return _metar->getIntValue( "base-wind-range-from" );
}
int MetarPropertiesATISInformationProvider::getWindMaxDeg() const
{
   return _metar->getIntValue( "base-wind-range-to" );
}
int MetarPropertiesATISInformationProvider::getWindSpeedKt() const
{
  return _metar->getIntValue( "base-wind-speed-kt" );
}

int MetarPropertiesATISInformationProvider::getGustsKt() const
{
  return _metar->getIntValue( "gust-wind-speed-kt" );
}

int MetarPropertiesATISInformationProvider::getQnh() const
{
  return _metar->getDoubleValue("pressure-inhg") * SG_INHG_TO_PA / 100.0;
}

double MetarPropertiesATISInformationProvider::getQnhInHg() const
{
  return _metar->getDoubleValue("pressure-inhg");
}

bool MetarPropertiesATISInformationProvider::isCavok() const
{
  return _metar->getBoolValue( "cavok" );
}

int MetarPropertiesATISInformationProvider::getVisibilityMeters() const
{
  return _metar->getIntValue( "min-visibility-m" );
}

string MetarPropertiesATISInformationProvider::getPhenomena() const
{
  return _metar->getStringValue( "decoded" );
}

ATISInformationProvider::CloudEntries MetarPropertiesATISInformationProvider::getClouds()
{
  CloudEntries reply;

  using simgear::PropertyList;

  PropertyList layers = _metar->getNode("clouds", true )->getChildren("layer");
  for( PropertyList::iterator it = layers.begin(); it != layers.end(); ++it ) {
    std::string coverage = (*it)->getStringValue("coverage", "clear");
    double elevation = (*it)->getDoubleValue("elevation-ft", -9999 );
    if( elevation > 0 ) {
      reply[elevation] = coverage;
    }
  }

  return reply;
}

int MetarPropertiesATISInformationProvider::getTemperatureDeg() const
{
  return _metar->getIntValue( "temperature-degc" );
}

int MetarPropertiesATISInformationProvider::getDewpointDeg() const
{
  return _metar->getIntValue( "dewpoint-degc" );
}

string MetarPropertiesATISInformationProvider::getTrend() const
{
  return "nosig";
}
