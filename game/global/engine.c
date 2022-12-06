//
//  engine.c
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#include "engine.h"
#include "physical_object.h"
#include "memory.h"
#include "mesh.h"

ENGINE_STRUCT struct Engine* engine_read(const uint32_t address)
{
    struct Engine *engine = malloc(sizeof *engine);
    engine->offset = address;
    engine->root = NULL;
    
    struct Stream *stream = stream_open(address);
    engine->mode = read8();
    engine->current_level_name = readstring(30);
    engine->next_level_name = readstring(30);
    engine->first_level_name = readstring(30);
    
    engine->input_mode = read8();
    engine->display_fix_mode = read8();
    advance(3);
    engine->display_mode = read32();
    engine->timer = engine_timer_read(stream);
    engine->multimode_player_count = read8();
    engine->multimode_column_count = read8();
    engine->multimode_mini_screen_ratio = read8();
    advance(1);
    
    
    uint64_t checkpoint = stream->position;
    for (int i = 0; i < 4; i++)
    {
        uint32_t ptr = readpointer();
//        if (ptr != 0x00)
//        {
//            info("Ptr: %X\n",ptr);
//            stream_seek(stream, ptr);
//            engine->current_main_players[i] = superobject_read(stream);
//            info("Main player: %X\n", engine->current_main_players[i]);
//            stream_seek(stream, checkpoint + 4 * i);
//        }
    }
    
    
    /* Skip actor pointers */
    //stream_seek(stream, checkpoint + 4 * 4);
    
    /* Seek to submap number */
    stream_seek(stream, stream->position + 0x5E0);
    
    engine->submap_id = read32();
    engine->engine_paused = read8();
    engine->engine_paused2 = read8();
    engine->engine_gamesave = read8();
    
    /* Read level name data */
    for (int i = 0; i < 150; i++) engine->level_names[i] = readstring(30);
    for (int i = 0; i <  30; i++) engine->demo_names[i] = readstring(12);
    for (int i = 0; i <  30; i++) engine->demo_level_names[i] = readstring(12);
    
    engine->demo_count = read8();
    engine->level_count = read8();
    engine->current_level_num = read8();
    engine->previous_level_num = read8();
    engine->previous_level_exit_id = read8();
    engine->global_level_counter = read8();
    engine->demo_mode = read8();
    engine->current_language = read8();
    engine->language_count = read8();
    engine->engine_frozen = read8();
    engine->resurrection = read8();
    
    engine->camera_mode = read8();
    engine->current_importance = read8();
    
    engine->n_superobjects_allocated = read32();
    engine->n_superobjects_loaded = read32();
    engine->n_nonpersistent_so_links = read32();
    
    //engine->so_links[0] = superobject_links_read(stream);
    /* graph_links = */read32();
    
    stream_close(stream);
    
    return engine;
}

ENGINE_STRUCT void engine_change_map(struct Engine *engine, const char* map)
{
    for (int i = 0; i < 10; i++) memory.write_8(0, 0x003E7C2B + i);
    for (int i = 0; i < strlen(map); i++) memory.write_8(map[i], 0x003E7C2B + i);
    //i_write8(0x003E7C2B + strlen(map));
    
    memory.write_8(6, 0x3E7C0C);
}

struct OBJParam
{
    FILE* fp;
    int prev_index;
};

char *replace(const char *s, char ch, const char *repl)
{
    int count = 0;
    const char *t;
    for(t=s; *t; t++)
        count += (*t == ch);

    size_t rlen = strlen(repl);
    char *res = malloc(strlen(s) + (rlen-1)*count + 1);
    char *ptr = res;
    for(t=s; *t; t++) {
        if(*t == ch) {
            memcpy(ptr, repl, rlen);
            ptr += rlen;
        } else {
            *ptr++ = *t;
        }
    }
    *ptr = 0;
    return res;
}

static void internal_export_obj(struct SuperObject *obj, void* p)
{
    struct OBJParam* param = p;
    if (obj->type == SUPEROBJECT_TYPE_IPO && obj->data)
    {
        struct IPO *ipo = obj->data;
        if (ipo != NULL)
        {
            if (ipo->data && ipo->data_ptr != 0x00)
            {
                struct PhysicalObject *po = ipo->data;
                if (po->collide_set_ptr != 0x00 && po->geometry)
                {
                    struct CollisionGeometry *geom = po->geometry;
                    if (geom->element_types)
                    {
                        for (unsigned e = 0; e < geom->n_elements; e++)
                        {
                            if (geom->element_types[e] == 1) /* mesh */
                            {
                                struct Mesh *mesh = geom->elements[e];
                                if (mesh->was_processed && mesh->processed.vertices)
                                {
                                    for (unsigned t = 0; t < mesh->n_triangles * 3; t++)
                                    {
                                        struct Vector3 v = mesh->processed.vertices[t];
                                        
                                        struct Vector4 v2;
                                        v2.x = v.x;
                                        v2.z = v.y;
                                        v2.y = v.z;
                                        v2.w = 1;
                                        
                                        struct Matrix4 transform = obj->matrix_default;
                                        struct Vector4 computed = vector4_mul_matrix4(v2, transform);
                                        
                                        char buf[128];
                                        memset(buf, 0, 128);
                                        sprintf(buf, "v %.7f %.7f %.7f\n", computed.x, computed.z, computed.y);
                                        char* a= replace(buf, ',', ".");
                                        
                                        fprintf(param->fp, "%s", a);
                                        
                                        free(a);
                                    }
                                    
                                    fprintf(param->fp, "\n");
                                    fprintf(param->fp, "o %s\n", ipo->name);
                                    for (unsigned t = 0; t < mesh->n_triangles; t++)
                                    {
                                        uint16_t idx0 = mesh->processed.indices[t * 3 + 0] + 1 + param->prev_index;
                                        uint16_t idx1 = mesh->processed.indices[t * 3 + 1] + 1 + param->prev_index;
                                        uint16_t idx2 = mesh->processed.indices[t * 3 + 2] + 1 + param->prev_index;
                                        fprintf(param->fp, "f %d %d %d\n", idx0, idx1, idx2);
                                    }
                                    
                                    fprintf(param->fp, "\n");
                                    
                                    param->prev_index += mesh->processed.indices[mesh->n_triangles * 3 - 1] + 2;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

ENGINE_STRUCT void engine_export_obj(struct Engine *engine)
{
//    char path[1024];
//    memset(path, 0, 1024);
//    sprintf(path, LIBR3TAS_DIR "/obj/%s.obj", engine->current_level_name);
//    
//    struct OBJParam param;
//    param.fp = fopen(path, "w");
//    param.prev_index = 0;
//    
//    fprintf(param.fp, "# %s.obj\n", engine->current_level_name);
//    fprintf(param.fp, "# Exported by r3-tas\n\n");
//    
//    superobject_for_each(SUPEROBJECT_TYPE_IPO, engine->root, &internal_export_obj, &param);
//    fclose(param.fp);
}
