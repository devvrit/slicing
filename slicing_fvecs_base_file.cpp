
#include <cstdio>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>
#include <random>
#include<vector>

void load_fvecs(char* filename, char* outputfile, size_t& num,
	unsigned& dim, unsigned& slice_size) {  // load data with fvecs pattern
	std::ifstream in(filename, std::ios::binary);
	if (!in.is_open()) {
		std::cout << "open file error" << std::endl;
		exit(-1);
	}
	std::ofstream out(outputfile, std::ios::binary | std::ios::out);
	in.read((char*)&dim, 4);
	in.seekg(0, std::ios::end);
	std::ios::pos_type ss = in.tellg();
	size_t fsize = (size_t)ss;
	num = (size_t)(fsize / (dim + (1*4) ));
	std::cout<<"number of points are: "<<num <<std::endl;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 generator(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<size_t> distribution(0, num - 1);
	size_t tmp;
	bool flag[num];
	for (size_t i = 0; i < num; i++) {
		flag[i] = false;
	}
	//data = new float[(size_t)num * (size_t)dim];
	char* data = new char[dim];
	
	for (unsigned j = 0; j<slice_size; j++)
	{
		tmp = distribution(generator);
		while (flag[tmp] == true) {
			tmp = distribution(generator);
		}
		flag[tmp] = true;
		in.seekg((tmp*(dim+4)), std::ios::beg);
		in.seekg(4, std::ios::cur);
		in.read((char*)data, dim);
		out.write((char*)&dim, sizeof(float));
		//std::cout<<"js is: " << j << std::endl;
		for (unsigned k=0; k<dim; k++){
			char* fil=new char();
			*fil = NULL;
			out.write((char*)fil,1);
			out.write((char*)fil,1);
			out.write((char*)fil,1);
			out.write((char*)data+k,1);
		}
		//std::memcpy(pivot_data + j * dim, data_load + tmp_pivot * dim, dim * sizeof(float));
		//		std::cout<<pivot_ids[j]<<" ";
	}
	
	in.close();
	out.close();
}


int main(int argc, char** argv) {

	if (argc != 4) {
		std::cout << argv[0] << "format: original_file new_file slice_size"
			<< std::endl;
		exit(-1);
	}

	unsigned dim;
	size_t num;

	unsigned int slice_size = (unsigned int)strtol(argv[3], NULL, 10);

	load_fvecs(argv[1], argv[2], num, dim, slice_size);

	std::cout << "number of points in original file were: " << num << std::endl;

}
