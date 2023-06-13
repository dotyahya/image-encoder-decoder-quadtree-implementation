/* 
    Author:         Muhammad Yahya
    Date:           13/04/2023
    Time:           12:09:51 AM
    Description:    This program encodes an image using quadtree data structure. It takes an image as input
					and outputs a text file containing the necessary information to reconstruct the image.
    
    Note:           This program is written in C++ and uses OpenCV library to read the image.
    
    Github:         Please feel free to contribute to this project by submitting pull requests or 
                    reporting bugs through the issue tracker.
*/

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

// structure
class LinkedList2d {
public: 
    int data;
    LinkedList2d* head;
	LinkedList2d* right;
	LinkedList2d* down;

    // default constructor
    LinkedList2d() {
		data = 0;
        head = nullptr;
		right = nullptr;
		down = nullptr;
	}

    // parameterized constructor
    LinkedList2d(int data) {
        this->data = data;
        head = nullptr;
        right = nullptr;
        down = nullptr;
    }

    void convertTo2dLL(int** arr, int row, int col);
    void print2dLL();
};

void LinkedList2d::convertTo2dLL(int** array, int row, int col) {
    LinkedList2d** head = new LinkedList2d* [row];
    LinkedList2d* ptr = nullptr;
    LinkedList2d* newWhiteNode = nullptr;
    LinkedList2d* newBlackNode = nullptr;

    queue<int> blackIndexes;

    // traversal smth?
    for (int i = 0; i < row; i++) {
        head[i] = new LinkedList2d(i + 1); // head of each row is the row number
        ptr = head[i]; // ptr points to the head of the row

        // to check if the pixel is in black sequence or white sequence
        bool isInBlack, isInWhite;
        isInBlack = isInWhite = false;

        // to store the starting and end indices of black and white sequences
        int blackStartIndex, blackEndIndex;
        int whiteStartIndex, whiteEndIndex;

        // initially undefined
        whiteStartIndex = whiteEndIndex = -1;
        blackStartIndex = blackEndIndex = -1;

        for (int j = 0; j < col; j++) {
            // if the pixel is white 
            if (array[i][j] == 255) {
                if (isInWhite == false) {
                    isInWhite = true;
                    whiteStartIndex = j + 1;
                }
                whiteEndIndex = j + 1;

                if (isInBlack == true) {
                    blackIndexes.push(blackStartIndex);
                    blackIndexes.push(blackEndIndex);

                    isInBlack = false;
                }
            }

            // if the pixel is black
            else {
                // stores black pixel sequence's start index
                if (isInBlack == false) {
                    isInBlack = true;
                    blackStartIndex = j + 1;
                }
                blackEndIndex = j + 1;

                if (isInWhite == true) {
                    // stores white pixel sequence's start index
                    newWhiteNode = new LinkedList2d(whiteStartIndex);
                    //cout << "Adding node with data: " << whiteStartIndex << " iteration "  << i + 1 << ", " <<  j + 1 << endl;
                    ptr->right = newWhiteNode;
                    ptr = ptr->right;

                    newWhiteNode = new LinkedList2d(whiteEndIndex);
                    //cout << "Adding node with data: " << whiteEndIndex << " iteration " << i + 1 << ", " << j + 1 << endl;
                    ptr->right = newWhiteNode;
                    ptr = ptr->right;

                    isInWhite = false;
                }
            }
        }

        // if the last pixel is white
        if (isInWhite == true) {
            // stores white pixel sequence's -start- index
            newWhiteNode = new LinkedList2d(whiteStartIndex);
            //cout << "Adding node with data: " << whiteStartIndex << " iteration " << i + 1 << endl;
            ptr->right = newWhiteNode;
            ptr = ptr->right;

            // stores white pixel sequence's -end- index
            newWhiteNode = new LinkedList2d(whiteEndIndex);
            //cout << "Adding node with data: " << whiteEndIndex << " iteration " << i + 1 << endl;
            ptr->right = newWhiteNode;
            ptr = ptr->right;

            // -2 indicates the end of white pixel sequence
            newWhiteNode = new LinkedList2d(-2);
            ptr->right = newWhiteNode;
            ptr = ptr->right;

            //isInWhite = false;

        }

        else {
            // -2 indicates the end of white pixel sequence
            newWhiteNode = new LinkedList2d(-2);
            ptr->right = newWhiteNode;
            ptr = ptr->right;
        }

        // adds black pixel sequences to the linked list at the end of each row followed by -1 
        while (!blackIndexes.empty()) {
            // stores black pixel sequence's -start- index
            newBlackNode = new LinkedList2d(blackIndexes.front());
            //cout << "Adding node with data: " << blackIndexes.front() << " iteration " << i + 1 << endl;
            ptr->right = newBlackNode;
            ptr = ptr->right;

            blackIndexes.pop();

            // stores black pixel sequence's -end- index
            newBlackNode = new LinkedList2d(blackIndexes.front());
            //cout << "Adding node with data: " << blackIndexes.front() << " iteration " << i + 1 << endl;
            ptr->right = newBlackNode;
            ptr = ptr->right;

            blackIndexes.pop();
        }

        // if the last pixel is black
        if (isInBlack == true) {
			// stores black pixel sequence's -start- index
			newBlackNode = new LinkedList2d(blackStartIndex);
			//cout << "Adding node with data: " << blackStartIndex << " iteration " << i + 1 << endl;
			ptr->right = newBlackNode;
			ptr = ptr->right;

			// stores black pixel sequence's -end- index
			newBlackNode = new LinkedList2d(blackEndIndex);
			//cout << "Adding node with data: " << blackEndIndex << " iteration " << i + 1 << endl;
			ptr->right = newBlackNode;
			ptr = ptr->right;
			
            // -1 indicates the end of black pixel sequence
			newBlackNode = new LinkedList2d(-1);
			ptr->right = newBlackNode;
			ptr = ptr->right;

			//isInBlack = false;
		}

        else {
            // -1 indicates the end of black pixel sequence
            newBlackNode = new LinkedList2d(-1);
            ptr->right = newBlackNode;
            ptr = ptr->right;
        }


        // set down pointers to null for the whole row
        ptr = head[i];
        while (ptr->right != nullptr) {
            ptr = ptr->right;
        }
        ptr->down = nullptr;

    }

    // set down pointers of each rows head to the next row's head
    for (int i = 0; i < (row - 1); i++) {
        ptr = head[i];
        ptr->down = head[i + 1];
    }

    // set the head to point to the first row's head
    this->head = head[0];

    // free memory allocated to the array of head pointers
    delete[] head;
}

