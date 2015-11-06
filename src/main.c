#include <pebble.h>
#include <inttypes.h>
#include "windows/pin_window.h"
/*
#define WAKEUP_REASON 0
#define PERSIST_KEY_WAKEUP_ID 42
*/
static Window *s_main_window;
static TextLayer *s_output_layer;
/*
static WakeupId s_wakeup_id;

static void wakeup_handler(WakeupId id, int32_t reason) {
  // A wakeup event has occurred
  
  // text_layer_set_text(s_output_layer, "Wakey wakey!");
  
  // Delete the ID
  persist_delete(PERSIST_KEY_WAKEUP_ID);
}


static void check_wakeup() {
  // Get the ID
  s_wakeup_id = persist_read_int(PERSIST_KEY_WAKEUP_ID);

  if (s_wakeup_id > 0) {
    // There is a wakeup scheduled soon
    time_t timestamp = 0;
    wakeup_query(s_wakeup_id, &timestamp);
    int seconds_remaining = timestamp - time(NULL);

    // Show how many seconds to go
    static char s_buffer[64];
    snprintf(s_buffer, sizeof(s_buffer), "The event is already scheduled for %d seconds from now!", seconds_remaining);
    // text_layer_set_text(s_output_layer, s_buffer);
  }
}
*/
static void pin_complete_callback(PIN pin, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Pin was %d %d %d", pin.digits[0], pin.digits[1], pin.digits[2]);
  /*
  uint32_t hours = pin.digits[0];
  uint32_t mins = pin.digits[1];
  uint32_t secs = pin.digits[2];
  int timeout = (hours * 3600) + (mins * 60) + secs;
  
  //Check the event is not already scheduled
  if (!wakeup_query(s_wakeup_id, NULL)) {
    // Current time + timestamp
    time_t future_time = time(NULL) + timeout;
    
    APP_LOG(APP_LOG_LEVEL_INFO, "timeout: %d", timeout);

    // Schedule wakeup event and keep the WakeupId
    s_wakeup_id = wakeup_schedule(future_time, WAKEUP_REASON, true);
    persist_write_int(PERSIST_KEY_WAKEUP_ID, s_wakeup_id);

    // Prepare for waking up later
    text_layer_set_text(s_output_layer, "This app will now wake up in 30 seconds.\n\nClose me!");
  } else {
    // Check existing wakeup
    check_wakeup();
  }
  
  */
  pin_window_pop((PinWindow*)context, true);
}




static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_output_layer, "Up pressed!");
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_output_layer, "Select pressed!");
  

}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_output_layer, "Down pressed!");
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}


static void main_window_load(Window *window) {
  
  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(0, 55, 144, 55));
  text_layer_set_background_color(s_output_layer, GColorClear);
  text_layer_set_text_color(s_output_layer, GColorBlack);
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_output_layer));

  PinWindow *pin_window = pin_window_create((PinWindowCallbacks) {
    .pin_complete = pin_complete_callback
  });
  pin_window_push(pin_window, true);
  
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_output_layer);
}

static void init() {
  s_main_window = window_create();
  


  window_set_click_config_provider(s_main_window, click_config_provider);

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  /*
  // Subscribe to Wakeup API
  wakeup_service_subscribe(wakeup_handler);
  

  // Was this a wakeup launch?
  if (launch_reason() == APP_LAUNCH_WAKEUP) {
    // The app was started by a wakeup
    WakeupId id = 0;
    int32_t reason = 0;

    // Get details and handle the wakeup
    wakeup_get_launch_event(&id, &reason);
    wakeup_handler(id, reason);
  } else {
    // Check whether a wakeup will occur soon
    check_wakeup();
  }
  */
  
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}