//
// Created by rcohen on 20/6/2021.
//
#pragma once
#ifndef LIBTOSA_TOSA_CPU_COMMANDS_H
#define LIBTOSA_TOSA_CPU_COMMANDS_H

#include "tosa_commands.h"
#include "tosa_operator.h"

namespace libtosa
{
    namespace impl
    {

        class CPUCommand : virtual public Command
        {
        public:
            virtual ~CPUCommand() = default;
            virtual void execute() = 0;
        };

        class CPUComputeCmd : virtual public ComputeCmd, CPUCommand
        {
        public:
            CPUComputeCmd(const std::string &name) : ComputeCmd(name) {}
            CPUComputeCmd(const std::string &name,
                          const TensorsList &in,
                          const TensorsList &out,
                          const AttrList &attr) : ComputeCmd(name, in, out, attr) {}

            virtual void execute()
            {
                std::cout << " excuting " << _name << std::endl;
            }
        };

        class CPUSignal : virtual public Signal, CPUCommand
        {
            std::condition_variable _cv;
            std::mutex _mutex;

        public:
            void wait();
            virtual void execute();
            virtual std::shared_ptr<Wait> getWaitCmd();
        };

        class CPUWait : virtual public Wait, CPUCommand
        {
        public:
            CPUWait(const std::shared_ptr<Signal> &wait_on) : Wait(wait_on) {}
            virtual void execute();
        };

        class TestCommand : virtual public Command, CPUCommand
        {
            int *_var;
            int _test_val;
            int _msec_sleep;

        public:
            TestCommand(int *variable, int test_val, int sleep_ms = 0) : _var(variable), _test_val(test_val), _msec_sleep(sleep_ms) {}

            virtual void execute();
        };

    }
}

#endif // LIBTOSA_TOSA_CPU_COMMANDS_H