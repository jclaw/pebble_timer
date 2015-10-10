#include <pebble.h>

static Window *s_main_window;


static void main_window_load(Window *window) {
  // Create Window's child Layers here

}

static void main_window_unload(Window *window) {
  // Destroy Window's child Layers here

}

static void init() {
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}