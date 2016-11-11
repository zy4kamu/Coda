#include <iostream>
#include <assert.h>
#include "Tools.h"
#include "features-writer.h"

namespace softmax_regression
{

void BinaryModelWriter::save(const std::vector<std::wstring>& i_features, const std::vector<std::wstring>& i_labels, const std::vector<double>& i_theta, const std::string& fileName) const
{
    std::ofstream out(fileName, std::ios::binary);

    Tools::WriteWStringArray_Binary(out, i_features);
    Tools::WriteWStringArray_Binary(out, i_labels);

    size_t sizeOfTheta = i_theta.size();
    out.write((char*)&sizeOfTheta, sizeof(size_t));

    for(auto iter = i_theta.begin(); iter != i_theta.end(); iter++)
    {
        out.write((char*)& (*iter), sizeof(double));
    }

    out.close();
}

} // namespace _softmax_regression
