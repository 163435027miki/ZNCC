/*
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>	//画像読み込み
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result;
char* image_window = "Source Image";
char* result_window = "Result window";
char date_directory[255] = "..\\result\\";
char Input_directory[255];
char Template_directory[255];
char result_image_name[255];
int kurikaeshi = 0;
FILE *fp_date;
char filename_log[128];

int match_method;
int max_Trackbar = 5;

int max_x = 0;
int max_y = 0;

int score(int correct_x, int correct_y, int image_xt, int image_yt,int max_x, int max_y, int data_num);
int score_record(char date_directory[], int correct_score, int max_x, int max_y);

/// Function Headers
void MatchingMethod(int, void*);


int main(int argc, char** argv)
{
	/// Load image and template

	int iamge_kurikaeshi = 1;
	int image_tenpulate = 1;
	int data_num = 180;
	int image_xt = 80;
	int image_yt = 80;
	////////////////////////logファイルの作成//////////////////////////////////////////////////////////////////////////
	sprintf(filename_log, "%s\\log.txt", date_directory);	//logファイル作成のディレクトリ指定
	printf("%s\n", filename_log);	//logファイル作成のディレクトリ指定
	if ((fp_date = fopen(filename_log, "w")) == NULL) { printf("logファイルが開けません"); exit(1); }

	for (iamge_kurikaeshi = 1; iamge_kurikaeshi <= data_num; ++iamge_kurikaeshi) {
		max_x = 0;
		max_y = 0;
		if (iamge_kurikaeshi == 31 || iamge_kurikaeshi == 61 || iamge_kurikaeshi == 91 || iamge_kurikaeshi == 121 || iamge_kurikaeshi == 151)++image_tenpulate;
		sprintf(Input_directory, "..\\bmp\\simulation18-0115\\inputimage\\%d.bmp", iamge_kurikaeshi);
		sprintf(Template_directory, "..\\bmp\\simulation18-0115\\template\\%d.bmp", image_tenpulate);
		printf("template=%s\n", Template_directory);


		img = imread(Input_directory, 1);
		templ = imread(Template_directory, 1);



		MatchingMethod(0, 0);
		//waitKey(1000);

		int correct_score = 0;
		int correct_x = 0;
		int correct_y = 0;
		printf("max_x=%d,max_y=%d\n",max_x,max_y);
		correct_score = score(correct_x, correct_y, max_x, max_y, image_xt, image_yt,  data_num);
		score_record(date_directory,  correct_score, max_x, max_y);
	}

	

	fclose(fp_date);
	return 0;
}


void MatchingMethod(int, void*)
{
	
	++kurikaeshi;
	cv::namedWindow(image_window);
	cv::namedWindow(result_window);

	/// Source image to display
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_cols, result_rows, CV_32FC1);

	/// Do the Matching and Normalize
	//matchTemplate(img, templ, result, match_method);

	//テンプレートマッチングを行う
	matchTemplate(img, templ, result, 5);

	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	//if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	//{
	//	matchLoc = minLoc;
	//}
	//else
	//{
	//	matchLoc = maxLoc;
	//}
	//ZNCCのとき
	matchLoc = maxLoc;

	/// Show me what you got
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar(0, 0, 255), 1, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar(0, 0, 255), 1, 8, 0);
	//printf("x=%d,y=%d\n", matchLoc.x, matchLoc.y);

	imshow(image_window, img_display);
	imshow(result_window, result);

	//画像保存
	sprintf(result_image_name, "%s%d_result_image.bmp", date_directory,kurikaeshi );
	imwrite(result_image_name, img_display);

	fprintf(fp_date,"%d:%d,%d\n",kurikaeshi, matchLoc.x + templ.cols, matchLoc.y + templ.rows);
	max_x = matchLoc.x;
	max_y = matchLoc.y;
	

	return;
}
*/

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result;
char* image_window = "Source Image";
char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod(int, void*);


int main(int argc, char** argv)
{
	/// Load image and template
	img = imread("..\\bmp\\simulation18-0202\\inputimage\\1.bmp", 1);
	templ = imread("..\\bmp\\simulation18-0202\\template\\1.bmp", 1);

	/// Create windows
	namedWindow(image_window, WINDOW_AUTOSIZE);
	namedWindow(result_window, WINDOW_AUTOSIZE);

	/// Create Trackbar
	char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);

	MatchingMethod(0, 0);

	waitKey(0);
	return 0;
}


void MatchingMethod(int, void*)
{
	/// Source image to display
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_cols, result_rows, CV_32FC1);

	/// Do the Matching and Normalize
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	/// Show me what you got
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

	printf("x=%d,y=%d\n", matchLoc.x + templ.cols, matchLoc.y + templ.rows);
		
	imshow(image_window, img_display);
	imshow(result_window, result);

	return;
}

