/* Copyright 2015 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/core/framework/partial_tensor_shape.h"

#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/status_test_util.h"
#include "tensorflow/core/platform/test.h"

namespace tensorflow {
namespace {

TEST(PartialTensorShapeTest, Default) {
  // The default PartialTensorShape constructor constructs a shape
  // with unknown rank.
  const PartialTensorShape s;
  EXPECT_EQ(s.dims(), -1);
}

TEST(PartialTensorShapeTest, Concatenate) {
  const PartialTensorShape s({10, 5});
  ASSERT_EQ(2, s.dims());
  EXPECT_EQ(10, s.dim_size(0));
  EXPECT_EQ(5, s.dim_size(1));

  const auto s1 = s.Concatenate(s);
  ASSERT_EQ(4, s1.dims());
  EXPECT_EQ(10, s1.dim_size(0));
  EXPECT_EQ(5, s1.dim_size(1));
  EXPECT_EQ(10, s1.dim_size(2));
  EXPECT_EQ(5, s1.dim_size(3));

  const auto s2 = s.Concatenate(-1);
  const auto s3 = s2.Concatenate(0);
  ASSERT_EQ(3, s2.dims());
  ASSERT_EQ(4, s3.dims());
  EXPECT_EQ(10, s2.dim_size(0));
  EXPECT_EQ(10, s3.dim_size(0));
  EXPECT_EQ(5, s2.dim_size(1));
  EXPECT_EQ(5, s3.dim_size(1));
  EXPECT_EQ(-1, s2.dim_size(2));
  EXPECT_EQ(-1, s3.dim_size(2));
  EXPECT_EQ(0, s3.dim_size(3));

  const auto s4 = s.Concatenate(PartialTensorShape());
  ASSERT_EQ(-1, s4.dims());
}

TEST(PartialTensorShapeTest, InvalidShapeProto) {
  TensorShapeProto proto;
  EXPECT_TRUE(PartialTensorShape::IsValid(proto));

  proto.add_dim()->set_size(357);
  proto.add_dim()->set_size(982);
  EXPECT_TRUE(PartialTensorShape::IsValid(proto));

  proto.Clear();
  proto.add_dim()->set_size(0);
  proto.add_dim()->set_size(-1);
  EXPECT_TRUE(PartialTensorShape::IsValid(proto));

  proto.Clear();
  proto.set_unknown_rank(true);
  EXPECT_TRUE(PartialTensorShape::IsValid(proto));

  proto.add_dim()->set_size(1);
  EXPECT_FALSE(PartialTensorShape::IsValid(proto));

  proto.Clear();
  proto.add_dim()->set_size(-2);
  EXPECT_FALSE(PartialTensorShape::IsValid(proto));
}

TEST(PartialTensorShapeTest, PartialShapeFullyDefined) {
  const PartialTensorShape a({-1, 0, 1});
  const PartialTensorShape b({1, 0, 1});
  const PartialTensorShape c({-1, -1, 1});
  const PartialTensorShape d({1, 0});
  const PartialTensorShape e({});
  const PartialTensorShape f;
  EXPECT_FALSE(a.IsFullyDefined());
  EXPECT_FALSE(c.IsFullyDefined());
  EXPECT_TRUE(b.IsFullyDefined());
  EXPECT_TRUE(d.IsFullyDefined());
  EXPECT_TRUE(e.IsFullyDefined());
  EXPECT_FALSE(f.IsFullyDefined());
}

TEST(PartialTensorShapeTest, ToTensorShape) {
  const PartialTensorShape a({});
  const PartialTensorShape b({1, 0});
  const PartialTensorShape c({-1, 0});
  const PartialTensorShape d;
  TensorShape full;
  EXPECT_TRUE(a.AsTensorShape(&full));
  EXPECT_EQ(full.dims(), 0);
  EXPECT_TRUE(b.AsTensorShape(&full));
  EXPECT_EQ(full.dims(), 2);
  EXPECT_EQ(full.dim_size(0), 1);
  EXPECT_EQ(full.dim_size(1), 0);
  EXPECT_FALSE(c.AsTensorShape(&full));
  EXPECT_FALSE(d.AsTensorShape(&full));
}


TEST(PartialTensorShapeTest, PartialShapeCompatibleWith) {
  const PartialTensorShape a({-1, 0, 1});
  const PartialTensorShape b({1, 0, 1});
  const PartialTensorShape c({-1, -1, 1});
  const PartialTensorShape d({1, 0});
  const PartialTensorShape e({-1, 0, 2});
  const PartialTensorShape f({});
  const PartialTensorShape g;

  EXPECT_TRUE(f.IsCompatibleWith(f));
  EXPECT_TRUE(a.IsCompatibleWith(b));
  EXPECT_TRUE(a.IsCompatibleWith(a));
  EXPECT_TRUE(b.IsCompatibleWith(b));
  EXPECT_TRUE(a.IsCompatibleWith(c));
  EXPECT_TRUE(b.IsCompatibleWith(c));
  EXPECT_FALSE(a.IsCompatibleWith(d));
  EXPECT_FALSE(b.IsCompatibleWith(d));
  EXPECT_FALSE(c.IsCompatibleWith(d));
  EXPECT_FALSE(a.IsCompatibleWith(e));
  EXPECT_FALSE(b.IsCompatibleWith(e));
  EXPECT_FALSE(c.IsCompatibleWith(e));
  EXPECT_FALSE(a.IsCompatibleWith(f));
  EXPECT_FALSE(b.IsCompatibleWith(f));
  EXPECT_FALSE(c.IsCompatibleWith(f));
  EXPECT_TRUE(a.IsCompatibleWith(g));
  EXPECT_TRUE(g.IsCompatibleWith(a));
  EXPECT_TRUE(g.IsCompatibleWith(g));
}

TEST(PartialTensorShapeTest, ShapeCompatibleWith) {
  const PartialTensorShape a({-1, 0, 1});
  const PartialTensorShape unknown;
  TensorShape b({0, 1});
  TensorShape c({0, 0, 1});
  TensorShape d({1, 0, 1});
  TensorShape e({1, 1, 1});

  EXPECT_FALSE(a.IsCompatibleWith(b));
  EXPECT_TRUE(a.IsCompatibleWith(c));
  EXPECT_TRUE(a.IsCompatibleWith(d));
  EXPECT_FALSE(a.IsCompatibleWith(e));

  EXPECT_TRUE(unknown.IsCompatibleWith(b));
  EXPECT_TRUE(unknown.IsCompatibleWith(c));
  EXPECT_TRUE(unknown.IsCompatibleWith(d));
  EXPECT_TRUE(unknown.IsCompatibleWith(e));
}

TEST(PartialTensorShapeTest, PartialShapeMergeWith) {
  const PartialTensorShape a({-1, 0, 1});
  const PartialTensorShape b({1, 0, 1});
  const PartialTensorShape c({-1, -1, 1});
  const PartialTensorShape d({1, 0});
  const PartialTensorShape e({-1, 0, 2});
  const PartialTensorShape f({});
  const PartialTensorShape g;

  PartialTensorShape test;
  EXPECT_EQ(Status::OK(), a.MergeWith(a, &test));
  EXPECT_EQ(test.dims(), 3);
  EXPECT_EQ(test.dim_size(0), -1);
  EXPECT_EQ(test.dim_size(1), 0);
  EXPECT_EQ(test.dim_size(2), 1);

  test = PartialTensorShape();
  EXPECT_EQ(Status::OK(), a.MergeWith(b, &test));
  EXPECT_EQ(test.dims(), 3);
  EXPECT_EQ(test.dim_size(0), 1);
  EXPECT_EQ(test.dim_size(1), 0);
  EXPECT_EQ(test.dim_size(2), 1);

  test = PartialTensorShape();
  EXPECT_TRUE(errors::IsInvalidArgument(a.MergeWith(d, &test)));

  test = PartialTensorShape();
  EXPECT_EQ(Status::OK(), a.MergeWith(c, &test));
  EXPECT_EQ(test.dims(), 3);
  EXPECT_EQ(test.dim_size(0), -1);
  EXPECT_EQ(test.dim_size(1), 0);
  EXPECT_EQ(test.dim_size(2), 1);

  test = PartialTensorShape();
  EXPECT_EQ(Status::OK(), c.MergeWith(a, &test));
  EXPECT_EQ(test.dims(), 3);
  EXPECT_EQ(test.dim_size(0), -1);
  EXPECT_EQ(test.dim_size(1), 0);
  EXPECT_EQ(test.dim_size(2), 1);

  test = PartialTensorShape();
  EXPECT_EQ(Status::OK(), a.MergeWith(g, &test));
  EXPECT_EQ(test.dims(), 3);
  EXPECT_EQ(test.dim_size(0), -1);
  EXPECT_EQ(test.dim_size(1), 0);
  EXPECT_EQ(test.dim_size(2), 1);

  test = PartialTensorShape();
  EXPECT_EQ(Status::OK(), g.MergeWith(a, &test));
  EXPECT_EQ(test.dims(), 3);
  EXPECT_EQ(test.dim_size(0), -1);
  EXPECT_EQ(test.dim_size(1), 0);
  EXPECT_EQ(test.dim_size(2), 1);
}

TEST(PartialTensorShapeTest, MakePartialShapeEmpty) {
  // Empty made partial shapes should still be fully defined
#ifndef _MSC_VER
  const int64 dims[0] = {};
#else
  const int64* dims = NULL;
#endif
  PartialTensorShape shape;
  EXPECT_FALSE(shape.IsFullyDefined());
  TF_ASSERT_OK(PartialTensorShape::MakePartialShape(dims, 0, &shape));
  EXPECT_TRUE(shape.IsFullyDefined());
}

TEST(PartialTensorShapeTest, MakePartialShapeFull) {
  // Check that arrays are copied through correctly
  const int64 dims[3] = {7, -1, 2};
  PartialTensorShape shape;
  TF_ASSERT_OK(PartialTensorShape::MakePartialShape(dims, 3, &shape));
  ASSERT_EQ(shape.dims(), 3);
  for (int i = 0; i < 3; i++) {
    EXPECT_EQ(shape.dim_size(i), dims[i]);
  }
}

}  // namespace
}  // namespace tensorflow
