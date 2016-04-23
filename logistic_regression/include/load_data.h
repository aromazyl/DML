#ifndef LOAD_DATA_H_
#define LOAD_DATA_H_

#include <iostream>
#include <vector>
#include "data_struct.h"

class Load_Data {

public:
    Load_Data();
    ~Load_Data();
    void load_data(const char *train_file, std::string split_tag);
    std::vector<std::vector<sparse_feature> > fea_matrix;
    std::vector<double> label;
    long int fea_dim;

private:
    void split_line(const std::string& line, const std::string& split_tag, std::vector<std::string>& feature_index);
    void get_feature_struct(std::vector<std::string>& feature_index, std::vector<sparse_feature>& key_val);
    sparse_feature sf;
};
#endif
