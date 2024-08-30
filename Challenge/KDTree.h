#pragma once

#ifndef KDTree_H
#define KDTree_H
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits> 

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// Structure to store a point with latitude and longitude
struct Point {
    double x; // Latitude
    double y; // Longitude
};

// Structure to store information about a city, including its name and location (point)
struct City {
    string name;
    Point point;
};

// Structure of a node in the KD-Tree
struct Node {

    City city;  // City information (including latitude and longitude)
    Node* left, * right; // Left and right children

};

// Function to create a new node in the KD-Tree
Node* createNewNode(string city, double lat, double lng);

// Recursive function to insert a city into the KD-Tree
Node* insertRec(Node* root, string city, double lat, double lng, unsigned depth);
// Non-recursive function to insert a city into the KD-Tree
Node* insert(Node* root, string city, double lat, double lng);

// Recursive function to search for a location (latitude and longitude) in the KD-Tree
bool searchRec(Node* root, double lat, double lng, unsigned depth);
// Non-recursive function to search for a location (latitude and longitude) in the KD-Tree
bool search(Node* root, double lat, double lng);

// Function to read data from a file and insert cities into the KD-Tree
void readFileToKDTree(string fileName, Node*& root);

// Function to print the KD-Tree
void printTree(Node* root, int depth = 0);

// Function to calculate the distance between two points using the haversine formula
double haversine(double lat1, double lon1, double lat2, double lon2);

// Function to check if a point lies within a rectangle defined by bottomLeft and topRight
bool isPointInRectangle(Point point, Point bottomLeft, Point topRight);

// Recursive function to search for cities within a rectangular region
vector<City> rangeSearch(Node* root, Point bottomLeft, Point topRight);

// Function to calculate the total distance from a city to all points in the vector `points`
double calculateTotalDistance(Node* root, vector<Point>& points);

// Recursive function to find the best city with the minimum total distance to a set of points
void findBestCityRec(Node* root, vector<Point>& points, City& nearestCity, double& minTotalDistance, unsigned depth);

// Function to find the nearest neighbor city to a set of points
City nearestNeighborSearch(Node* root, vector<Point> coordinates);

// Function to delete the entire KD-Tree
void deleteTree(Node* root);

