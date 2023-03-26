/* Should sum up to 1, I guess */
/* NOTE: These projection parameters vary per level */
static const float projX = 0.377f;
static const float projY = 0.708f;

static ImVec4 project_world_coordinate(const tdstVector3D P)
{
    tdstCameraGLI* camera = (tdstCameraGLI*)pointer(engine->viewport_camera[0]);
    
    tdstMatrix4D view = matrix4_host_byteorder(camera->transform.matrix);
    /* Change sign of the two middle columns (flipping the rotation) */
    view.m01 = -view.m01;
    view.m11 = -view.m11;
    view.m21 = -view.m21;
    view.m31 = -view.m31;
    view.m02 = -view.m02;
    view.m12 = -view.m12;
    view.m22 = -view.m22;
    view.m32 = -view.m32;
    /* const tdstMatrix4D view = matrix4_lookat(campos, vector3_host_byteorder(*(tdstVector3D*)(memory.base + 0x00c53910)) (dsgvar_150), vector3_new(0, 0, -1)); */
    
    const float fov = host_byteorder_f32(*(float32*)&camera->alpha_x);
    
    const tdstMatrix4D projection = matrix4_perspective(fov, 528.0f / 640.0f, 0.1f, 1000.0f);
    const tdstMatrix4D viewprojection = matrix4_mul(projection, view);
    const tdstVector4D P2 = vector4_new(P.x, P.y, P.z, 1.0f);
    const tdstVector4D R = vector4_mul_matrix4(P2, viewprojection);
    
    float xp = (R.x / R.w) * projX + 0.5f;
    float yp = (R.y / R.w) * projY + 0.5f;
    
    return ImVec4(xp, yp, R.z, R.w);
}

static ImVec4 project_world_coordinate(const tdstVector4D P)
{
    return project_world_coordinate(vector3_new(P.x, P.y, P.z));
}

static void RecursiveProjectSuperobjectPosition(const tdstSuperObject* object,
                                                const tdstMatrix4D transform,
                                                const tdstVector3D player,
                                                const float fov,
                                                ImVec2 offset,
                                                ImVec2 size,
                                                ImVec2 mouse,
                                                ImDrawList* drawlist)
{
    if (!object) return;
    
    const tdstMatrix4D T = matrix4_mul(superobject_matrix_global(object), transform);
    
    const char* text = superobject_name(object);
    if (text)
    {
        const tdstVector3D P = game_matrix4_position(T);
        /* Project the object's position to the screen */
        ImVec4 projected = project_world_coordinate(P);
        /* Only draw objects in view */
        if (projected.w >= 0)
        {
            
            ImVec2 pos = ImVec2(offset.x + size.x - projected.x * size.x, offset.y + projected.y * size.y);
            ImVec2 text_pos = ImVec2(pos.x - 1, pos.y);
            ImVec2 text_extent = ImVec2(pos.x + strlen(text) * 7, pos.y + 13);
            
            /* Fade out objects far away */
            const float alpha = 1.0f / vector3_length(vector3_sub(P, player));
            
            ImColor color = ImColor(1.0f, 1.0f, 1.0f, 0.25f);
            ImColor background = color;
            
            if (superobject_type(object) == superobject_type_ipo)
            {
                background.Value.w = screen_projections_bg_alpha;
            }
            
            if (superobject_type(object) == superobject_type_actor)
            {
                background = color = ImColor(actor_color((tdstEngineObject*)superobject_data(object)));
                color.Value.w = alpha * ((1.0f - screen_projections_fade_factor) * 200.0f) / (fov / 2.0f);
                background.Value.w = screen_projections_bg_alpha;
            }
            
            if (mouse.x >= pos.x && mouse.y >= pos.y && mouse.x <= text_extent.x && mouse.y <= text_extent.y)
            {
                ImGui::BeginTooltip();
                ImGui::TextColored(color, "%s", text);
                ImGui::Text("(%.3f, %.3f, %.3f)", P.x, P.y, P.z);
                ImGui::Text("Click to view");
                ImGui::EndTooltip();
                
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_superobject = (tdstSuperObject*)object;
                    
                    bool already_open = false;
                    for (SuperobjectInfoWindow* window : superobject_info_windows)
                        if (window->selected == object) already_open = true;
                    
                    if (!already_open) superobject_info_windows.push_back(SuperobjectInfoWindow::CreateWindow(object));
                }
                
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                {
                }
                
                color = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
            }
            
            /* Draw label */
            drawlist->AddCircleFilled(text_pos, 1.0f, color);
            drawlist->AddRectFilled(text_pos, text_extent, background);
            drawlist->AddText(pos, color, text);
        }
    }
    
    superobject_for_each(object, child) RecursiveProjectSuperobjectPosition(child, T, player, fov, offset, size, mouse, drawlist);
}

