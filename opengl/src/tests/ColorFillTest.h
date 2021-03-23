#pragma once
#include "Test.h"

namespace test
{
    class ColorFillTest : public Test
    {
    private:
        float m_ClearColor[4];

    public:
        ColorFillTest();
        virtual ~ColorFillTest();
        void OnRender() override;
        void OnImGuiRender() override;
    };
}
