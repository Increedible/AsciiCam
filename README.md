# AsciiCam
Generate Ascii Art of your Camera in Real Time!

**Requirements**

Camera permission! Make sure your antivirus is allowing cam permission to the file. This program does not connect to the internet in any way.

Opencv2 is used to get the camera output, so make sure you have that linked aswell.

**Usage**

The yres constant in the start defines the size of the square, in command prompt you can just use ctrl + mouse wheel to zoom out and see the whole picture. Low yres means good fps, but bad quality. Opposite is true aswell!
Brightnessmult defines how bright your output will be. If it is too bright, tune it down, elsewise, tune it up.

If you have multiple cameras plugged, make sure to select the right one with the line `VideoCapture.open(x);` in int main, where x is your index for that camera.
