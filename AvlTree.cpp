/*
 * MIT License
 *
 *  Copyright (c) 2023 Mahmoud Yaman Ayman Seraj Alddin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

using namespace std;


/**
 * @tparam T type of the data stored in the AVL tree.
 */
template <typename T>
class AVL {
public:
    /* Custom size type for the AVL tree */
    typedef long long size_type;

    /**
     * Simple class used to represent nodes in the AVL tree.
     */
    class Node {
    public:
        /*
         * Data field of type T, stores the value inside the node.
         */
        T key;

        /*
         * Height of the subtree whose root is this node instance.
         */
        size_type height;

        /*
         * Node pointer to the left child.
         */
        Node *left;

        /*
         * Node pointer to the left child.
         */
        Node *right;

        /**
         * Initializes height to 1, left to nullptr, & right to nullptr.
         *
         * @param value stored inside the node.
         */
        explicit Node(T value): height{1}, key{value}, left{nullptr}, right{nullptr} {}
    };

    /**
     * @param value to be inserted into the AVL tree.
     */
    [[maybe_unused]] void insert(const T& value) {
        root = insert(root, value);
    }

    /**
     * @param value first value to be inserted
     * @param values rest of the value to be inserted
     */
    template <typename ...Args>
    [[maybe_unused]] void insert(const T& value, Args... values) {
        root = insert(root, value);
        insert(values...);
    }

    /**
     * @param value to be deleted from the AVL tree, if present.
     */
    [[maybe_unused]] void remove(const T& value) {
        root = remove(root, value);
    }

    /**
     * @param value to be searched for in the AVL tree.
     * @return nullptr if the value does not exist in the tree.
     *         Otherwise, a pointer to the node, closest to the root, having the value.
     */
    [[maybe_unused]] Node* search(const T& value) {
        return search(root, value);
    }

    /**
     * @return pointer to the root node.
     */
    [[maybe_unused]] inline Node* getRoot() {
        return root;
    }

    /**
     * @param node pointer to a node in the tree.
     * @return the height of the node. If nullptr, returns 0.
     */
    static size_type height(Node *node) {
        return node ? node->height : 0;
    }

    /**
     * @return the height of the tree.
     */
    [[nodiscard]] size_type height() {
        return height(root);
    }
private:
    /**
     * Struct used in in the DisplayGrid data-type alias.
     */
    struct cell_display {
        // String value in the cell.
        string value;

        // True indicates that the value is present and should be displayed.
        bool present;

        // No-arg constructor for the struct.
        cell_display(): present{false} {}

        /**
         * @param value to be stored in the value data field. Initializes present to true.
         */
        explicit cell_display(string value): value{std::move(value)}, present{true} {}
    };

    // Type alias for the display rows.
    using DisplayGrid = vector<vector<cell_display>>;

    /*
     * Node pointer to the root node of the tree.
     */
    Node *root{nullptr};

    /**
     * @param root to start the search from.
     * @param value to search for in the AVL tree.
     * @return nullptr if the value does not exist in the tree.
     *         Otherwise, a pointer to the node, closest to the root, having the value.
     */
    static Node* search(Node * root, const T& value) {
        if (not root) {
            return nullptr;
        }

        if (root->key == value) {
            return root;
        }

        if (root->key < value) {
            return search(root->right, value);
        }

        // root->key > value
        return search(root->left, value);
    }

    /**
     * @param root of a subtree to perform a right rotation on.
     * @return the new root for the subtree.
     */
    static Node* rightRotation(Node *root) {
        Node *new_root = root->left;
        root->left = new_root->right;
        new_root->right = root;

        // update height for roots
        root->height = 1 + max(height(root->left), height(root->right));
        new_root->height = 1 + max(height(new_root->left), height(new_root->right));

        return new_root;
    }

    /**
     * @param root of a subtree to perform a left rotation on.
     * @return the new root for the subtree.
     */
    static Node* leftRotation(Node *root) {
        Node *new_head = root->right;
        root->right = new_head->left;
        new_head->left = root;

        // update height for roots
        root->height = 1 + max(height(root->left), height(root->right));
        new_head->height = 1 + max(height(new_head->left), height(new_head->right));

        return new_head;
    }

    /**
     * @param root of the subtree to insert the value in.
     * @param value to be inserted in the subtree.
     * @return pointer to the new root of the subtree after performing the insertion & possible rotations.
     */
    static Node* insert(Node *root, const T& value) {
        // the subtree is empty
        if (not root) {
            return new Node(value);
        }

        if (value < root->key) {
            // Value less than root, go to left subtree
            root->left = insert(root->left, value);
        } else if (value > root->key) {
            // Value greater than root, go to right subtree
            root->right = insert(root->right, value);
        }

        // update root height
        root->height = 1 + max(height(root->left), height(root->right));

        // calculate new root balance
        const auto balance{height(root->left) - height(root->right)};

        if (1 < balance) {
            if (value < root->left->key) {
                return rightRotation(root);
            } else {
                root->left = leftRotation(root->left);
                return rightRotation(root);
            }
        } else if (balance < -1) {
            if (value > root->right->key) {
                return leftRotation(root);
            } else {
                root->right = rightRotation(root->right);
                return leftRotation(root);
            }
        }

        return root;
    }

