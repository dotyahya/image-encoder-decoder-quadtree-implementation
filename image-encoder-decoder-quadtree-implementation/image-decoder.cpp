/*
    Author:         Muhammad Yahya
    Date:           24/04/2023
    Time:           4:41:28 PM
    Description:    This program reads the node information from the files and converts the quadtree
					to 2d image array. Then it uses the image array to create an image and saves it
					in the same folder.

    Note:           This program is written in C++ and uses OpenCV library to write the image.

    Github:         Please feel free to contribute to this project by submitting pull requests or
                    reporting bugs through the issue tracker.
*/

// headers
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>

// namespaces
using namespace std;
using namespace cv;

// treenode structure
struct TreeNode {
	int color;
	int xStart, yStart, xEnd, yEnd;

	bool checkLeaf;

	TreeNode* children[4];
};

class QuadTree {
public:
	TreeNode* root;

	QuadTree() {
		root = nullptr;
	}

    // using binary file handling read the information stored in files
    void readNodeInfo(TreeNode*& node, bool isRoot = true) {
        if (node == nullptr) {
            node = new TreeNode;
        }

        static int nodeCount = 2;
        ifstream nodeFile;

        if (isRoot)
            nodeFile.open("D:/nodeInformation/root.txt", ios::binary);
        else
            nodeFile.open("D:nodeInformation/node" + to_string(nodeCount++) + ".txt", ios::binary);

        nodeFile.read((char*)&node->xStart, sizeof(int));
        nodeFile.read((char*)&node->yStart, sizeof(int));
        nodeFile.read((char*)&node->xEnd, sizeof(int));
        nodeFile.read((char*)&node->yEnd, sizeof(int));
        nodeFile.read((char*)&node->color, sizeof(int));
        nodeFile.read((char*)&node->checkLeaf, sizeof(bool));

        // if node is leaf then read the children nodes addresses
        for (int i = 0; i < 4; i++) {
            TreeNode* childNode;
            nodeFile.read((char*)&childNode, sizeof(TreeNode*));

            // if child is not null (i.e. node is not leaf)
            if (childNode != nullptr) {
                node->children[i] = new TreeNode;
                readNodeInfo(node->children[i], false);
            }
        }

        nodeFile.close();
    }

    // converting quadtree to 2d image array
    void quadTreeToImageArray(TreeNode* node, int**& imageArray) {
        if (node == nullptr) {
			return;
		}
		// if node is leaf
        if (node->checkLeaf) {
			// fill the image array with the color of the leaf node
            for (int i = node->xStart; i < node->xEnd; i++) {
                for (int j = node->yStart; j < node->yEnd; j++) {
					imageArray[i][j] = node->color;
				}
			}
			return;
		}
		// recursively call the function for the children nodes
        for (int i = 0; i < 4; i++) {
            if (node->children[i] != nullptr) {
				quadTreeToImageArray(node->children[i], imageArray);
			}
		}
	}

    //// *__FOR__DEBUGGING__PURPOSES__*
    //void printQuadTree(TreeNode* node, bool isRoot = true, int depth = 0, const std::string& parentQuadrant = "") {
    //    if (node == nullptr) {
    //        return;
    //    }

    //    static int nodeCount = 2;

    //    // Print node information
    //    if (isRoot)
    //        cout << "Root Node -> ";
    //    else {
    //        //cout << "Node " << nodeCount++ << " (" << parentQuadrant << ") -> ";
    //        cout << "Node " << "(" << parentQuadrant << ") -> ";
    //    }

    //    cout << "Position: (" << node->xStart << ", " << node->yStart << ") - (" << node->xEnd << ", " << node->yEnd << ") " << "Color: " << node->color;
    //    // print if node is leaf
    //    if (node->checkLeaf) {
    //        cout << " ==> is a Leaf Node\n";
    //        return;
    //    }

    //    cout << endl;

    //    // quadrants for child nodes (nw, ne, sw, se)
    //    string childQuadrants[4] = { "Top-Left", "Top-Right", "Bottom-Left", "Bottom-Right" };

    //    // recursively print the children nodes
    //    for (int i = 0; i < 4; i++) {
    //        if (node->children[i] != nullptr) {
    //            printQuadTree(node->children[i], false, depth + 1, childQuadrants[i]);
    //        }
    //    }
    //}
};

// driver code
int main() {
	// create a QuadTree object
    QuadTree qt;
    TreeNode* root = nullptr;

    // read the node information from the files
    qt.readNodeInfo(qt.root, true);

    int width;

    // t1 = 200px, t2 = 250px
    cout << "Enter image dimensions(must be checked in 'image-encoder.cpp' file): "; // locked aspect ratio!
    cout << "\nWidth(px): ";
    cin >> width;

    // create an image array
    int** imageArray = new int* [width];
    for (int i = 0; i < width; i++) {
		imageArray[i] = new int[width];
	}

    // convert the quad tree to image array
    qt.quadTreeToImageArray(qt.root, imageArray);

    cout << endl;

    // print the image array *__FOR__DEBUGGING__PURPOSES__*
    // display the pixels of the image
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            if (imageArray[i][j] == 255)
			    cout << "0 "; // white pixel
            else 
                cout << "1 "; // black pixel
		}
        
		cout << "| End of Row: " << i + 1 << endl;
	}

    // display the image
    Mat decodedImage(width, width, CV_8UC1);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            decodedImage.at<uchar>(i, j) = static_cast<uchar>(imageArray[i][j]);
        }
    }

    // write image to file
    imwrite("D:/TestImages/decodedImage.bmp", decodedImage);

    // display the image in a window
    /*imshow("Decoded Image", image);
    waitKey(0);*/

    // print the quad tree
    //qt.printQuadTree(qt.root, true, 0);

	// terminate program
	return 0;
}

