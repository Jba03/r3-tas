#ifndef actor_h
#define actor_h

#include "structure.h"

struct actor
{
    readonly pointer p_3ddata;
    readonly pointer stdgame;
    readonly pointer dynam;
    readonly pointer brain;
    readonly pointer cineinfo;
    readonly pointer collset;
    readonly pointer msway;
    readonly pointer mslight;
    readonly pointer sectorinfo;
    readonly pointer micro;
    readonly pointer mssound;
};

#define actor_3Ddata(actor) ((void*)pointer(actor->p_3ddata))
#define actor_stdgame(actor) ((struct standard_game_info*)pointer(actor->stdgame))
#define actor_dynam(actor) ((struct dynam*)pointer(actor->dynam))
#define actor_brain(actor) ((struct brain*)pointer(actor->brain))
#define actor_cineinfo(actor) ((struct cineinfo*)pointer(actor->cineinfo))
#define actor_collset(actor) ((struct actor_collideset*)pointer(actor->collset))
#define actor_msway(actor) ((struct msway*)pointer(actor->msway))
#define actor_mslight(actor) ((struct mslight*)pointer(actor->mslight))
#define actor_sectorinfo(actor) ((struct sectorinfo*)pointer(actor->sectorinfo))
#define actor_micro(actor) ((struct micro*)pointer(actor->micro))
#define actor_mssound(actor) ((struct mssound*)pointer(actor->mssound))

/* Object type */
#define actor_family_type(actor) (actor_stdgame(actor) ? stdgame_family_type(actor_stdgame(actor)) : -1)
#define actor_model_type(actor) (actor_stdgame(actor) ? stdgame_model_type(actor_stdgame(actor)) : -1)
#define actor_instance_type(actor) (actor_stdgame(actor) ? stdgame_instance_type(actor_stdgame(actor)) : -1)

/**
 * actor_superobject:
 *  Get the superobject associated with this actor.
 */
#define actor_superobject(actor) (actor ? (actor_stdgame(actor) ? stdgame_get_superobject(actor_stdgame(actor)) : NULL) : NULL)

#endif /* actor_h */