    /**
     * @param root of the subtree to remove the value from.
     * @param value to be removed from the subtree, if present.
     * @return pointer to the new root of the subtree after performing the removal & possible rotations.
     */
    static Node* remove(Node *root, const T& value) {
        // Tree is empty
        if (not root) {
            return nullptr;
        }

        if (value < root->key) {
            // Check left subtree
            root->left = remove(root->left, value);
        } else if (value > root->key) {
            // Check right subtree
            root->right = remove(root->right, value);
        } else { // We found the value
            // Temporary holder for the right node
            Node *right{root->right};

            if (not root->right) { // Does not have right child
                Node *left{root->left};

                // Deallocate old value
                delete root;

                root = left;
            } else if (not root->left) { // Does not have left child
                delete root;

                root = right;
            } else { // Has both children
                while(right->left and right->left->left) {
                    // go right until we encounter a node that either does not have a left child
                    // or has a left child that does not have a left child
                    right = right->left;
                }

                if (not right->left) {
                    // right does not have a left child
                    root->key = right->key;
                    root->right = right->right;
                    delete right;
                } else {
                    // right has a child that does not have a left child
                    const auto temp{right->left};
                    root->key = right->left->key;
                    right->left = nullptr;
                    delete temp;
                }
            }
        }

        // Tree became empty
        if (not root) {
            return root;
        }

        // update height
        root->height = 1 + max(height(root->left), height(root->right));

        // calculate new balance
        auto balance{height(root->left) - height(root->right)};

        if (balance > 1) {
            if (height(root->left) >= height(root->right)) {
                return rightRotation(root);
            } else {
                root->left = leftRotation(root->left);
                return rightRotation(root);
            }
        } else if (balance < -1) {
            if (height(root->right) >= height(root->left)) {
                return leftRotation(root);
            } else {
                root->right = rightRotation(root->right);
                return leftRotation(root);
            }
        }

        return root;
    }

    /**
     * @return a vector of vectors of cell_display structs.
     *         Each vector of cell_display structs represents one row, starting at the root.
     */
    DisplayGrid generateDisplayGrid() const {
        // build a vector of vectors of Node pointers
        vector<Node*> traversal_stack;
        vector<vector<Node*>> rows;

        if (not root) {
            return DisplayGrid();
        }

        Node *current{root};
        const auto max_depth{root->height};
        rows.resize(max_depth);
        size_type depth{0};

        while(true) {
            // Max-height Nodes are always a leaf or null
            // This special case blocks deeper traversal
            if (depth == max_depth - 1) {
                rows[depth].push_back(current);

                if (depth == 0) {
                    break;
                }

                depth--;
                continue;
            }

            // First visit to Node? Go to left child.
            if (traversal_stack.size() == depth) {
                rows[depth].push_back(current);
                traversal_stack.push_back(current);

                if (current) {
                    current = current->left;
                }

                ++depth;
                continue;
            }

            // Odd child count? Go to right child.
            if (rows[depth + 1].size() % 2) {
                current = traversal_stack.back();
                if (current) {
                    current = current->right;
                }

                ++depth;
                continue;
            }

            // Time to leave if we get here

            // Exit loop if this is the root
            if (depth == 0) {
                break;
            }

            traversal_stack.pop_back();
            current = traversal_stack.back();
            depth--;
        }

        /*
         * Use rows of Node pointers to populate rows of cell_display structs.
         * All possible slots in the tree get a cell_display struct,
         * so if there is no actual Node at the struct location,
         * its boolean "present" field is set to false.
         * The struct also contains a string representation of
         * its Node's value, created using a stringstream object.
         */
        DisplayGrid result{};
        stringstream ss{};

        for(const auto& row: rows) {
            result.emplace_back();

            for(Node* pn: row) {
                if (pn) {
                    ss << pn->key;
                    result.back().push_back(cell_display(ss.str()));
                    ss = stringstream();
                } else {
                    result.back().push_back(cell_display());
                }
            }
        }

        return result;
    }

