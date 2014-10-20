//
// Vortexje -- Lifting surface.
//
// Copyright (C) 2012 - 2014 Baayen & Heinz GmbH.
//
// Authors: Jorn Baayen <jorn.baayen@baayen-heinz.com>
//

#include <vortexje/lifting-surface.hpp>
#include <vortexje/parameters.hpp>

using namespace std;
using namespace Eigen;
using namespace Vortexje;

/**
   Constructs an empty LiftingSurface.
*/
LiftingSurface::LiftingSurface() : Surface()
{
    // Do nothing.
}

/**
   Returns the number of chordwise nodes.

   @returns The number of chordwise nodes.
*/
int 
LiftingSurface::n_chordwise_nodes() const
{
    return (int) upper_nodes.rows();
}

/**
   Returns the number of chordwise panels.
   
   @returns The number of chordwise panels.
*/
int 
LiftingSurface::n_chordwise_panels() const
{
    return (int) upper_panels.rows();
}

/**
   Returns the number of spanwise nodes.

   @returns The number of spanwise nodes.
*/
int 
LiftingSurface::n_spanwise_nodes() const
{
    return (int) upper_nodes.cols();
}

/**
   Returns the number of spanwise panels.
   
   @returns The number of spanwise panels.
*/
int 
LiftingSurface::n_spanwise_panels() const
{
    return (int) upper_panels.cols();
}

/**
   Returns the index'th trailing edge node.
  
   @param[in]   index   Trailing edge node index.
   
   @returns The node number of the index'th trailing edge node.
*/
int 
LiftingSurface::trailing_edge_node(int index) const
{
    return upper_nodes(upper_nodes.rows() - 1, index);
}

/**
   Returns the index'th upper trailing edge panel.
  
   @param[in]   index   Trailing edge panel index.
   
   @returns The panel number of the index'th upper trailing edge panel.
*/
int
LiftingSurface::trailing_edge_upper_panel(int index) const
{
    return upper_panels(upper_panels.rows() - 1, index);
}

/**
   Returns the index'th lower trailing edge panel.
  
   @param[in]   index   Trailing edge panel index.
   
   @returns The panel number of the index'th lower trailing edge panel.
*/
int
LiftingSurface::trailing_edge_lower_panel(int index) const
{
    return lower_panels(lower_panels.rows() - 1, index);
}

/**
   Returns the unit vector bisecting the trailing edge, at the node_index'th trailing edge node.
  
   @param[in]   node_index   Trailing edge node index.
   
   @returns The unit vector bisecting the trailing edge, at the node_index'th trailinge edge node.
*/
Eigen::Vector3d
LiftingSurface::trailing_edge_bisector(int node_index) const
{
    Vector3d upper = nodes[upper_nodes(upper_nodes.rows() - 1, node_index)] - nodes[upper_nodes(upper_nodes.rows() - 2, node_index)];
    Vector3d lower = nodes[lower_nodes(lower_nodes.rows() - 1, node_index)] - nodes[lower_nodes(lower_nodes.rows() - 2, node_index)];
    
    upper.normalize();
    lower.normalize();
    
    Vector3d trailing_edge_bisector = upper + lower;
    trailing_edge_bisector.normalize();
    
    return trailing_edge_bisector;
}

/**
   Returns the wake emission velocity, at the node_index'th trailing edge node.
  
   @param[in]   apparent_velocity   Apparent velocity.
   @param[in]   node_index          Trailing edge node index.
   
   @returns The wake emission velocity, at the node_index'th trailinge edge node.
*/
Eigen::Vector3d
LiftingSurface::wake_emission_velocity(const Eigen::Vector3d &apparent_velocity, int node_index) const
{
    Vector3d wake_emission_velocity;
    if (Parameters::wake_emission_follow_bisector && n_chordwise_nodes() > 1) {
        // Use bisector to determine wake emission direction:
        int prev_node, next_node;
        
        if (node_index > 0)
            prev_node = trailing_edge_node(node_index - 1);
        else
            prev_node = trailing_edge_node(node_index);
        
        if (node_index < n_spanwise_nodes() - 1)
            next_node = trailing_edge_node(node_index + 1);
        else
            next_node = trailing_edge_node(node_index);
            
        Vector3d bisector = trailing_edge_bisector(node_index);
            
        if (prev_node != next_node) {
            // Project apparent velocity onto the plane spanned by the span direction, and the bisector:
            Vector3d span_direction = nodes[next_node] - nodes[prev_node];
            
            Vector3d wake_normal = span_direction.cross(bisector);
            wake_normal.normalize();
            
            wake_emission_velocity = -(apparent_velocity - apparent_velocity.dot(wake_normal) * wake_normal);
            
        } else {
            // No span direction available.  Project apparent velocity onto bisector:
            wake_emission_velocity = -apparent_velocity.dot(bisector) * bisector;
            
        } 
        
    } else {
        // Emit wake in direction of apparent velocity:
        wake_emission_velocity = -apparent_velocity;
        
    }
    
    // Done:
    return wake_emission_velocity;
}
