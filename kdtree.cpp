/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */
template<int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim> & first, const Point<Dim> & second, int curDim) const
{
    if (first[curDim]==second[curDim])
        return first<second;
    return first[curDim]<second[curDim];
}


template<int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim> & target, const Point<Dim> & currentBest, const Point<Dim> & potential) const
{
    int dp=sqdis(target,potential);
    int dc=sqdis(target,currentBest);
    if (dp==dc)
        return potential<currentBest;
    return dp<dc;
}

template<int Dim>
KDTree<Dim>::KDTree(const vector< Point<Dim> > & newPoints)
{
    points.clear();
    points=newPoints;
    if (points.size()!= 0)
        sort(0,points.size()-1,0);
}

template<int Dim>
void KDTree<Dim>::sort(int left, int right, int curDim)
{
    if (left<right){
        partall(left,right,curDim,(left+right)/2);
        sort((left+right)/2+1,right,(curDim+1)%Dim);
        sort(left,(left+right)/2-1,(curDim+1)%Dim);
    }
}

template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> & query) const
{
   return nearest(query,0,points.size()-1,0);
}

template<int Dim>
Point<Dim> KDTree<Dim>::nearest(const Point<Dim>&query, int left, int right, int curDim) const
{
    if (left < right) {    
        int mid = (left + right) / 2;
        Point<Dim> currBest, tmp;
        //traverse to the closet hyperrectangle
        if (smallerDimVal(query, points[mid], curDim))
            currBest = nearest(query,left, mid - 1,  (curDim + 1) % Dim);            
        else 
            currBest = nearest(query, mid + 1, right, (curDim + 1) % Dim);
        //back traversal
        //check if parent node closer, set to currbest
        if (shouldReplace(query, currBest, points[mid]))
            currBest = points[mid];
        if (dimdis(query,points[mid],curDim) <= sqdis(query, currBest)) { //if splitting plane is closer than current best distance(sqdis)
            if (smallerDimVal(query,points[mid],curDim))
                tmp = nearest(query, mid + 1, right,  (curDim + 1) % Dim);   // search through the opposite side 
            else
                tmp = nearest(query, left, mid - 1,  (curDim + 1) % Dim);
            if (shouldReplace(query, currBest, tmp))
                currBest = tmp;
        }
        return currBest;
    }
    return points[left];
}


template<int Dim>
int KDTree<Dim>::sqdis(const Point<Dim> & target, const Point<Dim> & p) const
{
    int ret=0;
    for (int i=0;i<Dim;i++)
        ret+=(p[i]-target[i])*(p[i]-target[i]);
    return ret;
}

template<int Dim>
int KDTree<Dim>::dimdis(const Point<Dim> & target, const Point<Dim> & p, int curDim) const
{
    return (p[curDim]-target[curDim])*(p[curDim]-target[curDim]);
}

template<int Dim>
int KDTree<Dim>::partition(int left,int right, int curDim, int pivotIndex)
{
     Point<Dim> pivotValue = points[pivotIndex];
     std::swap(points[pivotIndex],points[right]);  // Move pivot to end
     int storeIndex = left;
     for (int i=left;i<right;i++){
         if (smallerDimVal(points[i],pivotValue,curDim)){
             std::swap(points[storeIndex],points[i]);
             storeIndex++;
            }
        }
     std::swap(points[right],points[storeIndex]);  // Move pivot to its final place
     return storeIndex;
 }

template<int Dim>
void KDTree<Dim>:: partall(int left, int right, int curDim,int n){
    if (left < right){        // If the list contains more than one element 
         // select a pivotIndex to be meadian(n here)
        int pivotIndex  = partition(left, right, curDim, n);
         // The pivot is in median position
        if (n < pivotIndex)
            partall(left, pivotIndex - 1, curDim, n);
        if (n > pivotIndex)
            partall(pivotIndex + 1, right, curDim, n);
    }
}
