#ifndef SOFT_GRAPHIC_H
#define SOFT_GRAPHIC_H

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Dependencies

#include <SDL3/SDL.h>
#include "soft_core.h"

int soft_graphic_init(AppState appstate);

SDL_GPUShader *soft_graphic_load_shader(
    SDL_GPUDevice* device,
	const char* shader_file_name,
	Uint32 sampler_count,
	Uint32 uniform_buffer_count,
	Uint32 storage_buffer_count,
	Uint32 storage_texture_count
);




#endif //SOFT_GRAPHIC_H