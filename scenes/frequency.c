
#include "frequency.h"

/**
 * @brief 
 * 
 * @param context 
 */
void app_scene_frequency_on_enter(void* context) {
    message("app_scene_frequency_on_enter");
    App* app = (App*)context;

    Knob* knob = app->knob;
    knob_set_callback(knob, app_knob_callback, app);
    knob_set_heading(knob, "FREQUENCY");
    knob_set_counter(knob, (uint32_t)app->volume);

    view_dispatcher_switch_to_view(app->view_dispatcher, AppViewKnob);
}

bool app_scene_frequency_on_event(void* context, SceneManagerEvent event) {
    message("app_scene_frequency_on_event");
    App* app = context;

    bool consumed = false;
    if(event.type == SceneManagerEventTypeBack) {
        // Back button pressed
    } else if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        if(event.event == AppKnobEventDone) {
            FURI_LOG_I(TAG, "Custom Event!");
            uint32_t counter = knob_get_counter(app->knob);
            FURI_LOG_I(TAG, "The counter is %ld.", counter);
            app->frequency = counter;
            scene_manager_next_scene(app->scene_manager, AppSceneSummary);
        }
    }
    return consumed;
}

void app_scene_frequency_on_exit(void* context) {
    UNUSED(context);
}
