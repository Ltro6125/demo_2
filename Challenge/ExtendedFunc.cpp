#include "KDTree.h"

void serializeNode(Node* root, ofstream& ofs) {
    // If the node is NULL, return without writing anything

    if (root == NULL) {
        return;
    }

    // Write the length of the city name as an integer
    int nameLength = root->city.name.size();
    ofs.write((char*)(&nameLength), sizeof(nameLength));

    // Write the city name as a string of characters
    ofs.write(root->city.name.c_str(), nameLength);

    // Write the latitude and longitude of the city
    ofs.write((char*)(&root->city.point.x), sizeof(root->city.point.x));
    ofs.write((char*)(&root->city.point.y), sizeof(root->city.point.y));

    // Recursively serialize the left and right child nodes
    serializeNode(root->left, ofs);
    serializeNode(root->right, ofs);
}

// Function to serialize the entire KD-Tree and save it to a binary file
void serialize(Node* root, string fileName) {

    ofstream ofs(fileName, ios::binary); // Open the file in binary mode

    if (!ofs) {
        // If the file cannot be opened, print an error message and return
        cout << "Can not open the file " << fileName << endl;
        return;
    }

    // Serialize the root node of the tree
    serializeNode(root, ofs);

    ofs.close();  // Close the file after writing

}

// Function to deserialize a KD-Tree from a binary file and reconstruct the tree
Node* deserialize(string fileName) {
    ifstream ifs(fileName, ios::binary);

    if (!ifs) {
        // If the file cannot be opened, print an error message and return NULL
        cout << "Can not open the file " << fileName << endl;
        return NULL;
    }

    Node* root = NULL; // Initialize the root node to NULL

    // Continue reading until the end of the file
    while (!ifs.eof()) {
        City temp; // Temporary variable to store the city data
        int nameLength;

        // Read the length of the city name
        ifs.read((char*)&nameLength, sizeof(nameLength));
        if (!ifs) break; // Break if the read operation fails

        // Allocate memory for the city name and read it from the file
        char* cityName = new char[nameLength + 1];
        cityName[nameLength] = '\0';
        ifs.read(cityName, nameLength);

        temp.name = cityName; // Assign the read city name to the temp variable

        // Read the latitude and longitude of the city
        ifs.read((char*)&temp.point.x, sizeof(temp.point.x));
        ifs.read((char*)&temp.point.y, sizeof(temp.point.y));

        // Insert the read city into the KD-Tree
        root = insert(root, temp.name, temp.point.x, temp.point.y);

        delete[] cityName; // Free the allocated memory for the city name
    }

    ifs.close(); // Close the file after reading

    return root; // Return the reconstructed tree's root node
}