static void ProjectSuperobjectPositions(const tdstSuperObject* root)
{
    float fov = host_byteorder_f32(*(float32*)(memory.base + 0x00C751B4));
    fov = fov == 0.0f ? 1.3 : fov;
    
    const float new_width = display_size.y * (640.0f / 528.0f) * 1.075f;
    const float x_offset = display_size.x - new_width;
    ImGui::SetWindowPos(ImVec2(x_offset / 2.0f, 0));
    ImGui::SetWindowSize(ImVec2(new_width, display_size.y));
    
    const tdstVector3D playerposition = game_matrix4_position(actor_matrix(actor_rayman));
    
    ImVec2 off = ImGui::GetWindowPos();
    ImVec2 sz = ImGui::GetWindowSize();
    ImVec2 cursor = ImGui::GetMousePos();
    ImDrawList* drawlist = ImGui::GetBackgroundDrawList();
    
    if (screen_projections_use_cliprect)
        drawlist->PushClipRect(ImVec2(x_offset / 2.0f, 0), ImVec2(new_width + x_offset / 2.0f, display_size.y));
    
    /* Overlay to make seeing the text easier */
    drawlist->AddRectFilled(off, ImVec2(sz.x + off.x, display_size.y), ImColor(ImVec4(0.0f, 0.0f, 0.0f, screen_projections_overlay_alpha)));
    
    RecursiveProjectSuperobjectPosition(root, matrix4_identity, playerposition, fov, off, sz, cursor, drawlist);
    
    if (screen_projections_use_cliprect)
        drawlist->PopClipRect();
}

static void PairwiseProjection(const tdstMatrix4D T,
                               tdstVector3D* A,
                               tdstVector3D* B,
                               tdstVector3D* C,
                               ImVec2 offset,
                               ImVec2 size,
                               ImVec4* projectedA,
                               ImVec4* projectedB,
                               ImVec4* projectedC)
{
    const tdstVector4D TA = vector4_mul_matrix4(vector4_new(A->x, A->y, A->z, 1.0f), T);
    const tdstVector4D TB = vector4_mul_matrix4(vector4_new(B->x, B->y, B->z, 1.0f), T);
    const tdstVector4D TC = vector4_mul_matrix4(vector4_new(C->x, C->y, C->z, 1.0f), T);
    
    *A = vector3_new(TA.x, TA.y, TA.z);
    *B = vector3_new(TB.x, TB.y, TB.z);
    *C = vector3_new(TC.x, TC.y, TC.z);
    
    ImVec4 pA = project_world_coordinate(TA);
    ImVec4 pB = project_world_coordinate(TB);
    ImVec4 pC = project_world_coordinate(TC);
    
    ImVec4 tA = ImVec4(offset.x + size.x - pA.x * size.x, offset.y + pA.y * size.y, pA.z, pA.w);
    ImVec4 tB = ImVec4(offset.x + size.x - pB.x * size.x, offset.y + pB.y * size.y, pB.z, pB.w);
    ImVec4 tC = ImVec4(offset.x + size.x - pC.x * size.x, offset.y + pC.y * size.y, pC.z, pC.w);
    
    *projectedA = tA;
    *projectedB = tB;
    *projectedC = tC;
}

