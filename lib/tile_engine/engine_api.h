#ifndef ENGINE_API_H
#define ENGINE_API_H

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct { 
	uint16_t* render_data; //variable size, constant location.
	struct Layer* layer;
	//size_t render_buf_size;

	uint8_t h_scanline_counter; //use for hscanline renders

	uint16_t x,y; //for buf. true count of current pixel pointer
	uint8_t x_tile, y_tile; //for tile-by-tile read.
	
	bool collision_flag; // TODO implement.useful when collision is present in the system 
} Engine;
extern Engine* e; //le engine

typedef struct { //structure useful for app-level logic with sprites.
				 //needs to be decoupled because of C++ lang barrier
	int16_t  x,y; //coords of the top-left of sprite rect.
	uint8_t* map;
	uint8_t tiles_wide, tiles_high;
	uint8_t id; //id sets apart/secondary handle. Struct itself is the primary handle

	uint8_t collision_hash; //TODO implement collision hash
} Entity_Handle;

Engine* engine_init(struct Layer* layer);
uint16_t* engine_render(); //needs to return a contiguous arr
void engine_reset(); //resets scanline counter, resets rendering cursor

struct Layer* add_layer(const uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high); 
int update_layer(struct Layer* layer, uint8_t* map);

Entity_Handle* add_sprite(const uint16_t* tiles, size_t num_tiles, uint8_t* tilemap, uint8_t tiles_wide, uint8_t tiles_high, struct Layer* associated_layer);
void update_sprite_map(Entity_Handle* eh, uint8_t* map);
void update_sprite_position(Entity_Handle* eh, int16_t x, int16_t y); 
void increment_sprite_x(Entity_Handle* eh);
void increment_sprite_y(Entity_Handle* eh); 

void delete_layer(struct Layer* layer); //TODO weird system behaviours..
void delete_sprite(Entity_Handle* eh); //TODO essential!

#ifdef __cplusplus
} //extern "C"
#endif 

#endif //ENGINE_API_H
