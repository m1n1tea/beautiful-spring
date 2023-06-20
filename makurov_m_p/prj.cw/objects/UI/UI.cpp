#include "UI/UI.h"
namespace cellworld{

    static void HelpMarker(const char* desc, int width)
    {
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
        {
            ImGui::BeginTooltip();
            ImGui::SetWindowFontScale(width / 4096.0f);
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }



    
    void UI::loadScene()
    {
        switch (scene_)
        {
        case start_screen:
            startScreen();
            break;
        case creation_of_the_world:
            CreationOfTheWorld();
            break;
        case load_world_screen:
            loadWorldScreen();
            break;
        case simulation_of_the_world:
            SimulationOfTheWorld(); 
            break;
        
        default:
            scene_= start_screen;
            break;
        }
        if (scene_is_changed_)
            sceneUpdate(scene_);
        scene_is_changed_ = (scene_ != previous_scene_);
    }
    void UI::sceneUpdate(int scene)
    {
        previous_scene_=scene_;
        scene_=scene;
    }
    void UI::startScreen()
    {
        ImGui::SetNextWindowPos({ width_ * 0.25f,height_ * 0.2f });
        ImGui::SetNextWindowSize({ width_ * 0.5f,height_ * 0.2f });
        ImGui::Begin("Start screen 1", NULL, WindowTemplates::invisible_window);
        ImGui::SetWindowFontScale(width_ / 2048.0f);
        if (ImGui::Button("Create new world", { width_ * 0.5f, height_ * 0.2f })) {
            sceneUpdate(creation_of_the_world);
        }
        ImGui::End();

        ImGui::SetNextWindowPos({ width_ * 0.25f,height_ * 0.6f });
        ImGui::SetNextWindowSize({ width_ * 0.5f,height_ * 0.3f });
        ImGui::Begin("Start screen 2", NULL, WindowTemplates::invisible_window);
        ImGui::SetWindowFontScale(width_ / 2048.0f);
        if (ImGui::Button("Load world", { width_ * 0.5f, height_ * 0.2f })) {
            sceneUpdate(load_world_screen);
        }
        ImGui::End();

    }

    void UI::CreationOfTheWorld()
    {
        static unsigned int size_x=100;
        static unsigned int size_y =50;
        static std::random_device rd;
        static int initial_population=100;
        static int cycle_len=0;
        static float strenght=0;
        if (scene_is_changed_) {
            size_x=scenario_.sizeX();
            size_y = scenario_.sizeY();
            initial_population = scenario_.getInitialPopulation();
            cycle_len=scenario_.cycle_len_;
            scenario_.makeNew();
            scenario_.createTexture();
            scenario_.updateRewardsTexture();
        }

        ImGui::SetNextWindowPos({ 0,height_ * 0.05f });
        ImGui::SetNextWindowSize({ width_ * 1.f,height_ * 0.9f });
        ImGui::Begin("Creation of a world", NULL, WindowTemplates::scroll_bar_only);
        ImGui::SetWindowFontScale(width_ / 3072.0f);

        ImGui::Indent(width_ * 0.35f);
        if (ImGui::Button("Reset all", { width_ * 0.3f,height_ * 0.1f })) {
            size_x = 100;
            size_y = 50;
            seed_ = 0;
            initial_population = 0;
            strenght = 0;
            for (int i = 0; i < coefficients_count; ++i)
                Creature::coeff_[i] = 0;

            scenario_ = Scenario(size_x, size_y);
            scenario_.createTexture();
            scenario_.updateRewardsTexture();
        }




        ImGui::Unindent(width_ * 0.35f);
        ImGui::NewLine();
        ImGui::Indent(width_ * 0.15f);
        ImGui::Checkbox("breed", &Creature::is_breedable);
        ImGui::InputScalar("seed", ImGuiDataType_U32, &seed_);  HelpMarker("if seed_=0, program generates random seed_", width_);
        ImGui::InputScalar("width", ImGuiDataType_U32, &size_x);


        if (ImGui::IsItemDeactivatedAfterEdit() || (size_x > width_ * 0.85f && !ImGui::IsItemActive())) {
            if (size_x > width_ * 0.85f)
                size_x = width_ * 0.85f;
            scenario_.unbindTexture();
            glDeleteTextures(1, &scenario_.getGLTexture());
            scenario_ = Scenario(size_x, size_y);
            scenario_.createTexture();
            scenario_.updateRewardsTexture();
        }

        ImGui::InputScalar("height", ImGuiDataType_U32, &size_y);


        if (ImGui::IsItemDeactivatedAfterEdit() || (size_y > height_ * 0.75f && !ImGui::IsItemActive())) {
            if (size_y > height_ * 0.75f)//
                size_y = height_ * 0.75f;
            scenario_.unbindTexture();
            glDeleteTextures(1, &scenario_.getGLTexture());
            scenario_ = Scenario(size_x, size_y);
            scenario_.createTexture();
            scenario_.updateRewardsTexture();
        }

        ImGui::InputScalar("initial polulation", ImGuiDataType_U32, &initial_population);
        ImGui::NewLine();
        ImGui::Text("Coefficients");

        ImGui::InputFloat("Mutation strength", &Creature::coeff_[mutation_strength]);
        HelpMarker("defines genome differnce between parent and child \n 0 - child is exact copy of parent \n 1 - child's genome is independent from parent", width_);

        ImGui::InputFloat("Change speed module cost", &Creature::coeff_[change_speed_module_cost]);
        HelpMarker("when creature changes speed, creature's energy decreases by coeff*(change_speed_module^2)", width_);

        ImGui::InputFloat("Weight capacity", &Creature::coeff_[mass_capacity]);
        HelpMarker("creature's max energy is coeff*creature's mass", width_);

        ImGui::InputFloat("Weight into energy", &Creature::coeff_[mass_into_energy]);
        HelpMarker("1*mass <=> coeff*energy \n mass transforms into energy when creature dies \n energy transforms into mass when creature creates child ", width_);

        ImGui::InputFloat("Starting energy", &Creature::coeff_[starting_energy]);
        HelpMarker("after spawn creature's energy is coeff*max energy", width_);

        ImGui::InputFloat("Weight cost", &Creature::coeff_[mass_cost]);
        HelpMarker("every simulation step creature's energy decreases by mass*coeff", width_);

        ImGui::InputFloat("Braking force", &Creature::coeff_[braking_force]);
        HelpMarker("every simulation step creature's module speed decreases by coeff", width_);

        ImGui::InputInt("Cycle length", &(cycle_len));
        HelpMarker("if 0 no cycle, else after every n steps of simulation happens new iteration of simulation,\nwhere initial creatures' genome based on survivors of previous iteration", width_);

        ImGui::NewLine();
        ImGui::InputFloat("Strength", &strenght);
        if (ImGui::Button("Cancel", { width_ * 0.3f,height_ * 0.1f })) {
            scenario_.CancelRewardsChange();
            scenario_.updateRewardsTexture();
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset rewards", { width_ * 0.3f,height_ * 0.1f })) {
            scenario_.resetRewards();
            scenario_.updateRewardsTexture();
        }
        ImGui::Unindent(width_ * 0.15f);
        ImGui::NewLine();


        ImGui::Indent(width_ * 0.1f);
        ImGui::BeginChild("Rewards editor", { width_ * 0.85f,height_ * 0.75f },0,WindowTemplates::invisible_window);
        scenario_.rewardsEditor(ImGui::GetWindowPos(), ImGui::GetWindowSize(),strenght,scenario_.getTexture());
        ImGui::EndChild();
        ImGui::Unindent(width_ * 0.1f);
        


        ImGui::Indent(width_ * 0.15f);
        if (ImGui::Button("Return", { width_ * 0.3f, height_ * 0.1f })) {
            glDeleteTextures(1, &scenario_.getGLTexture());
            scenario_.unbindTexture();
            sceneUpdate(start_screen);
        }
        ImGui::SameLine();
        if (ImGui::Button("Create new world", { width_ * 0.3f, height_ * 0.1f })) {
            if (seed_ == 0) {
                seed_ = rd();
            }
            generator_ = std::mt19937(seed_);
            scenario_.spawnCreatures(initial_population);
            scenario_.cycle_len_=cycle_len;
            scenario_.unbindTexture();
            sceneUpdate(simulation_of_the_world);
        }
        ImGui::Unindent(width_ * 0.15f);
        ImGui::End();
    }

    void UI::loadWorldScreen()
    {   
        const std::vector<std::string>& file_names= file_names_.getFileNames();
        ImGui::Begin("Choose world", NULL, WindowTemplates::scroll_bar_only);
        if (scene_is_changed_) {
            file_names_.checkFileNames();
            file_names_.saveFileNames();
        }

        if (ImGui::Button("Return")) {
            sceneUpdate(start_screen);
        }

        ImGui::SetWindowFontScale(width_ / 3072.0f);
        for (int i=file_names.size()-1; i>=0 ; --i) {
            ImGui::PushID(i);
            ImGui::Text(file_names[i].c_str());
            ImGui::SameLine();
            if (ImGui::Button("Load")) {
                loadWorld(file_names[i].c_str(), &scenario_, seed_);
                sceneUpdate(simulation_of_the_world);
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                file_names_.removeFileName(i);
                scene_is_changed_ = 1;
            }
            ImGui::PopID();
            
        }
        ImGui::End();
        
    }

    void UI::SimulationOfTheWorld()
    {

        static bool pause;
        static bool visualise;
        static bool show_rewards;
        static bool options_window;
        static bool info_window;
        static std::string file_name;
        static GLuint rewards_texture_;
        if (scene_is_changed_) {
            pause=1;
            visualise=1;
            show_rewards=0;
            options_window=1;
            info_window=0;

            scenario_.createTexture();
            scenario_.updateRewardsTexture();
            rewards_texture_ =scenario_.getGLTexture();
            scenario_.unbindTexture();

            scenario_.createTexture();
            file_name="New world";
        }  
        float fps = ImGui::GetIO().Framerate;
        ImGui::SetNextWindowPos({ 0,0 });
        ImGui::SetNextWindowSize({ width_ * 1.f,height_ * 0.1f });
        ImGui::Begin("Interface", NULL, WindowTemplates::invisible_window | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus );
        ImGui::SetWindowFontScale(width_ / 3584.0f);

        if (ImGui::Button("Save as")) {
            if (file_names_.findFileName(file_name.c_str())) {
                std::string new_name = file_names_.getValidFileName(file_name);
                saveWorld(new_name.c_str(), &scenario_, seed_);
                file_names_.addFileName(new_name.c_str());
                file_names_.saveFileNames();
            }
            else {
                saveWorld(file_name.c_str(), &scenario_, seed_);
                file_names_.addFileName(file_name.c_str());
                file_names_.saveFileNames();
            }
        }
        ImGui::SameLine();
        ImGui::InputText("##foo", &file_name);

        ImGui::SameLine();
        if (ImGui::Button("Make one step")) {
            scenario_.makeOneStep();
        }


        ImGui::SameLine();
        if (ImGui::Button("Options")) {
            options_window=1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Info")) {
            info_window = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Return")) {
            sceneUpdate(start_screen);
            glDeleteTextures(1, &scenario_.getGLTexture());
            glDeleteTextures(1, &rewards_texture_);
            scenario_.unbindTexture();
        }
        ImGui::End();


        if (!pause) {
            scenario_.makeOneStep();
        }

        if (visualise) {
            ImGui::SetNextWindowPos({ width_ * 0.01f,height_ * 0.1f });
            ImGui::SetNextWindowSize({ width_ * 0.99f,height_ * 0.9f });
            ImGui::Begin("Field", NULL, WindowTemplates::invisible_window | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
            scenario_.updateTexture();
            int square_size= width_ * 0.98f/ scenario_.sizeX();
            if (square_size>height_*0.85f/ scenario_.sizeY())
                    square_size= height_ * 0.85f / scenario_.sizeY();
            ImGui::Image(scenario_.getTexture(), {scenario_.sizeX() * square_size * 1.f,  scenario_.sizeY() * square_size * 1.f});
            if (show_rewards) {
                ImGui::SameLine(8.f);//не понимаю почему 8
                ImGui::Image((void*)(rewards_texture_), { scenario_.sizeX() * square_size * 1.f,  scenario_.sizeY() * square_size * 1.f });
            }
            ImGui::End();
        }
        if (options_window){
            ImGui::Begin("Options", &options_window);
            ImGui::SetWindowFontScale(width_ / 3584.0f);
            ImGui::Checkbox("Paused", &pause);
            ImGui::Checkbox("Visualise", &visualise);
            ImGui::Checkbox("Show Rewards", &show_rewards);
            ImGui::End();
        }
        if (info_window) {
            ImGui::Begin("Info", &info_window);
            ImGui::SetWindowFontScale(width_ / 3584.0f);
            ImGui::Text("FPS: %f",fps);
            ImGui::Text("Iteration: %i",scenario_.getIteration());
            ImGui::Text("Alive creatures: %i", scenario_.getAlive());
            ImGui::Text("Dead creatures: %i", scenario_.getDead());
            ImGui::End();
        }
    }




}
