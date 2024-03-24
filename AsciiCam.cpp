#include <opencv2/opencv.hpp>
#include <climits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <map>
#include <vector>
#include <sstream>
#include <stack>
#include <thread>

using namespace cv;
using namespace std;

const int yres = 150;
const float brightnessmult = 1.2;

const string ASCII_CHARS = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
const vector<float> BRIGHTNESS_VALUES = {
    0, 0.0751, 0.0829, 0.0848, 0.1227, 0.1403, 0.1559, 0.185, 0.2183, 0.2417, 0.2571, 0.2852, 0.2902, 0.2919, 0.3099,
    0.3192, 0.3232, 0.3294, 0.3384, 0.3609, 0.3619, 0.3667, 0.3737, 0.3747, 0.3838, 0.3921, 0.396, 0.3984, 0.3993, 0.4075,
    0.4091, 0.4101, 0.42, 0.423, 0.4247, 0.4274, 0.4293, 0.4328, 0.4382, 0.4385, 0.442, 0.4473, 0.4477, 0.4503, 0.4562,
    0.458, 0.461, 0.4638, 0.4667, 0.4686, 0.4693, 0.4703, 0.4833, 0.4881, 0.4944, 0.4953, 0.4992, 0.5509, 0.5567, 0.5569,
    0.5591, 0.5602, 0.5602, 0.565, 0.5776, 0.5777, 0.5818, 0.587, 0.5972, 0.5999, 0.6043, 0.6049, 0.6093, 0.6099, 0.6465,
    0.6561, 0.6595, 0.6631, 0.6714, 0.6759, 0.6809, 0.6816, 0.6925, 0.7039, 0.7086, 0.7235, 0.7302, 0.7332, 0.7602, 0.7834,
    0.8037, 0.9999
};

char getASCIIChar(float brightness) {
    brightness *= brightnessmult;
    int left = 0;
    int right = BRIGHTNESS_VALUES.size() - 1;

    if (right < 0) { // Handle empty BRIGHTNESS_VALUES
        return '?'; // Return a placeholder or error character
    }

    while (left < right) {
        int mid = left + (right - left) / 2;
        if (brightness == BRIGHTNESS_VALUES[mid]) {
            return ASCII_CHARS[mid];
        }
        else if (brightness < BRIGHTNESS_VALUES[mid]) {
            right = mid;
        }
        else {
            if (left == mid) break; // Prevents infinite loop
            left = mid;
        }
    }

    // After narrowing down, choose the closest among left and right
    // This handles the scenario where brightness doesn't exactly match any value in BRIGHTNESS_VALUES
    if (std::abs(brightness - BRIGHTNESS_VALUES[left]) <= std::abs(brightness - BRIGHTNESS_VALUES[right])) {
        return ASCII_CHARS[left];
    }
    else {
        return ASCII_CHARS[right];
    }
}

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        // The function failed. Call GetLastError() for details.
        COORD invalid = { 0, 0 };
        return invalid;
    }
}

void processFrame(Mat& frame) {
    Mat gray, resized;
    cvtColor(frame, gray, COLOR_BGR2GRAY); // Convert to grayscale
    resize(gray, resized, Size(yres * 3, yres)); // Resize the frame
    for (int i = 0; i < resized.rows; ++i) {
        string str = "";
        for (int j = 0; j < resized.cols; ++j) {
            float brightness = resized.at<uchar>(i, j) / 255.0f;
            str += getASCIIChar(brightness);
        }
        cout << str << endl;
    }
}

void goBack(int b) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = GetConsoleCursorPosition(hConsole);
    pos.Y -= b; // return to first pos to rerender
    SetConsoleCursorPosition(hConsole, pos);
}

void set_cursor(bool visible) {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 10;
    info.bVisible = visible;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

int main() {
    std::cout.sync_with_stdio(false);
    cin.tie(NULL);
    cv::VideoCapture VideoCapture;
    // Frame read from Camera will be stored in CamFrame MAT
    cv::Mat CamFrame, flippedFrame;
    // Open camera
    VideoCapture.open(0);
    if (VideoCapture.isOpened())  // Check if we succeeded
    {
        set_cursor(false);
        while (true) {
            VideoCapture.read(CamFrame);
            flip(CamFrame, flippedFrame, 1);
            // Calculate new dimensions
            goBack(yres);
            processFrame(flippedFrame);
            if (waitKey(30) >= 0) break; // Break on any key press

        }
        //   for (;;){
    /*
          // Read frame from Camera
        VideoCapture.read(CamFrame);
    // check if we succeeded
    if (CamFrame.empty()) {
        cerr << "ERROR! blank frame grabbed\n";
    }
    cv::imshow("live frame ", CamFrame);
    waitKey(0);

    // }*/
    }
    /*VideoCapture cap(0); // Open the default camera
    cout << 2;
    if (!cap.isOpened()) {
        cerr << "ERROR: Unable to open the camera" << endl;
        return 1;
    }

    cout << "\x1b[H"; // Move cursor to the beginning of the output

    while (true) {
        Mat frame;
        cap2 >> frame; // Capture a new frame

        if (frame.empty()) {
            cerr << "ERROR: Frame is empty" << endl;
            break;
        }

        processFrame(frame);

        // Overwrite the previous output by resetting the cursor position
        cout << "\x1b[H";

        if (waitKey(30) >= 0) break; // Break on any key press
    }

    return 0;*/
}
