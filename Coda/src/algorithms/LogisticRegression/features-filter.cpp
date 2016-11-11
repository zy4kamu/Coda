#include "features-filter.h"
//#include <math.h>
#include <cmath>
#include <algorithm>
#include <limits.h>

using std::shared_ptr;
using std::vector;
using std::wstring;

namespace softmax_regression
{
    void FeaturesFilter::Filter(shared_ptr<TrainingSet> set) const
    {
        vector< vector<double> > features_for_classes;
        fillFeatureStatistics( set, features_for_classes );

        vector<size_t> new_to_old_features = filterFeatures(features_for_classes, 150000);
        refillSet(set, new_to_old_features);
    }

    void FeaturesFilter::fillFeatureStatistics( shared_ptr<TrainingSet> set, vector< vector<double> >& features_for_classes) const
    {
        size_t n_features = set->getNumberOfFeatures();
        size_t n_classes = set->getNumberOfClasses();

        vector<double> feature_vector(n_features, 0.);
        features_for_classes.resize(n_classes);
        fill(features_for_classes.begin(), features_for_classes.begin()+n_classes, feature_vector);

        for(size_t sample_index = 0; sample_index < set->getNumberOfElements(); sample_index++)
        {
            size_t classLabel = set->getLabelOfElement(sample_index);
            vector<double>& classFeatures = features_for_classes[classLabel];
            const std::unordered_map<size_t,double>& sample_features = set->getElement(sample_index);
            for(auto it = sample_features.begin(); it != sample_features.end(); it++)
            {
                classFeatures[it->first]+=it->second;
            }
        }
    }

    vector<size_t> FeaturesFilter::filterFeatures( const vector< vector<double> >& features_for_classes, size_t number) const
    {
        size_t features_count = features_for_classes[0].size();
        vector<size_t> result(std::min(number, std::min(m_maxFeaturesCount, features_count)));
        if(features_for_classes.empty()) {
            return result;
        }

        std::multimap<double, size_t> filtered;

        double n_total = 0.;
        vector<double> n_classes(features_for_classes.size(), 0.);
        vector<double> n_features(features_count, 0.);

        for(size_t class_index = 0; class_index < features_for_classes.size(); class_index++)
        {
            for(size_t feature_index = 0; feature_index < features_for_classes[class_index].size(); feature_index++)
            {
                n_total += features_for_classes[class_index][feature_index];
                n_classes[class_index] += features_for_classes[class_index][feature_index];
                n_features[feature_index] += features_for_classes[class_index][feature_index];
            }
        }

        for(size_t feature_index = 0; feature_index < features_count; feature_index++) {
            double feature_log_pr = - n_features[feature_index]  + n_total;
            if(feature_log_pr <= m_PMIThreshold) {
                continue;
            }
            double max_pmi = INT_MIN;
            for(size_t class_index = 0; class_index < features_for_classes.size(); class_index++) {
                if(features_for_classes[class_index][feature_index] == 0) {
                    continue;
                }
                double pmi_feature_class = features_for_classes[class_index][feature_index] - n_classes[class_index];
                max_pmi = std::max(pmi_feature_class, max_pmi);
            }
            if(max_pmi > m_PMIThreshold) {
                filtered.insert(std::make_pair(max_pmi, feature_index));
            }
        }

        // checking by max_features_count
        size_t counter = 0;
        for(auto iter = filtered.rbegin(); iter != filtered.rend(); iter++) {
           if(counter >= result.size()) {
               break;
           }
           result[counter++] = iter->second;
        }
        return result;
    }

    void FeaturesFilter::refillSet(std::shared_ptr<TrainingSet> set, const std::vector<size_t>& filtered ) const
    {
         vector<size_t> oldToNew( set->getNumberOfFeatures(), INT_MAX );
        for(size_t i = 0; i < filtered.size(); i++) {
            oldToNew[ filtered[i] ] = i;
        }

        vector<wstring> newIndexToFeatureName(filtered.size());
        for(size_t index = 0; index < filtered.size(); index++) {
            newIndexToFeatureName[index] = set->m_indexToFeatureName[ filtered[index] ];
        }
        std::map<std::wstring, size_t> newFeatureNameToIndex;
        for(size_t index = 0; index < filtered.size(); index++) {
            newFeatureNameToIndex[ newIndexToFeatureName[index] ] = index;
        }
        for(size_t element = 0; element < set->getNumberOfElements(); element++) {
            std::unordered_map<size_t,double> newElementFeatures;
            const std::unordered_map<size_t,double>& elementFeatures = set->getElement(element);
            for(auto it = elementFeatures.begin(); it != elementFeatures.end(); it++) {
                if(oldToNew[it->first] != INT_MAX) {
                    newElementFeatures[oldToNew[it->first]] = it->second;
                }
            }
            set->m_trainingSet[element] = newElementFeatures;
        }
        set->m_indexToFeatureName = newIndexToFeatureName;
        set->m_featureNameToIndex = newFeatureNameToIndex;
    }
}
