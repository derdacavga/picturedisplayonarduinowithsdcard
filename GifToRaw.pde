import java.io.*;

void setup() {
  String folderPath = sketchPath("");
  File folder = new File(folderPath);
  
  if (!folder.exists()) {
    println("Dir not Found!");
    return;
  }
  
  File[] files = folder.listFiles();
  
  if (files == null || files.length == 0) {
    println("İmage file not Found!");
    return;
  }
  
  for (File file : files) {
    if (file.isFile() && isImageFile(file.getName())) {
      convertToBMP(file);
    }
  }
  
  println("All image can turned");
}

boolean isImageFile(String filename) {
  String lower = filename.toLowerCase();
  return lower.endsWith(".jpg") || lower.endsWith(".jpeg") || lower.endsWith(".png");
}

void convertToBMP(File file) {
  PImage img = loadImage(file.getAbsolutePath());
  
  if (img == null) {
    println("error: " + file.getName() + " not installed!");
    return;
  }

  img.resize(280, 240);

  if (img.height > img.width) {
    img = rotateImage(img);
  }

  String bmpFileName = file.getName().substring(0, file.getName().lastIndexOf(".")) + ".bmp";
  String outputPath = sketchPath(bmpFileName);
  
  img.save(outputPath);
  println("Saved: " + bmpFileName);
}

PImage rotateImage(PImage img) {
  PImage rotated = createImage(img.height, img.width, RGB);
  img.loadPixels();
  rotated.loadPixels();
  
  for (int x = 0; x < img.width; x++) {
    for (int y = 0; y < img.height; y++) {
      int srcIndex = x + y * img.width;
      int dstIndex = y + (img.width - 1 - x) * img.height;
      rotated.pixels[dstIndex] = img.pixels[srcIndex];
    }
  }
  
  rotated.updatePixels();
  return rotated;
}
