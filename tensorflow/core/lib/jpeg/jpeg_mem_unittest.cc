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

#include "tensorflow/core/lib/jpeg/jpeg_mem.h"

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <memory>

#include "tensorflow/core/lib/jpeg/jpeg_handle.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/test.h"
#include "tensorflow/core/platform/types.h"

#include "tensorflow/core/lib/core/casts.h"

namespace tensorflow {
namespace jpeg {
namespace {

const char kTestData[] = "../tensorflow/core/lib/jpeg/testdata/";

int ComputeSumAbsoluteDifference(const uint8* a, const uint8* b, int width,
                                 int height, int a_stride, int b_stride) {
  int totalerr = 0;
  for (int i = 0; i < height; i++) {
    const uint8* const pa = a + i * a_stride;
    const uint8* const pb = b + i * b_stride;
    for (int j = 0; j < 3 * width; j++) {
      totalerr += abs(static_cast<int>(pa[j]) - static_cast<int>(pb[j]));
    }
  }
  return totalerr;
}

// Reads the contents of the file into output
void ReadFileToStringOrDie(Env* env, const string& filename, string* output) {
  TF_CHECK_OK(ReadFileToString(env, filename, output));
}

void TestJPEG(Env* env, const string& jpegfile) {
  // Read the data from the jpeg file into memory
  string jpeg;
  ReadFileToStringOrDie(Env::Default(), jpegfile, &jpeg);
  const int fsize = jpeg.size();
  const uint8* const temp = bit_cast<const uint8*>(jpeg.data());

  // Try partial decoding (half of the data)
  int w, h, c;
  std::unique_ptr<uint8[]> imgdata;

  UncompressFlags flags;
  flags.components = 3;

  // Set min_acceptable_fraction to something insufficient
  flags.min_acceptable_fraction = 0.8;
  imgdata.reset(Uncompress(temp, fsize / 2, flags, &w, &h, &c, NULL));
  CHECK(imgdata.get() == NULL);

  // Now, use a value that makes fsize/2 be enough for a black-filling
  flags.min_acceptable_fraction = 0.01;
  imgdata.reset(Uncompress(temp, fsize / 2, flags, &w, &h, &c, NULL));
  CHECK(imgdata.get() != NULL);

  // Finally, uncompress the whole data
  flags.min_acceptable_fraction = 1.0;
  imgdata.reset(Uncompress(temp, fsize, flags, &w, &h, &c, NULL));
  CHECK(imgdata.get() != NULL);
}

TEST(JpegMemTest, Jpeg) {
  Env* env = Env::Default();
  const string data_path = kTestData;

  // Name of a valid jpeg file on the disk
  TestJPEG(env, data_path + "jpeg_merge_test1.jpg");

  // Exercise CMYK machinery as well
  TestJPEG(env, data_path + "jpeg_merge_test1_cmyk.jpg");
}

TEST(JpegMemTest, Jpeg2) {
  // create known data, for size in_w x in_h
  const int in_w = 256;
  const int in_h = 256;
  const int stride1 = 3 * in_w;
  const std::unique_ptr<uint8[]> refdata1(new uint8[stride1 * in_h]);
  for (int i = 0; i < in_h; i++) {
    for (int j = 0; j < in_w; j++) {
      const int offset = i * stride1 + 3 * j;
      refdata1[offset + 0] = i;
      refdata1[offset + 1] = j;
      refdata1[offset + 2] = static_cast<uint8>((i + j) >> 1);
    }
  }

  // duplicate with weird input stride
  const int stride2 = 3 * 357;
  const std::unique_ptr<uint8[]> refdata2(new uint8[stride2 * in_h]);
  for (int i = 0; i < in_h; i++) {
    memcpy(&refdata2[i * stride2], &refdata1[i * stride1], 3 * in_w);
  }

  // Test compression
  string cpdata1, cpdata2;
  {
    const string kXMP = "XMP_TEST_123";

    // Compress it to JPEG
    CompressFlags flags;
    flags.format = FORMAT_RGB;
    flags.quality = 97;
    flags.xmp_metadata = kXMP;
    cpdata1 = Compress(refdata1.get(), in_w, in_h, flags);
    flags.stride = stride2;
    cpdata2 = Compress(refdata2.get(), in_w, in_h, flags);
    // Different input stride shouldn't change the output
    CHECK_EQ(cpdata1, cpdata2);

    // Verify valid XMP.
    CHECK_NE(string::npos, cpdata1.find(kXMP));

    // Test the other API, where a storage string is supplied
    string cptest;
    flags.stride = 0;
    Compress(refdata1.get(), in_w, in_h, flags, &cptest);
    CHECK_EQ(cptest, cpdata1);
    flags.stride = stride2;
    Compress(refdata2.get(), in_w, in_h, flags, &cptest);
    CHECK_EQ(cptest, cpdata2);
  }

  // Uncompress twice: once with 3 components and once with autodetect
  std::unique_ptr<uint8[]> imgdata1;
  for (const int components : {0, 3}) {
    // Uncompress it
    UncompressFlags flags;
    flags.components = components;
    int w, h, c;
    imgdata1.reset(
        Uncompress(cpdata1.c_str(), cpdata1.length(), flags, &w, &h, &c, NULL));

    // Check obvious formatting stuff
    CHECK_EQ(w, in_w);
    CHECK_EQ(h, in_h);
    CHECK_EQ(c, 3);
    CHECK(imgdata1.get());

    // Compare the two images
    const int totalerr = ComputeSumAbsoluteDifference(
        imgdata1.get(), refdata1.get(), in_w, in_h, stride1, stride1);
    CHECK_LE(totalerr, 85000);
  }

  // check the second image too. Should be bitwise identical to the first.
  // uncompress using a weird stride
  {
    UncompressFlags flags;
    flags.stride = 3 * 411;
    const std::unique_ptr<uint8[]> imgdata2(new uint8[flags.stride * in_h]);
#ifndef _MSC_VER
    CHECK(imgdata2.get() == Uncompress(cpdata2.c_str(), cpdata2.length(), flags,
                                       nullptr /* nwarn */,
                                       [&imgdata2](int w, int h, int c) {
                                         CHECK_EQ(w, in_w);
                                         CHECK_EQ(h, in_h);
                                         CHECK_EQ(c, 3);
                                         return imgdata2.get();
                                       }));
#else
    CHECK(imgdata2.get() == Uncompress(cpdata2.c_str(), cpdata2.length(), flags,
                                       nullptr /* nwarn */,
                                       [&imgdata2, &in_w, &in_h](int w, int h, int c) {
                                         CHECK_EQ(w, in_w);
                                         CHECK_EQ(h, in_h);
                                         CHECK_EQ(c, 3);
                                         return imgdata2.get();
                                       }));
#endif
    const int totalerr = ComputeSumAbsoluteDifference(
        imgdata1.get(), imgdata2.get(), in_w, in_h, stride1, flags.stride);
    CHECK_EQ(totalerr, 0);
  }
}

// Takes JPEG data and reads its headers to determine whether or not the JPEG
// was chroma downsampled.
bool IsChromaDownsampled(const string& jpegdata) {
  // Initialize libjpeg structures to have a memory source
  // Modify the usual jpeg error manager to catch fatal errors.
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  jmp_buf jpeg_jmpbuf;
  cinfo.err = jpeg_std_error(&jerr);
  cinfo.client_data = &jpeg_jmpbuf;
  jerr.error_exit = CatchError;
  if (setjmp(jpeg_jmpbuf)) return false;

  // set up, read header, set image parameters, save size
  jpeg_create_decompress(&cinfo);
  SetSrc(&cinfo, jpegdata.c_str(), jpegdata.size(), false);

  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);  // required to transfer image size to cinfo
  const int components = cinfo.output_components;
  if (components == 1) return false;

