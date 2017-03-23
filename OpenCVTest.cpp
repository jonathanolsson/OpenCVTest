#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <ctime>
#include <windows.h>

using namespace std;
using namespace cv;

//Enum class, required to be one of the contains.
enum Classifier
{
	LBP, HAAR
};

/** Function Headers */
int detectLBP(Mat frame);
int detectHaar(Mat frame);

void testSpeed(Classifier al, Mat frame);
void testAccuracy(Classifier al, Mat frame);
void displayFaces(Mat frame, std::vector<Rect> faces, String windowName = "Window");


/** Global variables */
String face_cascade_name_haar = "C:/Users/jonathan/Documents/Visual Studio 2015/Projects/OpenCVTest/data/haarcascades/haarcascade_frontalface_alt2.xml";
String face_cascade_name_lbp = "C:/Users/jonathan/Documents/Visual Studio 2015/Projects/OpenCVTest/data/lbpcascades/lbpcascade_frontalface.xml";

String image = "C:/Users/jonathan/Documents/Visual Studio 2015/Projects/OpenCVTest/Images/group2.jpg";

//String face_cascade_name = "../../data/haarcascades_cuda/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade_haar;
CascadeClassifier face_cascade_lbp;

RNG rng(12345);

/** @function main */
int main(int argc, const char** argv)
{
	//VideoCapture capture(0);
	Mat frame;

	//Load the cascades
	if (!face_cascade_haar.load(face_cascade_name_haar)) {
		printf("--(!)Error loading HAAR cascade\n");
		return -1;
	}

	if (!face_cascade_lbp.load(face_cascade_name_lbp)) {
		printf("--(!)Error loading LBP cascade\n");
		return -1;
	}

	frame = imread(image, CV_LOAD_IMAGE_COLOR);

	
	if (!frame.empty()) {

		testSpeed(LBP, frame);

		testSpeed(HAAR, frame);
		/*
		Accuracy test

		int previousCount = 0;
		for (int i = 0; i < 250; i++) {
			int count = detectLBP(frame);
			
			if (i == 75) {
				frame = frame2;
				cout << "new, group 2" << endl;
			}
			if (i == 150) {
				frame = frame3;
				cout << "new, group 3" << endl;
			}

			if (previousCount != count) {
				cout << "Previous: " << previousCount << endl;
				cout << "This: " << count << endl;
			}
			previousCount = count;

			//detectHaar(frame);
		}
		*/



		/*
		for (int i = 0; i < 2; i++) {
			namedWindow((window_name + "lbp" + i), WINDOW_AUTOSIZE);
			imshow((window_name + "lbp" + i), detectLBP(frame));
		}

		for (int i = 0; i < 2; i++) {
			namedWindow((window_name + "haar" + i), WINDOW_AUTOSIZE);
			imshow((window_name + "haar" + i), detectHaar(frame2));
		}
		*/
	}

	waitKey(0);



	/*
	std::clock_t start;
	double seconds;

	start = std::clock();



	seconds = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	std::cout << "time: " << seconds << std::endl;
	*/
	
	/*
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
				detectHaar(frame);
				detectLBP(frame);

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
	*/
	return 0;
}


//Display the faces of a frame
void displayFaces(Mat frame, std::vector<Rect> faces, String windowName) {
	for (size_t i = 0; i < faces.size(); i++) {
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, Size(faces[i].width*0.6, faces[i].height*0.7), 0, 0, 360, Scalar(0, 0, 255), 4, 8, 0);
	}

	namedWindow((windowName), WINDOW_AUTOSIZE);
	imshow((windowName), frame);

	waitKey(15);

}


//Test the speed of a algorithm
void testSpeed(Classifier al, Mat frame) {
	//Face Vector
	std::vector<Rect> faces;
	
	Mat tempFrame;
	frame.copyTo(tempFrame);

	switch (al)
	{
		case LBP:
			cout << "SPEED TEST LBP: \n";
			

			std::clock_t start;
			double seconds;

			start = std::clock();

			for (int i = 0; i < 10; i++) {
				face_cascade_lbp.detectMultiScale(tempFrame, faces, 1.1, 2, 0, Size(30, 30));
			}

			seconds = (std::clock() - start) / (double)CLOCKS_PER_SEC;

			std::cout << "time: " << seconds << std::endl;
			

			displayFaces(tempFrame, faces, "LBP");

		break;
		case HAAR:
			cout << "SPEED TEST HAAR: \n";
			
			cvtColor(tempFrame, tempFrame, CV_BGR2GRAY);
			//equalizeHist(tempFrame, tempFrame);

			face_cascade_haar.detectMultiScale(tempFrame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
			displayFaces(tempFrame, faces, "HAAR");
		break;
		default:
			
		break;
	}
}



//Test the accuracy of a algorithm (Not necessary?)
void testAccuracy(Classifier al, Mat frame) {
	//FaceVector
	std::vector<Rect> faces;
	int previousCount;
	int count;

	switch (al){
		case LBP:
			cout << "LBP \n";

			for (int i = 0; i < 10; i++) {
				Mat tempFrame = frame;

				face_cascade_lbp.detectMultiScale(tempFrame, faces, 1.1, 3, 0, Size(30, 30));
				
				if (i == 0) {
					previousCount = faces.size();
				}
				count = faces.size();

				if (previousCount != count) {
					cout << "Previous: " << previousCount << endl;
					cout << "This: " << count << endl;
				}
				previousCount = count;
			}
				
			break;
		case HAAR:
			cout << "HAAR \n";

			for (int i = 0; i < 15; i++) {
				Mat tempFrame;
				frame.copyTo(tempFrame);

				
				cvtColor(frame, tempFrame, CV_BGR2GRAY);
				
				/*
				if (i % 2 == 0)
					equalizeHist(tempFrame, tempFrame);
				*/
				
				face_cascade_haar.detectMultiScale(tempFrame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
				displayFaces(tempFrame, faces);
				
				if (i == 0) {
					previousCount = faces.size();
				}
				count = faces.size();

				if (previousCount != count) {
					cout << "Previous: " << previousCount << endl;
					cout << "This: " << count << endl;
					displayFaces(tempFrame, faces);
				}
				previousCount = count;
			}
			break;
		default:
			cout << "";
			break;
	}
}

/*		Detect and Display with LBP classifier		*/
int detectLBP(Mat frame) {
	std::vector<Rect> faces;

	//Detect
	face_cascade_lbp.detectMultiScale(frame, faces, 1.1, 2, 0, Size(30, 30));
	/*
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, Size(faces[i].width*0.6, faces[i].height*0.7), 0, 0, 360, Scalar(0, 0, 255), 4, 8, 0);
	}
	namedWindow((window_name), WINDOW_AUTOSIZE);
	imshow((window_name), frame);
	*/

	return faces.size();
}


/*		Detect and Display with HAAR classifier		*/
int detectHaar(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//Detect
	face_cascade_haar.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	
	/*
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, Size(faces[i].width*0.6, faces[i].height*0.7), 0, 0, 360, Scalar(0, 0, 255), 4, 8, 0);
	}
	namedWindow((window_name), WINDOW_AUTOSIZE);
	imshow((window_name), frame);

	*/
	//cout << "HAAR" << faces.size() << endl;


	return faces.size();
}
