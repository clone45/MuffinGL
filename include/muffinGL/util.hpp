// util.hpp
#pragma once
#include <string>
#include <SDL2/SDL.h>

inline std::string blendModeToString(SDL_BlendMode mode) {
    if(mode == SDL_BLENDMODE_NONE) return "NONE";
    if(mode == SDL_BLENDMODE_BLEND) return "BLEND";
    if(mode == SDL_BLENDMODE_ADD) return "ADD";
    if(mode == SDL_BLENDMODE_MOD) return "MOD";
    
    // For custom blend modes, let's decode the components
    SDL_BlendFactor srcColorFactor, dstColorFactor, srcAlphaFactor, dstAlphaFactor;
    SDL_BlendOperation colorOperation, alphaOperation;
    
    if(SDL_ComposeCustomBlendMode(
        SDL_BLENDFACTOR_SRC_ALPHA,              // our AlphaPreserve source color
        SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,    // destination color factor
        SDL_BLENDOPERATION_ADD,                 // color operation
        SDL_BLENDFACTOR_ONE,                    // source alpha factor
        SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,    // destination alpha factor
        SDL_BLENDOPERATION_ADD                  // alpha operation
    ) == mode) {
        return "ALPHA_PRESERVE";
    }
    
    return "UNKNOWN_CUSTOM(" + std::to_string(mode) + ")";
}