  // Check validity
  CHECK_EQ(3, components);
  CHECK_EQ(cinfo.comp_info[1].h_samp_factor, cinfo.comp_info[2].h_samp_factor)
      << "The h sampling factors should be the same.";
  CHECK_EQ(cinfo.comp_info[1].v_samp_factor, cinfo.comp_info[2].v_samp_factor)
      << "The v sampling factors should be the same.";
  for (int i = 0; i < components; ++i) {
    CHECK_GT(cinfo.comp_info[i].h_samp_factor, 0) << "Invalid sampling factor.";
    CHECK_EQ(cinfo.comp_info[i].h_samp_factor, cinfo.comp_info[i].v_samp_factor)
        << "The sampling factor should be the same in both directions.";
  }

  // We're downsampled if we use fewer samples for color than for brightness.
  // Do this before deallocating cinfo.
  const bool downsampled =
      cinfo.comp_info[1].h_samp_factor < cinfo.comp_info[0].h_samp_factor;

  jpeg_destroy_decompress(&cinfo);
  return downsampled;
}

TEST(JpegMemTest, ChromaDownsampling) {
  // Read the data from a test jpeg file into memory
  const string jpegfile = string(kTestData) + "jpeg_merge_test1.jpg";
  string jpeg;
  ReadFileToStringOrDie(Env::Default(), jpegfile, &jpeg);

  // Verify that compressing the JPEG with chroma downsampling works.
  //
  // First, uncompress the JPEG.
  UncompressFlags unflags;
  unflags.components = 3;
  int w, h, c;
  int64 num_warnings;
  std::unique_ptr<uint8[]> uncompressed(Uncompress(
      jpeg.c_str(), jpeg.size(), unflags, &w, &h, &c, &num_warnings));
  CHECK(uncompressed.get() != NULL);
  CHECK_EQ(num_warnings, 0);

  // Recompress the JPEG with and without chroma downsampling
  for (const bool downsample : {false, true}) {
    CompressFlags flags;
    flags.format = FORMAT_RGB;
    flags.quality = 85;
    flags.chroma_downsampling = downsample;
    string recompressed;
    Compress(uncompressed.get(), w, h, flags, &recompressed);
    CHECK(!recompressed.empty());
    CHECK_EQ(IsChromaDownsampled(recompressed), downsample);
  }
}

void TestBadJPEG(Env* env, const string& bad_jpeg_file, int expected_width,
                 int expected_height, const string& reference_RGB_file,
                 const bool try_recover_truncated_jpeg) {
  string jpeg;
  ReadFileToStringOrDie(env, bad_jpeg_file, &jpeg);

  UncompressFlags flags;
  flags.components = 3;
  flags.try_recover_truncated_jpeg = try_recover_truncated_jpeg;

  int width, height, components;
  std::unique_ptr<uint8[]> imgdata;
  imgdata.reset(Uncompress(jpeg.c_str(), jpeg.size(), flags, &width, &height,
                           &components, NULL));
  if (expected_width > 0) {  // we expect the file to decode into 'something'
    CHECK_EQ(width, expected_width);
    CHECK_EQ(height, expected_height);
    CHECK_EQ(components, 3);
    CHECK(imgdata.get());
    if (!reference_RGB_file.empty()) {
      string ref;
      ReadFileToStringOrDie(env, reference_RGB_file, &ref);
      CHECK(!memcmp(ref.data(), imgdata.get(), ref.size()));
    }
  } else {  // no decodable
    CHECK(!imgdata.get()) << "file:" << bad_jpeg_file;
  }
}

TEST(JpegMemTest, BadJpeg) {
  Env* env = Env::Default();
  const string data_path = kTestData;

  // Test corrupt file
  TestBadJPEG(env, data_path + "bad_huffman.jpg", 1024, 768, "", false);
  TestBadJPEG(env, data_path + "corrupt.jpg", 0 /*120*/, 90, "", false);

  // Truncated files, undecodable because of missing lines:
  TestBadJPEG(env, data_path + "corrupt34_2.jpg", 0, 3300, "", false);
  TestBadJPEG(env, data_path + "corrupt34_3.jpg", 0, 3300, "", false);
  TestBadJPEG(env, data_path + "corrupt34_4.jpg", 0, 3300, "", false);

  // Try in 'recover' mode now:
  TestBadJPEG(env, data_path + "corrupt34_2.jpg", 2544, 3300, "", true);
  TestBadJPEG(env, data_path + "corrupt34_3.jpg", 2544, 3300, "", true);
  TestBadJPEG(env, data_path + "corrupt34_4.jpg", 2544, 3300, "", true);
}

}  // namespace
}  // namespace jpeg
}  // namespace tensorflow
