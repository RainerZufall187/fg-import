// inputvalue.hxx - provide input to autopilot components
//
// Written by Torsten Dreyer
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

#include <cstdlib>

#include "inputvalue.hxx"

#include <simgear/misc/strutils.hxx>

using namespace FGXMLAutopilot;

//------------------------------------------------------------------------------
PeriodicalValue::PeriodicalValue( SGPropertyNode& prop_root,
                                  SGPropertyNode& cfg )
{
  SGPropertyNode_ptr minNode = cfg.getChild( "min" );
  SGPropertyNode_ptr maxNode = cfg.getChild( "max" );
  if( !minNode || !maxNode )
  {
    SG_LOG
    (
      SG_AUTOPILOT,
      SG_ALERT,
      "periodical defined, but no <min> and/or <max> tag. Period ignored."
    );
  }
  else
  {
    minPeriod = new InputValue(prop_root, *minNode);
    maxPeriod = new InputValue(prop_root, *maxNode);
  }
}

//------------------------------------------------------------------------------
double PeriodicalValue::normalize( double value ) const
{
  return SGMiscd::normalizePeriodic( minPeriod->get_value(),
                                     maxPeriod->get_value(),
                                     value );
}

//------------------------------------------------------------------------------
double PeriodicalValue::normalizeSymmetric( double value ) const
{
  double minValue = minPeriod->get_value();
  double maxValue = maxPeriod->get_value();
  
  value = SGMiscd::normalizePeriodic( minValue, maxValue, value );
  double width_2 = (maxValue - minValue)/2;
  return value > width_2 ? width_2 - value : value;
}

//------------------------------------------------------------------------------
InputValue::InputValue( SGPropertyNode& prop_root,
                        SGPropertyNode& cfg,
                        double value,
                        double offset,
                        double scale ):
  _value(0.0),
  _abs(false)
{
  parse(prop_root, cfg, value, offset, scale);
}

InputValue::~InputValue()
{
    if (_pathNode) {
        _pathNode->removeChangeListener(this);
    }
}

void InputValue::initPropertyFromInitialValue()
{
    double s = get_scale();
    if( s != 0 )
      _property->setDoubleValue( (_value - get_offset())/s );
    else
      _property->setDoubleValue(0); // if scale is zero, value*scale is zero
}

//------------------------------------------------------------------------------
void InputValue::parse( SGPropertyNode& prop_root,
                        SGPropertyNode& cfg,
                        double aValue,
                        double aOffset,
                        double aScale )
{
  _value = aValue;
  _property = NULL;
  _offset = NULL;
  _scale = NULL;
  _min = NULL;
  _max = NULL;
  _periodical = NULL;

  SGPropertyNode * n;

  if( (n = cfg.getChild("condition")) != NULL )
    _condition = sgReadCondition(&prop_root, n);

  if( (n = cfg.getChild( "scale" )) != NULL )
    _scale = new InputValue(prop_root, *n, aScale);

  if( (n = cfg.getChild( "offset" )) != NULL )
    _offset = new InputValue(prop_root, *n, aOffset);

  if( (n = cfg.getChild( "max" )) != NULL )
    _max = new InputValue(prop_root, *n);

  if( (n = cfg.getChild( "min" )) != NULL )
    _min = new InputValue(prop_root, *n);

  if( (n = cfg.getChild( "abs" )) != NULL )
    _abs = n->getBoolValue();

  if( (n = cfg.getChild( "period" )) != NULL )
    _periodical = new PeriodicalValue(prop_root, *n);


  SGPropertyNode *valueNode = cfg.getChild("value");
  if( valueNode != NULL )
    _value = valueNode->getDoubleValue();

  if( (n = cfg.getChild("expression")) != NULL )
  {
    _expression = SGReadDoubleExpression(&prop_root, n->getChild(0));
    return;
  }

    if ((n = cfg.getChild("property-path"))) {
        // cache the root node, in case of changes
        _rootNode = &prop_root;
        const auto trimmed = simgear::strutils::strip(n->getStringValue());
        _pathNode = prop_root.getNode(trimmed, true);
        _pathNode->addChangeListener(this);
        
        // if <property> is defined, should we use it to initialise
        // the path prop? not doing so for now.
        
        const auto path = simgear::strutils::strip(_pathNode->getStringValue());
        if (!path.empty()) {
            _property = _rootNode->getNode(path);
        }

        return;
    }

    
  // if no <property> element, check for <prop> element for backwards
  // compatibility
  if(    (n = cfg.getChild("property"))
      || (n = cfg.getChild("prop"    )) )
  {
    _property = prop_root.getNode(n->getStringValue(), true);
    if( valueNode )
    {
        initPropertyFromInitialValue();
    }

    return;
  } // of have a <property> or <prop>

  if( !valueNode )
  {
    // no <value>, <prop> or <expression> element, use text node
    const char * textnode = cfg.getStringValue();
    char * endp = NULL;
    // try to convert to a double value. If the textnode does not start with a number
    // endp will point to the beginning of the string. We assume this should be
    // a property name
    _value = strtod( textnode, &endp );
    if( endp == textnode )
      _property = prop_root.getNode(textnode, true);
  }
}

void InputValue::set_value( double aValue ) 
{
    if (!_property)
      return;
      
    double s = get_scale();
    if( s != 0 )
        _property->setDoubleValue( (aValue - get_offset())/s );
    else
        _property->setDoubleValue( 0 ); // if scale is zero, value*scale is zero
}

double InputValue::get_value() const
{
    double value = _value;

    if (_expression) {
        // compute the expression value
        value = _expression->getValue(NULL);

        if (SGMiscd::isNaN(value)) {
            SG_LOG(SG_AUTOPILOT, SG_DEV_ALERT, "AP input: read NaN from expression");
        }
    } else if( _property != NULL ) {
        value = _property->getDoubleValue();

        if (SGMiscd::isNaN(value)) {
            SG_LOG(SG_AUTOPILOT, SG_DEV_ALERT, "AP input: read NaN from:" << _property->getPath() );
        }
    } else {
        if (SGMiscd::isNaN(value)) {
            SG_LOG(SG_AUTOPILOT, SG_DEV_ALERT, "AP input is NaN." );
        }
    }
    
    if( _scale ) 
        value *= _scale->get_value();

    if( _offset ) 
        value += _offset->get_value();

    if( _min ) {
        double m = _min->get_value();
        if( value < m )
            value = m;
    }

    if( _max ) {
        double m = _max->get_value();
        if( value > m )
            value = m;
    }

    if( _periodical ) {
      value = _periodical->normalize( value );
    }

    return _abs ? fabs(value) : value;
}

bool InputValue::is_enabled() const
{
    if (_pathNode && !_property) {
        // if we have a configurable path, and it's currently not valid,
        // mark ourselves as disabled
        return false;
    }
    
    if (_condition) {
        return _condition->test();
    }

    return true; // default to enab;ed
}

void InputValue::valueChanged(SGPropertyNode *node)
{
    assert(node == _pathNode);
    const auto path = simgear::strutils::strip(_pathNode->getStringValue());
    if (path.empty()) {
        // don't consider an empty string to mean the root node, that's not
        // useful behaviour
        _property.reset();
        return;
    }
    
    // important we don't create here: this allows an invalid path
    // to give us a null _property, which causes us to be marked as
    // disabled, allowing another input to be used
    auto propNode = _rootNode->getNode(path);
    if (propNode) {
        _property = propNode;
    } else {
        _property.reset();
    }
}
    