// print 2d linked list
void LinkedList2d::print2dLL() {
    LinkedList2d* ptrRow = this->head;

    while (ptrRow != nullptr) {
        LinkedList2d* ptrCol = ptrRow;

        while (ptrCol != nullptr) {
			cout << ptrCol->data << " ";
            ptrCol = ptrCol->right;
		}
		cout << endl;
		ptrRow = ptrRow->down;
	}
}

// treenode
struct TreeNode {
    int color;
    int xStart, yStart, xEnd, yEnd;

    bool checkLeaf;

    // children of the node (nw, ne, sw, se) 
    TreeNode* children[4];
};

// quadtree class
class QuadTree {
public:
    // root of the quadtree
    TreeNode* root;

    // constructor
    QuadTree() {
        root = nullptr;
    }

    // recursive function to build the quadtree from the 2d array of the image having 0s and 255s 
    // 0s represent black pixels and 255s represent white pixels
    // the function takes the 2d image array, positions x and y (0, 0) initially and the size of the image (rows and cols)
    // if the size of the image is 1x1 , the function creates a leaf node and returns it
    // if the size of the image is greater than 1x1 and if the quadtree is not homogeneous (i.e: grey colour, 
    // combination of black and white pixels), the function creates a node and calls itself recursively for each quadrant
    bool isHomogeneous(int** imageArr, int x, int y, int rows, int cols) {
        // stores the color of the first pixel
        int color = imageArr[x][y];
        // checks if all the pixels in the image are of the same color
        for (int i = x; i < (x + rows); i++) {
            for (int j = y; j < (y + cols); j++) {
                if (imageArr[i][j] != color) {
                    return false;
                }
            }
        }
        return true;
    }

