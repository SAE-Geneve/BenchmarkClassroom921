/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <vector>
#include <list>
#include <array>
#include <iostream>
#include <benchmark/benchmark.h>
#include <memory>
#include <cmath>
#include "intrinsics.h"


#define ENTITY_NUMBERS (1'024*1'024)

namespace sfge
{
struct Vec2f
{
    float x = 0.0f;
    float y = 0.0f;
    Vec2f(float x, float y)
        : x(x), y(y)
    {

    }
    Vec2f()
    {

    }

    float GetMagnitude()
    {
        return sqrtf(x * x + y * y);
    }

    Vec2f Normalized()
    {
        return (*this) / (*this).GetMagnitude();
    }

    bool operator==(const Vec2f& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(const Vec2f& rhs) const
    {
        return !(rhs == *this);
    }

    Vec2f operator+(const Vec2f& rhs) const
    {
        return Vec2f(x + rhs.x, y + rhs.y);
    }

    Vec2f operator-(const Vec2f& rhs) const
    {
        return Vec2f(x - rhs.x, y - rhs.y);
    }

    Vec2f operator*(float rhs) const
    {
        return Vec2f(x * rhs, y * rhs);
    }

    Vec2f operator/(float rhs) const
    {
        return (*this) * (1.0f / rhs);
    }

    Vec2f& operator+=(const Vec2f& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }
    Vec2f& operator*=(const float& rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        return *this;
    }

    Vec2f Lerp(const Vec2f& v1, const Vec2f& v2, float t)
    {
        return v1 + (v2 - v1) * t;
    }
    float Dot(const Vec2f& v1, const Vec2f& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }
    Vec2f& operator-=(const Vec2f& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }
};
}

#define SOA_VECTOR2
namespace SOA
{
#ifndef SOA_VECTOR2
class TransformSystem
{
public:
    TransformSystem()
    {
        m_PositionsX.resize(ENTITY_NUMBERS);
        m_PositionsY.resize(ENTITY_NUMBERS);
        m_ScalesX.resize(ENTITY_NUMBERS);
        m_ScalesY.resize(ENTITY_NUMBERS);
        m_EulerAngles.resize(ENTITY_NUMBERS);
        for (int i = 0; i < ENTITY_NUMBERS; i++)
        {
            m_PositionsX[i] = rand();
            m_PositionsY[i] = rand();
            m_ScalesX[i] = rand();
            m_ScalesY[i] = rand();
            m_EulerAngles[i] = rand();
        }
    }
    void Translate(sfge::Vec2f moveValue)
    {

        for (float& i : m_PositionsX)
        {
            i += moveValue.x;
        }

        for (float& i : m_PositionsY)
        {
            i += moveValue.y;
        }
    }
    void Scale(float scaleValue)
    {
        for (auto& scaleX : m_ScalesX)
        {
            scaleX *= scaleValue;
        }
        for (auto& scaleY : m_ScalesY)
        {
            scaleY *= scaleValue;
        }
    }
    void Rotate(float rotateValue)
    {
        for (int i = 0; i < ENTITY_NUMBERS; i++)
        {
            m_EulerAngles[i] += rotateValue;
        }
    }
private:
    std::vector<float> m_PositionsX;
    std::vector<float> m_PositionsY;
    std::vector<float> m_ScalesX;
    std::vector<float> m_ScalesY;
    std::vector<float> m_EulerAngles;
};
#else
class TransformSystem
{
public:
    TransformSystem()
    {
        m_Positions.resize(ENTITY_NUMBERS);
        m_Scales.resize(ENTITY_NUMBERS);
        m_EulerAngles.resize(ENTITY_NUMBERS);
        for (int i = 0; i < ENTITY_NUMBERS; i++)
        {
            m_Positions[i] = sfge::Vec2f(rand(), rand());
            m_Scales[i] = sfge::Vec2f(rand(), rand());
            m_EulerAngles[i] = rand();
        }
    }
    void Translate(sfge::Vec2f moveValue)
    {
        for (int i = 0; i < ENTITY_NUMBERS; i++)
        {
            m_Positions[i] += moveValue;
        }
    }
    void Scale(float scaleValue)
    {
        for (int i = 0; i < ENTITY_NUMBERS; i++)
        {
            m_Scales[i] *= scaleValue;
        }
    }
    void Rotate(float rotateValue)
    {
        for (int i = 0; i < ENTITY_NUMBERS; i++)
        {
            m_EulerAngles[i] += rotateValue;
        }
    }
private:
    std::vector<sfge::Vec2f> m_Positions;
    std::vector<sfge::Vec2f> m_Scales;
    std::vector<float> m_EulerAngles;
};
#endif
}
//#define AOS_LIST
namespace AOS
{
struct Transform
{
    sfge::Vec2f position;
    sfge::Vec2f scale;
    float eulerAngle;
};

class TransformSystem
{
public:
    TransformSystem()
    {
        m_Transforms.resize(ENTITY_NUMBERS);
        for (auto& transform : m_Transforms)
        {
            transform.position = sfge::Vec2f(rand(), rand());
            transform.scale = sfge::Vec2f(rand(), rand());
            transform.eulerAngle = rand();
        }
    }

