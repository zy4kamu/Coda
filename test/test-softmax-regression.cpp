#include "gtest/gtest.h"

#include <fstream>
#include <memory>

#include "softmax-regression.h"

TEST_F (TestInflector, Noun_phrase_singular_to_genitive_singular)
{
    std::shared_ptr<softmax_regression::IFeaturesReader>  sampleReader("/home/mkudinov/Data/logreg");

    std::wstring result = inflector->inflect(L"синий мяч", inflector::GENITIVE, inflector::NUMBER_UNSPEC);
    EXPECT_EQ(L"синего мяча", result.c_str());
}