    /**
     * @param grid vector of rows of cell_display structs.
     * @return formats the grid generated by generateDisplayGrid() into a test representation
     *         as a vector of strings to be printed.
     */
    vector<string> formatGrid(const DisplayGrid& grid) const {
        //  maximum value string length, minimum width is 3.
        size_type cell_width{3};

        for(const auto& row: grid) {
            for(const auto& cell: row) {
                if (cell.present and cell_width < cell.value.length()) {
                    cell_width = cell.value.length();
                }
            }
        }

        // make sure the cell_width is an odd number
        if (cell_width % 2 == 0) {
            cell_width++;
        }

        // formatted rows
        vector<string> result{};

        // some of these counting variables are related,
        // so its should be possible to eliminate some of them.
        size_type row_count{static_cast<size_type>(grid.size())};

        // this row's element count, a power of two
        size_type row_elem_count{1 << (row_count - 1)};

        // left_pad holds the number of space chars at the beginning of the bottom row
        size_type left_pad{0};

        // holds the current row value
        string row{};

        // Work from the level of maximum height, up to the root
        // ("formatted_rows" will need to be reversed when done)
        for(size_type r{0}; r < row_count; r++) {
            const auto& gridRow = grid[row_count - r - 1]; // r reverse-indexes the row

            /*
             * "space" will be the number of rows of slashes needed to get
             * from this row to the next.  It is also used to determine other
             * text offsets.
             */
            size_type space{(size_type(1) << r) * (cell_width + 1) / 2 - 1};

            row.clear();

            // iterate over each element in this grid row
            for(size_type c{0}; c < row_elem_count; c++) {
                // add padding, more when this is not the leftmost element
                row += string(c ? 2 * left_pad + 1 : left_pad, ' ');

                if (gridRow[c].present) {
                    // This position corresponds to an existing Node
                    const string& nodeValue = gridRow[c].value;

                    /*
                     * Try to pad the left and right sides of the value string
                     * with the same number of spaces.  If padding requires an
                     * odd number of spaces, right-sided children get the longer
                     * padding on the right side, while left-sided children
                     * get it on the left side.
                     */
                    size_type long_padding{cell_width - static_cast<size_type>(nodeValue.length())};
                    size_type short_padding{long_padding / 2};

                    long_padding -= short_padding;

                    row += string(c % 2 ? short_padding : long_padding, ' ');
                    row += nodeValue;
                    row += string(c % 2 ? long_padding : short_padding, ' ');
                } else {
                    // This position is empty
                    row += string(cell_width, ' ');
                }
            }

            // A row of spaced-apart value strings is ready, add it to the result vector
            result.push_back(row);

            // The root has been added, so this loop is done
            if (row_elem_count == 1) {
                break;
            }

            /*
             * Add rows of forward- and back- slash characters, spaced apart
             * to "connect" two rows' Node value strings.
             * The "space" variable counts the number of rows needed here.
             */
            size_type left_space{space + 1};
            size_type right_space{space - 1};

            for(size_type i{0}; i < space; i++) {
                row.clear();

                for (size_type c{0}; c < row_elem_count; c++) {
                    if (c % 2 == 0) {
                        row += string(c ? 2 * left_space + 1 : left_space, ' ');
                        row += gridRow[c].present ? '/' : ' ';
                        row += string(right_space + 1, ' ');
                    } else {
                        row += string(right_space, ' ');
                        row += gridRow[c].present ? '\\' : ' ';
                    }
                }

                result.push_back(row);
                left_space++;
                right_space--;
            }

            left_pad += space + 1;
            row_elem_count /= 2;
        }

        // Reverse the result, placing the root Node at the beginning (top)
        reverse(result.begin(), result.end());

        return result;
    }

    /**
     * Trims an equal number of space characters from
     * the beginning of each string in the vector.
     * At least one string in the vector will end up beginning
     * with no space characters.
     *
     * @param rows to be trimmed
     */
    static void trimRowsLeft(vector<string>& rows) {
        if (rows.empty()) {
            return;
        }

        auto min_space = rows.front().length();
        for(const auto& row: rows) {
            auto i = row.find_first_not_of(' ');

            if (i == string::npos) {
                i = row.length();
            }

            if (i == 0) {
                // A trimmed string already exists
                return;
            }

            if (i < min_space) {
                min_space = i;
            }
        }

        for(auto& row: rows) {
            row.erase(0, min_space);
        }
    }

    /**
     * @param out output stream to display the AVL tree to
     * @return reference to the given output stream
     */
    ostream& display(ostream& out) {
        // If this tree is empty, tell someone
        if (height() == 0) {
            return out << "<empty tree>" << endl;
        }

        const auto grid = generateDisplayGrid();

        // then format these into a text representation...
        auto formattedGrid = formatGrid(grid);

        // then trim excess space characters from the left sides of the text...
        trimRowsLeft(formattedGrid);

        // then dump the text to out.
        for (const auto& row: formattedGrid) {
            out << ' ' << row << '\n';
        }

        return out;
    }

    /**
     * @param out output stream to display the tree to.
     * @param tree to be displayed.
     * @return reference to the given output stream.
     */
    template<typename C>
    friend ostream& operator<<(ostream& out, AVL<C>& tree);
};

/**
 * @tparam T type stored in the AVL tree. Implicitly inferred.
 * @param out output stream to display the tree to.
 * @param tree to be displayed.
 * @return reference to the given output stream.
 */
template<typename T>
ostream& operator<<(ostream &out, AVL<T>& tree) {
    return tree.display(out);
}
