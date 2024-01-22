
#include "summary.h"
#include "misc.h"

/**
 * @brief Summary scene initialization code
 * 
 * @param context 
 */
void app_scene_summary_on_enter(void* context) {
    App* app = (App*)context;

    // Retrieve the widget and initialize its content
    Widget* widget = app->widget;
    widget_reset(widget);
    widget_add_text_scroll_element(
        widget,
        20,
        10,
        80,
        30,
        "This will be the summary page where we show the data. "
        "At the moment it is just a placeholder.");
    widget_add_button_element(
        widget, GuiButtonTypeCenter, "OK", app_scene_summary_button_callback, app);

    // Show view AppViewWidget
    view_dispatcher_switch_to_view(app->view_dispatcher, AppViewWidget);
}

/**
 * @brief Event handler
 * 
 * @param context 
 * @param event 
 */
bool app_scene_summary_on_event(void* context, SceneManagerEvent event) {
    App* app = (App*)context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        if(event.event == AppWidgetEventDone) {
            scene_manager_next_scene(app->scene_manager, AppSceneSetVolume);
        }
    }

    return consumed;
}

void app_scene_summary_on_exit(void* context) {
    App* app = (App*)context;
    Widget* widget = app->widget;
    widget_reset(widget);
}

/**
 * @brief Callback for the 'OK' button
 * 
 * @param result 
 * @param type 
 * @param context 
 */
void app_scene_summary_button_callback(GuiButtonType result, InputType type, void* context) {
    App* app = (App*)context;
    UNUSED(type);

    if(result == GuiButtonTypeCenter) {
        view_dispatcher_send_custom_event(app->view_dispatcher, AppWidgetEventDone);
    }
}