#include "main.hpp"
#include "Dataset.hpp"
/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
/*Help Function*/
void mergeSort(vector<vector<int>>& pointList, int st, int end, int axis);
void merge(vector<vector<int>>& pointList, int st, int mid, int end, int axis);
/////////////////
struct kDTreeNode
{
    vector<int> data;
    kDTreeNode *left;
    kDTreeNode *right;
    kDTreeNode(vector<int> data, kDTreeNode *left = nullptr, kDTreeNode *right = nullptr)
    {
        this->data = data;
        this->left = left;
        this->right = right;
    }
    friend ostream &operator<<(ostream &os, const kDTreeNode &node){
        os << "(";
        for (size_t i = 0; i < node.data.size(); i++)
        {
            os << node.data[i];
            if (i != node.data.size() - 1)
            {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }
};

class kDTree
{
private:
    int k;
    kDTreeNode *root;
private:
    void print(const vector<int>& arr) const;
    void inorderTraversal(kDTreeNode* root) const;
    void preorderTraversal(kDTreeNode* root, bool check) const;
    void postorderTraversal(kDTreeNode* root, bool check) const;
    kDTreeNode* insert(kDTreeNode* root,const vector<int>& point, int idx);
    kDTreeNode* remove(kDTreeNode* root,const vector<int>& point, int depth);
    kDTreeNode* buildTree(const vector<vector<int>>& pointList, int depth);
    kDTreeNode* minPoint(kDTreeNode* node1, kDTreeNode* node2,int dim);
    kDTreeNode* findMinimum(kDTreeNode* node, int dim, int depth);
    double RCalculate(const vector<int>& node1, const vector<int>& node2);
    int height(kDTreeNode* root) const;
    int nodeCount(kDTreeNode* root) const;
    int leafCount(kDTreeNode* root) const;
    void clear(kDTreeNode* root);
    bool search(const vector<int> &point, kDTreeNode* root, int axis);
    kDTreeNode* nearestNeighbour(const vector<int> &target, kDTreeNode *root, kDTreeNode *&best, double &bestDist, int depth);
    kDTreeNode* Dcopy(kDTreeNode* other);
    kDTreeNode* kNearestNeighbour(const vector<int> &target, vector<kDTreeNode *> &bestList,kDTreeNode* root, kDTreeNode*& best, double& bestDist, int depth);
public:
    kDTree(int k = 2): k(k), root(NULL){}
    ~kDTree(){}
    kDTree(const kDTree &other): k(other.k), root(other.root){}
    const kDTree &operator=(const kDTree &other);
    void inorderTraversal() const;
    void preorderTraversal() const;
    void postorderTraversal() const;
    int height() const;
    int nodeCount() const;
    int leafCount() const;

    void insert(const vector<int> &point);
    void remove(const vector<int> &point);
    bool search(const vector<int> &point);
    void buildTree(const vector<vector<int>> &pointList);
    void nearestNeighbour(const vector<int> &target, kDTreeNode *&best);
    void kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList);
    friend void mergeSort(vector<vector<int>>& pointList, int st, int end, int axis);
};

class kNN
{
private:
    int k;
    Dataset *X_train;
    Dataset *y_train;
    kDTree  *kD;
    int numClasses;

public:
    kNN(int k = 5): k(k){}
    void fit(Dataset &X_train, Dataset &y_train);
    Dataset predict(Dataset &X_test);
    double score(const Dataset &y_test, const Dataset &y_pred);
};

// Please add more or modify as needed
