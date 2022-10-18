#include "pch.h"
#include "pointers.h"

void DisableEV_HeadOnFrames(task* obj, taskwk* data, playerwk* co2)
{
    if (data && data->ewp)
    {
        if (&data->ewp->face)
        {
            EV_ClrFace(obj);
        }
    }

    if (co2 && co2->pfp)
    {   
        co2->pfp->frame = 0.0f;
        co2->pfp->framespeed = 0.0f;
        co2->pfp->face = 0;
        co2->pfp->tblpoint = 0;
        co2->pfp->tbl = 0;
    }
}