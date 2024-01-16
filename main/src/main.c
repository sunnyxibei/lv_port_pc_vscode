
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "lv_drv_conf.h"
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#if USE_SDL
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "lv_drivers/sdl/sdl.h"
#elif USE_X11
#include "lv_drivers/x11/x11.h"
#endif
// #include "lv_drivers/display/monitor.h"
// #include "lv_drivers/indev/mouse.h"
// #include "lv_drivers/indev/keyboard.h"
// #include "lv_drivers/indev/mousewheel.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static void hal_deinit(void);
static void *tick_thread(void *data);

static void my_ui_init(void);

/**********************
 *  STATIC VARIABLES
 **********************/
static pthread_t thr_tick;    /* thread */
static bool end_tick = false; /* flag to terminate thread */

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
#if 0
static void user_image_demo()
{
  lv_obj_t * img = lv_gif_create(lv_scr_act());
  lv_gif_set_src(img, "A:lvgl/examples/libs/gif/bulb.gif");
  lv_obj_align(img, LV_ALIGN_BOTTOM_RIGHT, -20, -20);

  lv_color_t bg_color = lv_palette_lighten(LV_PALETTE_LIGHT_BLUE, 5);
    lv_color_t fg_color = lv_palette_darken(LV_PALETTE_BLUE, 4);

    lv_obj_t * qr = lv_qrcode_create(lv_scr_act(), 150, fg_color, bg_color);

    /*Set data*/
    const char * data = "https://lvgl.io";
    lv_qrcode_update(qr, data, strlen(data));
    lv_obj_center(qr);

    /*Add a border with bg_color*/
    lv_obj_set_style_border_color(qr, bg_color, 0);
    lv_obj_set_style_border_width(qr, 5, 0);

    /*Create a font*/
    static lv_ft_info_t info;
    /*FreeType uses C standard file system, so no driver letter is required.*/
    info.name = "./lvgl/examples/libs/freetype/Lato-Regular.ttf";
    info.weight = 24;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    /*Create style with the new font*/
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, info.font);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

    /*Create a label with the new style*/
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_obj_add_style(label, &style, 0);
    lv_label_set_text(label, "Hello world\nI'm a font created with FreeType");
    lv_obj_set_pos(label, 10, 10);

    lv_obj_t *  img1 = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    lv_img_set_src(img1, "A:lvgl/examples/libs/png/wink.png");
    lv_obj_align(img1, LV_ALIGN_LEFT_MID, 20, 0);

    lv_obj_t * wp;

    wp = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    lv_img_set_src(wp, "A:lvgl/examples/libs/sjpg/small_image.sjpg");
    lv_obj_align(wp, LV_ALIGN_RIGHT_MID, -20, 0);

    lv_obj_t * img2 = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    lv_img_set_src(img2, "A:lvgl/examples/libs/sjpg/lv_example_jpg.jpg");
    //lv_obj_center(img);
    lv_obj_align(img2, LV_ALIGN_TOP_RIGHT, -20, 20);

    lv_obj_t * img3 = lv_img_create(lv_scr_act());
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
#if LV_COLOR_DEPTH == 32
    lv_img_set_src(img3, "A:lvgl/examples/libs/bmp/example_32bit.bmp");
#elif LV_COLOR_DEPTH == 16
    lv_img_set_src(img, "A:lvgl/examples/libs/bmp/example_16bit.bmp");
#endif
    lv_obj_align(img3, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t * img4 = lv_img_create(lv_scr_act());
    lv_img_set_src(img4, "A:lvgl/examples/libs/ffmpeg/ffmpeg.png");
    lv_obj_align(img4, LV_ALIGN_BOTTOM_LEFT, 20, -20);

    lv_obj_t * player = lv_ffmpeg_player_create(lv_scr_act());
    lv_ffmpeg_player_set_src(player, "./lvgl/examples/libs/ffmpeg/birds.mp4");
    lv_ffmpeg_player_set_auto_restart(player, true);
    lv_ffmpeg_player_set_cmd(player, LV_FFMPEG_PLAYER_CMD_START);
    lv_obj_align(player, LV_ALIGN_TOP_MID, 0, 20);
}
#endif

