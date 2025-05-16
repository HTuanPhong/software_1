#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

    SDL_GPUDevice *device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
        false,
        NULL);
    SDL_GPUShaderFormat formats = SDL_GetGPUShaderFormats(device);
    SDL_Log("%d", formats);
    SDL_Log("PRIVATE %d", formats & SDL_GPU_SHADERFORMAT_PRIVATE);
    SDL_Log("SPIRV %d", formats & SDL_GPU_SHADERFORMAT_SPIRV);
    SDL_Log("DXBC %d", formats & SDL_GPU_SHADERFORMAT_DXBC);
    SDL_Log("DXIL %d", formats & SDL_GPU_SHADERFORMAT_DXIL);
    SDL_Log("MSL %d", formats & SDL_GPU_SHADERFORMAT_MSL);
    SDL_Log("METALLIB %d", formats & SDL_GPU_SHADERFORMAT_METALLIB);

    return 0;
}