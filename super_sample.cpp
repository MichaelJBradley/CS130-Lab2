#include "super_sample.h"

#include "globals.h"
#include "assert.h"

#include <cstdlib>

const unsigned Super_Sample::SAMPLES = 4;


Super_Sample::Super_Sample(Render_World& world_input, Camera& camera_input) 
    :camera(camera_input), world(world_input)
{}

vec3 Super_Sample::Sample(const ivec2& pixel_index) {
    vec3 total_color = {0, 0, 0};
    vec3 pixels[SAMPLES];
    
    Pixel_Positions(pixel_index, pixels);

    for (unsigned i = 0; i < SAMPLES; i++) {
        total_color += world.Cast_Ray(pixels[i]);
    }

    return total_color / SAMPLES;
}

void Super_Sample::Pixel_Positions(const ivec2& pixel_index, vec3* pixels) {
    vec2 pix_pos[SAMPLES];
    vec2 c = vec2(pixel_index) + vec2(.5f, .5f);

    pix_pos[0] = vec2(c[X] - .25f, c[Y] + .25f);
    pix_pos[1] = vec2(c[X] + .25f, c[Y] + .25f);
    pix_pos[2] = vec2(c[X] - .25f, c[Y] - .25f);
    pix_pos[3] = vec2(c[X] + .25f, c[Y] - .25f);
    
    for (unsigned i = 0; i < SAMPLES; i++) {
        Jitter(pix_pos[i]);
        pix_pos[i] = camera.min + pix_pos[i] * camera.pixel_size;
        pixels[i] = camera.film_position
                  + (camera.horizontal_vector * pix_pos[i][X])
                  + (camera.vertical_vector * pix_pos[i][Y]);
    }

}

void Super_Sample::Jitter(vec2& pixel_position) {
    bool add;
    float amount;
    for (unsigned i = 0; i < 2; i++) {
        add = rand() % 2;
        amount = (rand() % 25) / 100.0f;
        pixel_position[i] += add ? amount : -amount;
    }
}