#define LV_COLOR_LIST_BTN_BG lv_color_hex(0x74D0CB)

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init();

  //  lv_example_switch_1();
  //  lv_example_calendar_1();
  //  lv_example_btnmatrix_2();
  //  lv_example_checkbox_1();
  //  lv_example_colorwheel_1();
  //  lv_example_chart_6();
  //  lv_example_table_2();
  //  lv_example_scroll_2();
  //  lv_example_textarea_1();
  //  lv_example_msgbox_1();
  //  lv_example_dropdown_2();
  //  lv_example_btn_1();
  //  lv_example_scroll_1();
  //  lv_example_tabview_1();
  //  lv_example_tabview_1();
  //  lv_example_flex_3();
  //  lv_example_label_1();

  // lv_demo_widgets();
  my_ui_init();

  //  lv_demo_keypad_encoder();
  //  lv_demo_benchmark();
  //  lv_demo_stress();
  //  lv_demo_music();

  //  user_image_demo();

  while (1)
  {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    lv_timer_handler();
    usleep(5 * 1000);
  }

  hal_deinit();
  return 0;
}

lv_obj_t *parentObj;
lv_obj_t *childObj;
lv_obj_t *right_Roller;

static void roller_cb(lv_event_t *event)
{
  // 获取选中item index
  lv_obj_t *target = lv_event_get_target(event);
  printf("%d ", lv_roller_get_selected(target));

  // 获取选中item str
  char buf[2];
  lv_roller_get_selected_str(target, buf, sizeof(buf));
  printf("%s \n", buf);
}

int count = 0;

static void click_cb(lv_event_t *event)
{
  lv_obj_t *target = lv_event_get_target(event);
  if (target == parentObj)
  {
    printf("父控件点击\r\n");
    lv_obj_align(target, LV_ALIGN_CENTER, 0, 0);
  }
  else if (target == childObj)
  {
    printf("子控件点击\r\n");
    lv_obj_align(target, LV_ALIGN_RIGHT_MID, 200, 0);
  }
}

lv_obj_t *fan_Checkbox;
lv_obj_t *fan_Bar;
lv_obj_t *fan_Led;
lv_obj_t *lv_List;
lv_obj_t *right_Title;
lv_obj_t *right_Arc;
lv_obj_t *right_Arc1;

static void arc_cb(lv_event_t *event)
{
  lv_obj_t *target = lv_event_get_target(event);
  int value = lv_arc_get_value(target);
  printf("Arc 拖动进度：%d \n", value);
  lv_arc_set_value(right_Arc, value);
}

static void list_item_cb(lv_event_t *event)
{
  lv_obj_t *target = lv_event_get_target(event);
  lv_label_set_text(right_Title, lv_list_get_btn_text(lv_List, target));
  lv_obj_add_state(target, LV_STATE_FOCUS_KEY);
}

static void value_change_cb(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_VALUE_CHANGED)
  {
    if (lv_obj_has_state(fan_Checkbox, LV_STATE_CHECKED))
    {
      printf("复选框选中\n");
      lv_bar_set_value(fan_Bar, 80, LV_ANIM_ON);
      lv_led_toggle(fan_Led);
    }
    else
    {
      printf("复选框取消选中\n");
      lv_bar_set_value(fan_Bar, 50, LV_ANIM_ON);
      lv_led_toggle(fan_Led);
    }
  }
}

