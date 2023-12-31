/*
 * SPDX-FileName: FGPUIMenuBar.hxx
 * SPDX-FileComment: XML-configured menu bar
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <GUI/menubar.hxx>

#include <forward_list>
#include <map>
#include <memory>
#include <string>
#include <vector>


// forward decls, avoid pulling in PLIB headers here
class puMenuBar;
class puObject;
class SGPropertyNode;
class SGBinding;

typedef void (*puCallback)(class puObject*);

/**
 * XML-configured PUI menu bar.
 *
 * This class creates a menu bar from a tree of XML properties.  These
 * properties are not part of the main FlightGear property tree, but
 * are read from a separate file ($FG_ROOT/gui/menubar.xml).
 *
 */
class FGPUIMenuBar : public FGMenuBar
{
public:
    /**
     * Constructor.
     */
    FGPUIMenuBar();


    /**
     * Destructor.
     */
    virtual ~FGPUIMenuBar();


    /**
     * Initialize the menu bar from $FG_ROOT/gui/menubar.xml
     */
    virtual void init();

    /**
     * Make the menu bar visible.
     */
    virtual void show();


    /**
     * Make the menu bar invisible.
     */
    virtual void hide();


    /**
     * Test whether the menu bar is visible.
     */
    virtual bool isVisible() const;

    void setHideIfOverlapsWindow(bool hide) override;

    bool getHideIfOverlapsWindow() const override;

    /**
     * IGNORE THIS METHOD!!!
     *
     * This is necessary only because plib does not provide any easy
     * way to attach user data to a menu item.  FlightGear should not
     * have to know about PUI internals, but this method allows the
     * callback to pass the menu item one-shot on to the current menu.
     */
    virtual void fireItem(puObject* item);


    /**
     * create a menubar based on a PropertyList within the PropertyTree
     */
    void make_menubar(SGPropertyNode* props);


    /**
     * destroy a menubar based on a PropertyList within the PropertyTree
     */
    void destroy_menubar();


    /**
     * Disable/enable menu titles and entries
     */
    bool enable_item(const SGPropertyNode* item, bool state);


private:
    // Make a single menu.
    void make_menu(SGPropertyNode* node);

    // Make the top-level menubar.
    void make_menubar();

    // Create a property-path -> puObject map for menu node
    void make_object_map(SGPropertyNode* node);

    // Add <enabled> listener that enables/disables menu entries.
    void add_enabled_listener(SGPropertyNode* node);

    void recomputeVisibility();

    // Is the menu visible?
    bool _visible;
    bool _hideOverlapping = false;

    // The top-level menubar itself.
    puMenuBar* _menuBar;

    // Each element contains the list of bindings for a particular menu entry.
    // Not an std::vector because we want the addresses of previous elements
    // to remain valid when we add new ones.
    std::forward_list<std::vector<std::unique_ptr<SGBinding>>> _bindings;

    // These are hoops that we have to jump through because PUI doesn't
    // do memory management for lists.  We have to allocate the arrays,
    // hang onto pointers, and then delete them when the menubar is
    // freed.
    char** make_char_array(int size);
    puCallback* make_callback_array(int size);
    // The return value points to an array where each element is a pointer to a
    // vector that gives the list of bindings assigned to a given menu entry.
    const std::vector<std::unique_ptr<SGBinding>>** make_userdata_array(int size);
    std::vector<char**> _char_arrays;
    std::vector<puCallback*> _callback_arrays;
    std::vector<const std::vector<std::unique_ptr<SGBinding>>**> _userdata_arrays;

    // A map for {menu node path}->puObject translation.
    std::map<std::string, puObject*> _objects;
};
