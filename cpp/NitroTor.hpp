#pragma once

#include "HybridNitroTorSpec.hpp"

namespace margelo::nitro::nitrotor
{
    class NitroTor : public HybridNitroTorSpec
    {

    public:
        NitroTor() : HybridObject(TAG) {}

    public:
        double multiply(double a, double b) override;
    };
}
