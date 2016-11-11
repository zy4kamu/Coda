#include "libsvm-wrapper.h"

#include <iostream>
#include <exception>

using std::cout;
using std::exception;
using std::endl;

void exec();
void del(struct A* arr);
struct A* create(int i);

int main(int argc, char* argv[])
{
    try
    {
 //       PrepareConsole();
        exec();
        //exec2(argv[1],argv[2],argv[3]);
        cout<< "Hello world!\n";
//        exec3(argv[1],argv[2],argv[3]);
    }
    catch(exception& e)
    {
        cout << "Exception: "<< e.what() << endl;
    }
    return 0;
}

struct A
{
    int a;
};

struct A* create(int i)
{
    struct A* arr = new A[i];
    return arr;
}

void del(struct A* arr)
{
    delete [] arr;
}



void exec()
{
    _svm::SVM svm;
    svm.loadModel("/home/aromanenko/projects/libsvm-3.18/data_train/SVM_3000c/SVM_model.model");
    vector<pair<int,double> > x;
    for (int i = 0; i < 10; i++)
    {
        pair<int,double> elem;
        elem.first = 10+i;
        elem.second = 0.9+(double)i/10;
        x.push_back(elem);
    }
    svm.predictFile("/home/aromanenko/projects/libsvm-3.18/data_train/SVM_3000c/data_test.txt",
                    "/home/aromanenko/projects/libsvm-3.18/data_train/SVM_3000c/result_my.txt");

}
