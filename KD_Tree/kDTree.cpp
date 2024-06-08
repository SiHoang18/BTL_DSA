#include "kDTree.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */
////////////Help Function////////////
void mergeSort(vector<vector<int>>& pointList, int st, int end, int axis) {
    if(st >= end) return;
    int mid = st + (end - st) / 2;
    mergeSort(pointList,st,mid,axis);
    mergeSort(pointList,mid + 1, end, axis);
    merge(pointList,st,mid,end,axis);
}

void merge(vector<vector<int>>& pointList, int st, int mid, int end, int axis) {
    int n1 = mid - st + 1;
    int n2 = end - mid;
    vector<vector<int>> left;
    vector<vector<int>> right;
    for(int i = 0;i < n1; ++i) left.push_back(pointList[i + st]);
    for(int i = 0;i < n2; ++i) right.push_back(pointList[mid + 1 + i]);
    int i = 0, j = 0, idx = st;
    while(i < n1 && j < n2){
        if(left[i][axis] < right[j][axis]){
            pointList[idx++] = move(left[i++]);
        }else{
            pointList[idx++] = move(right[j++]);
        }
    }
    while(i < n1) pointList[idx++] = move(left[i++]);
    while(j < n2) pointList[idx++] = move(right[j++]);

}
/////////////////////////////////////
//////////////kDTree/////////////////
kDTreeNode* kDTree::Dcopy(kDTreeNode* other){
    if(!other) return NULL;
    kDTreeNode* newNode = new kDTreeNode(*other);
    newNode->left = Dcopy(other->left);
    newNode->right = Dcopy(other->right);
    return newNode;
}

void kDTree::clear(kDTreeNode* root){
    if(!root) return
    clear(root->left);
    clear(root->right);
    delete root;
}

double kDTree::RCalculate(const vector<int>& node1, const vector<int>& node2){
    double res = 0.0;
    int sum = 0;
    for(int i = 0;i < this->k; ++i){
        sum += pow(node1[i] - node2[i],2);
    }
    res = sqrt(sum);
    return res;
}

kDTreeNode* kDTree::insert(kDTreeNode* root, const vector<int>& point, int idx){
    if(point.size() != this->k) return root;
    if(!root){
        return new kDTreeNode(point);
    }
    int axis = idx % k;
    if(point[axis] < root->data[axis]){
        root->left = insert(root->left,point,idx + 1);
    }
    else if(point[axis] >= root->data[axis]){
        root->right = insert(root->right,point,idx + 1);
    }
    return root;
}
void kDTree::print(const vector<int>& arr) const{
    cout<<'('<<arr[0];
    for(int i = 1;i < arr.size(); ++i) cout<<", "<<arr[i];
    cout<<')';
}
const kDTree &kDTree::operator=(const kDTree &other){
    if(this != &other){
        clear(this->root);
        this->k = other.k;
        this->root = Dcopy(other.root);
    }
    
    return *this;
}
void kDTree::preorderTraversal(kDTreeNode* root, bool check) const{
    if(!root) return;
    if(check) cout<<' ';
    print(root->data);
    preorderTraversal(root->left,true);
    preorderTraversal(root->right,true);
}
void kDTree::inorderTraversal(kDTreeNode* root) const{
    if(!root) return;
    inorderTraversal(root->left);
    print(root->data);
    cout<<' ';
    inorderTraversal(root->right);
}
void kDTree::postorderTraversal(kDTreeNode* root, bool check) const{
    if(!root) return;
    postorderTraversal(root->left,true);
    postorderTraversal(root->right,true);
    print(root->data);
    if(check) cout<<' ';
}

void kDTree::inorderTraversal() const{
    inorderTraversal(root);
}
void kDTree::preorderTraversal() const{
    preorderTraversal(root,false);
}
void kDTree::postorderTraversal() const{
    postorderTraversal(root,false);
}

void kDTree::insert(const vector<int>& point){
    root = insert(root,point,0);
}

