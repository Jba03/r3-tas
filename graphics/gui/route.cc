static bool recording_route = false;

static void DrawRoute(struct route route, ImVec2 offset, ImVec2 size, ImDrawList* drawlist)
{
    if (xrayHandle.sourceRecord.numNodes != 0)
    {
        for (int i = 0; i < xrayHandle.sourceRecord.numNodes - 1; i++)
        {
            tdstVector3D v1 = xrayHandle.sourceRecord.nodes[i].position;
            tdstVector3D v2 = xrayHandle.sourceRecord.nodes[i + 1].position;
            
            ImVec4 projected1 = project_world_coordinate(v1);
            ImVec4 projected2 = project_world_coordinate(v2);
            ImVec2 screen1 = ImVec2(offset.x + size.x - projected1.x * size.x, offset.y + projected1.y * size.y);
            ImVec2 screen2 = ImVec2(offset.x + size.x - projected2.x * size.x, offset.y + projected2.y * size.y);
            
            if (screen1.x >= 0 && screen1.x < display_size.x && screen1.y >= 0 && screen1.y < display_size.y && projected1.z > 0)
            {
                drawlist->AddLine(screen1, screen2, ImColor(route.color.x, route.color.y, route.color.z, 1.0f));
                
                v1 = vector3_add(v1, xrayHandle.sourceRecord.nodes[i + 0].normal);
                v2 = vector3_add(v2, xrayHandle.sourceRecord.nodes[i + 1].normal);
                projected1 = project_world_coordinate(v1);
                projected2 = project_world_coordinate(v2);
                screen1 = ImVec2(offset.x + size.x - projected1.x * size.x, offset.y + projected1.y * size.y);
                screen2 = ImVec2(offset.x + size.x - projected2.x * size.x, offset.y + projected2.y * size.y);
                drawlist->AddLine(screen1, screen2, ImColor(1.0f, 0.0f, 1.0f, 1.0f));
            }
        }
    }
}

static void DisplayRouteWindow()
{
    ImGui::Begin("Current routes");
    
    if (!recording_route)
    {
        if (ImGui::Button("Start new route"))
        {
            xrayInitialize(&xrayHandle);
            //xraySourceRecordInit(&sourceRecord);
            recording_route = true;
            current_route = n_routes;
            initializeRoute(&routes[current_route]);
            ImVec4 color = ImColor(color_table_index(current_route));;
            routes[current_route].color = vector3_new(color.x, color.y, color.z);
            n_routes++;
        }
        
        ImGui::Spacing();
        for (int i = 0; i < n_routes; i++)
        {
            struct route route = routes[i];
            const int ms = (int)(route.complete_time * (1000.0 / 60.0)) % 1000;
            const int s  = (route.complete_time / 60) % 60;
            const int m  = (route.complete_time / 60 / 60) % 60;
            const int h  = (route.complete_time / 60 / 60 / 60) % 24;
            
            ImGui::TextColored(ImColor(route.color.x, route.color.y, route.color.z), "Route #%d: %02d:%02d:%02d.%03d", i+1, h, m, s, ms);
        }
    }
    else
    {
        if (ImGui::Button("Stop route"))
        {
            recording_route = false;
            xraySourceFinished(&xrayHandle);
            //xraySourceRecordOpen(&sourceRecord);
            //printf("num octrees: %d\n", sourceRecord.numOctrees);
        }
    }
    
    ImGui::End();
    
    if (recording_route)
    {
        const tdstDynamics* dynamics = fnActorGetDynamics(actor_rayman);
        tdstDynamicsReport* report = (tdstDynamicsReport*)pointer(dynamics->base.report);
        tdstSuperObject* obstacle = (tdstSuperObject*)pointer(report->ground.superobject);
        
        xrayNode node;
        node.position = vector3_host_byteorder(report->ground.contact); //vector3_add(actor_position(actor_rayman), vector3_host_byteorder(report->ground.normal));
        node.normal = vector3_host_byteorder(report->ground.normal);
        node.type = XRAY_NODE_TYPE_WALK;
        node.superobject = obstacle;
        
        xraySourceAddNode(&xrayHandle, node);
        //xraySourceRecordAppendObject(&sourceRecord, obstacle);
        //xraySourceRecordAppendNode(&sourceRecord, node);
        //printf("n obstacles: %d\n", xrayHandle.sourceRecord.numObjects);
//        if ((host_byteorder_32(engine->timer.frame) % 2) == 0)
//        {
//            tdstVector3D v = actor_position(actor_rayman);
//            recordRoute(&routes[current_route], v);
//        }
    }
}
