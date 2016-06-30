#pragma once

#include <iostream>
#include <cstdio>
#include <cstdatomic>
#include <shared_ptr>

#include "../../common/include/base/logging.h"
#include "../../common/include/base/threadpool.h"

#define FREE(p)    \
    do {                    \
      if (NULL != p) {    \
        free(p);        \
        p = NULL;       \
      }                   \
    } while (0);
#define DELETE(p)  \
  do {                    \
    if (NULL != p) {    \
      delete p;       \
      p = NULL;       \
    }                   \
  } while(0);


#define MALLOC(num,type) (type *)alloc( (num) * sizeof(type) )

namespace DML {
namespace gbdt {

struct Point {
  int32_t idx;
  float feaValue;
};

struct Instance {
  float label;
  int32_t insNum;
  Point points[0];
  float fx;
  float gx;
};

inline const std::string& DumpPoint(const Point& point) {
  char tmpStr[20];
  sprintf(tmpStr, "%s", point.feaValue);
  return std::string(tmpStr);
}

struct TreeNode {
  int64_t leftInsNums;
  int64_t rightInsNums;
  double sumInf;
  int32_t leftChildIdx;
  int32_t rightChildIdx;
  Point splitFeature;
};

}
}