int kDTree::height(kDTreeNode* root) const{
    if(!root) return 0;
    int l = height(root->left);
    int r = height(root->right);
    return max(l,r) + 1;
}
int kDTree::height() const{
    return height(root);
}
kDTreeNode* kDTree::minPoint(kDTreeNode* node1, kDTreeNode* node2, int dim){
    if(!node1) return node2;
    if(!node2) return node1;
    if(node1->data[dim] < node2->data[dim]) return node1;
    return node2;
}
kDTreeNode* kDTree::findMinimum(kDTreeNode* node, int dim, int depth){
    if(!node) return NULL;
    int pos = depth % this->k;
    if(pos == dim){
        if(!node->left) return node;
        return this->minPoint(node,this->findMinimum(node->left,dim,depth + 1),dim);
    }
    return this->minPoint(node,this->minPoint(this->findMinimum(node->left,dim,depth + 1), this->findMinimum(node->right,dim,depth + 1),dim),dim);
}
kDTreeNode* kDTree::remove(kDTreeNode* root, const vector<int>& point, int depth) {
    if (!root) return NULL;
    int dim = depth % this->k;
    if (root->data == point) {
        if (root->right && root->left) {
            kDTreeNode* min = this->findMinimum(root->right, dim, 0);
            root->data = min->data;
            root->right = this->remove(root->right, min->data, depth + 1);
        }
        else {
            kDTreeNode* temp = root;
            if (root->left)
                root = root->left;
            else if (root->right)
                root = root->right;
            else
                root = NULL;
            delete temp;
        }
        return root;
    }
    if (root->data[dim] > point[dim]) {
        root->left = this->remove(root->left, point, depth + 1);
    }
    else {
        root->right = this->remove(root->right, point, depth + 1);
    }
    return root;
}


void kDTree::remove(const vector<int> &point){
    if(!search(point)) return;
    root = remove(root,point,0);
}
int kDTree::nodeCount(kDTreeNode* root) const{
    if(!root) return 0;
    return nodeCount(root->left) + nodeCount(root->right) + 1;
}
int kDTree::nodeCount() const{
    return nodeCount(root);
}

int kDTree::leafCount(kDTreeNode* root) const{
    if(!root) return 0;
    if(!root->left && !root->right) return 1;
    return leafCount(root->left) + leafCount(root->right);
}
int kDTree::leafCount() const{
    return leafCount(root);
}

