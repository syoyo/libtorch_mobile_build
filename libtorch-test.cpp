// libtorch-test.cpp : Defines the entry point for the application.
//

#include "libtorch-test.h"
#include <torch/torch.h>
#include <torch/script.h>

// https://github.com/pytorch/pytorch/issues/27726#issuecomment-541789562
struct JITCallGuard {
  // AutoGrad is disabled for mobile by default.
  torch::autograd::AutoGradMode no_autograd_guard{false};
  // Disable graph optimizer to ensure list of unused ops are not changed for
  // custom mobile build.
  torch::jit::GraphOptimizerEnabledGuard no_optimizer_guard{false};
};

int main(int argc, char **argv)
{
	torch::Tensor tensor = torch::rand({ 2, 3 });
	std::cout << tensor << std::endl;

  torch::jit::script::Module text2mel_module;
  torch::jit::script::Module ssrn_module;
  try {
    // Deserialize the ScriptModule from a file using torch::jit::load().
    text2mel_module = torch::jit::load(argv[1]);
    ssrn_module = torch::jit::load(argv[2]);
  }
  catch (const c10::Error& e) {
    std::cerr << "error loading the model\n";
    return -1;
  }

  // Create a vector of inputs.
  std::vector<torch::jit::IValue> inputs;

  // For text2mel
  //L.shape torch.Size([1, 43]), int64 type
  //Y.shape torch.Size([1, 80, 1])
  //auto Loptions =
  //  torch::TensorOptions()
  //    .dtype(torch::kInt64);

  //inputs.push_back(torch::ones({1, 43}, Loptions));
  //inputs.push_back(torch::ones({1, 80, 1}));

  // For ssrn
  inputs.push_back(torch::ones({1, 80, 60}));

  // Disable autograd otherwise backward op error will happen
  // (e.g. Unknown builtin op: aten::_adaptive_avg_pool2d_backward)
  JITCallGuard guard;

  // Execute the model and turn its output into a tensor.
  // For text2mel
  //auto outputs = module.forward(inputs).toTuple();
  // _, Y_t, A
	//torch::Tensor outY_t = outputs->elements()[1].toTensor();

  //std::cout << "bora\n";
	//std::cout << outY_t  << std::endl;
  // For ssrn
  auto outputs = ssrn_module.forward(inputs).toTuple();
	torch::Tensor outZ = outputs->elements()[1].toTensor();

	std::cout << outZ  << std::endl;

	return 0;
}
