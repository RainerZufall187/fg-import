// component.cxx - Base class for autopilot components
//
// Written by Torsten Dreyer
// Based heavily on work created by Curtis Olson, started January 2004.
//
// Copyright (C) 2004  Curtis L. Olson  - http://www.flightgear.org/~curt
// Copyright (C) 2010  Torsten Dreyer - Torsten (at) t3r (dot) de
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
#include "component.hxx"
#include <Main/fg_props.hxx>
#include <simgear/structure/exception.hxx>
#include <simgear/props/condition.hxx>

using namespace FGXMLAutopilot;

Component::Component() :
  _enable_value(NULL),
  _enabled(false),
  _debug(false),
  _honor_passive(false)
{
}

Component::~Component()
{
  delete _enable_value;
}

//------------------------------------------------------------------------------
bool Component::configure( SGPropertyNode& prop_root,
                           SGPropertyNode& cfg )
{
  for( int i = 0; i < cfg.nChildren(); ++i )
  {
    SGPropertyNode_ptr child = cfg.getChild(i);
    std::string cname(child->getNameString());

    if(    !configure(*child, cname, prop_root)
        && cname != "params" ) // 'params' is usually used to specify parameters
                               // in PropertList files.

        // consider using the error reporting mechanism here, at level warning
        SG_LOG(
            SG_AUTOPILOT,
            SG_DEV_WARN,
            "Component::configure: unknown node: " << cname);
  }

  return true;
}

//------------------------------------------------------------------------------
bool Component::configure( SGPropertyNode& cfg_node,
                           const std::string& cfg_name,
                           SGPropertyNode& prop_root )
{
  if ( cfg_name == "name" )
  {
    set_name(cfg_node.getStringValue());
    return true;
  }

  if( cfg_name == "update-interval-secs" )
    // This is handled in autopilot.cxx
    return true;

  if ( cfg_name == "debug" )
  {
    _debug = cfg_node.getBoolValue();
    return true;
  }

  if ( cfg_name == "enable" )
  {
    SGPropertyNode_ptr prop;

    if( (prop = cfg_node.getChild("condition")) != NULL ) {
      _condition = sgReadCondition(fgGetNode("/"), prop);
      return true;
    } 
    if ( (prop = cfg_node.getChild( "property" )) != NULL ) {
      _enable_prop = fgGetNode( prop->getStringValue(), true );
    }
       
    if ( (prop = cfg_node.getChild( "prop" )) != NULL ) {
      _enable_prop = fgGetNode( prop->getStringValue(), true );
    }

    if ( (prop = cfg_node.getChild( "value" )) != NULL ) {
      delete _enable_value;
      _enable_value = new std::string(prop->getStringValue());
    }

    if ( (prop = cfg_node.getChild( "honor-passive" )) != NULL ) {
      _honor_passive = prop->getBoolValue();
    }

    return true;
  }

  return false;
}

//------------------------------------------------------------------------------
bool Component::isPropertyEnabled()
{
    if( _condition )
        return _condition->test();

    if( _enable_prop ) {
        if( _enable_value ) {
            return *_enable_value == _enable_prop->getStringValue();
        } else {
            return _enable_prop->getBoolValue();
        }
    }
    return true;
}

void Component::update( double dt )
{
  bool firstTime = false;
  if( isPropertyEnabled() ) {
    firstTime = !_enabled;
    _enabled = true;
  } else {
    _enabled = false;
  }

  if( _enabled ) update( firstTime, dt );
  else disabled( dt );
}
