#pragma once

#ifndef ExtendedFunc_H
#define ExtendedFunc_H
#endif

#include "KDTree.h"
#include <cstring>

// Function to serialize a single node of the KD-Tree to a binary file
void serializeNode(Node* root, ofstream& ofs);

// Function to serialize the entire KD-Tree and save it to a binary file
void serialize(Node* root, string fileName);

// Function to deserialize a KD-Tree from a binary file and reconstruct the tree
Node* deserialize(string fileName);

