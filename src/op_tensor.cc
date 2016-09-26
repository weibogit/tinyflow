// Copyright (c) 2016 by Contributors
// implementation of common tensor operators
#include <tinyflow/base.h>
#include <dmlc/parameter.h>
#include <utility>
#include "./op_util.h"

namespace tinyflow {

// shape parameter for zeros, ones
struct ZeroParam : public dmlc::Parameter<ZeroParam> {
  TShape shape;
  int dtype;
  DMLC_DECLARE_PARAMETER(ZeroParam) {
    DMLC_DECLARE_FIELD(shape).set_default(TShape());
    DMLC_DECLARE_FIELD(dtype).set_default(kFloat32);
  }
};
DMLC_REGISTER_PARAMETER(ZeroParam);

// shape given the ZeroParam
using namespace nnvm;

inline bool ZeroShape(const NodeAttrs& attrs,
                       std::vector<TShape> *ishape,
                       std::vector<TShape> *oshape) {
  const TShape& ts = dmlc::get<ZeroParam>(attrs.parsed).shape;
  if (ts.ndim() != 0) {
    SHAPE_ASSIGN(oshape->at(0), ts);
    return true;
  } else {
    return false;
  }
}

inline bool ZeroType(const NodeAttrs& attrs,
                     std::vector<int> *iattr,
                     std::vector<int> *oattr) {
  int dtype = dmlc::get<ZeroParam>(attrs.parsed).dtype;
  DTYPE_ASSIGN(oattr->at(0), dtype);
  return true;
}

NNVM_REGISTER_OP(zeros)
.describe("zeros")
.set_num_inputs(0)
.set_attr_parser(ParamParser<ZeroParam>)
.set_attr<FInferShape>("FInferShape", ZeroShape)
.set_attr<FInferType>("FInferType", ZeroType)
.set_attr<FLuaCompute>(
    "FLuaCompute", R"(
function(x, y)
  y[1]:fill(0)
end
)");


NNVM_REGISTER_OP(ones)
.describe("ones")
.set_num_inputs(0)
.set_attr_parser(ParamParser<ZeroParam>)
.set_attr<FInferShape>("FInferShape", ZeroShape)
.set_attr<FInferType>("FInferType", ZeroType)
.set_attr<FLuaCompute>(
    "FLuaCompute", R"(
function(x, y)
  y[1]:fill(1)
end
)");


NNVM_REGISTER_OP(ones_like)
.describe("ones_like")
.set_num_inputs(1)
.set_attr<FInferShape>("FInferShape", SameShape)
.set_attr<FLuaCompute>(
    "FLuaCompute", R"(
function(x, y)
  y[1]:fill(1)
end
)");


NNVM_REGISTER_OP(__add_symbol__)
.describe("add two data together")
.set_num_inputs(2)
.set_attr<FInferShape>("FInferShape", SameShape)
.set_attr<FInplaceOption>("FInplaceOption", InplaceIn0Out0)
.set_attr<FGradient>(
    "FGradient", [](const NodePtr& n,
                    const std::vector<NodeEntry>& ograds){
      return std::vector<NodeEntry>{ograds[0], ograds[0]};
    })
.set_attr<FLuaCompute>(
    "FLuaCompute", R"(
function(x, y)
  torch.add(y[1], x[1], x[2])
end
)");


NNVM_REGISTER_OP(__mul_symbol__)
.describe("add two data together")
.set_num_inputs(2)
.set_attr<FInferShape>("FInferShape", SameShape)
.set_attr<FInplaceOption>("FInplaceOption", InplaceIn0Out0)
.set_attr<FLuaCompute>(
    "FLuaCompute", R"(
function(x, y)
  torch.cmul(y[1], x[1], x[2])
end
)");


NNVM_REGISTER_OP(__mul_scalar__)
.describe("Multiply symbol with scalar")
.set_num_inputs(1)
.set_attr<FInferShape>("FInferShape", SameShape)
.set_attr<FInplaceOption>("FInplaceOption", InplaceIn0Out0);


NNVM_REGISTER_OP(matmul)
.describe("Matrix multiplication")
.set_num_inputs(2);


NNVM_REGISTER_OP(reduce_mean)
.describe("reduce mean")
.set_num_inputs(1);


NNVM_REGISTER_OP(reduce_sum)
.describe("reduce mean")
.set_num_inputs(1);


NNVM_REGISTER_OP(log)
.describe("take elemtnwise logarithm")
.set_num_inputs(1);
}  // namespace tinyflow