static void DrawLevelGeometryRecursive(const tdstSuperObject* root, const tdstMatrix4D transform, const tdstVector3D player, ImVec2 off, ImVec2 sz, ImDrawList* drawlist, ImVec4 color = ImColor(1.0f, 0.75f, 0.0f, 1.0f), bool edges = false)
{
    if (!root) return;
    
    /* Calculate the new transformation */
    const tdstMatrix4D T = matrix4_mul(superobject_matrix_global(root), transform);

    if (root == viewed_sector) color = ImColor(0.0f, 1.0f, 0.25f, 1.0f);
    
    if (superobject_type(root) == superobject_type_ipo)
    {
        const tdstInstantiatedPhysicalObject* ipo = (const tdstInstantiatedPhysicalObject*)superobject_data(root);
        if (ipo)
        {
            const tdstCollideObject* zdr = ipo_collide_object(ipo);
            if (zdr)
            {
                int mesh_idx = 0;
                const tdstCollideElementIndexedTriangles* mesh;
                while ((mesh = collide_object_mesh(zdr, mesh_idx)))
                {
                    const tdstVector3D* vertices = (const tdstVector3D*)pointer(zdr->vertices);
                    
                    if (!edges)
                    {
                        const tdstVector3D* normals = (const tdstVector3D*)pointer(mesh->normals);
                        const uint16* indices = (const uint16*)pointer(mesh->face_indices);
                        
                        for (int16 index = 0; index < host_byteorder_16(mesh->n_faces); index++)
                        {
                            uint16 idx0 = host_byteorder_16(*(indices + index * 3 + 0));
                            uint16 idx1 = host_byteorder_16(*(indices + index * 3 + 1));
                            uint16 idx2 = host_byteorder_16(*(indices + index * 3 + 2));
                            
                            const tdstVector3D Pa = vector3_host_byteorder(*(vertices + idx0));
                            const tdstVector3D Pb = vector3_host_byteorder(*(vertices + idx1));
                            const tdstVector3D Pc = vector3_host_byteorder(*(vertices + idx2));
                            
                            const tdstVector3D Na = vector3_host_byteorder(*(normals + index / 3));
                            
                            const tdstVector4D TpA = vector4_mul_matrix4(vector4_new(Pa.x, Pa.y, Pa.z, 1.0f), T);
                            const tdstVector4D TpB = vector4_mul_matrix4(vector4_new(Pb.x, Pb.y, Pb.z, 1.0f), T);
                            const tdstVector4D TpC = vector4_mul_matrix4(vector4_new(Pc.x, Pc.y, Pc.z, 1.0f), T);
                            
                            const tdstVector4D TnA = vector4_mul_matrix4(vector4_new(Na.x, Na.y, Na.z, 1.0f), matrix4_identity);
//                            const tdstVector4D TnB = vector4_mul_matrix4(vector4_new(Nb.x, Nb.y, Nb.z, 1.0f), T);
//                            const tdstVector4D TnC = vector4_mul_matrix4(vector4_new(Nc.x, Nc.y, Nc.z, 1.0f), T);
                            
                            ImVec4 coord_a = project_world_coordinate(TpA);
                            ImVec4 coord_b = project_world_coordinate(TpB);
                            ImVec4 coord_c = project_world_coordinate(TpC);
                            
                            if (coord_a.w >= 0 && coord_b.w >= 0 && coord_c.w >= 0)
                            {
                                /* Calculate the triangle center for highlighting of the face */
                                tdstVector3D center = vector3_new((TpA.x + TpB.x + TpC.x) / 3.0f, (TpA.y + TpB.y + TpC.y) / 3.0f, (TpA.z + TpB.z + TpC.z) / 3.0f);
                                tdstVector4D centerT = vector4_mul_matrix4(vector4_new(center.x, center.y, center.z, 1.0f), T);
                                /* Distance fading factor */
                                const float alpha = 1.0f / (vector3_length(vector3_sub(center, player)) + 1.0f);
                                
                                ImVec2 pos1 = ImVec2(off.x + sz.x - coord_a.x * sz.x, off.y + coord_a.y * sz.y);
                                ImVec2 pos2 = ImVec2(off.x + sz.x - coord_b.x * sz.x, off.y + coord_b.y * sz.y);
                                ImVec2 pos3 = ImVec2(off.x + sz.x - coord_c.x * sz.x, off.y + coord_c.y * sz.y);
                                
                                ImColor outline = color;
                                outline.Value.w = alpha * (1.0f - level_geometry_fade_factor) + level_geometry_fade_factor;
                                
                                ImColor base = outline;
                                base.Value.w *= level_geometry_tri_fill;
                                
                                drawlist->AddTriangle(pos1, pos2, pos3, outline);
                                drawlist->AddTriangleFilled(pos1, pos2, pos3, base);
                                
                                tdstVector3D normal = vector3_new(TnA.x, TnA.y, TnA.z);
                                ImVec4 coord_center = project_world_coordinate(vector4_mul_matrix4(vector4_new(center.x, center.y, center.z, 1.0f), T));
                                ImVec4 coord_norm = project_world_coordinate(vector3_add(center, normal));
                                ImVec2 poscenter = ImVec2(off.x + sz.x - coord_center.x * sz.x, off.y + coord_center.y * sz.y);
                                ImVec2 posnorm = ImVec2(off.x + sz.x - coord_norm.x * sz.x, off.y + coord_norm.y * sz.y);
                                
                                /* To the center vector, add the offset of the normal. */
                                //if (normals && 1.0f / vector3_length(vector3_sub(Na, center)) < 1.0f) drawlist->AddLine(poscenter, posnorm, ImColor(normal.x, normal.y, normal.z, 1.0f));
                            }
                        }
                    }
                    else /* Draw edges */
                    {
//                        const uint16* indices = (const uint16*)pointer(mesh->face_edges);
//                        for (int16 index = 0; index < host_byteorder_16(mesh->n_edges); index++)
//                        {
//                            uint16 idx0 = host_byteorder_16(*(indices + index * 3 + 0));
//                            uint16 idx1 = host_byteorder_16(*(indices + index * 3 + 1));
//                            uint16 idx2 = host_byteorder_16(*(indices + index * 3 + 2));
//
//                            const tdstVector3D a = vector3_host_byteorder(*(vertices + idx0));
//                            const tdstVector3D b = vector3_host_byteorder(*(vertices + idx1));
//                            const tdstVector3D c = vector3_host_byteorder(*(vertices + idx2));
//
//                            const tdstVector4D A = vector4_mul_matrix4(vector4_new(a.x, a.y, a.z, 1.0f), T);
//                            const tdstVector4D B = vector4_mul_matrix4(vector4_new(b.x, b.y, b.z, 1.0f), T);
//                            const tdstVector4D C = vector4_mul_matrix4(vector4_new(c.x, c.y, c.z, 1.0f), T);
//
//                            ImVec4 coord_a = project_world_coordinate(A);
//                            ImVec4 coord_b = project_world_coordinate(B);
//                            ImVec4 coord_c = project_world_coordinate(C);
//
//                            if (coord_a.w >= 0 && coord_b.w >= 0)
//                            {
//                                ImVec2 pos1 = ImVec2(off.x + sz.x - coord_a.x * sz.x, off.y + coord_a.y * sz.y);
//                                ImVec2 pos2 = ImVec2(off.x + sz.x - coord_b.x * sz.x, off.y + coord_b.y * sz.y);
//                                ImVec2 pos3 = ImVec2(off.x + sz.x - coord_c.x * sz.x, off.y + coord_c.y * sz.y);
//
//                                ImColor outline = color;
//                                outline.Value.w = 1.0f;
//
//                                ImColor base = outline;
//                                base.Value.w *= 0.25f;
//
//                                drawlist->AddTriangle(pos1, pos2, pos3, base);
//
//                                //drawlist->AddTriangle(pos1, pos2, pos3, outline);
//                                //drawlist->AddTriangleFilled(pos1, pos2, pos3, base);
//                            }
//                        }
                    }
                    
                    mesh_idx++;
                }
            }
        }
    }
    
    /* Draw children */
    superobject_for_each(root, child) DrawLevelGeometryRecursive(child, T, player, off, sz, drawlist, color);
}

