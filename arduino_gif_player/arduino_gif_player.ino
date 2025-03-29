#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8
#define SD_CS 4

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);

  SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));

  tft.init(240, 280);
  tft.setRotation(3);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD card not connected");
    while (1)
      ;
  }

  showImagesFromSD();
}

void loop() {
  showImagesFromSD();
}

void showImagesFromSD() {
  File root = SD.open("/");

  while (true) {
    root.rewindDirectory();
    File entry = root.openNextFile();

    while (entry) {
      if (!entry.isDirectory() && isBMP(entry.name())) {

        drawBMP(entry.name(), 0, 0);
        delay(2000);  // Showing time
      }
      entry.close();
      entry = root.openNextFile();
    }
  }
}

bool isBMP(const char *filename) {
  String name = String(filename);
  name.toLowerCase();
  return name.endsWith(".bmp");
}

void drawBMP(const char *filename, int16_t x, int16_t y) {
  File bmpFile = SD.open(filename);
  if (!bmpFile) {
    Serial.println("Bmd file not open!");
    return;
  }

  uint8_t bmpHeader[54];
  bmpFile.read(bmpHeader, 54);

  uint16_t width = bmpHeader[18] + (bmpHeader[19] << 8);
  uint16_t height = bmpHeader[22] + (bmpHeader[23] << 8);

  uint32_t imageOffset = bmpHeader[10] + (bmpHeader[11] << 8);
  bmpFile.seek(imageOffset);

  tft.startWrite();
  tft.setAddrWindow(x, y, width, height);

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      uint8_t b = bmpFile.read();
      uint8_t g = bmpFile.read();
      uint8_t r = bmpFile.read();

      uint16_t color = tft.color565(r, g, b);
      tft.pushColor(color);
    }
  }

  tft.endWrite();
  bmpFile.close();
}
