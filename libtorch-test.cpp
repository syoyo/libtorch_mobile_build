// libtorch-test.cpp : Defines the entry point for the application.
//

#include "libtorch-test.h"
#include <torch/torch.h>

int main()
{
	torch::Tensor tensor = torch::rand({ 2, 3 });
	std::cout << tensor << std::endl;
	return 0;
}