    void Translate(sfge::Vec2f moveValue)
    {
        for (auto& transform : m_Transforms)
        {
            transform.position += moveValue;
        }
    }
    void Scale(float scaleValue)
    {
        for (auto& transform : m_Transforms)
        {
            transform.scale *= scaleValue;
        }
    }
    void Rotate(float rotateValue)
    {
        for (auto& transform : m_Transforms)
        {
            transform.eulerAngle += rotateValue;
        }
    }
private:
#ifdef AOS_LIST
    std::list <Transform> m_Transforms;
#else
    std::vector<Transform> m_Transforms;
#endif
};
}

namespace AOSOA
{
template<size_t N>
struct  alignas(N * sizeof(float)) NPos
{
    std::array<float, N> posX;
    std::array<float, N> posY;
    void TranslateIntrinsics(sfge::Vec2f moveValue);
};
#ifdef __SSE__
template<>
inline void NPos<4>::TranslateIntrinsics(sfge::Vec2f moveValue)
{
    auto x = _mm_load1_ps(&moveValue.x);
    auto y = _mm_load1_ps(&moveValue.y);

    auto px = _mm_load_ps(posX.data());
    auto py = _mm_load_ps(posY.data());

    px = _mm_add_ps(px, x);
    py = _mm_add_ps(py, y);

    _mm_store_ps(posX.data(), px);
    _mm_store_ps(posY.data(), py);
}
#endif
#ifdef __AVX__
template<>
inline void NPos<8>::TranslateIntrinsics(sfge::Vec2f moveValue)
{
    auto x = _mm256_broadcast_ss(&moveValue.x);
    auto y = _mm256_broadcast_ss(&moveValue.y);

    auto px = _mm256_load_ps(posX.data());
    auto py = _mm256_load_ps(posY.data());

    px = _mm256_add_ps(px, x);
    py = _mm256_add_ps(py, y);

    _mm256_store_ps(posX.data(), px);
    _mm256_store_ps(posY.data(), py);
}
#endif

template<size_t N>
struct  alignas(N * sizeof(float)) NScale
{
    std::array<float, N> scaleX;
    std::array<float, N> scaleY;
    void ScaleIntrinsics(float scaleValue);
};

#ifdef __SSE__
template <>
inline void NScale<4>::ScaleIntrinsics(float scaleValue)
{
    auto x = _mm_load1_ps(&scaleValue);

    auto px = _mm_load_ps(scaleX.data());
    auto py = _mm_load_ps(scaleY.data());

    px = _mm_mul_ps(px, x);
    py = _mm_mul_ps(py, x);

    _mm_store_ps(scaleX.data(), px);
    _mm_store_ps(scaleY.data(), py);
}
#endif
#ifdef __AVX__
template<>
inline void NScale<8>::ScaleIntrinsics(float scaleValue)
{
    auto x = _mm256_broadcast_ss(&scaleValue);

    auto px = _mm256_load_ps(scaleX.data());
    auto py = _mm256_load_ps(scaleY.data());

    px = _mm256_mul_ps(px, x);
    py = _mm256_mul_ps(py, x);

    _mm256_store_ps(scaleX.data(), px);
    _mm256_store_ps(scaleY.data(), py);
}
#endif

template <size_t N>
struct  alignas(N * sizeof(float)) NAngle
{
    std::array<float, N> eulerAngles;

