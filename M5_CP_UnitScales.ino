#include <M5Unified.h>
#include "UNIT_SCALES.h"    // https://github.com/m5stack/M5Unit-Miniscale

M5Canvas canvas(&M5.Lcd);
UNIT_SCALES scales;

uint16_t dw, dh;

void setup() {
    auto cfg = M5.config();
    cfg.serial_baudrate = 115200;
    M5.begin(cfg);
    M5.Lcd.begin();
    Serial.begin(115200);
    Serial.println("System Start!");

    // Initialize the canvas
    canvas.setRotation(0); // Adjust rotation as necessary
    canvas.setColorDepth(8);  // mono color
    canvas.setFont(&fonts::efontCN_12);
    dh = M5.Lcd.width();
    dw = M5.Lcd.height();
    canvas.createSprite(dh, dw);
    canvas.setTextSize(2);

    // Initialize the scales
    while (!scales.begin(&Wire, G2, G1, DEVICE_DEFAULT_ADDR)) { // Updated I2C pins for M5Cardputer
        Serial.println("scales connect error");
        canvas.drawString("scales connect error", 10, 10);
        canvas.pushSprite(0, 0);
        delay(1000);
    }
    scales.setLEDColor(0x001000);
}

void loop() {
    float weight = scales.getWeight();
    float gap    = scales.getGapValue();
    int adc      = scales.getRawADC();

    canvas.fillSprite(BLACK);
    canvas.setTextColor(YELLOW);
    canvas.setTextSize(2);
    canvas.drawString("M5Cardputer Scales", 10, 5);

    canvas.setTextColor(WHITE);
    canvas.setCursor(10, 35);
    canvas.setTextSize(2);
    canvas.printf("Weight:");

    canvas.setTextColor(GREEN);
canvas.setTextSize(4);
char buff[16];
sprintf(buff, "%.1f g", weight);
canvas.drawString(buff, dw / 3, 60); // Adjust this to ensure proper alignment

canvas.pushSprite(0, 0);

    M5.update();
    if (M5.BtnA.wasPressed()) {
        scales.setOffset();
    }
    if (M5.BtnB.wasPressed()) {
        delay(500);
        esp_restart();
    }
    delay(10);
}
