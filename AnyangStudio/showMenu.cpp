#include "pch.h"
#include "drawKeyChoose.h"

cv::Mat image;
string title = "Anyang Studio";
int drawCase = 0;
cv::Point pt(-1, -1);
cv::Point prevPt(-1,-1);

void onKeyboard(int typedKey) {
	cout << "typed Key is .. " << typedKey << endl;
	switch (typedKey) {

		case BASE_CURSOR : // Key 0
			drawCase = 0;
			pt.x = -1;
			break;

		case DRAW_SQUARE: // Key 1
			drawCase = 1;
			pt.x = -1;
			break;

		case DRAW_CIRCLE: // Key 2
			drawCase = 2;
			pt.x = -1;
			break;

		case DRAW_BRUSH :
			drawCase = 3;
			pt.x = -1;
			break;
	}
}

void onMouse(int event, int x, int y, int flags, void * param) {

	static bool mouseDowned = false;

	
	if (event == CV_EVENT_LBUTTONDOWN) { // Left Click
		mouseDowned = true;
		if (pt.x == -1) { // Choose startPoint in FirstCase
			pt = cv::Point(x, y);
		}

		else { // drawing by drawCase
			switch (drawCase) {
			case CHOOSED_DRAW_SQUARE:
				cv::rectangle(image, pt, cv::Point(x, y), cv::Scalar(0), 2);
				break;

			case CHOOSED_DRAW_CIRCLE:
				cv::Point pt2 = pt - cv::Point(x, y);
				int radius = (int)sqrt(pt2.x * pt2.x + pt2.y * pt2.y);
				cv::circle(image, pt, radius, cv::Scalar(0), 2);
				break;
			}
			pt = cv::Point(-1, -1);
		}
	}

	if (event == CV_EVENT_LBUTTONUP) {
		mouseDowned = false;
		cout << "Uped" << endl;
		prevPt = cv::Point(-1, -1);
	}

	if (mouseDowned) {
		if (drawCase == CHOOSED_DRAW_BRUSH) {
			if (prevPt.x != -1) {
				cv::line(image, prevPt, cv::Point(x, y), cv::Scalar(0), 2);
			}
			prevPt = cv::Point(x, y);
		}
	}

	if (pt.x != -1) { // When draw activated, dotted figure showd
		switch (drawCase) {
		case CHOOSED_DRAW_SQUARE:
			cv::rectangle(image, pt, prevPt, cv::Scalar(255,255,255), 2);
			cv::rectangle(image, pt, cv::Point(x, y), cv::Scalar(0),2);
			prevPt = cv::Point(x, y);
			break;

		case CHOOSED_DRAW_CIRCLE:

			int radius = (int)sqrt(prevPt.x * prevPt.x + prevPt.y * prevPt.y);
			cv::circle(image, pt, radius, cv::Scalar(255,255,255), 2);
			cv::Point pt2 = pt - cv::Point(x, y);
			radius = (int)sqrt(pt2.x * pt2.x + pt2.y * pt2.y);
			cv::circle(image, pt, radius, cv::Scalar(0), 2);
			prevPt = pt2;
			break;
		}
	}
	cv::imshow(title, image);
}

void showMenu() {
	image = cv::Mat(300, 500, CV_8UC1, cv::Scalar(255));
	cv::imshow(title, image);
	cv::setMouseCallback(title, onMouse, 0);

	while (1) {
		int typedKey = cv::waitKeyEx(100);
		if (typedKey == 27) break;
		else {
			onKeyboard(typedKey);
		}
	}
}