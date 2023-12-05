#pragma once

#include "pecs.hpp"
#include "player.hpp"
#include "config.hpp"
#include "render.hpp"

inline void startupSystem(pecs::Commands& command) {
    command.Spawn<Player, RenderInfo>(
        Player{"w6rsty"},
        RenderInfo{
            .model = &ModelTransform,
            .drawable = std::make_shared<prism::Model>(modelNanosuitPath, true),
            .shaderType = prism::ShaderType::HAS_TEX,
        }
    );
    command.Spawn<RenderInfo>(
        RenderInfo{
            .model = &groundModelTransform,
            .drawable = std::make_shared<prism::Plane>(),
            .shaderType = prism::ShaderType::WITH_TEX
        }
    );
    command.SetResource(std::shared_ptr<Camera>(camera));
    command.SetResource(std::make_shared<prism::ShaderManager>(createShaderInfo));
    command.SetResource(Ticker{});
    command.SetResource(std::make_shared<prism::SkyBox>());
    command.SetResource(std::make_shared<SkyBoxTexture>(faces, false));

    int bufw = 512, bufh = 512;
    unsigned char* buffer = new unsigned char[bufw * bufh * 3];
    createCheckboardTexture(buffer, bufw, bufh, 16); 
    command.SetResource(std::make_shared<Texture>(bufw, bufh, buffer));
}