    TreeNode* buildQuadTree(int** imageArr, int xStart, int yStart, int xEnd, int yEnd, int rows, int cols) {
		// if the size of the image is 1x1
        if (rows == 1 && cols == 1) {
            // create a leaf node
            TreeNode* newNode = new TreeNode;
            newNode->color = imageArr[xStart][yStart];
            newNode->xStart = xStart;
            newNode->yStart = yStart;
            newNode->xEnd = xEnd;
            newNode->yEnd = yEnd;
            newNode->checkLeaf = true;
            // set all children to null
            for (int i = 0; i < 4; i++) {
				newNode->children[i] = nullptr;
			}
            // return the leaf node
			return newNode;

        }

        // if the size of the image is greater than 1x1 && not of same color (i.e grey)
        else {
            // create a node
            TreeNode* newNode = new TreeNode;
            newNode->color = imageArr[xStart][yStart];
            newNode->xStart = xStart;
            newNode->yStart = yStart;
            newNode->xEnd = xEnd;
            newNode->yEnd = yEnd;
            newNode->checkLeaf = true;

            // set all children to null
            for (int i = 0; i < 4; i++) {
				newNode->children[i] = nullptr;
			}

            if (isHomogeneous(imageArr, xStart, yStart, rows, cols) == false) {
                newNode->checkLeaf = false; // not a leaf node
                newNode->color = -1; // grey color
                // north-west child
                newNode->children[0] = buildQuadTree(imageArr, xStart, yStart, xStart + (rows / 2), yStart + (cols / 2), rows / 2, cols / 2);
                // north-east child
                newNode->children[1] = buildQuadTree(imageArr, xStart, yStart + (cols / 2), xStart + (rows / 2), yEnd, rows / 2, cols / 2);
                // south-west child
                newNode->children[2] = buildQuadTree(imageArr, xStart + (rows / 2), yStart, xEnd, yStart + (cols / 2), rows / 2, cols / 2);
                // south-east child
                newNode->children[3] = buildQuadTree(imageArr, xStart + (rows / 2), yStart + (cols / 2), xEnd, yEnd, rows / 2, cols / 2);
            }
            // return the node
			return newNode;
        }
    }

    // print quad tree *__FOR__DEBUGGING__PURPOSES__*
    void printQuadTree(TreeNode* node, bool isRoot = true, const std::string& parentQuadrant = "") {
        if (node == nullptr) {
            return;
        }

        static int nodeCount = 2;

        // Print node information
        if (isRoot) 
            cout << "Root Node -> ";
        else {
            //cout << "Node " << nodeCount++ << ": (" << parentQuadrant << "): ";
            cout << "Node " << "(" << parentQuadrant << ") -> ";
        }
        
        cout << "Position: (" << node->xStart << ", " << node->yStart << ") - (" << node->xEnd << ", " << node->yEnd << ") " << "Color: " << node->color;
        // print if node is leaf
        if (node->checkLeaf) {
            cout << " ==> is a Leaf Node";
		}

        cout << endl;

        // quadrants for child nodes (nw, ne, sw, se)
        string childQuadrants[4] = { "Top-Left", "Top-Right", "Bottom-Left", "Bottom-Right" };

        // recursively print the children nodes
        for (int i = 0; i < 4; i++) {
            if (node->children[i] != nullptr) {
                printQuadTree(node->children[i], false, childQuadrants[i]);
            }
        }
    }

