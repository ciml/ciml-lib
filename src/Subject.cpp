#include "Subject.h"

Subject::Subject() {
    //cout << "subject set fit = 0 \n";
    fitness = 0;
}

void Subject::addTree(Tree* t){
    trees.push_back(t);
}


Subject::~Subject() {
//   cout << "Destrutor Subject" << endl;
//   cin.get();
  for(Tree* t : trees)
        delete t;
  trees.clear();
  //
}
