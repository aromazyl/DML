#ifndef FTRL_H
#define FTRL_H
#include "load_data.h"
#include "mpi.h"
#include <math.h>

class FTRL{

public:
	FTRL(Load_Data* data, int total_num_proc, int my_rank);
	~FTRL();
	float* glo_w;
	float* loc_w;
	float** glo_v;
	float** loc_v;
        void run();
private:
	Load_Data* data;
	void init();
	float sigmoid(float x);
	void update_v();
	void update_w();
        void update_other_parameter();
	void ftrl();
  	int step;

        float* loc_f_val;
	float* loc_g_w;
	float* glo_g_w;
	float** loc_g_v;
	float** glo_g_v;

	float* loc_z_w;
	float* loc_sigma_w;
	float* loc_n_w;
 	float** loc_z_v;
	float** loc_sigma_v;
	float** loc_n_v;

	float factor;
	float alpha;
	float beta;
	float lambda1;
	float lambda2;
	int batch_size;

	int num_proc;
	int rank;
};

#endif
