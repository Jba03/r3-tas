//
//  dynamics.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-28.
//

#include "stDynamics.hh"
#include "memory.hh"

/** fnDynamicsGetSpeed: get the speed of the specified dynamics structure */
const struct stVector3D fnDynamicsGetSpeed(const tdstDynamics* dynamics)
{
    /* no dynamics => no speed */
    if (!dynamics) return vector3_new(0.0f, 0.0f, 0.0f);
    
    const tdstDynamicsReport* report = pointer(dynamics->base.report);
    /* If the report for the frame doesn't exist, return the previous speed, */
    /* which in most cases, happens to be the same as the current speed. */
    if (!report) return vector3_host_byteorder(dynamics->base.previousSpeed);
    
    return vector3_host_byteorder(report->currentAbsoluteSpeed.linear);
}

/** fnDynamicsCollideWith: get the surface collision state for the specified obstacle type */
bool fnDynamicsCollideWith(const tdstDynamics* dynamics, unsigned obstacle)
{
    if (!dynamics) return false;
    const tdstDynamicsReport* report = pointer(dynamics->base.report);
    if (!report) return false;
    
    
    return (host_byteorder_32(report->currentSurfaceState) & obstacle) == obstacle;
}