bool kDTree::search(const vector<int> &point, kDTreeNode* root, int axis){
    if(!root) return false;
    if(root->data == point) return true;
    bool l = false, r = false;
    if(point[axis] < root->data[axis]) l = search(point,root->left,(axis + 1) % this->k);
    else if(point[axis] > root->data[axis]) r = search(point,root->right,(axis + 1) % this->k);
    return l || r;
}
bool kDTree::search(const vector<int> &point){
    if(point.size() != this->k) return false;
    return search(point,root,0);
}
kDTreeNode* kDTree::buildTree(const vector<vector<int>>& pointList, int depth) {
    if(pointList.empty()) return NULL;
    if(pointList.size() == 1) return new kDTreeNode(pointList[0]);
    int k = pointList[0].size();
    int axis = depth % k;
    vector<vector<int>> sorted = pointList;
    mergeSort(sorted,0,sorted.size() - 1, axis);
    int median = (sorted.size() % 2 == 1) ? sorted.size() / 2 : (sorted.size() - 1) / 2;
    kDTreeNode* node = new kDTreeNode(sorted[median]);
    node->left = buildTree(vector<vector<int>>(sorted.begin(),sorted.begin() + median), depth + 1);
    node->right = buildTree(vector<vector<int>>(sorted.begin() + median + 1, sorted.end()), depth + 1);
    return node;
}
void kDTree::buildTree(const vector<vector<int>>& pointList) {
    root = buildTree(pointList, 0);
}
kDTreeNode* kDTree::nearestNeighbour(const vector<int> &target, kDTreeNode* root, kDTreeNode *&best, double &bestDist, int depth){
    if (!root) return best;
    int dim = depth % this->k;
    kDTreeNode* next = NULL;
    kDTreeNode* other = NULL;
    if (target[dim] < root->data[dim]) {
        next = root->left;
        other = root->right;
    } else {
        next = root->right;
        other = root->left;
    }
    
    nearestNeighbour(target, next, best, bestDist, depth + 1);

    double dist = RCalculate(target,root->data);
    if(dist < bestDist){
        bestDist = dist;
        best = root;
    }
    if(fabs(target[dim] - root->data[dim]) < bestDist){
        nearestNeighbour(target,other,best,bestDist,depth + 1);
    }
    return best;
}
void kDTree::nearestNeighbour(const vector<int> &target, kDTreeNode *&best){
    double bestDist = 1e9;
    best = nearestNeighbour(target,root,best,bestDist,0);
}
kDTreeNode* kDTree::kNearestNeighbour(const vector<int> &target, vector<kDTreeNode *> &bestList, kDTreeNode* root, kDTreeNode*& best ,double& bestDist, int depth){
    if (!root) return best;
    int dim = depth % this->k;
    kDTreeNode* next = NULL;
    kDTreeNode* other = NULL;
    if (target[dim] < root->data[dim]) {
        next = root->left;
        other = root->right;
    } else {
        next = root->right;
        other = root->left;
    }
    
    kNearestNeighbour(target, bestList, next, best, bestDist, depth + 1);

    double dist = RCalculate(target,root->data);
    if(dist < bestDist){
        bool flag = true;
        for(auto i : bestList){
            if(i->data == root->data){
                flag = false;
                break;
            }
        }
        if(flag){
            bestDist = dist;
            best = root;            
        }
    }
    if(fabs(target[dim] - root->data[dim]) < bestDist){
        kNearestNeighbour(target,bestList,other,best,bestDist,depth + 1);
    }
    return best;
}
void kDTree::kNearestNeighbour(const vector<int> &target, int k, vector<kDTreeNode *> &bestList){
    if(target.size() != this->k) return;
    for(int i = 0;i < k; ++i){
        double bestDist = 1e9;
        kDTreeNode* tmp = NULL;
        kNearestNeighbour(target,bestList,root,tmp,bestDist,0);
        bestList.push_back(tmp);
    }
}
/////////////////////////////////////
/////////////////kNN/////////////////
void kNN::fit(Dataset& X_train, Dataset& y_train){
    this->X_train = &X_train;
    this->y_train = &y_train;
    int row = 0, col = 0;
    this->X_train->getShape(row,col);
    this->kD = new kDTree(col);
    vector<vector<int>> vec;
    for(auto& l : this->X_train->data){
        vector<int> v(l.begin(),l.end());
        vec.push_back(v);
    }
    kD->buildTree(vec);
}

Dataset kNN::predict(Dataset& X_test){
    Dataset y_train;
    list<list<int>> l_y;
    for(auto test : X_test.data){
        vector<int> count(10,0);
        vector<kDTreeNode*> bestList;
        vector<int> tmp(test.begin(),test.end());
        kD->kNearestNeighbour(tmp,this->k,bestList);
        for(auto i : bestList){
            list<int> l_temp(i->data.begin(),i->data.end());
            for(int j = 0;j < this->X_train->data.size(); ++j){
                auto it = this->X_train->data.begin();
                advance(it,j);
                if(l_temp == *it){
                    auto it_y = this->y_train->data.begin();
                    advance(it_y,j);
                    count[*((*it_y).begin())]++;
                    break;
                }
            }
        }
        int mi = 0, cnt = 0;
        for(int i = 0;i < 10; ++i){
            if(cnt < count[i]){
                mi = i;
                cnt = count[i];
            }
        }
        l_y.push_back({mi});
    }
    y_train.data = l_y;
    y_train.columnName = this->y_train->columnName;
    return y_train;
}
double kNN::score(const Dataset &y_test, const Dataset &y_pred){
    int cnt = 0;
    for(auto i = y_test.data.begin(), j = y_pred.data.begin(); i != y_test.data.end() && j != y_pred.data.end(); ++i, ++j){
        if(*((*i).begin()) == *((*j).begin())) ++cnt;
    }
    double accuracy = (double) ((double)cnt / (double) y_pred.data.size());
    return accuracy;
}
/////////////////////////////////////