#include "features-reader.h"
#include "Tools.h"

namespace softmax_regression
{

void TextSampleReader::load(std::vector<std::wstring>& io_indexToFeatureName
                              , std::map<std::wstring, size_t>& io_featureNameToIndex
                              , std::vector<std::unordered_map<size_t, double>>& io_elements
							  , std::map<std::wstring, int>& io_classLabelToIndex
                              , std::vector<size_t>& io_labels) const
{
    std::wifstream in(m_fileName);
	Tools::SetLocale(in);
    std::wstring data;
    std::wstring label;

    std::vector<std::wstring> feats_from_file;

    while(std::getline(in, data))
    {
        data = Tools::Trim(data);
        feats_from_file = Tools::Split(data, L"\t");

        vector<wstring> features;
        vector<double> values;

        for(size_t i = 0; i < feats_from_file.size() - 1; i++)
        {
            vector<wstring> feature_value = Tools::Split(feats_from_file[i], L"=");
            features.push_back(feature_value[0]);
            if(feature_value.size() > 1) {
                double value;
                wstringstream converter;
                converter << feature_value.back();
                converter >> value;
                values.push_back(value);
            } else {
                values.push_back(1.0);
            }
        }

        for(size_t i = 0; i < features.size(); i++)
        {

            if(io_featureNameToIndex.find(features[i]) == io_featureNameToIndex.end())
            {
                io_indexToFeatureName.push_back(features[i]);
                io_featureNameToIndex[features[i]] = io_indexToFeatureName.size() - 1;
            }
        }

        label = feats_from_file[feats_from_file.size() - 1];

        if(io_classLabelToIndex.find(label) == io_classLabelToIndex.end())
        {
            io_classLabelToIndex[label] = io_classLabelToIndex.size() - 1;
        }

        io_labels.push_back(io_classLabelToIndex[label]);

        std::unordered_map<size_t,double> trainingPoint;

        for(size_t i = 0; i < features.size(); i++)
        {
            trainingPoint[io_featureNameToIndex[features[i]]] = values[i];
        }

        io_elements.push_back(trainingPoint);

    }

    in.close();
}

void ModelBinaryReader::load(std::vector<std::wstring>& io_indexToFeatureName
          , std::vector<std::wstring>& io_indexToLabel
          , std::vector<double>& io_theta) const
{
    std::ifstream in(m_fileName, std::ios::in | std::ios::binary);
    Tools::ReadWStringArray(in,io_indexToFeatureName);
    Tools::ReadWStringArray(in,io_indexToLabel);

    size_t sizeOfTheta = 0;

    in.read((char*)&sizeOfTheta, sizeof(sizeOfTheta));

    for(size_t i = 0; i < sizeOfTheta; ++i)
    {
        double thetha_i = 0;
        in.read((char*)&thetha_i, sizeof(double));
        io_theta.push_back(thetha_i);
    }
}

} //namespace _softmax_regression