static void DrawOctreeRecursive(const tdstOctreeNode* node, const tdstMatrix4D T, ImVec2 off, ImVec2 sz, ImDrawList* drawlist, ImColor color = ImColor(1.0f, 0.75f, 0.0f, 1.0f))
{
    if (!node) return;
    
    tdstVector3D min = vector3_host_byteorder(node->min);
    tdstVector3D max = vector3_host_byteorder(node->max);
    tdstVector4D min4 = vector4_mul_matrix4(vector4_new(min.x, min.y, min.z, 1.0f), T);
    tdstVector4D max4 = vector4_mul_matrix4(vector4_new(max.x, max.y, max.z, 1.0f), T);
    min = vector3_new(min4.x, min4.y, min4.z);
    max = vector3_new(max4.x, max4.y, max4.z);
    
    
    ImVec4 coord_min = project_world_coordinate(min);
    ImVec4 coord_max = project_world_coordinate(max);
    
    if (coord_min.w >= 0.0f && coord_max.w >= 0.0f)
    {
        ImVec2 pos1 = ImVec2(off.x + sz.x - coord_min.x * sz.x, off.y + coord_min.y * sz.y);
        ImVec2 pos2 = ImVec2(off.x + sz.x - coord_max.x * sz.x, off.y + coord_max.y * sz.y);
        
        drawlist->AddNgonFilled(pos1, 5.0f, color, 3);
        drawlist->AddNgonFilled(pos2, 5.0f, color, 3);
        drawlist->AddLine(pos1, pos2, color);
    }
    
    
    const pointer* childlist = (const pointer*)pointer(node->children);
    if (childlist)
    {
//        for (int i = 0; i < 8; i++)
//        {
//            const tdstOctreeNode* node = (const tdstOctreeNode*)pointer(*(childlist + i));
//            DrawOctreeRecursive(node, T, off, sz, drawlist, color);
//        }
    }
}

