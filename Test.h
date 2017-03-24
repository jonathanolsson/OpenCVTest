#pragma once

#include <iostream>
#include <ctime>


/** Global variables */
std::string face_cascade_name_haar = "C:/Users/jonathan/Documents/Visual Studio 2015/Projects/OpenCVTest/data/haarcascades/haarcascade_mcs_upperbody.xml";
std::string face_cascade_name_lbp = "C:/Users/jonathan/Documents/Visual Studio 2015/Projects/OpenCVTest/data/lbpcascades/lbpcascade_frontalface.xml";

std::string image = "C:/Users/jonathan/Documents/Visual Studio 2015/Projects/OpenCVTest/Images/round.jpg";

//String face_cascade_name = "../../data/haarcascades_cuda/haarcascade_frontalface_alt.xml";
cv::CascadeClassifier face_cascade_haar;
cv::CascadeClassifier face_cascade_lbp;

cv::RNG rng(12345);


//Enum, choose one of these to test.
enum Classifier
{
	LBP, HAAR
};


//Display the faces of a frame
void displayFaces(cv::Mat frame, std::vector<cv::Rect> faces, std::string windowName = "Window") {
	for (size_t i = 0; i < faces.size(); i++) {
		cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(0, 0, 255), 4, 8, 0);
	}

	cv::namedWindow((windowName), cv::WINDOW_AUTOSIZE);
	imshow((windowName), frame);

}

void captureVideo(Classifier al) {
	//Video capture
	cv::VideoCapture capture(0);
	cv::Mat frame;
	cv::Mat tempFrame;

	//Vector of faces.
	std::vector<cv::Rect> faces;

	//Switch-case on the classifier. sets webcam as input.
	switch (al)
	{
	case LBP:
		//std::cout << "DETECT FACE LBP: \n";
		if (capture.isOpened()) {
			while (true) {
				capture.read(frame);

				//Apply the wanted classifier to the frame
				if (!frame.empty()) {
					face_cascade_lbp.detectMultiScale(frame, faces, 1.1, 2, 0, cv::Size(20, 20));

					displayFaces(frame, faces, "LBP");

				}
				else {
					printf(" --(!) No captured frame -- Break!"); break;
				}

				int c = cv::waitKey(15);
				if ((char)c == 'c') {
					break;
				}
			}
		}

		break;

	case HAAR:
		//std::cout << "DETECT FACE HAAR: \n";
		frame.copyTo(tempFrame);

		if (capture.isOpened()) {
			while (true) {
				capture.read(frame);

				//Apply the wanted classifier to the frame
				if (!frame.empty()) {
					cv::cvtColor(frame, tempFrame, CV_BGR2GRAY);
					cv::equalizeHist(tempFrame, tempFrame);

					face_cascade_haar.detectMultiScale(tempFrame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(40, 40));

					displayFaces(frame, faces, "HAAR");

				}
				else {
					printf(" --(!) No captured frame -- Break!"); break;
				}

				int c = cv::waitKey(15);
				if ((char)c == 'c') {
					break;
				}
			}
		}

		break;

	default:
		std::cout << "undefined classifier. \n";
		break;
	}
}


//Test the speed of a algorithm
void testSpeed(Classifier al, size_t loopSize = 10) {
	//Vector of faces.
	std::vector<cv::Rect> faces;

	//New Mat = frame with image.
	cv::Mat frame = cv::imread(image, CV_LOAD_IMAGE_COLOR);

	//A temporary frame so that the frame is new on each call.
	cv::Mat tempFrame;
	frame.copyTo(tempFrame);

	//Clock timer and total time.
	std::clock_t timer;
	double totalTime;

	//Check if image was loaded.
	if (frame.empty()) {
		return ;
	}

	//Switch-case on the classifier, set the timer to now and then run the classifier "loopSize" times.
	//The total time this took and the average time is printed.
	switch (al)
	{
	case LBP:
		std::cout << "SPEED TEST LBP: \n";

		timer = std::clock();

		for (int i = 0; i < loopSize; i++) {
			face_cascade_lbp.detectMultiScale(tempFrame, faces, 1.1, 2, 0, cv::Size(20, 20));
		}

		totalTime = (std::clock() - timer) / (double)CLOCKS_PER_SEC;

		std::cout << "Total time: " << totalTime << "s" << std::endl;
		std::cout << "Average time of " << loopSize <<" times: " << totalTime/loopSize << "s" << std::endl;

		displayFaces(tempFrame, faces, "LBP");
		cv::waitKey(0);

		break;

	case HAAR:
		std::cout << "SPEED TEST HAAR: \n";

		cv::cvtColor(tempFrame, tempFrame, CV_BGR2GRAY);
		cv::equalizeHist(tempFrame, tempFrame);

		timer = std::clock();

		for (int i = 0; i < loopSize; i++) {
			face_cascade_haar.detectMultiScale(tempFrame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(50, 50));
		}

		totalTime = (std::clock() - timer) / (double)CLOCKS_PER_SEC;

		std::cout << "Total time: " << totalTime << "s" << std::endl;
		std::cout << "Average time of " << loopSize << " times: " << totalTime/loopSize << "s" << std::endl;

		displayFaces(frame, faces, "HAAR");
		cv::waitKey(0);
		
		break;
	default:
		std::cout << "undefined classifier. \n";
		break;
	}
}


//Test the accuracy of a algorithm (Not necessary?)
void testAccuracy(Classifier al, cv::Mat frame) {
	//FaceVector
	std::vector<cv::Rect> faces;
	int previousCount;
	int count;

	switch (al) {

	case LBP:
		std::cout << "LBP \n";

		for (int i = 0; i < 10; i++) {
			cv::Mat tempFrame = frame;

			face_cascade_lbp.detectMultiScale(tempFrame, faces, 1.1, 3, 0, cv::Size(30, 30));

			if (i == 0) {
				previousCount = faces.size();
			}
			count = faces.size();

			if (previousCount != count) {
				std::cout << "Previous: " << previousCount << std::endl;
				std::cout << "This: " << count << std::endl;
			}
			previousCount = count;
		}

		break;

	case HAAR:
		std::cout << "HAAR \n";

		for (int i = 0; i < 15; i++) {
			cv::Mat tempFrame;
			frame.copyTo(tempFrame);


			cv::cvtColor(frame, tempFrame, CV_BGR2GRAY);

			/*
			if (i % 2 == 0)
				equalizeHist(tempFrame, tempFrame);
			*/

			face_cascade_haar.detectMultiScale(tempFrame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
			displayFaces(tempFrame, faces);

			if (i == 0) {
				previousCount = faces.size();
			}
			count = faces.size();

			if (previousCount != count) {
				std::cout << "Previous: " << previousCount << std::endl;
				std::cout << "This: " << count << std::endl;
				displayFaces(tempFrame, faces);
			}
			previousCount = count;
		}
		break;

	default:
		std::cout << "";
		break;
	}
}