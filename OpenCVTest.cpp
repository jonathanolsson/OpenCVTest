#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <ctime>

using namespace std;
using namespace cv;

/** Function Headers */
void detectDisplayHaar(Mat frame);
void detectDisplayLBP(Mat frame);

/** Global variables */
String face_cascade_name_haar = "../../data/haarcascades/haarcascade_frontalface_alt.xml";
String face_cascade_name_lbp = "../../data/lbpcascades/lbpcascade_frontalface.xml";

//String face_cascade_name = "../../data/haarcascades_cuda/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade_haar;
CascadeClassifier face_cascade_lbp;

string window_name = "Capture - Face detection";
RNG rng(12345);

/** @function main */
int main(int argc, const char** argv)
{
	VideoCapture capture(0);
	Mat frame;

	//Load the cascades
	if (!face_cascade_haar.load(face_cascade_name_haar)) {
		printf("--(!)Error loading HAAR cascade\n");
		return -1;
	};

	if (!face_cascade_lbp.load(face_cascade_name_lbp)) {
		printf("--(!)Error loading LBP cascade\n");
		return -1;
	};


	//Read video stream
	if (capture.isOpened())
	{
		while (true)
		{
			capture.read(frame);

			//Apply the wanted classifier to the frame
			if (!frame.empty())
			{

				std::clock_t start;
				double seconds;

				start = std::clock();
				detectDisplayHaar(frame);
				//detectDisplayLBP(frame);

				seconds = (std::clock() - start) / (double)CLOCKS_PER_SEC;

				std::cout << "time: " << seconds << std::endl;
			}
			else
			{
				printf(" --(!) No captured frame -- Break!"); break;
			}

			int c = waitKey(15);
			if ((char)c == 'c') { break; }

		}
	}
	return 0;
}


/*		Detect and Display with LBP classifier		*/
void detectDisplayLBP(Mat frame) {
	std::vector<Rect> faces;

	//Detect
	face_cascade_lbp.detectMultiScale(frame, faces, 1.1, 2, 0, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

	}
	//Display
	imshow(window_name, frame);
}


/*		Detect and Display with HAAR classifier		*/
void detectDisplayHaar(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//Detect
	face_cascade_haar.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		
		ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.7), 0, 0, 360, Scalar(0, 0, 255), 4, 8, 0);

	}
	//Display
	imshow(window_name, frame);
}
