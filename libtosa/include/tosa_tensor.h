//
// Created by rancohen on 23/5/2021.
//
// this file contains the tensor object definition, including tensor views
#pragma once

#ifndef LIBTOSA_TOSA_TENSOR_H
#define LIBTOSA_TOSA_TENSOR_H
#include <vector>
#include <memory>

#include "tosa_types.h"
#include "tosa_memory.h"

namespace libtosa {
    class Tensor;
    /**
     * TensorImpl is an Immutable object that describes a tensor or a sub view of another tensor
     * Shapes (and Strides) are Framework order, meaning the last dim is changing fastest in memory
     */
    class TensorImpl {
        friend class Tensor;
    public:
        explicit TensorImpl(const Shape &shape, DType dtype, const WorkspacePtr &workspace);
        explicit TensorImpl(const Shape &shape, const Shape &stride, DType dtype, const WorkspacePtr &workspace);
        explicit TensorImpl(const std::shared_ptr<TensorImpl> &base, const TensorRange &t_range);

        // Shapes (and Strides) are Framework order, meaning the last dim is changing fastest in memory
        inline const Shape &shape() const { return _shape; }
        inline unsigned rank() const { return _shape.size(); }
        // Element stride, not bytes, last value is always 1
        inline const Shape &stride() const { return _stride; }

        size_t get_pos_offset(const Shape &pos); // in elements units

    private:
        DType _dtype;
        Shape _shape;
        Shape _stride;
        // shall we add weakptr list for the reverse order?
        std::shared_ptr<TensorImpl> _view_base;

        size_t _base_offset=0;
        MemoryBlockPtr  _memory;
    };

    // Wrap it publicly so users can treat is regular object and pass it via value and create temp in stack
    class Tensor
    {
        std::shared_ptr<TensorImpl> _impl;
    public:
        explicit Tensor(const Shape &shape, DType dtype, const WorkspacePtr &workspace):
            _impl(std::make_shared<TensorImpl>(shape, dtype, workspace)) {}
        explicit Tensor(const Shape &shape, const Shape &stride, DType dtype, const WorkspacePtr &workspace):
                _impl(std::make_shared<TensorImpl>(shape, stride, dtype, workspace)) {}
        explicit Tensor(const Tensor &base, const TensorRange &t_range):
                _impl(std::make_shared<TensorImpl>(base._impl, t_range)) {}

        // access impl private members to avoid nested function calls
        inline const Shape &shape() const { return _impl->_shape; }
        inline DType dtype() const { return _impl->_dtype; }
        inline unsigned rank() const { return _impl->_shape.size(); }
        // Element stride, not bytes, last value is always 1
        inline const Shape &stride() const { return _impl->_stride; }
        inline const WorkspacePtr &workspace() const { return _impl->_memory->workspace(); }

        Tensor operator+(const Tensor &rhs) const;
    };
}
#endif //LIBTOSA_TOSA_TENSOR_H