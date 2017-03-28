#pragma once

#include <iostream>
#include <ctime>,

//Enum, choose one of these to test.
enum Classifier
{
	LBP, HAAR
};

/** Global variables */
std::string cascadeName = "";
std::string image = "";

Classifier algorithm = HAAR;
std::string webcam;

//String face_cascade_name = "../../data/haarcascades_cuda/haarcascade_frontalface_alt.xml";
cv::CascadeClassifier cascade;

cv::RNG rng(12345);

//Display the faces of a frame
void displayFaces(cv::UMat frame, std::vector<cv::Rect> faces, std::string windowName = "Window") {
	for (size_t i = 0; i < faces.size(); i++) {
		cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(0, 0, 255), 4, 8, 0);
	}

	cv::namedWindow((windowName), cv::WINDOW_AUTOSIZE);
	imshow((windowName), frame);

}

void captureVideo(Classifier al) {
	//Video capture
	
	cv::VideoCapture capture(std::stoi(webcam));
	cv::UMat frame;
	cv::UMat tempFrame;

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
					cascade.detectMultiScale(frame, faces, 1.1, 2, 0, cv::Size(30, 30));

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
		std::cout << "Closing the camera" << std::endl;
		capture.release();
		cv::destroyAllWindows();
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

					cascade.detectMultiScale(tempFrame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(40, 40));

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

		std::cout << "Closing the camera" << std::endl;
		capture.release();
		cv::destroyAllWindows();
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

	//New UMat = frame with image.
	cv::UMat frame = cv::imread(image, CV_LOAD_IMAGE_COLOR).getUMat(cv::ACCESS_RW);

	//A temporary frame so that the frame is new on each call.
	cv::UMat tempFrame;
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
			cascade.detectMultiScale(tempFrame, faces, 1.1, 2, 0, cv::Size(20, 20));
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
			cascade.detectMultiScale(tempFrame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(50, 50));
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