#include "KDTree.h"

// Function to create a new node in the KD-Tree with the given city name, latitude, and longitude
Node* createNewNode(string city, double lat, double lng) {
    Node* temp = new Node; // Allocate memory for a new node

    // Allocate memory for a new node
    temp->city.name = city;
    temp->city.point.x = lat;
    temp->city.point.y = lng;

    // Initialize the left and right children to NULL
    temp->left = NULL;
    temp->right = NULL;

    return temp; // Return the newly created node
}

// Recursive function to insert a new city into the KD-Tree
Node* insertRec(Node* root, string city, double lat, double lng, unsigned depth) {
    // If the current root is NULL, create a new node and return it
    if (root == NULL) {
        return createNewNode(city, lat, lng);
    }

    // Determine which coordinate to compare (latitude or longitude) based on the current depth
    unsigned cd = depth % 2; // Since we're dealing with 2D (latitude and longitude)

    // 0 for latitude, 1 for longitude

    if (cd == 0) { // Compare x (latitude)
        // If the new point's latitude is less, insert into the left subtree
        if (lat < root->city.point.x) {
            root->left = insertRec(root->left, city, lat, lng, depth + 1);
        }
        // Otherwise, insert into the right subtree
        else {
            root->right = insertRec(root->right, city, lat, lng, depth + 1);
        }
    }
    // If depth is odd, compare by longitude (y)
    else { // Compare y (longitude)
        // If the new point's longitude is less, insert into the left subtree
        if (lng < root->city.point.y) {
            root->left = insertRec(root->left, city, lat, lng, depth + 1);
        }
        // Otherwise, insert into the right subtree
        else {
            root->right = insertRec(root->right, city, lat, lng, depth + 1);
        }
    }

    return root; // Return the root of the tree after insertion
}

// Function to insert a new city into the KD-Tree (non-recursive wrapper)
Node* insert(Node* root, string city, double lat, double lng) {
    return insertRec(root, city, lat, lng, 0); // Call the recursive insert function starting with depth 0
}

// Recursive function to search for a specific latitude and longitude in the KD-Tree
bool searchRec(Node* root, double lat, double lng, unsigned depth) {
    // If the current root is NULL, the point is not found
    if (root == NULL)
        return false;
    // If the current node matches the search point, return true
    if (root->city.point.x == lat && root->city.point.y == lng)
        return true;

    // Determine which coordinate to compare based on the current depth
    unsigned cd = depth % 2;

    // If depth is even, compare by latitude (x)
    if (cd == 0) {
        // If the search latitude is less, search in the left subtree
        if (lat < root->city.point.x) {
            return searchRec(root->left, lat, lng, depth + 1);
        }
        // Otherwise, search in the right subtree
        else {
            return searchRec(root->right, lat, lng, depth + 1);
        }
    }
    // If depth is odd, compare by longitude (y)
    else {
        // If the search longitude is less, search in the left subtree
        if (lng < root->city.point.y) {
            return searchRec(root->left, lat, lng, depth + 1);
        }
        // Otherwise, search in the right subtree
        else {
            return searchRec(root->right, lat, lng, depth + 1);
        }
    }
}

// Function to search for a specific latitude and longitude in the KD-Tree (non-recursive wrapper)
bool search(Node* root, double lat, double lng) {
    return searchRec(root, lat, lng, 0); // Call the recursive search function starting with depth 0
}

// Function to read city data from a file and insert each city into the KD-Tree
void readFileToKDTree(string fileName, Node*& root) {
    ifstream ifs(fileName); // Open the file for reading

    // If the file cannot be opened, print an error message
    if (!ifs) {
        cout << "Khong doc duoc file " << fileName;
        return;
    }

    string ignore_line;
    getline(ifs, ignore_line, '\n'); // Ignore the first line (header)

    string line;
    string city, lat, lng;

    // Read each line from the file and extract the city name, latitude, and longitude
    while (getline(ifs, line, '\n')) {
        stringstream ss(line);

        getline(ss, city, ',');
        getline(ss, lat, ',');
        getline(ss, lng, ',');

        // Insert the city into the KD-Tree
        root = insert(root, city, stof(lat), stof(lng));

    }

    ifs.close(); // Close the file after reading
}

// Function to print the entire KD-Tree in pre-order traversa
void printTree(Node* root, int depth ) {
    // If the current node is NULL, return
    if (root == NULL) {
        return;
    }

    // Print the city's name and coordinates
    cout << "City: " << root->city.name << ", Latitude: " << root->city.point.x << ", Longitude: " << root->city.point.y << endl;

    // Recursively print the left and right subtrees
    printTree(root->left, depth + 1);
    printTree(root->right, depth + 1);
}


// Function to calculate the distance between two points using the haversine formula
double haversine(double lat1, double lon1, double lat2, double lon2) {
    // Convert latitude and longitude from degrees to radians
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;

    // Apply the haversine formula
    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double rad = 6371; // Earth radius in kilometers
    double c = 2 * asin(sqrt(a));
    return rad * c; // Return the calculated distance
}

// Function to check if a point is within a rectangle defined by bottom-left and top-right corners
bool isPointInRectangle(Point point, Point bottomLeft, Point topRight) {
    return (point.x >= bottomLeft.x && point.x <= topRight.x && point.y >= bottomLeft.y && point.y <= topRight.y);
}

