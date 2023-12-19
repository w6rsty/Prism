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
            .drawable = std::make_shared<prism::Plane>(0.5), // floor
            .shaderType = prism::ShaderType::WITH_TEX,
            .texIndex = 0,
        }
    );
    command.Spawn<RenderInfo>(
        RenderInfo{
            .model = &lightCubeTransform,
            .drawable = std::make_shared<prism::Cube>(), // light cube
            .shaderType = prism::ShaderType::WITH_TEX,
            .texIndex = 3,
        }
    );
    command.Spawn<RenderInfo>(
        RenderInfo{
            .model = &backWallTransform,
            .drawable = std::make_shared<prism::Plane>(),
            .shaderType = prism::ShaderType::WITH_TEX,
            .texIndex = 2,
        }
    );
    command.Spawn<RenderInfo>(
        RenderInfo{
            .model = &leftWallTransform,
            .drawable = std::make_shared<prism::Plane>(),
            .shaderType = prism::ShaderType::WITH_TEX,
            .texIndex = 4
        }
    );
    command.Spawn<RenderInfo>(
        RenderInfo{
            .model = &rightWallTransform,
            .drawable = std::make_shared<prism::Plane>(),
            .shaderType = prism::ShaderType::WITH_TEX,
            .texIndex = 5
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
    TextureBase.push_back(std::make_shared<Texture>(bufw, bufh, buffer)); // texIndex 0

    createPureColorTexture(buffer, bufw, bufh, 255, 0, 255);
    TextureBase.push_back(std::make_shared<Texture>(bufw, bufh, buffer)); // texIndex 1

    createPureColorTexture(buffer, bufw, bufh, 200, 200, 200);
    TextureBase.push_back(std::make_shared<Texture>(bufw, bufh, buffer)); // texIndex 2
    
    createPureColorTexture(buffer, bufw, bufh, 255 * lightColor[0], 255 * lightColor[1], 255 * lightColor[2]);
    TextureBase.push_back(std::make_shared<Texture>(bufw, bufh, buffer)); // texIndex 3

    createPureColorTexture(buffer, bufw, bufh, 220, 20, 60);
    TextureBase.push_back(std::make_shared<Texture>(bufw, bufh, buffer)); // texIndex 4

    createPureColorTexture(buffer, bufw, bufh, 60, 179, 113);
    TextureBase.push_back(std::make_shared<Texture>(bufw, bufh, buffer)); // texIndex 5

    command.SetResource(&TextureBase);

    delete [] buffer;
}