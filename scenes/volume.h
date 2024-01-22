#ifndef __VOLUME_H__
#define __VOLUME_H__

#include "misc.h"

#include "knob.h"

void app_scene_volume_on_enter(void* context);

bool app_scene_volume_on_event(void* context, SceneManagerEvent event);

void app_scene_volume_on_exit(void* context);

#endif