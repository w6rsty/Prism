#pragma once

#include "geo/geometry.hpp"
#include "pecs.hpp"
#include "player.hpp"
#include "config.hpp"
#include "render.hpp"
#include "render/texture.hpp"

inline void startupSystem(pecs::Commands& command) {
    command.Spawn<Player, RenderInfo>(
        Player{"w6rsty"},
        RenderInfo{
            .model = &ModelTransform,
            .drawable = std::make_shared<prism::Sphere>(),
            .shaderType = prism::ShaderType::WITH_TEX,
            .texIndex = 1,
        }
    );
    command.Spawn<RenderInfo>(
        RenderInfo{
            .model = &groundModelTransform,
            .drawable = std::make_shared<prism::Plane>(5.0), // floor
            .shaderType = prism::ShaderType::WITH_TEX,
        }
    );
    command.Spawn<RenderInfo>(
        RenderInfo{
            .model = &backWallTransform,
            .drawable = std::make_shared<prism::Plane>(), // floor
            .shaderType = prism::ShaderType::WITH_TEX,
            .texIndex = 1
        }
    );

    command.SetResource(std::shared_ptr<Camera>(camera));
    command.SetResource(std::make_shared<prism::ShaderManager>(createShaderInfo));
    command.SetResource(Ticker{});
    command.SetResource(std::make_shared<prism::SkyBox>());
    command.SetResource(std::make_shared<SkyBoxTexture>(faces, false));

    TextureBase.push_back(std::make_shared<Texture>("D:/home/Prism/resources/img/gray_rocks_diff.jpg")); // texIndex 0
    int bufw = 512, bufh = 512;
    unsigned char* buffer = new unsigned char[bufw * bufh * 3];
    createBerlinNoiseTexture(buffer, bufw, bufh, 8);
    TextureBase.push_back(std::make_shared<Texture>(bufw, bufh, buffer)); // texIndex 1
    command.SetResource(&TextureBase);
}