#pragma once
#include "Maths/vec.h"
#include "System/Output.h"
#include <GUI/Basics/Button.h>
#include <GUI/Primitives/Box.h>
#include <Graphics/Texture2D.h>
#include <Essentials/Noise.h>
#include <functional>
#include <unordered_map>

static inline std::unordered_map<Texture2D*, std::function<color(ivec2)>> textures;
static inline RenderGUI* noisetexturetab = nullptr;

static void addTexture(std::string name, std::function<color(ivec2)> generator)
{
    Gum::Output::print("adding " + name);
    Texture2D* tex = new Texture2D(name, ivec2(100, 100), [](ivec2 pixelcoord) { return rgba(255); });
    ((Box*)noisetexturetab->findChildByID(name))->setTexture(tex);

    textures[tex] = generator;
}

static inline float f = 1.0f / 100.0f;

static void buildNoiseTextureSection(RenderGUI* noisepatternsTab)
{
    noisetexturetab = noisepatternsTab;

    // Classic noise
    addTexture("randomnoise", [](ivec2 pixelcoord) {
        vec3 noise = Gum::Noise::hash3D(vec2(pixelcoord) + Gum::Noise::randomNumber) * 255.0f;
        noise = vec3::clamp(noise, 0, 255);
        return rgba(rgb(noise.x, noise.y, noise.z), 255);
    });

    addTexture("noise", [](ivec2 pixelcoord) {
        float noise = (Gum::Noise::noise(vec2(pixelcoord) * f, 10) + 1.0f) * 127.0f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("noised", [](ivec2 pixelcoord) {
        vec3 noise = (Gum::Noise::noised(vec2(pixelcoord) * f, 10) + 1.0f) * 127.0f;
        noise = vec3::clamp(noise, 0, 255);
        return rgba(rgb(noise.x, noise.y, noise.z), 255);
    });

    addTexture("gridnoise", [](ivec2 pixelcoord) {
        vec3 noise = Gum::Noise::gridNoise(vec2(pixelcoord) * f, 10.0f) * 255.0f;
        noise = vec3::clamp(noise, 0, 255);
        return rgba(rgb(noise.x, noise.y, noise.z), 255);
    });
    


    // Gradient noise
    addTexture("gradientnoise", [](ivec2 pixelcoord) {
        float noise = (Gum::Noise::gradientNoise(vec2(pixelcoord) * f, 10.0f) + 1.0f) * 127.5f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("gradientnoised", [](ivec2 pixelcoord) {
        vec3 noise = (Gum::Noise::gradientNoised(vec2(pixelcoord) * f, 10.0f) + 1.0f) * 127.5f;
        noise = vec3::clamp(noise, 0, 255);
        return rgba(rgb(noise.x, noise.y, noise.z), 255);
    });

    addTexture("gradientnoisedisorder", [](ivec2 pixelcoord) {
        float noise = (Gum::Noise::gradientNoiseDisorder(vec2(pixelcoord) * f, 10.0f, 0.5f) + 1.0f) * 127.5f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });
    


    // Perlin Noise
    addTexture("perlinnoise", [](ivec2 pixelcoord) {
        float noise = (Gum::Noise::perlinNoise(vec2(pixelcoord) * f, 10) + 1.0f) * 127.5f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("perlinnoised", [](ivec2 pixelcoord) {
        vec3 noise = (Gum::Noise::perlinNoised(vec2(pixelcoord) * f, 10) + 1.0f) * 127.5f;
        noise = vec3::clamp(noise, 0, 255);
        return rgba(rgb(noise.x, noise.y, noise.z), 255);
    });

    addTexture("organicnoise", [](ivec2 pixelcoord) {
        vec3 noise = Gum::Noise::organicNoise(vec2(pixelcoord) * f, 10) * 255.5f;
        noise = vec3::clamp(noise, 0, 255);
        return rgba(rgb(noise.x, noise.y, noise.z), 255);
    });



    // Cellular Noise
    addTexture("cellularnoise1", [](ivec2 pixelcoord) {
        float noise = (Gum::Noise::cellularNoise(vec2(pixelcoord) * f, 10)).x * 255.0f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("cellularnoise2", [](ivec2 pixelcoord) {
        float noise = (Gum::Noise::cellularNoise(vec2(pixelcoord) * f, 10)).y * 255.0f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("metaballsnoise", [](ivec2 pixelcoord) {
        float noise = (Gum::Noise::metaballs(vec2(pixelcoord) * f, 10)) * 255.0f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("cellularnoised", [](ivec2 pixelcoord) {
        vec3 noise = (Gum::Noise::cellularNoised(vec2(pixelcoord) * f, 10) + 1.0f) * 127.5f;
        noise = vec3::clamp(noise, 0, 255);
        return rgba(rgb(noise.x, noise.y, noise.z), 255);
    });



    // Cellular Noise metrics
    addTexture("cellularmanhattannoise", [](ivec2 pixelcoord) {
        float noise = (Gum::Noise::cellularNoisem(vec2(pixelcoord) * f, 10, 1.0f, 0.0f, 1).y) * 255.0f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("cellularchebyshevnoise", [](ivec2 pixelcoord) {
        float noise = (Gum::Noise::cellularNoisem(vec2(pixelcoord) * f, 10, 1.0f, 0.0f, 2).y) * 255.0f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("cellulartriangularnoise", [](ivec2 pixelcoord) {
        float noise = (Gum::Noise::cellularNoisem(vec2(pixelcoord) * f, 10, 1.0f, 0.0f, 3).y) * 255.0f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });



    // Voronoi Noise
    addTexture("voronoinoise", [](ivec2 pixelcoord) {
        float noise = Gum::Noise::voronoi(vec2(pixelcoord) * f, 10).x * 255.0f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });

    /*addTexture("cracksnoise", [](ivec2 pixelcoord) {
        float noise = Gum::Noise::cracks(vec2(pixelcoord) * f, 10).x * 255.0f;
        return rgba(rgb(noise, noise, noise), 255);
    });*/

    addTexture("voronoipattern", [](ivec2 pixelcoord) {
        vec3 noise = Gum::Noise::voronoiPattern(vec2(pixelcoord) * f, 10) * 255;
        noise = vec3::clamp(noise, 0, 255);
        return rgba(rgb(noise.x, noise.y, noise.z), 255);
    });

    addTexture("voronoiids", [](ivec2 pixelcoord) {
        vec2 id = Gum::Noise::voronoi(vec2(pixelcoord) * f, 10) * 255.0f;
        id = vec2::clamp(id, 0, 255);
        return rgba(rgb(id.x, id.y, 0), 255);
    });



    //Hexagons
    addTexture("hexagonsnoise1", [](ivec2 pixelcoord) {
        float noise = Gum::Noise::noiseHexagons(vec2(pixelcoord) * f, 2, 1.0f, 1.0f, false, true) * 255.0f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });

    /*addTexture("hexagonsdnoise", [](ivec2 pixelcoord) {
        vec3 noise = Gum::Noise::voronoiPattern(vec2(pixelcoord) * f, 10) * 255;
        noise = vec3::clamp(noise, 0, 255);
        return rgba(rgb(noise.x, noise.y, noise.z), 255);
    });*/

    addTexture("hexagonsnoise2", [](ivec2 pixelcoord) {
        float noise = Gum::Noise::sdfHexagon(vec2(pixelcoord) * f, 2.0f, false) * 255.0f;
        noise = Gum::Maths::clamp(noise, 0.0f, 255.0f);
        return rgba(rgb(noise, noise, noise), 255);
    });



    //Patterns
    addTexture("checkerboardnoise", [](ivec2 pixelcoord) {
        vec3 noise = Gum::Noise::checkerboard(vec2(pixelcoord) * f, 20);
        return rgba(rgb(noise.x * 255), 255);
    });

    addTexture("randomlinesnoise", [](ivec2 pixelcoord) {
        vec2 pattern = Gum::Noise::randomLines(vec2(pixelcoord) * f, 10.0f, 4, 0.1f);
        return hsva(hsv(Gum::Noise::hash1D(pattern.y) * 360.0f, 50.0f, pattern.x * 100.0f), 255);
    });

    addTexture("dotsnoise", [](ivec2 pixelcoord) {
        vec3 pattern = Gum::Noise::dotsNoise(vec2(pixelcoord) * f, 10.0f);
        return hsva(hsv(pattern.y * 360, 50.0f, pattern.x * 100.0f), 255);
    });


    addTexture("metaballsnoise2", [](ivec2 pixelcoord) {
        float noise = Gum::Noise::metaballs(vec2(pixelcoord) * f, 10.0f) * 255;
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("wavepattern", [](ivec2 pixelcoord) {
        float noise = Gum::Noise::wavePattern(vec2(pixelcoord) * f, 10.0f) * 255;
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("stairspattern", [](ivec2 pixelcoord) {
        float noise = Gum::Noise::stairsPattern(vec2(pixelcoord) * f, 10.0f, 1.0f, 0.0f, 0.5f) * 255;
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("crosspattern", [](ivec2 pixelcoord) {
        float noise = Gum::Noise::crossPattern(vec2(pixelcoord) * f, 10.0f) * 255;
        return rgba(rgb(noise, noise, noise), 255);
    });



    //Tile Hexagons
    addTexture("tilehexagonsuv", [](ivec2 pixelcoord) {
        vec4 noise = Gum::Noise::tileHexagons(vec2(pixelcoord) * f, 5.0f) * 255;
        return rgba(rgb(noise.x, noise.y, 0), 255);
    });

    addTexture("tilehexagonspos", [](ivec2 pixelcoord) {
        vec4 noise = Gum::Noise::tileHexagons(vec2(pixelcoord) * f, 5.0f) * 255;
        return rgba(rgb(noise.z, noise.w, 0), 255);
    });



    //FBM
    addTexture("fbmnoise", [](ivec2 pixelcoord) {
        float noise = Gum::Noise::fbm(vec2(pixelcoord) * f, 10.0f) * 255;
        return rgba(rgb(noise, noise, noise), 255);
    });

    /*addTexture("fbmperlinnoise", [](ivec2 pixelcoord) {
        vec4 noise = Gum::Noise::tileHexagons(vec2(pixelcoord) * f, 10.0f) * 255;
        return rgba(rgb(noise.z, noise.w, 0), 255);
    });*/

    addTexture("fbmgridnoise", [](ivec2 pixelcoord) {
        float noise = Gum::Noise::fbmGrid(vec2(pixelcoord) * f, 10.0f) * 255;
        return rgba(rgb(noise, noise, noise), 255);
    });

    addTexture("fbmmetaballsnoise", [](ivec2 pixelcoord) {
        float noise = Gum::Noise::fbmMetaballs(vec2(pixelcoord) * f, 10.0f) * 255;
        return rgba(rgb(noise, noise, noise), 255);
    });
    

    Button* generateTexturesButton = (Button*)noisepatternsTab->findChildByID("generatebutton");
    generateTexturesButton->onClick([](RenderGUI* gui, uint16_t btn) {
        Gum::Noise::setSeed(time(0));

        for(auto it : textures)
            it.first->generate(it.second);
    }, GUM_MOUSE_BUTTON_LEFT);
}