static void DrawLevelGeometry(const tdstSuperObject* sector)
{
    const float ar = display_size.x / display_size.y;
    const float new_width = display_size.y * (640.0f / 528.0f) * 1.075f;
    const float x_offset = display_size.x - new_width;
    ImGui::SetWindowPos(ImVec2(x_offset / 2.0f, 0));
    ImGui::SetWindowSize(ImVec2(new_width, display_size.y));
    
    ImVec2 off = ImGui::GetWindowPos();
    ImVec2 sz = ImGui::GetWindowSize();
    ImDrawList* drawlist = ImGui::GetBackgroundDrawList();
    
    if (level_geometry_use_cliprect)
        drawlist->PushClipRect(ImVec2(x_offset / 2.0f, 0), ImVec2(new_width + x_offset / 2.0f, display_size.y));
        
    const tdstVector3D playerposition = game_matrix4_position(actor_matrix(actor_rayman));
//    const tdstVector3D speed = vector3_add(playerposition, actor_speed(actor_rayman));
//
//    ImVec4 r = project_world_coordinate(playerposition);
//    ImVec4 spd = project_world_coordinate(speed);
//    ImVec2 rpos = ImVec2(off.x + sz.x - r.x * sz.x, off.y + r.y * sz.y);
//    ImVec2 spos = ImVec2(off.x + sz.x - spd.x * sz.x, off.y + spd.y * sz.y);
//
//    drawlist->AddLine(rpos, spos, ImColor(1.0f, 0.0f, 0.0f, 1.0f));
    
    DrawLevelGeometryRecursive(sector, matrix4_identity, playerposition, off, sz, drawlist);
    
    if (level_geometry_use_cliprect)
        drawlist->PopClipRect();
}
