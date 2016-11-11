/** 
 * @file    libsvm-wrapper.cpp
 * @brief	source file for class-wrapper for main libsvm functions
 */

#include "libsvm-wrapper.h"
#include "Tools.h"

#include <iostream>

using std::cerr;
using std::endl;
using std::cout;

using namespace _svm;

SVM::SVM()
{
    m_model = nullptr;
    m_predict_probability = 0;
    m_nr_class = 0;
    m_svm_type = 0;
}

void SVM::loadModel(const string &i_filename)
{
    m_model = shared_ptr<struct svm_model>(svm_load_model(i_filename.c_str()),[&](svm_model* p){svm_free_and_destroy_model(&p);});
    if (!m_model.get())
        std::cerr<<L"Can\'t load SVM model\n";
    m_predict_probability = svm_check_probability_model(m_model.get());
    m_svm_type=svm_get_svm_type(m_model.get());
    m_nr_class=svm_get_nr_class(m_model.get());

    if (m_predict_probability)
    {
        m_labels = vector<int>(m_nr_class, 0);
        int *labels=(int *) malloc(m_nr_class*sizeof(int));
        svm_get_labels(m_model.get(),labels);
        for (int j = 0; j < m_nr_class; j++)
        {
            m_labels[j] = labels[j];
        }
        free(labels);
    }
}

double SVM::predictInstanse(shared_ptr<svm_node> i_x, vector<double>& o_prob_estimates)
{
    if (m_model == nullptr)
    {
        cerr<<"SVM model is not loaded"<<endl;
        return 0.0;
    }

    double predict_label;
    o_prob_estimates = vector<double>(m_nr_class,0.0);

    if (m_predict_probability && (m_svm_type==C_SVC || m_svm_type==NU_SVC))
    {
        double* prob_estimates = (double *) malloc(m_nr_class*sizeof(double));
        predict_label = svm_predict_probability(m_model.get(),i_x.get(),prob_estimates);
        for (int j = 0; j < m_nr_class; j++)
        {
            o_prob_estimates[j] =  prob_estimates[j];
        }
        free(prob_estimates);
    }
    else
    {
        predict_label = svm_predict(m_model.get(),i_x.get());
    }
    return predict_label;
}

double SVM::predictInstanse(shared_ptr<svm_node> i_x, shared_ptr<double> o_prob_estimates)
{
    if (m_model == nullptr)
    {
        cerr<<"SVM model is not loaded"<<endl;
        return 0.0;
    }

    double predict_label;

    if (m_predict_probability && (m_svm_type==C_SVC || m_svm_type==NU_SVC))
    {
        predict_label = svm_predict_probability(m_model.get(),i_x.get(),o_prob_estimates.get());
    }
    else
    {
        predict_label = svm_predict(m_model.get(),i_x.get());
    }
    return predict_label;
}

double SVM::predictInstanse(shared_ptr<svm_node> i_x)
{
    if (m_model == nullptr)
    {
        cerr<<"SVM model is not loaded"<<endl;
        return 0.0;
    }
    double predict_label;

    if (m_predict_probability && (m_svm_type==C_SVC || m_svm_type==NU_SVC))
    {
        cout<<"You can use probability estimation, but evoid this ability"<<endl;
    }
    if (!m_predict_probability)
    {
        predict_label = svm_predict(m_model.get(),i_x.get());
    }
    return predict_label;
}

vector<int> SVM::labels(void)
{
    if (m_model == nullptr)
    {
        cerr<<"SVM model is not loaded"<<endl;
        return vector<int>();
    }
    if (m_predict_probability)
        cout << "SVM model do not able to estimate probability. So returned vector will be empty"<<endl;
    return m_labels;
}

shared_ptr<svm_node> SVM::convertInstance(vector<pair<int,double> >& i_x)
{
    size_t size = i_x.size();
    shared_ptr<svm_node> x(new svm_node[size+1],[&](svm_node* ptr){delete [] ptr;});
    size_t i = 0;
    for (i = 0; i < i_x.size(); i++)
    {
        (x.get()+i)->index = i_x[i].first;
        (x.get()+i)->value = i_x[i].second;
    }
    (x.get()+i)->index = -1;
    (x.get()+i)->value = 0;

    return x;
}

double SVM::predictFile(const string &i_ifile, const string &i_ofile)
{
    if (m_model == nullptr)
    {
        cerr<<"SVM model is not loaded"<<endl;
        return 0.0;
    }

    ifstream iStream(i_ifile);
    ofstream oStream(i_ofile);
    if (!iStream||!oStream)
    {
        cerr<<"Can\'t open file"<<endl;
        return 0.0;
    }
    string instance;
    string delim = " ";
    string delim2 = ":";

    char* loc = setlocale(LC_NUMERIC,"C");

    double target_label, predict_label;
    int correct = 0;
    int total = 0;
    double error = 0;
    double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

    shared_ptr<double> prob_estimates;
    if (m_predict_probability)
    {
        prob_estimates = shared_ptr<double>(new double[this->m_nr_class],[&](double* ptr){delete [] ptr;});
        oStream<<"pred target";
        for (size_t i = 0; i < m_labels.size(); i++)
        {
            oStream<<" "<<m_labels[i];
        }
        oStream<<endl;
    }
    while (getline(iStream,instance))
    {
        vector<string> splitted = Tools::Split(instance, delim);
        if (splitted.size() < 3)
        {
            cerr<<"Wrong format of instance description"<<endl;
            continue;
        }
        target_label = (double)atof(splitted[0].c_str());
        vector<pair<int,double> > x;
        pair<int,double> indVal;
        for (size_t i = 1; i < splitted.size(); i++)
        {
            if (splitted[i].empty())
                continue;
            vector<string> indexValue = Tools::Split(splitted[i],delim2);
            if (indexValue.size()!=2)
            {
                cerr<<"Wrong format of index:value"<<endl;
                continue;
            }
            indVal.first = atoi(indexValue[0].c_str());
            indVal.second = (double)atof(indexValue[1].c_str());
            x.push_back(indVal);
        }
        shared_ptr<svm_node> sx = this->convertInstance(x);

        if (m_predict_probability && (m_svm_type==C_SVC || m_svm_type==NU_SVC))
        {
            predict_label = this->predictInstanse(sx,prob_estimates);
            oStream<<predict_label<<" "<<target_label;
            for (int j = 0;j < m_nr_class; j++)
                oStream<<" "<<*(prob_estimates.get()+j);
            oStream<<endl;
        }
        else
        {
            predict_label = this->predictInstanse(sx);
            oStream<<predict_label<<endl;
        }

        if(predict_label == target_label)
            ++correct;
        error += (predict_label-target_label)*(predict_label-target_label);
        sump += predict_label;
        sumt += target_label;
        sumpp += predict_label*predict_label;
        sumtt += target_label*target_label;
        sumpt += predict_label*target_label;
        ++total;
    }

    if (m_svm_type==NU_SVR || m_svm_type==EPSILON_SVR)
    {
        cout<<"Mean squared error = "<< error/total<<" (regression)"<<endl;
        cout<<"Squared correlation coefficient = ";
        cout<<((total*sumpt-sump*sumt)*(total*sumpt-sump*sumt))/
              ((total*sumpp-sump*sump)*(total*sumtt-sumt*sumt));
        cout<<" (regression)"<<endl;
    }
    else
        cout<<"Accuracy = "<<(double)correct/total*100<<"("<<correct<<"/"<<total<<") (classification)"<<endl;

    iStream.close();
    oStream.close();
    setlocale(LC_NUMERIC,loc);
    return (double)correct/total;
}
