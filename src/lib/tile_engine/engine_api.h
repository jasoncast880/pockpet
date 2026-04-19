#ifndef ENGINE_API_H
#define ENGINE_API_H

#ifdef __cplusplus
extern "C" {
#endif 

struct LayerHandle_t;
struct SpriteHandle_t;

struct LayerHandle_t add_layer(uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high); 
struct SpriteHandle_t add_sprite(uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high, struct LayerHandle_t associated_layer);

int update_layer(struct LayerHandle_t layer_handle, uint8_t* map, uint8_t x, uint8_t y);
int update_sprite(struct LayerHandle_t layer_handle, uint8_t sprite_id, uint8_t* map, uint8_t x, uint8_t y);

int delete_layer(struct LayerHandle_t layer);
int delete_sprite(struct SpriteHandle_t sprite);

void soft_render();

#ifdef __cplusplus
} //extern "C"
#endif 

#endif //ENGINE_API_H
