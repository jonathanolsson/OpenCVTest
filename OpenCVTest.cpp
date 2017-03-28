#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Test.h"

#include <windows.h>


/** @function main */
int main(int argc, char** argv)
{

	/*
	
	HELP: 
	openCV.exe -w <Webcam-number> -i <image> -c <Cascade> -a <algorithm(HAAR, LBP) -l <amount of runs on image>

	*/

	bool test;
	int numberOfTests = 10;

	if (argc < 0) {
		std::cerr << "Error, the program needs at least 2 parameters, with value.\n"
			<< "The input should look like:\n \"openCV.exe -w <Webcam-number> -i <image> -c <Cascade> -a <algorithm(HAAR, LBP)>\"";
		return -1;
	}

	if (std::string(argv[1]) == "-w") {
		webcam = argv[2];
		test = false;
	}
	else if (std::string(argv[1]) == "-i") {
		image = argv[2];
		test = true;
	}
	else {
		std::cerr << "Error, chose either the webcamera or a image.\n you chose: " << argv[1] << std::endl;
		return -1;
	}


	if (std::string(argv[3]) == "-c") {
		cascadeName = argv[4];
	}
	else {
		std::cerr << "Error, enter a directory to the cascade.\n";
		return -1;
	}


	if (std::string(argv[5]) == "-a") {
		if (std::string(argv[6]) == "HAAR") {
			algorithm = HAAR;
		}
		else if (std::string(argv[6]) == "LBP") {
			algorithm = LBP;
		}
	}

	if (std::string(argv[7]) == "-l") {
		numberOfTests = std::stoi(argv[8]);
	}

	//Load the cascades
	if (!cascade.load(cascadeName)) {
		printf("--(!)Error loading HAAR cascade\n");
		return -1;
	}

	if (test) {
		testSpeed(algorithm, numberOfTests);
	}
	else {
		captureVideo(algorithm);
	}

	//Run the tests
	//testSpeed(LBP, 5);
	//testSpeed(algorithm);
/*
	captureVideo(HAAR);
	captureVideo(LBP);
*/


	//cv::Mat frame = cv::imread(image, CV_LOAD_IMAGE_COLOR);

	////Run the tests
	//if (!frame.empty()){
	//	//testSpeed(LBP, 5);
	//	testSpeed(HAAR, 50);
	//}

	return 0;
}






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
