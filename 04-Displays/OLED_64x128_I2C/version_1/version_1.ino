#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);

char bufferX[20];
char bufferY[20];
char bufferZ[20];

void setup() {
}

void loop() {
  sprintf(bufferX, "Hello");
  sprintf(bufferY, "Test");
  sprintf(bufferZ, "123");
  u8g.firstPage();
  do {
    draw();
  } while (u8g.nextPage());
  delay(10);
}

void draw() {
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, 20, bufferX);
  u8g.drawStr(0, 40, bufferY);
  u8g.drawStr(0, 60, bufferZ);
}