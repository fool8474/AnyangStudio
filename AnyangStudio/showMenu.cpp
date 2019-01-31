#include "pch.h"
#include "drawKeyChoose.h"



cv::Mat image;
cv::Mat selectedLayer;
cv::Mat copyImage;

list<cv::Mat> layers;
list<cv::Mat>::iterator layerIter;
list<cv::Mat>::iterator circuitIter;
string title = "Anyang Studio";

int drawCase = 0;
int brushSize = 2;
int curLayer = 0;

cv::Point pt(-1, -1);
cv::Point prevPt(-1, -1);

void onKeyboard(int typedKey) {
	cout << "typed Key is .. " << typedKey << endl;
	
	switch (typedKey) {

	case BASE_CURSOR: // Key 0
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

	case DRAW_BRUSH: // Key 3
		drawCase = 3;
		pt.x = -1;
		break;

	case ADD_LAYER: // Key a
		addNewLayer();
		break;

	case DEL_LAYER: // Key d
		if (layers.size() > 1) {
			layers.erase(layerIter);
			if (curLayer != 0) {
				curLayer -= 1;
				--layerIter;
			}
		}
		break;

	case NEXT_LAYER: // Key x
		if (layers.size() == curLayer + 1) {
			layerIter = layers.begin();
			curLayer = 0;
		}

		else {
			++layerIter;
			curLayer++;
		}

		selectedLayer = *layerIter;

		break;

	case PREV_LAYER: // Key z
		if (curLayer == 0) {
			layerIter = layers.end();
			curLayer = layers.size() - 1;
		}
		else {
			--layerIter;
			curLayer--;
		}
		selectedLayer = *layerIter;
		break;

	case SIZE_UP: // Key ]
		brushSize++;
		break;

	case SIZE_DOWN: // Key [
		if (brushSize > 1) {
			brushSize--;
		}
		break;
	}
}

void addNewLayer() {
	cv::Mat tempLayer;
	copyImage.copyTo(tempLayer);

	cout << (void*)tempLayer.data << endl;
	layers.push_back(tempLayer);
	curLayer = layers.size() - 1;
	layerIter = layers.end();
}

void makeShowLayer() {

	circuitIter = layers.begin();
	image.zeros(960, 1280, CV_8UC1);
	for (int i = 0; i < layers.size(); i++) {
		cv::Mat circuitMat = *circuitIter;

		for (int j = 0; j < circuitMat.rows; j++) {
			for (int i = 0; i < circuitMat.cols; i++) {
				if (circuitMat.at<int>(j, i) == 0) {
					image.at<int>(j, i) = 0;
				}
			}
		}
		circuitIter++;
	}
}

void onMouse(int event, int x, int y, int flags, void * param) {

	static bool mouseDowned = false;

	cout << curLayer << " " << layers.size() << endl;
	if (event == CV_EVENT_LBUTTONDOWN) { // Left Click
		mouseDowned = true;
		if (pt.x == -1) { // Choose startPoint in FirstCase
			pt = cv::Point(x, y);
		}

		else { // drawing by drawCase
			switch (drawCase) {
			case CHOOSED_DRAW_SQUARE:
				drawRectangle(x, y);
				break;

			case CHOOSED_DRAW_CIRCLE:
				drawCircle(x,y);
				break;
			}
			pt = cv::Point(-1, -1);
		}
	}

	if (event == CV_EVENT_LBUTTONUP) {
		mouseDowned = false;
		prevPt = cv::Point(-1, -1); // for Draw Brush Case :: bugFix brush is Connected
	}

	if (mouseDowned) { // Dragged Case
		if (drawCase == CHOOSED_DRAW_BRUSH) {
			if (prevPt.x != -1) {
				cv::line(selectedLayer, prevPt, cv::Point(x, y), cv::Scalar(0), brushSize);
			}
			prevPt = cv::Point(x, y);
		}
	}

	if (pt.x != -1) { // When draw activated, dotted figure showed
		switch (drawCase) {
		case CHOOSED_DRAW_SQUARE:
			drawExRectangle(x, y);
			break;

		case CHOOSED_DRAW_CIRCLE:
			drawExCircle(x,y);
			break;
		}
	}

	makeShowLayer();
	cv::imshow(title, image);
}

void drawExCircle(int x, int y) {
	int radius = (int)sqrt(prevPt.x * prevPt.x + prevPt.y * prevPt.y);
	cv::circle(selectedLayer, pt, radius, cv::Scalar(255, 255, 255), brushSize);
	cv::Point pt2 = pt - cv::Point(x, y);
	radius = (int)sqrt(pt2.x * pt2.x + pt2.y * pt2.y);
	cv::circle(selectedLayer, pt, radius, cv::Scalar(0), brushSize);
	prevPt = pt2;
}

void drawExRectangle(int x, int y) {
	cv::rectangle(selectedLayer, pt, prevPt, cv::Scalar(255, 255, 255), brushSize);
	cv::rectangle(selectedLayer, pt, cv::Point(x, y), cv::Scalar(0), brushSize);
	prevPt = cv::Point(x, y);
}

void drawRectangle(int x, int y) {
	cv::rectangle(selectedLayer, pt, cv::Point(x, y), cv::Scalar(0), brushSize);
}

void drawCircle(int x, int y) {
	cv::Point pt2 = pt - cv::Point(x, y);
	int radius = (int)sqrt(pt2.x * pt2.x + pt2.y * pt2.y);
	cv::circle(selectedLayer, pt, radius, cv::Scalar(0), brushSize);
}

void showMenu() {
	copyImage = cv::Mat(960, 1280, CV_8UC1, cv::Scalar(255));
	image = copyImage.clone();
	selectedLayer = copyImage.clone();

	layers.push_back(selectedLayer);
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