#include <iostream>
#include <string>
#include "softmax-regression-trainer.h"

using namespace std;
using namespace softmax_regression;

const string trainPath = "/home/irina/Stanford ner/Event_test_new/train";
const string testPath = "/home/irina/Stanford ner/Event_test_new/test";
const string modelPath = "/home/irina/Stanford ner/Event_test_new/model.txt";

int main(int argc, char *argv[])
{
   setlocale(LC_ALL,"");

   shared_ptr<ISoftmaxRegressionTrainer> trainer = std::shared_ptr<ISoftmaxRegressionTrainer> (new SoftmaxRegressionTrainer(trainPath, modelPath));

   trainer->Train();
   trainer->Validate(trainPath);

   trainer->Validate(testPath);


   return 0;
}


