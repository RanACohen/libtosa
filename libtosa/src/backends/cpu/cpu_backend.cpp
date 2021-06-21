#include "cpu_backend.h"
#include "cpu_commands.h"
#include "cpu_stream.h"

using namespace libtosa;
using namespace libtosa::impl;

Stream *CPUBackend::createStream(int id) {
    return new CPUStream(id);
}


ComputeCmdPtr CPUBackend::createComputeCmd(const std::string &op_name, const TensorsList &inputs, const TensorsList &outputs, const AttrList &attributes)
{
    return std::make_shared<CPUComputeCmd>(op_name, inputs, outputs, attributes);
}

std::shared_ptr<Signal> CPUBackend::createSignal()
{
    return std::make_shared<CPUSignal>();
}

CommandPtr CPUBackend::createTestCmd(int *variable, int test_val, int sleep_ms)
{
    return std::make_shared<TestCommand>(variable, test_val, sleep_ms);
}

ComputeCmdPtr CPUBackend::AddCmd(const Tensor &lhs, const Tensor &rhs, const Tensor &output)
{
    // todo: put special AddCmd
    auto c = new CPUAddCmd(lhs, rhs, output);
    return std::make_shared<CPUAddCmd>(lhs, rhs, output);
}

