#ifndef SOFT_CORE_H
#define SOFT_CORE_H

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Dependencies

#include <SDL3/SDL.h>

typedef struct AppState AppState;
struct AppState
{
	SDL_Window *window;
	SDL_GPUDevice *gpu_device;
	const char* base_path;
	SDL_GPUGraphicsPipeline *fill_pipe;
	SDL_GPUGraphicsPipeline *line_pipe;
};



#endif //SOFT_CORE_H