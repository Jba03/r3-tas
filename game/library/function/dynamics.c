//
//  dynamics.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-28.
//

#include "dynamics.h"
#include "game.h"

/** dynamics_get_speed: get the speed of the specified dynamics structure */
const struct vector3 dynamics_get_speed(const struct dynamics* dynamics)
{
    /* no dynamics => no speed */
    if (!dynamics) return vector3_new(0.0f, 0.0f, 0.0f);
    
    const struct dynamics_report* report = pointer(dynamics->base.report);
    /* If the report for the frame doesn't exist, return the previous speed, */
    /* which in most cases, happens to be the same as the current speed. */
    if (!report) return vector3_host_byteorder(dynamics->base.speed_previous);
    
    return vector3_host_byteorder(report->speed_absolute_current.linear);
}

/** dynamics_collide_with: get the surface collision state for the specified obstacle type */
bool dynamics_collide_with(const struct dynamics* dynamics, unsigned obstacle)
{
    if (!dynamics) return false;
    const struct dynamics_report* report = pointer(dynamics->base.report);
    if (!report) return false;
    return (host_byteorder_32(report->surface_state_current) & obstacle) == obstacle;
}