    void RotateIntrinsics(float angle);

};

#ifdef __SSE__
template<>
inline void NAngle<4>::RotateIntrinsics(float angle)
{
    auto a = _mm_load1_ps(&angle);
    auto as = _mm_load_ps(eulerAngles.data());

    as = _mm_add_ps(a, as);

    _mm_store_ps(eulerAngles.data(), as);
}
#endif
#ifdef __AVX__
template<>
inline void NAngle<8>::RotateIntrinsics(float angle)
{
    auto a = _mm256_broadcast_ss(&angle);
    auto as = _mm256_load_ps(eulerAngles.data());

    as = _mm256_add_ps(a, as);

    _mm256_store_ps(eulerAngles.data(), as);
}
#endif

template<size_t N>
class TransformSystem
{
public:
    TransformSystem()
    {
        m_Positions.resize(ENTITY_NUMBERS / N);
        m_Scales.resize(ENTITY_NUMBERS / N);
        m_EulerAngles.resize(ENTITY_NUMBERS / N);
        for (int i = 0; i < ENTITY_NUMBERS / N; i++)
        {
            for (int j = 0; j < N; j++)
            {

                m_Positions[i].posX[j] = rand();
                m_Positions[i].posY[j] = rand();
                m_Scales[i].scaleX[j] = rand();
                m_Scales[i].scaleY[j] = rand();
                m_EulerAngles[i].eulerAngles[j] = rand();
            }
        }
    }
    void Translate(sfge::Vec2f moveValue)
    {

        for (auto& pos : m_Positions)
        {
            for (int j = 0; j < N; j++)
            {
                pos.posX[j] += moveValue.x;
                pos.posY[j] += moveValue.y;
            }
        }
    }
    void Scale(float scaleValue)
    {
        for (auto& scale : m_Scales)
        {
            for (int j = 0; j < N; j++)
            {
                scale.scaleX[j] *= scaleValue;
                scale.scaleY[j] *= scaleValue;
            }
        }
    }
    void Rotate(float rotateValue)
    {
        for (auto& angle : m_EulerAngles)
        {
            for (int j = 0; j < N; j++)
            {
                angle.eulerAngles[j] += rotateValue;
            }
        }
    }

    void TranslateInstrinsics(sfge::Vec2f moveValue)
    {
        for (auto& pos : m_Positions)
        {
            pos.TranslateIntrinsics(moveValue);
        }
    }
    void ScaleIntrinsics(float scaleValue)
    {
        for (auto& scale : m_Scales)
        {
            scale.ScaleIntrinsics(scaleValue);
        }
    }
    void RotateIntrinsics(float rotateValue)
    {
        for (auto& angle : m_EulerAngles)
        {
            angle.RotateIntrinsics(rotateValue);
        }
    }
private:
    std::vector<NPos<N>> m_Positions;
    std::vector<NScale<N>> m_Scales;
    std::vector<NAngle<N>> m_EulerAngles;
};
}

static void BM_AOS(benchmark::State& state)
{
    auto transformSystem = std::make_unique<AOS::TransformSystem>();
    for (auto _ : state)
    {
        transformSystem->Translate(sfge::Vec2f(22.0f, -4.0f));
        transformSystem->Scale(3.0f);
        transformSystem->Rotate(45.0f);
    }
}
// Register the function as a benchmark
BENCHMARK(BM_AOS);

static void BM_SOA(benchmark::State& state)
{
    auto transformSystem = std::make_unique<SOA::TransformSystem>();
    for (auto _ : state)
    {
        transformSystem->Translate(sfge::Vec2f(22.0f, -4.0f));
        transformSystem->Scale(3.0f);
        transformSystem->Rotate(45.0f);
    }
}

BENCHMARK(BM_SOA);

static void BM_AOSOA4(benchmark::State& state)
{
    auto transformSystem = std::make_unique<AOSOA::TransformSystem<4>>();
    for (auto _ : state)
    {
        transformSystem->Translate(sfge::Vec2f(22.0f, -4.0f));
        transformSystem->Scale(3.0f);
        transformSystem->Rotate(45.0f);
    }
}

BENCHMARK(BM_AOSOA4);

#ifdef __SSE__
static void BM_AOSOA4Intrinsics(benchmark::State& state)
{
    auto transformSystem = std::make_unique<AOSOA::TransformSystem<4>>();
    for (auto _ : state)
    {
        transformSystem->TranslateInstrinsics(sfge::Vec2f(22.0f, -4.0f));
        transformSystem->ScaleIntrinsics(3.0f);
        transformSystem->RotateIntrinsics(45.0f);
    }
}

BENCHMARK(BM_AOSOA4Intrinsics);

#endif
static void BM_AOSOA8(benchmark::State& state)
{
    auto transformSystem = std::make_unique<AOSOA::TransformSystem<8>>();
    for (auto _ : state)
    {
        transformSystem->Translate(sfge::Vec2f(22.0f, -4.0f));
        transformSystem->Scale(3.0f);
        transformSystem->Rotate(45.0f);
    }
}

BENCHMARK(BM_AOSOA8);

#ifdef __SSE__
static void BM_AOSOA8Intrinsics(benchmark::State& state)
{
    auto transformSystem = std::make_unique<AOSOA::TransformSystem<8>>();
    for (auto _ : state)
    {
        transformSystem->TranslateInstrinsics(sfge::Vec2f(22.0f, -4.0f));
        transformSystem->ScaleIntrinsics(3.0f);
        transformSystem->RotateIntrinsics(45.0f);
    }
}

BENCHMARK(BM_AOSOA8Intrinsics);

#endif

BENCHMARK_MAIN();