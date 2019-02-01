#include "pch.h"
#include "drawKeyChoose.h"

#define WIDTH 1280
#define HEIGHT 960

cv::Mat image;
cv::Mat selectedLayer;
cv::Mat selectedCheckLayer;
cv::Mat copyImage;
cv::Mat layerCheckMat;
cv::Mat copyImageBin;

vector<cv::Mat> layers;
vector<cv::Mat> layerChecks;
vector<bool> layerHidden;

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
			cout << "cur Layer is.. " << curLayer << endl;
			layers.erase(layers.begin() + curLayer);
			layerChecks.erase(layerChecks.begin() + curLayer);
			layerHidden.erase(layerHidden.begin() + curLayer);
			if (curLayer != 0) {
				curLayer -= 1;
			}
		}

		selectedLayer = layers.at(curLayer);
		selectedCheckLayer = layerChecks.at(curLayer);
		
		break;

	case NEXT_LAYER: // Key x
		if (layers.size() == curLayer + 1) {
			curLayer = 0;
		}

		else {
			curLayer++;
		}

		selectedLayer = layers.at(curLayer);
		selectedCheckLayer = layerChecks.at(curLayer);

		break;

	case PREV_LAYER: // Key z
		if (curLayer == 0) {
			curLayer = layers.size() - 1;
		}
		else {
			curLayer--;
		}
		
		selectedLayer = layers.at(curLayer);
		selectedCheckLayer = layerChecks.at(curLayer);

		break;

	case SET_LAYER_HIDDEN: // Key h
		if (layerHidden.at(curLayer)) {
			layerHidden.at(curLayer) = false;
		}

		else {
			layerHidden.at(curLayer) = true;
		}
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
	cv::Mat tempLayerCheck;
	copyImage.copyTo(tempLayer);
	copyImageBin.copyTo(tempLayerCheck);

	layers.push_back(tempLayer);
	layerChecks.push_back(tempLayerCheck);
	layerHidden.push_back(false);
	curLayer = layers.size() - 1;
	selectedLayer = layers.at(curLayer);
	selectedCheckLayer = layerChecks.at(curLayer);
}

void makeShowLayer() {
	
	setImageLayerToZero();

	for (int i = 0; i < layers.size(); i++) {
		if (!layerHidden.at(i)) {
		cv::Mat circuitMat = layers.at(i);
		cv::Mat circuitCheckMat = layerChecks.at(i);
			for (int j = 0; j < circuitMat.rows; j++) {
				for (int k = 0; k < circuitMat.cols; k++) {
					if (!circuitCheckMat.at<bool>(j, k)) {
						image.at<int>(j, k) = circuitMat.at<int>(j, k);
					}
				}
			}
		}
	}
}

void setImageLayerToZero() {
	for (int j = 0; j < image.rows; j++) {
		for (int i = 0; i < image.cols; i++) {
			image.at<uchar>(j, i) = 255;
		}
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
				cv::line(selectedLayer, prevPt, cv::Point(x, y), cv::Scalar(0,0,0), brushSize);
				cv::line(selectedCheckLayer, prevPt, cv::Point(x, y), true, brushSize);
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

	cv::imshow(title, image);
}

void drawExCircle(int x, int y) {
	int radius = (int)sqrt(prevPt.x * prevPt.x + prevPt.y * prevPt.y);
	cv::circle(selectedLayer, pt, radius, cv::Scalar(255, 255, 255), brushSize);
	cv::circle(selectedCheckLayer, pt, radius, false, brushSize);
	cv::Point pt2 = pt - cv::Point(x, y);
	radius = (int)sqrt(pt2.x * pt2.x + pt2.y * pt2.y);
	cv::circle(selectedLayer, pt, radius, cv::Scalar(0,0,0), brushSize);
	cv::circle(selectedCheckLayer, pt, radius, true, brushSize);
	prevPt = pt2;
}

void drawExRectangle(int x, int y) {
	cv::rectangle(selectedLayer, pt, prevPt, cv::Scalar(255, 255, 255), brushSize);
	cv::rectangle(selectedCheckLayer, pt, prevPt, cv::Scalar(false), brushSize);
	cv::rectangle(selectedLayer, pt, cv::Point(x, y), cv::Scalar(0,0,0), brushSize);
	cv::rectangle(selectedCheckLayer, pt, cv::Point(x, y), true, brushSize);
	prevPt = cv::Point(x, y);
}

void drawRectangle(int x, int y) {
	cv::rectangle(selectedLayer, pt, cv::Point(x, y), cv::Scalar(0,0,0), brushSize);
	cv::rectangle(selectedCheckLayer, pt, cv::Point(x, y), true, brushSize);
}

void drawCircle(int x, int y) {
	cv::Point pt2 = pt - cv::Point(x, y);
	int radius = (int)sqrt(pt2.x * pt2.x + pt2.y * pt2.y);
	cv::circle(selectedLayer, pt, radius, cv::Scalar(0,0,0), brushSize);
	cv::circle(selectedCheckLayer, pt, radius, true, brushSize);
}

void showMenu() {
	copyImage = cv::Mat(HEIGHT, WIDTH, CV_8UC3, cv::Scalar(255,255,255));
	copyImageBin = cv::Mat(HEIGHT, WIDTH, CV_8UC1, cv::Scalar(false));
	
	image = copyImage.clone();
	selectedLayer = copyImage.clone();
	layerCheckMat = copyImageBin.clone();

	layers.push_back(selectedLayer);
	layerChecks.push_back(layerCheckMat);
	layerHidden.push_back(false);

	cv::imshow(title, image);
	cv::setMouseCallback(title, onMouse, 0);

	while (1) {
		makeShowLayer();
		int typedKey = cv::waitKeyEx(100);
		if (typedKey == 27) break;
		else {
			onKeyboard(typedKey);
		}
	}
}