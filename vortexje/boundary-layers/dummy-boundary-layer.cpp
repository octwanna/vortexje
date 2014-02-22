//
// Vortexje -- Dummy boundary layer class.
//
// Copyright (C) 2014 Baayen & Heinz GmbH.
//
// Authors: Jorn Baayen <jorn.baayen@baayen-heinz.com>
//

#include <vortexje/boundary-layers/dummy-boundary-layer.hpp>

using namespace Eigen;
using namespace Vortexje;

/**
   Normally, this function solves the relevant boundary layer equations.  Here, it does nothing.
   
   @param[in]   surface_velocities   (n x 3)-matrix of surface velocities.
 */
void
DummyBoundaryLayer::recalculate(const MatrixXd &surface_velocities)
{
}

/**
   Returns the blowing velocity for the given panel.
   
   @param[in]   panel   Reference panel.
   
   @returns Blowing velocity for the given panel.
 */
double
DummyBoundaryLayer::blowing_velocity(int panel) const
{
    return 0.0;
}

/**
   Returns the friction force acting on the given panel.
   
   @param[in]   panel   Reference panel.
   
   @returns Friction force acting on the given panel.
 */
Vector3d
DummyBoundaryLayer::friction(int panel) const
{
    return Vector3d(0, 0, 0);
}