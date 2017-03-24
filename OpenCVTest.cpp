#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Test.h"

#include <windows.h>

/** @function main */
int main(int argc, const char** argv)
{

	//Load the cascades
	if (!face_cascade_haar.load(face_cascade_name_haar)) {
		printf("--(!)Error loading HAAR cascade\n");
		return -1;
	}

	if (!face_cascade_lbp.load(face_cascade_name_lbp)) {
		printf("--(!)Error loading LBP cascade\n");
		return -1;
	}
	captureVideo(HAAR);
	captureVideo(LBP);



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
