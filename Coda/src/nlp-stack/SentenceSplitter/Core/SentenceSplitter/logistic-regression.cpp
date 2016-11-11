#include <fstream>
#include <sstream>
#include <cmath>

#include "logistic-regression.h"
#include "LBFGS.h"
#include "Tools.h"
#include "ssplitter-exceptions.h"

using std::log;
using std::exp;

const int undefined_value = -1;

namespace _classifiers
{

void BinaryFeaturesReader::load(const char * i_fileName)
{
    std::wifstream in(i_fileName);
	Tools::SetLocale(in);
    std::wstring data;

    std::vector<std::wstring> feats_from_file;

    int lineCounter = -1;
    while(std::getline(in, data))
    {
        lineCounter++;
        data = Tools::Trim(data);

        if(data.empty())
        {
            continue;
        }

        feats_from_file = Tools::Split(data, L"\t");
        std::wistringstream buffer(feats_from_file[feats_from_file.size() - 1]);
        int label;
        if(!(buffer >> label)) {
            throw(_sentence_splitter::ModelException(i_fileName, lineCounter));
        }
        feats_from_file.pop_back();

        std::vector<std::wstring> binary_features;
        std::unordered_map<std::wstring, double> double_features;

        for(size_t i = 0; i < feats_from_file.size(); i++)
        {
            std::vector<std::wstring> res = Tools::Split(feats_from_file[i], L"==");
            if(res.size() == 2)
            {
                std::wistringstream buffer(res[1]);
                double value;
                if(buffer >> value) {
                    double_features[res[0]] = value;
                    continue;
                }
            }
            binary_features.push_back(feats_from_file[i]);
        }

        addDataEntry( binary_features, double_features, label);
    }

    in.close();

}

void BinaryFeaturesReader::addDataEntry(const std::vector<std::wstring>& features, const std::unordered_map<std::wstring, double>& double_features, int label, double weight)
{
    for(size_t i = 0; i < features.size(); i++)
    {
        if(m_feature_name_to_index.find(features[i]) == m_feature_name_to_index.end())
        {
            m_index_to_feature_name.push_back(features[i]);
            m_feature_name_to_index[features[i]] = m_index_to_feature_name.size() - 1;
        }
    }

    for(auto iter = double_features.begin(); iter != double_features.end(); iter++)
    {
        if(m_feature_name_to_index.find(iter->first) == m_feature_name_to_index.end())
        {
            m_index_to_feature_name.push_back(iter->first);
            m_feature_name_to_index[iter->first] = m_index_to_feature_name.size() - 1;
        }
    }

    m_train_labels.push_back(label);
    m_weights.push_back(weight);

    std::unordered_map<int,double> trainingPoint;

    for(size_t i = 0; i < features.size(); i++)
    {
        trainingPoint[m_feature_name_to_index[features[i]]] = 1.0;
    }

    for(auto iter = double_features.begin(); iter != double_features.end(); iter++)
    {
        trainingPoint[m_feature_name_to_index[iter->first]] = iter->second;
    }

    m_training_set.push_back(trainingPoint);
}

std::vector<std::wstring> BinaryFeaturesReader::getFeatureNames() const
{
    return m_index_to_feature_name;
}

double LogisticRegressionModelTrainer::CalculateValue(const std::vector<double>& i_theta)
{
    double result = 0;

    int m = m_model->m_training_set.size();

    for(int i = 0; i < m; ++i)
    {
        int yi = m_model->m_train_labels[i];
        double weight = m_model->m_weights[i];
        double fi = sigmoid_(i_theta,m_model->m_training_set[i]);

        result -= weight * ( yi ? log(fi) : log(1-fi) );//yi*log(fi) + (1 - yi)*log(1 - fi);

    }

    double norm = 0;

    for(int j = 0; j < dimension; j++)
    {
        norm += i_theta[j]*i_theta[j];
    }

    result += 0.0001*norm;

    return result;

}

void LogisticRegressionModelTrainer::CalculateGradient(
    const std::vector<double>& i_theta, std::vector<double>* gradient)
{
    int m = m_model->m_training_set.size();

    for(int i = 0; i < m; ++i)
    {
        int yi = m_model->m_train_labels[i];
        double weight = m_model->m_weights[i];
        double fi = sigmoid_(i_theta,m_model->m_training_set[i]);
        for(auto iter = m_model->m_training_set[i].begin(); iter != m_model->m_training_set[i].end(); iter++)
        {
            (*gradient)[iter->first] -= weight*(yi-fi); // * iter->second;
            (*gradient)[iter->first] += weight*(2*0.0001 * i_theta[iter->first]);
        }

    }
}

double LogisticRegressionModelTrainer::sigmoid_(const std::vector<double>& i_theta, const std::unordered_map<int, double>& i_item)
{
    double result = 0;

    for(auto iter = i_item.begin(); iter != i_item.end(); iter++)
    {
        result += i_theta[iter->first];// * iter->second = 1
    }

    result = 1/(1 + exp(-result));

    return result;
}

double EMLogisticRegressionTrainer::CalculateValue(const std::vector<double>& i_theta)
{
    double result = 0;

    int m = m_model->m_training_set.size();

    for(int i = 0; i < m; ++i)
    {
        int yi = m_model->m_train_labels[i];
        if(yi != undefined_value)
        {
            result -= log(likelihood(i_theta,m_model->m_training_set[i],yi));
        } else {
            double pointLikelihood = likelihood(i_theta,m_model->m_training_set[i],1);
            double expPointLikelihood = likelihood(expectationTheta,m_model->m_training_set[i],1);
            result -= expPointLikelihood*log(pointLikelihood) + (1-expPointLikelihood)*log(1-pointLikelihood);
        }
    }

    double norm = 0;

    for(int j = 0; j < dimension; j++)
    {
        norm += i_theta[j]*i_theta[j];
    }

    result += 0.0001*norm;

    return result;

}

double EMLogisticRegressionTrainer::likelihood(const std::vector<double>& i_theta, const std::unordered_map<int, double>& i_item, const int label)
{
    double fi = sigmoid_(i_theta,i_item);
    return label ? fi : 1-fi;
}

void EMLogisticRegressionTrainer::gradient(const std::vector<double>& i_theta, const std::unordered_map<int, double>& i_item, const int label, std::vector<double>* o_gradient)
{
    double fi = sigmoid_(i_theta,i_item);
    for(auto iter = i_item.begin(); iter != i_item.end(); iter++)
    {
        (*o_gradient)[iter->first] -= (label-fi); // * iter->second;
        (*o_gradient)[iter->first] += 2*0.0001 * i_theta[iter->first];
    }
}

void EMLogisticRegressionTrainer::CalculateGradient(
    const std::vector<double>& i_theta, std::vector<double>* o_gradient)
{
    int m = m_model->m_training_set.size();

    for(int i = 0; i < m; ++i)
    {
       int yi = m_model->m_train_labels[i];
       if(yi != undefined_value)
       {
           gradient(i_theta, m_model->m_training_set[i], yi, o_gradient);
       } else {
           std::vector<double> pointGradientPlus;
           pointGradientPlus.resize( i_theta.size());
           std::vector<double> pointGradientMinus;
           pointGradientMinus.resize( i_theta.size());
           gradient(i_theta, m_model->m_training_set[i], 1, &pointGradientPlus);
           gradient(i_theta, m_model->m_training_set[i], 0, &pointGradientMinus);
           double pointLikelihood = likelihood(expectationTheta,m_model->m_training_set[i],1);
           for(auto iter = m_model->m_training_set[i].begin(); iter != m_model->m_training_set[i].end(); iter++)
           {
               (*o_gradient)[iter->first] += pointLikelihood*pointGradientPlus[iter->first] + (1-pointLikelihood)*pointGradientMinus[iter->first];
           }
       }
    }
}

LogisticRegressionModel::LogisticRegressionModel(const std::vector<std::wstring>& i_features, const std::vector<double>& i_theta)
{
    m_index_to_feature_name = i_features;
    for(size_t i = 0; i < i_features.size(); i++)
    {
        m_feature_name_to_index[m_index_to_feature_name[i]] = i;
    }

    m_theta = i_theta;
}

LogisticRegressionModel::LogisticRegressionModel(const std::string& i_file_name)
{
    std::wifstream in(i_file_name);
	Tools::SetLocale(in);
    if(!in)
        throw ENoSuchFile(i_file_name);

    char * curloc = setlocale(LC_NUMERIC, "C");
    std::wstring buffer;
    std::string buffer2;

    std::getline(in, buffer);

    Tools::ConvertToString(buffer, buffer2);

    int dimension = std::atoi(buffer2.c_str());

    for(int i = 0; i < dimension; i++)
    {
        std::getline(in, buffer);
        m_index_to_feature_name.push_back(buffer);
        m_feature_name_to_index[buffer] = i;
    }

    for(int i = 0; i < dimension; i++)
    {
        std::getline(in, buffer);
        Tools::ConvertToString(buffer, buffer2);
        m_theta.push_back(std::atof(buffer2.c_str()));
    }
    setlocale(LC_NUMERIC, curloc);
    in.close();
}

void LogisticRegressionModel::save(const std::string& i_file_name) const
{
    std::wofstream out(i_file_name);

	Tools::SetLocale(out);

    out << m_index_to_feature_name.size() << std::endl;


    for(size_t i = 0; i < m_index_to_feature_name.size(); i++)
    {
        out << m_index_to_feature_name[i] << std::endl;
    }

    for(size_t i = 0; i < m_index_to_feature_name.size(); i++)
    {
        out << m_theta[i] << std::endl;
    }

    out.close();
}

int LogisticRegressionModel::getClass(const std::vector<std::wstring>& i_features) const
{
    double result = 0;

    for(size_t i = 0; i < i_features.size(); i++)
    {
        if(m_feature_name_to_index.find(i_features[i]) != m_feature_name_to_index.end())
        result += m_theta[m_feature_name_to_index.at(i_features[i])];// * iter->second = 1
    }

    result = 1.0/(1.0 + exp(-result));

    return result > 0.5 ? 1 : 0;
}

void LogisticRegressionManager::train(const std::string& i_path_to_training_set, const std::string& i_path_to_logistic_regression_model)
{
    std::cout << "Started" << std::endl;

    try {
        std::shared_ptr<_classifiers::BinaryFeaturesReader> lrr = std::shared_ptr<_classifiers::BinaryFeaturesReader>(new _classifiers::BinaryFeaturesReader());
        lrr->load(i_path_to_training_set.c_str());
        train(lrr, i_path_to_logistic_regression_model);
     } catch( std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void LogisticRegressionManager::train(std::shared_ptr<_classifiers::BinaryFeaturesReader>& i_lrr, const std::string& i_path_to_logistic_regression_model)
{
    _classifiers::LogisticRegressionModelTrainer lrt(i_lrr);
    Optimization::LBFGS lbfgs;

    vector<double> zeroInit(i_lrr->getDimension(),0);
    vector<double> theta = zeroInit;
        std::cout << "Training" << std::endl;
    lbfgs.Optimize(lrt, NULL, zeroInit, &theta,10000);

    _classifiers::LogisticRegressionModel lrm(i_lrr->getFeatureNames(), theta);
    lrm.save(i_path_to_logistic_regression_model);
}

void LogisticRegressionManager::trainEM(std::shared_ptr<_classifiers::BinaryFeaturesReader>& i_lrr, const std::string& i_path_to_logistic_regression_model)
{
    _classifiers::EMLogisticRegressionTrainer lrt(i_lrr);
    Optimization::LBFGS lbfgs;

    vector<double> zeroInit(i_lrr->getDimension(),0);
    vector<double> theta = zeroInit;
    const size_t Number = 100;
    size_t counter = 0;
    while( counter < Number) {
        std::cout << "Training" << ' ' << counter << std::endl;
        lbfgs.Optimize(lrt, NULL, lrt.GetExpectation(), &theta, 10000);
        for( auto iter = theta.begin(); iter != theta.end(); iter++ ) {
            std::cout << *iter << ' ';
        }
        std::cout << std::endl;
        double f_current = lrt.CalculateValue(theta);
        double f_past = lrt.CalculateValue(lrt.GetExpectation());
        double delta = 0.;
        if(f_current != 0)
        {
            delta = (f_past - f_current) / f_current;
        }
        std::cout << delta << std::endl;
        if(delta < 0.0001) {
            break;
        }
        counter++;
        lrt.SetExpectation(theta);
    }
    _classifiers::LogisticRegressionModel lrm(i_lrr->getFeatureNames(), theta);
    lrm.save(i_path_to_logistic_regression_model);
}


void LogisticRegressionManager::test(const std::string& i_path_to_test_set, const std::string& i_path_to_logistic_regression_model)
{
    _classifiers::LogisticRegressionModel model(i_path_to_logistic_regression_model);

    float total = 0;
    float success = 0;
    float positive = 0;
    float tru_positive = 0;
    float tru = 0;

    std::wifstream in(i_path_to_test_set);

	Tools::SetLocale(in);

    std::wstring data;
    while(std::getline(in, data))
    {
        std::vector<std::wstring> feats_from_file;
        int tru_cl;
        data = Tools::Trim(data);
        if(data.empty()) {
            continue;
        }
        feats_from_file = Tools::Split(data, L"\t");
        std::wistringstream buffer(feats_from_file[feats_from_file.size() - 1]);
        buffer >> tru_cl;
        feats_from_file.pop_back();
        int cl = model.getClass(feats_from_file);
        if(cl == 1)
        {
            positive++;
        }
        if(tru_cl == 1)
        {
            tru++;
        }
        if(cl == tru_cl)
        {
            success++;
            if(tru_cl == 1)
            {
                tru_positive++;
            }
        }
    total++;
    }

    double accuracy = success / total;
    double precision = tru_positive / positive;
    double recall = tru_positive / tru;

    std::cout << "Precision: " << precision << std::endl << "Recall: " << recall << std::endl << "Accuracy: " << accuracy << std::endl;
}

}
