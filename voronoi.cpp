#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <math.h>
#include <string.h>
#include <ncurses.h>

int SEED = 123;

float FREQUENCY = 0.001;
int OVERRIDE = 0;
int count = 1;

// the default values to fill my screen
int WIDTH = 238;
int HEIGHT = 60;

int fieldWidth = 0;

float P = 2;

const int symbolCount = 9;
char symbols[symbolCount] = {'.','#','/','\\','_','*','^','&','|',};

const char* fillString = {"|"};
int fillLength = 1;

void init();
void fill(std::vector<std::vector<int> >& matrix);
void generatePoints(std::vector<std::vector<int> >& matrix, std::vector<std::vector<int> >& points);
void generateCells(std::vector<std::vector<int> >& matrix, std::vector<std::vector<int> >& points);
void determineColor(std::vector<std::vector<int> >& matrix, std::vector<std::vector<int> >& points, std::vector<int>& colors);
void print(std::vector<std::vector<int> >& matrix, std::vector<std::vector<int> >& points, std::vector<int>& colors);

int main(int argc, char **argv){
	if(argc > 1){
		for(int i = 1; i < argc-1; i+=2){
			if(strcmp("--seed=", argv[i]) == 0){
				SEED = atoi(argv[i+1]);
			}else if(strcmp("--frequency=", argv[i]) == 0){
				FREQUENCY = atof(argv[i+1]);
			}else if(strcmp("--override=", argv[i]) == 0){
				OVERRIDE = atoi(argv[i+1]);
			}else if(strcmp("--width=", argv[i]) == 0){
				WIDTH = atoi(argv[i+1]);
			}else if(strcmp("--height=", argv[i]) == 0){
				HEIGHT = atoi(argv[i+1]);
			}else if(strcmp("--p=", argv[i]) == 0){
				P = atoi(argv[i+1]);
			}
		}
	}

	init();

	std::vector<std::vector<int> > matrix;
	std::vector<std::vector<int> > points;
	std::vector<int> colors;
	
	fill(matrix);
	generatePoints(matrix, points);

	colors.resize(points.size());

	generateCells(matrix, points);
	determineColor(matrix, points, colors);
	print(matrix, points, colors);

	getch();
	endwin();

	return 1;
}

void init(){
	initscr();
	raw();
	noecho();
	start_color();

	init_pair(1, COLOR_RED, COLOR_RED);
	init_pair(2, COLOR_GREEN, COLOR_GREEN);
	init_pair(3, COLOR_BLUE, COLOR_BLUE);
	init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(5, COLOR_BLACK, COLOR_BLACK);
}

// matrix is row major, i.e. get points by using matrix[y][x].

void fill(std::vector<std::vector<int> >& matrix){
	for(unsigned int i = 0; i < HEIGHT; i++){
		matrix.resize(matrix.size()+1);
		for(unsigned int j = 0; j < WIDTH; j++){
			matrix[i].push_back(0);
		}
	}
}

void generatePoints(std::vector<std::vector<int> >& matrix, std::vector<std::vector<int> >& points){
	std::default_random_engine generator(SEED);
	std::uniform_int_distribution<int> distribution(0, WIDTH*HEIGHT);

	int numPoints = (OVERRIDE == 0) ? FREQUENCY*WIDTH*HEIGHT : OVERRIDE;

	for(int i = 0; i < numPoints; i++){
		int number = distribution(generator);
		int row = number/WIDTH;
		int column = number%WIDTH;
		
		if(matrix[row][column] != 0){
			i--;
			continue;
		}

		fieldWidth = std::log10(count)+1;

		std::vector<int> coordinates;
		coordinates.push_back(row);
		coordinates.push_back(column);
		coordinates.push_back(count);

		matrix[row][column] = count++;

		points.push_back(coordinates);
	}
}

void generateCells(std::vector<std::vector<int> >& matrix, std::vector<std::vector<int> >& points){
	for(unsigned int i = 0; i < matrix.size(); i++){
		for(unsigned int j = 0; j < matrix[0].size(); j++){
			float distance = -1.0f;
			int fill = 0;
			
			for(unsigned int k = 0; k < points.size(); k++){
				int deltaX = std::abs(points[k][1]-(int)j);
				int deltaY = std::abs(points[k][0]-(int)i);
				float tempDistance = std::pow(std::pow(deltaX, P) + std::pow(deltaY, P), 1.0f/P);
				//float tempDistance = std::pow(deltaX, P) + std::pow(deltaY, P);
				if(tempDistance < distance || distance < 0){
					distance = tempDistance;
					fill = points[k][2];
				}
			}
			
			matrix[i][j] = fill;
		}
	}
}

void determineColor(std::vector<std::vector<int> >& matrix, std::vector<std::vector<int> >& points, std::vector<int>& colors){
	std::vector<std::vector<int> > adjacentNodes;
	adjacentNodes.resize(points.size());

	for(unsigned int i = 0; i < points.size(); i++){		// for every cell, step recursively step around boundary to find neighbors
		std::vector<std::vector<int> > initialPoints;
		recursiveCall(points[i][2], initialPoints, matrix, adjacentNodes);
	}
}

// points[0] is initial point
// points[1] is previous point
// points[2] is previous point
// points[i][0] is the y value for a point
// points[i][1] is the x value for a point
// positive y direction is up
// positive x direction is right

// starts at initial points and walks clockwise around a boundary until it comes back to the initial point. each time it steps, it checks all adjacent points and adds unique values to the adjacentNodes list
void recursiveStep(int value, std::vector<std::vector<int> >& points, std::vector<std::vector<int> >& matrix, std::vector<std::vector<int> >& adjacentNodes){
	if(points[0] == points[2]){
		if(points[1] != points[2]){
			return;		// break recursive calls when we are at the initial points and the previous point is not the current point
		}else{
			points[1][0]--;
		}
	}
	
	std::vector<int> nextPoint;
	nextPoint.resize(2);

	if(points[2][0] == points[1][0]){
		if(points[2][1] > points[1][1]){
			nextPoint.at
		}else{

		}
	}else{
		if(points[2][0] > points[1][0]){

		}else{

		}
	}
	

	points[1][0] = points[2][0];	// updates previous point to current point
	points[1][1] = points[2][1];

	points[2] = nextPoint;		// updates current point to next point

	recursiveStep(value, points, matrix, adjacentNodes);
}

void print(std::vector<std::vector<int> >& matrix, std::vector<std::vector<int> >& points, std::vector<int>& colors){
	for(unsigned int i = 0; i < matrix.size(); i++){
		for(unsigned int j = 0; j < matrix[0].size(); j++){
			bool printed = false;
			
			for(unsigned int k = 0; k < points.size(); k++){
				if(i == points[k][0] && j == points[k][1]){
					attron(COLOR_PAIR(5));
					mvaddstr(i, fillLength*j, fillString);
					attroff(COLOR_PAIR(5));
					refresh();
					
					printed = true;
					break;
				}
			}
			
			if(!printed){
				attron(COLOR_PAIR(matrix[i][j]%4+1));
				mvaddstr(i, fillLength*j, fillString);
				attroff(COLOR_PAIR(matrix[i][j]%4+1));
				refresh();
			}
		}
	}
}
