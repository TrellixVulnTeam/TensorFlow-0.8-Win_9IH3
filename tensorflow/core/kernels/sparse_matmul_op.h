/* Copyright 2016 Google Inc. All Rights Reserved.

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

#ifndef TENSORFLOW_KERNELS_SPARSE_MATMUL_OP_H_
#define TENSORFLOW_KERNELS_SPARSE_MATMUL_OP_H_

#include "third_party/eigen3/Eigen/Core"
#include "tensorflow/core/platform/types.h"

#ifdef _MSC_VER
#define _mm_load_pd1(x) _mm_load1_pd(x)
#endif

namespace Eigen {
namespace internal {

// Return the float representation of the bfloat16 value
// in the lower 16-bits of input
template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet pexpand_bf16_l(const Packet& from) {
  tensorflow::uint32 tmp =
      (reinterpret_cast<const tensorflow::uint32&>(from) << 16) & 0xffff0000;
  return reinterpret_cast<const float&>(tmp);
}

// Return the float representation of the bfloat16 value
// in the upper 16-bits of input
template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet pexpand_bf16_u(const Packet& from) {
  tensorflow::uint32 tmp =
      (reinterpret_cast<const tensorflow::uint32&>(from)) & 0xffff0000;
  return reinterpret_cast<const float&>(tmp);
}

template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet pinterleave4x64(const Packet& from) {
  return from;
}

template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet pbroadcast_first(const Packet& a) {
  return a;
}

template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet pbroadcast_second(const Packet& a) {
  assert(false && "Not applicable to Scalar Values");
  return a;
}

template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet pbroadcast_third(const Packet& a) {
  assert(false && "Not applicable to Scalar Values");
  return a;
}

template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet pbroadcast_fourth(const Packet& a) {
  assert(false && "Not applicable to Scalar Values");
  return a;
}

template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet pload4bf16(
    const typename unpacket_traits<Packet>::type* from) {
  assert(false && "Not applicable to Scalar Values");
  return *from;
}

template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet pload2bf16(
    const typename unpacket_traits<Packet>::type* from) {
  assert(false && "Not applicable to Scalar Values");
  return *from;
}

#ifdef EIGEN_VECTORIZE_SSE2
// For PacketSize of 4 floats the Packet is not modified
template <>
EIGEN_STRONG_INLINE Packet4f pinterleave4x64<Packet4f>(const Packet4f& from) {
  return from;
}

// Return a Packet with 4 floats loaded from 4 bfloat16 values
template <>
EIGEN_STRONG_INLINE Packet4f pload4bf16<Packet4f>(const float* from) {
  __m128i zero = _mm_setzero_si128();
  __m128i tmp = _mm_castpd_si128(_mm_load_pd1((const double*)from));
  return _mm_castsi128_ps(_mm_unpacklo_epi16(zero, tmp));
}

// Return a Packet with 2 floats loaded from 2 bfloat16 values
template <>
EIGEN_STRONG_INLINE Packet4f pload2bf16<Packet4f>(const float* from) {
  __m128i zero = _mm_setzero_si128();
  __m128i tmp = _mm_castps_si128(_mm_load_ps1(from));
  return _mm_castsi128_ps(_mm_unpacklo_epi16(zero, tmp));
}

// Return a Packet with 4 floats expanded from 4 bfloat16 values
// in the lower half of the 128-bit lane
template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet4f pexpand_bf16_l(const Packet4f& from) {
  __m128i zero = _mm_setzero_si128();
  __m128i tmp = _mm_castps_si128(from);
  return _mm_castsi128_ps(_mm_unpacklo_epi16(zero, tmp));
}

// Return a Packet with 4 floats expanded from 4 bfloat16 values
// in the upper half of the 128-bit lane
template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet4f pexpand_bf16_u(const Packet4f& from) {
  __m128i zero = _mm_setzero_si128();
  __m128i tmp = _mm_castps_si128(from);
  return _mm_castsi128_ps(_mm_unpackhi_epi16(zero, tmp));
}

// Return a packet with the first value of the input Packet replicated
template <>
EIGEN_STRONG_INLINE Packet4f pbroadcast_first<Packet4f>(const Packet4f& a) {
  return _mm_set1_ps(pfirst<Packet4f>(a));
}

// Return a packet with the second value of the input Packet replicated
template <>
EIGEN_STRONG_INLINE Packet4f pbroadcast_second<Packet4f>(const Packet4f& a) {
  return _mm_set1_ps(_mm_cvtss_f32(_mm_shuffle_ps(a, a, 1)));
}

// Return a packet with the third value of the input Packet replicated
template <>
EIGEN_STRONG_INLINE Packet4f pbroadcast_third<Packet4f>(const Packet4f& a) {
  return _mm_set1_ps(_mm_cvtss_f32(_mm_shuffle_ps(a, a, 2)));
}

// Return a packet with the fourth value of the input Packet replicated
template <>
EIGEN_STRONG_INLINE Packet4f pbroadcast_fourth<Packet4f>(const Packet4f& a) {
  return _mm_set1_ps(_mm_cvtss_f32(_mm_shuffle_ps(a, a, 3)));
}

#endif

#ifdef EIGEN_VECTORIZE_AVX
// For a Packet of Size 8 floats(256-bits), swap the 2nd and 3rd quadwords
template <>
EIGEN_STRONG_INLINE Packet8f pinterleave4x64<Packet8f>(const Packet8f& from) {
#ifdef EIGEN_VECTORIZE_AVX2
  return _mm256_castsi256_ps(_mm256_permute4x64_epi64(_mm256_castps_si256(from),
                                                      _MM_SHUFFLE(3, 1, 2, 0)));
#else
  __int64_t tmp1 = _mm256_extract_epi64(_mm256_castps_si256(from), 1);
  __int64_t tmp2 = _mm256_extract_epi64(_mm256_castps_si256(from), 2);
  __m256i tmp3 = _mm256_insert_epi64(_mm256_castps_si256(from), tmp1, 2);
  return _mm256_castsi256_ps(_mm256_insert_epi64(tmp3, tmp2, 1));
#endif
}
// Return a Packet with 4 floats loaded from 4 bfloat16 values
template <>
EIGEN_STRONG_INLINE Packet8f pload4bf16<Packet8f>(const float* from) {
  __m128i zero = _mm_setzero_si128();
  __m128i tmp = _mm_castpd_si128(_mm_load_pd1((const double*)from));
  return _mm256_castps128_ps256(
      _mm_castsi128_ps(_mm_unpacklo_epi16(zero, tmp)));
}
// Return a Packet with 2 floats loaded from 2 bfloat16 values
template <>
EIGEN_STRONG_INLINE Packet8f pload2bf16<Packet8f>(const float* from) {
  __m128i zero = _mm_setzero_si128();
  __m128i tmp = _mm_castps_si128(_mm_load_ps1(from));
  return _mm256_castps128_ps256(
      _mm_castsi128_ps(_mm_unpacklo_epi16(zero, tmp)));
}

// For each 128-bit lane convert 4 bfloat to 4 float values from the lower half
// of the 128-bit lane
template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet8f pexpand_bf16_l(const Packet8f& from) {
#ifdef EIGEN_VECTORIZE_AVX2
  __m256i zero = _mm256_setzero_si256();
  __m256i tmp = _mm256_castps_si256(from);
  return _mm256_castsi256_ps(_mm256_unpacklo_epi16(zero, tmp));
#else
  __m128i zero = _mm_setzero_si128();
  __m128i low = _mm_castps_si128(_mm256_extractf128_ps(from, 0));
  __m128i res_l = _mm_unpacklo_epi16(zero, low);
  __m128i high = _mm_castps_si128(_mm256_extractf128_ps(from, 1));
  __m128i res_h = _mm_unpacklo_epi16(zero, high);
  __m256 res = _mm256_castps128_ps256(_mm_castsi128_ps(res_l));
  res = _mm256_insertf128_ps(res, _mm_castsi128_ps(res_h), 1);
  return res;
#endif
}

// For each 128-bit lane convert 4 bfloat to 4 float values from the upper half
// of the 128-bit lane
template <typename Packet>
EIGEN_DEVICE_FUNC inline Packet8f pexpand_bf16_u(const Packet8f& from) {
#ifdef EIGEN_VECTORIZE_AVX2
  __m256i zero = _mm256_setzero_si256();
  __m256i tmp = _mm256_castps_si256(from);
  return _mm256_castsi256_ps(_mm256_unpackhi_epi16(zero, tmp));
#else
  __m128i zero = _mm_setzero_si128();
  __m128i low = _mm_castps_si128(_mm256_extractf128_ps(from, 0));
  __m128i res_l = _mm_unpackhi_epi16(zero, low);
  __m128i high = _mm_castps_si128(_mm256_extractf128_ps(from, 1));
  __m128i res_h = _mm_unpackhi_epi16(zero, high);
  __m256 res = _mm256_castps128_ps256(_mm_castsi128_ps(res_l));
  res = _mm256_insertf128_ps(res, _mm_castsi128_ps(res_h), 1);
  return res;
#endif
}

// Return a packet with the first value of the input Packet replicated
template <>
EIGEN_STRONG_INLINE Packet8f pbroadcast_first<Packet8f>(const Packet8f& a) {
  return _mm256_set1_ps(pfirst<Packet8f>(a));
}

// Return a packet with the second value of the input Packet replicated
template <>
EIGEN_STRONG_INLINE Packet8f pbroadcast_second<Packet8f>(const Packet8f& a) {
  return _mm256_set1_ps(
      _mm_cvtss_f32(_mm256_castps256_ps128(_mm256_permute_ps(a, 1))));
}

// Return a packet with the third value of the input Packet replicated
template <>
EIGEN_STRONG_INLINE Packet8f pbroadcast_third<Packet8f>(const Packet8f& a) {
  return _mm256_set1_ps(
      _mm_cvtss_f32(_mm256_castps256_ps128(_mm256_permute_ps(a, 2))));
}

// Return a packet with the fourth value of the input Packet replicated
template <>
EIGEN_STRONG_INLINE Packet8f pbroadcast_fourth<Packet8f>(const Packet8f& a) {
  return _mm256_set1_ps(
      _mm_cvtss_f32(_mm256_castps256_ps128(_mm256_permute_ps(a, 3))));
}

#endif
}  // namespace internal
}  // namespace Eigen
#endif
