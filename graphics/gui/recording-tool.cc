static bool is_recording = false;
static bool is_playback = false;
static int recording_frame = 0;
static int n_frames = 0;
static int start_at = 0;

static FILE* recording_file = NULL;

extern const char* (*get_config_path)(void);

static void record_input()
{
    /* Write */
    fwrite(input_struct, sizeof(struct input_structure), 1, recording_file);
    ++recording_frame;
}

static void playback_input()
{
    ++recording_frame;
    if (fread(input_struct, sizeof(struct input_structure), 1, recording_file) == 0 || recording_frame == n_frames)
    {
        is_playback = false;
        recording_frame = 0;
        fclose(recording_file);
    }
}

static void display_recording_tool(bool *display)
{
    if (*display && configuration.cheats.enabled)
    {
        if (is_recording) ImGui::Begin("Lazy recording tool");
        else ImGui::Begin("Lazy recording tool", display);
        
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "1. Load a savestate, wait a few seconds, then start recording.");
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "2. Stop the recording when desired");
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "3. Load the same savestate again and start the playback");
        ImGui::TextColored(ImVec4(1.0f, 0.75f, 0.75f, 0.5f), "High risk of desync.");
        
        ImGui::Spacing();
        
        if (!is_recording && !is_playback) ImGui::TextColored(ImVec4(0.75f, 0.0f, 0.0f, 1.0f), "Status: Not recording");
        if (is_recording) ImGui::TextColored(ImVec4(0.1f, 0.5f, 1.0f, 1.0f), "Recording frame %d (%.3fmb)", recording_frame, (recording_frame * sizeof(struct input_structure)) / 10e+5);
        if (is_playback && host_byteorder_32(engine->timer.frame) < start_at) ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "%d", host_byteorder_32(engine->timer.frame) - start_at);
        if (is_playback) ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Status: Playing back frame (%d / %d)", recording_frame, n_frames);
        
        
        if (!is_recording)
        {
            if (!is_playback)
            {
                if ((is_playback = ImGui::Button("Start playback")))
                {
                    std::string path = std::string(get_config_path()) + "/input.r3c";
                    recording_file = fopen(path.c_str(), "rb");
                    fread(&n_frames, sizeof(int), 1, recording_file);
                    fread(&start_at, sizeof(int), 1, recording_file);
                    printf("should start at: %d, currently: %d\n", start_at, host_byteorder_32(engine->timer.frame));
                }
            }
            else
            {
                if (host_byteorder_32(engine->timer.frame) >= start_at)
                {
                    playback_input();
                }
                
                if (!(is_playback = !ImGui::Button("Stop playback")))
                {
                    recording_frame = 0;
                    fclose(recording_file);
                }
            }
        }
        
        if (!is_playback)
        {
            if (!is_recording)
            {
                if ((is_recording = ImGui::Button("Start recording")))
                {
                    std::string path = std::string(get_config_path()) + "/input.r3c";
                    recording_file = fopen(path.c_str(), "wb");
                    
                    printf("started recording frame %d\n", host_byteorder_32(engine->timer.frame)+1);
                    int c = host_byteorder_32(engine->timer.frame)+1;
                    fputc(0, recording_file);
                    fputc(0, recording_file);
                    fputc(0, recording_file);
                    fputc(0, recording_file);
                    fwrite(&c, sizeof(int), 1, recording_file);
                }
            }
            else
            {
                record_input();
                if (!(is_recording = !ImGui::Button("Stop recording")))
                {
                    fseek(recording_file, 0, SEEK_SET);
                    fwrite(&recording_frame, sizeof(int), 1, recording_file);
                    recording_frame = 0;
                    fclose(recording_file);
                }
            }
        }
        
        ImGui::End();
    }
}