static void my_ui_init(void)
{
  int src_act_width = lv_obj_get_width(lv_scr_act());
  int src_act_height = lv_obj_get_height(lv_scr_act());

  // lv_obj_t *left_Container = lv_obj_create(lv_scr_act());
  // lv_obj_set_size(left_Container, 600, src_act_height);
  // lv_obj_align(left_Container, LV_ALIGN_LEFT_MID, 6, 6);
  // lv_obj_update_layout(left_Container);

  lv_List = lv_list_create(lv_scr_act());
  lv_obj_set_size(lv_List, 560, src_act_height - 48);
  lv_list_add_text(lv_List, "File");
  lv_obj_update_layout(lv_List);

  lv_obj_t *btn = lv_list_add_btn(lv_List, LV_SYMBOL_SD_CARD, "New");
  lv_obj_set_style_bg_color(btn, LV_COLOR_LIST_BTN_BG, LV_STATE_FOCUS_KEY);
  lv_obj_add_event_cb(btn, list_item_cb, LV_EVENT_CLICKED, NULL);

  btn = lv_list_add_btn(lv_List, LV_SYMBOL_EYE_OPEN, "Open");
  lv_obj_add_event_cb(btn, list_item_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btn, LV_COLOR_LIST_BTN_BG, LV_STATE_FOCUS_KEY);

  btn = lv_list_add_btn(lv_List, LV_SYMBOL_SAVE, "Save");
  lv_obj_add_event_cb(btn, list_item_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btn, LV_COLOR_LIST_BTN_BG, LV_STATE_FOCUS_KEY);

  btn = lv_list_add_btn(lv_List, LV_SYMBOL_DIRECTORY, "Delete");
  lv_obj_add_event_cb(btn, list_item_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btn, LV_COLOR_LIST_BTN_BG, LV_STATE_FOCUS_KEY);

  btn = lv_list_add_btn(lv_List, LV_SYMBOL_EDIT, "Edit");
  lv_obj_add_event_cb(btn, list_item_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btn, LV_COLOR_LIST_BTN_BG, LV_STATE_FOCUS_KEY);

  lv_list_add_text(lv_List, "Connectivity");

  btn = lv_list_add_btn(lv_List, LV_SYMBOL_BLUETOOTH, "Bluetooth");
  lv_obj_add_event_cb(btn, list_item_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btn, LV_COLOR_LIST_BTN_BG, LV_STATE_FOCUS_KEY);

  btn = lv_list_add_btn(lv_List, LV_SYMBOL_DOWNLOAD, "Nav");
  lv_obj_add_event_cb(btn, list_item_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btn, LV_COLOR_LIST_BTN_BG, LV_STATE_FOCUS_KEY);

  btn = lv_list_add_btn(lv_List, LV_SYMBOL_USB, "Usb");
  lv_obj_add_event_cb(btn, list_item_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btn, LV_COLOR_LIST_BTN_BG, LV_STATE_FOCUS_KEY);

  btn = lv_list_add_btn(lv_List, LV_SYMBOL_DRIVE, "Hardware");
  lv_obj_add_event_cb(btn, list_item_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btn, LV_COLOR_LIST_BTN_BG, LV_STATE_FOCUS_KEY);

  btn = lv_list_add_btn(lv_List, LV_SYMBOL_POWER, "Power");
  lv_obj_add_event_cb(btn, list_item_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(btn, LV_COLOR_LIST_BTN_BG, LV_STATE_FOCUS_KEY);

  lv_obj_t *right_Container = lv_obj_create(lv_scr_act());
  lv_obj_set_size(right_Container, src_act_width - lv_obj_get_width(lv_List) - 48, src_act_height);
  lv_obj_align(right_Container, LV_ALIGN_RIGHT_MID, 6, 6);

  right_Title = lv_label_create(right_Container);
  lv_obj_set_align(right_Title, LV_ALIGN_TOP_MID);

  right_Roller = lv_roller_create(right_Container);
  lv_obj_set_align(right_Roller, LV_ALIGN_CENTER);
  // lv_obj_set_width(right_Roller, 180);
  lv_obj_set_style_text_line_space(right_Roller, 30, LV_STATE_DEFAULT);
  lv_roller_set_options(right_Roller, "A\nB\nC\nD\nE\nF\nG\n", LV_ROLLER_MODE_NORMAL);
  lv_roller_set_selected(right_Roller, 2, LV_ANIM_ON);
  lv_roller_set_visible_row_count(right_Roller, 5);
  lv_obj_add_event_cb(right_Roller, roller_cb, LV_EVENT_VALUE_CHANGED, NULL);

  lv_obj_t *right_Slider = lv_slider_create(right_Container);

  right_Arc = lv_arc_create(right_Container);
  lv_obj_align(right_Arc, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_arc_set_value(right_Arc, 0);
  lv_arc_set_rotation(right_Arc, 270);
  lv_arc_set_bg_angles(right_Arc, 0, 360);
  lv_obj_remove_style(right_Arc, NULL, LV_PART_KNOB);
  lv_obj_clear_flag(right_Arc, LV_OBJ_FLAG_CLICKABLE);

  right_Arc1 = lv_arc_create(right_Container);
  lv_obj_align(right_Arc1, LV_ALIGN_BOTTOM_MID, 0, -200);
  lv_arc_set_value(right_Arc1, 0);
  lv_obj_add_event_cb(right_Arc1, arc_cb, LV_EVENT_VALUE_CHANGED, NULL);

  // parentObj = lv_obj_create(lv_scr_act());
  // lv_obj_set_size(parentObj, src_act_width * 2 / 3, src_act_height * 2 / 3);
  // lv_obj_align(parentObj, LV_ALIGN_TOP_MID, 0, 0);
  // lv_obj_set_style_bg_color(parentObj, lv_color_hex(0x99ccff), LV_STATE_DEFAULT);
  // lv_obj_set_style_bg_color(parentObj, LV_COLOR_LIST_BTN_BG, LV_STATE_PRESSED);
  // lv_obj_add_event_cb(parentObj, click_cb, LV_EVENT_CLICKED, NULL);

  // childObj = lv_obj_create(parentObj);
  // lv_obj_set_size(childObj, src_act_width / 3, src_act_height / 3);
  // lv_obj_align(childObj, LV_ALIGN_CENTER, 0, 0);
  // lv_obj_set_style_bg_color(childObj, lv_color_hex(0x003366), LV_STATE_DEFAULT);
  // lv_obj_add_event_cb(childObj, click_cb, LV_EVENT_CLICKED, NULL);

  // lv_obj_t *lableTitle = lv_label_create(parentObj);
  // lv_obj_align_to(lableTitle, childObj, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  // lv_label_set_recolor(lableTitle, true);
  // lv_label_set_text(lableTitle, "BrainCo #ff0000 Breeze# Bravo!");
  // lv_obj_set_style_text_font(lableTitle, &lv_font_montserrat_32, LV_PART_MAIN);

  // lv_obj_t *fan_Switch = lv_switch_create(parentObj);
  // lv_obj_align(fan_Switch, LV_ALIGN_LEFT_MID, 0, 0);
  // lv_obj_set_style_bg_color(fan_Switch, lv_color_hex(0x00ff00), LV_STATE_DEFAULT | LV_PART_KNOB);
  // lv_obj_set_style_bg_color(fan_Switch, lv_color_hex(0x0000FF), LV_STATE_CHECKED | LV_PART_INDICATOR);

  // fan_Checkbox = lv_checkbox_create(parentObj);
  // lv_obj_align(fan_Checkbox, LV_ALIGN_LEFT_MID, 0, 60);
  // lv_checkbox_set_text(fan_Checkbox, "Cooling");
  // lv_obj_set_style_text_color(fan_Checkbox, lv_color_hex(0xff0000), LV_STATE_DEFAULT);
  // lv_obj_set_style_bg_color(fan_Checkbox, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_set_style_bg_color(fan_Checkbox, lv_color_hex(0x00ff00), LV_PART_INDICATOR);
  // lv_obj_set_style_bg_color(fan_Checkbox, lv_color_hex(0x888888), LV_PART_INDICATOR | LV_STATE_CHECKED);
  // lv_obj_set_style_border_color(fan_Checkbox, lv_color_hex(0x00ff00), LV_PART_INDICATOR);
  // lv_obj_set_style_border_color(fan_Checkbox, lv_color_hex(0x888888), LV_PART_INDICATOR | LV_STATE_CHECKED);
  // lv_obj_add_event_cb(fan_Checkbox, value_change_cb, LV_EVENT_VALUE_CHANGED, NULL);

  // fan_Bar = lv_bar_create(parentObj);
  // lv_obj_align(fan_Bar, LV_ALIGN_LEFT_MID, 0, 100);
  // lv_obj_set_size(fan_Bar, 200, 20);
  // lv_bar_set_range(fan_Bar, 0, 100);
  // lv_obj_set_style_anim_time(fan_Bar, 200, LV_STATE_DEFAULT);

  // lv_bar_set_value(fan_Bar, 50, LV_ANIM_ON);

  // lv_obj_t *fan_Spanner = lv_spinner_create(parentObj, 3000, 48);
  // lv_obj_set_style_arc_color(fan_Spanner, lv_color_hex(0x888888), LV_PART_MAIN);
  // lv_obj_set_style_arc_color(fan_Spanner, lv_color_hex(0xff0000), LV_PART_INDICATOR);
  // lv_obj_set_style_arc_width(fan_Spanner, 12, LV_STATE_DEFAULT | LV_PART_MAIN);
  // lv_obj_set_style_arc_width(fan_Spanner, 16, LV_STATE_DEFAULT | LV_PART_INDICATOR);
  // lv_obj_set_align(fan_Spanner, LV_ALIGN_CENTER);

  // fan_Led = lv_led_create(parentObj);
  // lv_led_set_color(fan_Led, lv_color_hex(0xffff00));
  // lv_led_set_brightness(fan_Led, LV_LED_BRIGHT_MAX);

  // lv_obj_t *container = lv_obj_create(lv_scr_act());
  // lv_obj_set_size(container, 960, 480);
  // lv_obj_set_align(container, LV_ALIGN_CENTER);

  // static lv_style_t styleHover;
  // lv_style_init(&styleHover);
  // lv_style_set_bg_color(&styleHover, LV_COLOR_LIST_BTN_BG);
  // lv_obj_add_style(container, &styleHover, LV_STATE_PRESSED);

  // static lv_style_t style;
  // lv_style_init(&style);
  // lv_style_set_bg_color(&style, lv_color_hex(0xff0000));
  // lv_obj_add_style(container, &style, LV_STATE_DEFAULT);

  // // 添加事件响应
  // lv_obj_add_event_cb(container, event_cb, LV_EVENT_CLICKED, NULL);

  // lv_obj_t *text1 = lv_label_create(container);
  // char *str1 = "BrainCo";
  // lv_label_set_text(text1, str1);
  // lv_obj_align(text1, LV_ALIGN_TOP_MID, 0, 0);

  // lv_obj_t *text2 = lv_label_create(container);
  // char *str2 = "Breeze Bravo!";
  // lv_label_set_text(text2, str2);
  // lv_obj_align(text2, LV_ALIGN_CENTER, 0, 0);

  // lv_obj_t *text3 = lv_label_create(container);
  // char *str3 = "Breeze Bravo!";
  // lv_label_set_text(text3, str3);
  // lv_obj_align(text3, LV_ALIGN_BOTTOM_MID, 0, 0);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static void hal_init(void)
{
  /* mouse input device */
  static lv_indev_drv_t indev_drv_1;
  lv_indev_drv_init(&indev_drv_1);
  indev_drv_1.type = LV_INDEV_TYPE_POINTER;

  /* keyboard input device */
  static lv_indev_drv_t indev_drv_2;
  lv_indev_drv_init(&indev_drv_2);
  indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;

  /* mouse scroll wheel input device */
  static lv_indev_drv_t indev_drv_3;
  lv_indev_drv_init(&indev_drv_3);
  indev_drv_3.type = LV_INDEV_TYPE_ENCODER;

  lv_group_t *g = lv_group_create();
  lv_group_set_default(g);

  lv_disp_t *disp = NULL;

#if USE_SDL
  /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  sdl_init();

  /*Create a display buffer*/
  static lv_disp_draw_buf_t disp_buf1;
  static lv_color_t buf1_1[MONITOR_HOR_RES * 100];
  static lv_color_t buf1_2[MONITOR_HOR_RES * 100];
  lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, MONITOR_HOR_RES * 100);

  /*Create a display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/
  disp_drv.draw_buf = &disp_buf1;
  disp_drv.flush_cb = sdl_display_flush;
  disp_drv.hor_res = MONITOR_HOR_RES;
  disp_drv.ver_res = MONITOR_VER_RES;
  disp_drv.antialiasing = 1;

  disp = lv_disp_drv_register(&disp_drv);

  /* Add the input device driver */
  // mouse_init();
  indev_drv_1.read_cb = sdl_mouse_read;

  // keyboard_init();
  indev_drv_2.read_cb = sdl_keyboard_read;

  // mousewheel_init();
  indev_drv_3.read_cb = sdl_mousewheel_read;

#elif USE_X11
  lv_x11_init("LVGL Simulator Demo", DISP_HOR_RES, DISP_VER_RES);

  /*Create a display buffer*/
  static lv_disp_draw_buf_t disp_buf1;
  static lv_color_t buf1_1[DISP_HOR_RES * 100];
  static lv_color_t buf1_2[DISP_HOR_RES * 100];
  lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, DISP_HOR_RES * 100);

  /*Create a display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.draw_buf = &disp_buf1;
  disp_drv.flush_cb = lv_x11_flush;
  disp_drv.hor_res = DISP_HOR_RES;
  disp_drv.ver_res = DISP_VER_RES;
  disp_drv.antialiasing = 1;

  disp = lv_disp_drv_register(&disp_drv);

  /* Add the input device driver */
  indev_drv_1.read_cb = lv_x11_get_pointer;
  indev_drv_2.read_cb = lv_x11_get_keyboard;
  indev_drv_3.read_cb = lv_x11_get_mousewheel;
#endif
  /* Set diplay theme */
  lv_theme_t *th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
  lv_disp_set_theme(disp, th);

  /* Tick init */
  end_tick = false;
  pthread_create(&thr_tick, NULL, tick_thread, NULL);

  /* register input devices */
  lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);
  lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
  lv_indev_t *enc_indev = lv_indev_drv_register(&indev_drv_3);
  lv_indev_set_group(kb_indev, g);
  lv_indev_set_group(enc_indev, g);

  /* Set a cursor for the mouse */
  LV_IMG_DECLARE(mouse_cursor_icon);                  /*Declare the image file.*/
  lv_obj_t *cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor*/
  lv_img_set_src(cursor_obj, &mouse_cursor_icon);     /*Set the image source*/
  lv_indev_set_cursor(mouse_indev, cursor_obj);       /*Connect the image  object to the driver*/
}

/**
 * Releases the Hardware Abstraction Layer (HAL) for the LVGL graphics library
 */
static void hal_deinit(void)
{
  end_tick = true;
  pthread_join(thr_tick, NULL);

#if USE_SDL
  // nop
#elif USE_X11
  lv_x11_deinit();
#endif
}

/**
 * A task to measure the elapsed time for LVGL
 * @param data unused
 * @return never return
 */
static void *tick_thread(void *data)
{
  (void)data;

  while (!end_tick)
  {
    usleep(5000);
    lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
  }

  return NULL;
}