// Recursive function to search for all cities within a rectangular region in the KD-Tree
void rangeSearchRec(Node* root, Point bottomLeft, Point topRight, vector<City>& result, unsigned depth) {
    // If the current node is NULL, return
    if (root == NULL)
        return;

    // If the current city's point is within the rectangle, add it to the result vector
    if (isPointInRectangle(root->city.point, bottomLeft, topRight)) {
        result.push_back(root->city);
    }

    // Determine which coordinate to compare based on the current depth
    unsigned cd = depth % 2;
    if (cd == 0) { // Compare latitude (x)
        // Search in the left subtree if necessary
        if (root->city.point.x >= bottomLeft.x) {
            rangeSearchRec(root->left, bottomLeft, topRight, result, depth + 1);
        }
        // Search in the right subtree if necessary
        if (root->city.point.x <= topRight.x) {
            rangeSearchRec(root->right, bottomLeft, topRight, result, depth + 1);
        }
    }
    else { // Compare longitude (y)
        // Search in the left subtree if necessary
        if (root->city.point.y >= bottomLeft.y) {
            rangeSearchRec(root->left, bottomLeft, topRight, result, depth + 1);
        }
        // Search in the right subtree if necessary
        if (root->city.point.y <= topRight.y) {
            rangeSearchRec(root->right, bottomLeft, topRight, result, depth + 1);
        }
    }
}

// Function to search for all cities within a rectangular region in the KD-Tree
vector<City> rangeSearch(Node* root, Point bottomLeft, Point topRight) {
    vector<City> result; // Vector to store the cities within the rectangle

    // Start the recursive range search
    rangeSearchRec(root, bottomLeft, topRight, result, 0);

    return result; // Return the vector of cities
}

// Function to calculate the total distance from a city's location to a set of points
double calculateTotalDistance(Node* root, vector<Point>& points) {
    double totalDistance = 0.0;

    // Loop through all the points in the vector
    for (int i = 0; i < points.size(); i++) {
        // Add the haversine distance between the root city's coordinates and the current point to the total distance
        totalDistance += haversine(root->city.point.x, root->city.point.y, points[i].x, points[i].y);
    }
    return totalDistance; // Return the total distance calculated
}

// Recursive function to find the city with the minimum total distance to a set of points
void findBestCityRec(Node* root, vector<Point>& points, City& nearestCity, double& minTotalDistance, unsigned depth) {
    if (root == NULL)
        return; // Base case: if the node is null, return
    
    // Calculate the total distance from the current city to all the points
    double totalDistance = calculateTotalDistance(root, points);

    // If the calculated total distance is less than the current minimum total distance
    if (totalDistance < minTotalDistance) {
        minTotalDistance = totalDistance; // Update the minimum total distance
        nearestCity = root->city; // Update the nearest city to the current city
    } 

    // Calculate the current dimension (cd) to decide whether to compare by latitude (x) or longitude (y)
    unsigned cd = depth % 2;

    if (cd == 0) { 

        // If cd is 0, compare by latitude (x)
         // Check if we should explore the left subtree by comparing latitude with minTotalDistance
        for (const auto& point : points) {
            if (point.x - minTotalDistance < root->city.point.x) {
                findBestCityRec(root->left, points, nearestCity, minTotalDistance, depth + 1);
                break; // Exit loop once a point satisfies the condition
            }
        }

        // Check if we should explore the right subtree by comparing latitude with minTotalDistance
        for (const auto& point : points) {
            if (point.x + minTotalDistance > root->city.point.x) {
                findBestCityRec(root->right, points, nearestCity, minTotalDistance, depth + 1);
                break; // Exit loop once a point satisfies the condition
            }
        }
    }
    else { // If cd is 1, compare by longitude (y)

        // Check if we should explore the left subtree by comparing longitude with minTotalDistance
        for (const auto& point : points) {
            if (point.y - minTotalDistance < root->city.point.y) {
                findBestCityRec(root->left, points, nearestCity, minTotalDistance, depth + 1);
                break; // Exit loop once a point satisfies the condition
            }
        }

        // Check if we should explore the right subtree by comparing longitude with minTotalDistance
        for (const auto& point : points) {
            if (point.y + minTotalDistance > root->city.point.y) {
                findBestCityRec(root->right, points, nearestCity, minTotalDistance, depth + 1);
                break; // Exit loop once a point satisfies the condition
            }
        }

    }
}

// Function to perform the nearest neighbor search, returns the city with the minimum total distance to a set of points
City nearestNeighborSearch(Node* root, vector<Point> coordinates) {
    if (root == NULL) {
        cout << "The KD-Tree is empty" << endl;
        return City{ "", {0, 0} }; // Return an empty city if the tree is empty
    }

    City nearestCity; // Variable to store the city with the minimum total distance

    double minTotalDistance = numeric_limits<double>::max(); // Initialize the minimum total distance to a large value

    findBestCityRec(root, coordinates, nearestCity, minTotalDistance, 0);  // Start the recursive search

    return nearestCity;  // Return the city with the minimum total distance
}

// Function to delete the entire KD-Tree and free the allocated memory
void deleteTree(Node* root) {
    if (root == NULL) {
        return; // If the node is null, return
    }

    deleteTree(root->left); // Recursively delete the left subtree
    deleteTree(root->right); // Recursively delete the right subtree

    delete root; // Delete the current node
    root = NULL;  // Set the pointer to null after deletion
}