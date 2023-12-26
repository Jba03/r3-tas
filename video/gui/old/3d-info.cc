static void drawCollisionPlane(stDynamicsObstacle *obstacle, ImVec2 off, ImVec2 sz, ImDrawList* drawlist, ImColor color)
{
    stVector3D origin = vector3_host_byteorder(obstacle->contact);
    stVector3D offset = vector3_host_byteorder(obstacle->normal);
    stVector4D contactPoint = vector4_new(origin.x, origin.y, origin.z, 1.0f);
    stVector4D contactNormal = vector4_new(offset.x, offset.y, offset.z, 1.0f);
    
    /* Create quad vertices */
    stMatrix4D x90 = matrix4_make_rotation_x(M_PI / 2.0f);
    stMatrix4D z90 = matrix4_make_rotation_y(M_PI / 2.0f);
    stVector4D p1 = vector4_mul_matrix4(contactNormal, x90);
    stVector4D p2 = vector4_mul_matrix4(contactNormal, z90);
    stVector4D p3 = vector4_mulf(p1, -1.0f);
    stVector4D p4 = vector4_mulf(p2, -1.0f);
    
    ImVec4 a = project_world_coordinate(vector4_add(contactPoint, p1));
    ImVec4 b = project_world_coordinate(vector4_add(contactPoint, p2));
    ImVec4 c = project_world_coordinate(vector4_add(contactPoint, p3));
    ImVec4 d = project_world_coordinate(vector4_add(contactPoint, p4));
    
    ImVec2 pos1 = ImVec2(off.x + sz.x - a.x * sz.x, off.y + a.y * sz.y);
    ImVec2 pos2 = ImVec2(off.x + sz.x - b.x * sz.x, off.y + b.y * sz.y);
    ImVec2 pos3 = ImVec2(off.x + sz.x - c.x * sz.x, off.y + c.y * sz.y);
    ImVec2 pos4 = ImVec2(off.x + sz.x - d.x * sz.x, off.y + d.y * sz.y);
    
    ImColor fill = color;
    fill.Value.w = 0.5f;
    drawlist->AddQuadFilled(pos1, pos2, pos3, pos4, fill);
    drawlist->AddQuad(pos1, pos2, pos3, pos4, color);
}

static void drawCollisionPlanes(stActor *rayman, ImVec2 off, ImVec2 sz, ImDrawList* drawlist)
{
    stDynamicsReport *report = fnActorGetDynamicsReport(rayman);
    if (!report) return;
    

    
    drawCollisionPlane(&report->obstacle, off, sz, drawlist, ImColor(0.0f, 0.3f, 1.0f, 1.0f));
    drawCollisionPlane(&report->ground, off, sz, drawlist, ImColor(1.0f, 0.2f, 0.9f, 1.0f));
    drawCollisionPlane(&report->wall, off, sz, drawlist, ImColor(1.0f, 0.75f, 0.0f, 1.0f));
    drawCollisionPlane(&report->ceiling, off, sz, drawlist, ImColor(0.4f, 0.2f, 0.8f, 1.0f));
    drawCollisionPlane(&report->character, off, sz, drawlist, ImColor(1.0f, 0.2f, 0.1f, 1.0f));
    //drawCollisionPlane(&report->water, off, sz, drawlist, ImColor(1.0f, 0.2f, 0.1f, 1.0f));
    
    
}
