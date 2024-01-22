#ifndef __SUMMARY_H__
#define __SUMMARY_H__

#include "misc.h"

void app_scene_summary_on_enter(void* context);

bool app_scene_summary_on_event(void* context, SceneManagerEvent event);

void app_scene_summary_on_exit(void* context);

void app_scene_summary_button_callback(GuiButtonType result, InputType type, void* context);

#endif