    // write node information to separate files
    void writeNodeInfo(TreeNode* node, bool isRoot = true) {
        if (node == nullptr)
            return;
        // nodes starting after the root node (root node is 1)
        static int nodeCount = 2;
        
        // create a file for each node
        ofstream nodeFile;
        if (isRoot)
            nodeFile.open("D:/nodeInformation/root.txt", ios::binary);  
        else
        	nodeFile.open("D:/nodeInformation/node" + to_string(nodeCount++) + ".txt", ios::binary);

        // write node information to the file using binary mode
        nodeFile.write((char*)&node->xStart, sizeof(int));
        nodeFile.write((char*)&node->yStart, sizeof(int));
        nodeFile.write((char*)&node->xEnd, sizeof(int));
        nodeFile.write((char*)&node->yEnd, sizeof(int));
        nodeFile.write((char*)&node->color, sizeof(int));
        nodeFile.write((char*)&node->checkLeaf, sizeof(bool));

        // store addresses of children nodes
        for (int i = 0; i < 4; i++) {
            if (node->children[i] != nullptr) {
				nodeFile.write((char*)&node->children[i], sizeof(TreeNode*));
			}
            else {
				TreeNode* nullNode = nullptr;
				nodeFile.write((char*)&nullNode, sizeof(TreeNode*));
			}
		}

        // recursively write the children nodes
        for (int i = 0; i < 4; i++) {
        	writeNodeInfo(node->children[i], false);  // Pass false to indicate it's not the root node
        }

        // close the file
        nodeFile.close();
    }
};

// driver code
int main() {
    // create image object and load image
    cv::Mat image = cv::imread("D:TestImages/t1.bmp", cv::IMREAD_GRAYSCALE);
    
    if (image.empty()) {
        cout << "\nImage file corrupt or not found!" << endl;
        // return -1 shows that the program ended with an error code -1
        return -1;
    }

    // ** image dimensions **
    // set rows and cols to the number of rows and columns in the image
    int rows = image.rows;
    int cols = image.cols;

    int** imageArr = new int* [rows];

    for (int i = 0; i < rows; i++) {
		imageArr[i] = new int[cols];
	}

    // copy the image data into the array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
			imageArr[i][j] = static_cast<int>(image.at<uchar>(i, j));
		}
    }

    // display the pixels of the image
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (imageArr[i][j] == 255)
			    cout << "0 "; // white pixel
            else 
                cout << "1 "; // black pixel
		}
        
		cout << "| End of Row: " << i + 1 << endl;
	}

    cout << endl;

    cout << "--------------------------------\n";
    cout << "Image stored in linked list as:";
    cout << "\n--------------------------------\n" << endl;

    // create a linked list object
    LinkedList2d processedImage;

    // convert the image array into a 2d linked list
    processedImage.convertTo2dLL(imageArr, rows, cols);

    // print the 2d linked list
    processedImage.print2dLL();

    // create a quad tree object
    QuadTree quadTree;

    cout << "\n--------------------------------\n";
    cout << "\tQuad Tree:";
    cout << "\n--------------------------------\n";

    // build the quad tree from the 2d array of the image
    quadTree.root = quadTree.buildQuadTree(imageArr, 0, 0, rows, cols, rows, cols);

    // print the quad tree
    //quadTree.printQuadTree(quadTree.root, true);
    cout << endl;
    quadTree.printQuadTree(quadTree.root, true, "Root"); // this one to execute

    // write node information to separate files
    quadTree.writeNodeInfo(quadTree.root, true); // this one to execute

    // ignore below code

    /*cv::namedWindow("First OpenCV Application", cv::WINDOW_AUTOSIZE);
    cv::imshow("First OpenCV Application", image);
    cv::moveWindow("First OpenCV Application", 0, 45);
    cv::waitKey(0);
    cv::destroyAllWindows();*/
    
    // terminate program
    return 0;
}