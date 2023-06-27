#include "scenario/scenario.h"
namespace cellworld{
    //если что,начальная популяция равеа 100 по умолчанию
    Scenario::Scenario(int size_x, int size_y) : Field(size_x, size_y), initial_population_(100), cycle_len_(0), iteration_(0), 
              rewards_(size_x*size_y,0), rewards_backup_(size_x* size_y, 0),positions_(size_x* size_y, 0){
    survivors_.reserve(size());
}

    void Scenario::makeOneStep(){
        updatePositions();
        updateStates();
        giveRewards();
        ++iteration_;
        if (cycle_len_>0 && iteration_ % cycle_len_==0) {
            newCycle();
        }
    }

    void Scenario::makeNew() {
        iteration_=0;
        clear();
    }

    void Scenario::newCycle() {
        survivors_.clear();
        for (int i = 0; i < size(); ++i) {
            if (getCreature(i).getState() == alive) {
                survivors_.push_back(zoo_ptr_[i]);
            }
        }
        std::swap(zoo_ptr_, empty_zoo_ptr_);
        if (survivors_.empty()) {
            for (int i = 0; i < size_x_; ++i) {
                for (int j = 0; j < size_y_; ++j) {
                    empty_zoo_ptr_[i * size_y_ + j]->stopExisting();
                    empty_zoo_ptr_[i * size_y_ + j]->pos_x_ = i;
                    empty_zoo_ptr_[i * size_y_ + j]->pos_y_ = j;
                }
            }
            spawnCreatures(initial_population_);
            return;
        }
            
        std::shuffle(survivors_.begin(), survivors_.end(), generator_);
        dead_count_ = 0;
        alive_count_ = initial_population_;
        int random_creatures=initial_population_/8;

        int even_part= (initial_population_-random_creatures)/survivors_.size();
        int random_part=(initial_population_-random_creatures) % survivors_.size();
        
        
        for (int i = 0; i < size(); ++i) {
            positions_[i] = i;
        }
        std::shuffle(positions_.begin(), positions_.end(), generator_);
        int spawned_creatures = 0;

        for (int i = 0; i < random_creatures; ++i) {
            Position current = { positions_[spawned_creatures] / sizeY(), positions_[spawned_creatures] % sizeY() };
            ++spawned_creatures;
            getCreature(current).makeAlive(current);
        }

        for (Creature* ancestor : survivors_) {
          
            for(int i=0;i<even_part;++i){
                Position current = { positions_[spawned_creatures] / sizeY(), positions_[spawned_creatures] % sizeY() };
                ++spawned_creatures;
                getCreature(current).makeAlive(*ancestor,current);
                getCreature(current).addEnergy(-getCreature(current).getEnergy()+ getCreature(current).getEnergyLimit() * Creature::coeff_[starting_energy]);
            }
            
        }

        for (int i = 0; i < random_part; ++i) {
            Position current = { positions_[spawned_creatures] / sizeY(), positions_[spawned_creatures] % sizeY() };
            ++spawned_creatures;
            getCreature(current).makeAlive(*survivors_[i], current);
            getCreature(current).addEnergy(-getCreature(current).getEnergy() + getCreature(current).getEnergyLimit() * Creature::coeff_[starting_energy]);
        }

        for (int i = 0; i < size_x_; ++i) {
            for (int j = 0; j < size_y_; ++j) {
                empty_zoo_ptr_[i * size_y_ + j]->stopExisting();
                empty_zoo_ptr_[i * size_y_ + j]->pos_x_ = i;
                empty_zoo_ptr_[i * size_y_ + j]->pos_y_ = j;
            }
        }
    }

    void Scenario::spawnCreatures(int amount){
        if (amount>size())
            amount=size();
        initial_population_=amount;
        alive_count_ = amount;
        for (int i = 0; i < size(); ++i) {
            positions_[i]=i;
        }
        std::shuffle(positions_.begin(),positions_.end(),generator_);
        for (int i = 0; i < amount; ++i) {
            Position current={positions_[i] / sizeY(), positions_[i] % sizeY()};
            getCreature(current)=Creature(Creature::generateGenome(),current);
        }
    }


    void Scenario::makeRewards(Position pos1, Position pos2, float strength){
        if (pos2.first < pos1.first)
            std::swap(pos1.first, pos2.first);

        if (pos2.second < pos1.second)
            std::swap(pos1.second, pos2.second);

        if (pos1.first < 0)
            pos1.first = 0;

        if (pos1.second < 0)
            pos1.second = 0;

        if (pos2.first > sizeX())
            pos2.first = sizeX();

        if (pos2.second > sizeY())
            pos2.second = sizeY();
        rewards_backup_ = rewards_;
        for (int i = pos1.first; i <= pos2.first; ++i) {
            for (int j = pos1.second; j <= pos2.second; ++j) {
                rewards_[i * sizeY() + j] = strength;
            }
        }
    }
    
    void Scenario::resetRewards() {
        rewards_backup_ = rewards_;
        for (float& x : rewards_) {
            x = 0;
        }
    }


    
    
    void Scenario::updateRewardsTexture() {
        for (int i = 0; i < sizeX(); ++i) {
            for (int j = 0; j < sizeY(); ++j) {
                if (rewards_[i*sizeY()+j]==0){
                    colors_[j * sizeX() + i]=0;
                }
                else{
                    colors_[j * sizeX() + i] = Creature::energyColor(rewards_[i * sizeY() + j]);
                    colors_[j * sizeX() + i] ^= 128;
                }
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sizeX(), sizeY(), 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &(colors_[0]));
    }
     

    


    Position Scenario::convertInput(ImVec2 begin, ImVec2 input, int square_size) {
        ImVec2 relative_pos={input.x-begin.x,input.y-begin.y};
        Position result={relative_pos.x/square_size,relative_pos.y / square_size};
        return result;
    }

    void Scenario::rewardsEditor(ImVec2 window_pos, ImVec2 window_size, float strength, ImTextureID texture) {
        int square_size = window_size.x / sizeX();
        if (square_size > window_size.y / sizeY())
            square_size = window_size.y / sizeY();
        ImVec2 opposite_corner={window_pos.x+ sizeX() * square_size,window_pos.y + sizeY() * square_size};
        unsigned int base_color = Creature::base_color_;
        base_color=IM_COL32(base_color>>24, ((base_color >> 16) & 0xff), ((base_color >> 8) & 0xff), (base_color & 0xff));
        ImGui::GetWindowDrawList()->AddRectFilled(window_pos, opposite_corner, base_color);
        ImGui::InvisibleButton("##foo",window_size);
        if (ImGui::IsItemDeactivated()) {
            Position pos1 = convertInput(window_pos, ImGui::GetIO().MouseClickedPos[0], square_size);
            Position pos2= convertInput(window_pos, ImGui::GetIO().MousePos, square_size);
            
            makeRewards(pos1, pos2, strength);
            updateRewardsTexture();
        }
        ImGui::SameLine(0.01f);
        ImGui::Image(texture, { sizeX() * square_size * 1.f, sizeY() * square_size * 1.f });
    }
    


    //std::pair<int,float> Scenario::getState(ImVec2 begin, int square_size) {
    //    ImVec2 mouse_pos= ImGui::GetIO().MousePos;
    //    mouse_pos.x -= 8;
    //    mouse_pos.y -= 8;
    //    Position pos= convertInput(begin, mouse_pos,square_size);
    //    if (validX(pos.first) && validY(pos.second)) {
    //        return {getCreature(pos).getState(),getCreature(pos).getEnergy()};
    //    }
    //    else {
    //        return { -1 ,-1} ;
    //    }
    //    
    //}

#if OPEN_MP_FOUND == 1
    void Scenario::giveRewards() {
        #pragma omp parallel for
        for (int i = 0; i < size(); ++i) {
            if (getCreature(i).getState() == alive && rewards_[i]!=0.f) {
                getCreature(i).addEnergy(rewards_[i]);
            }
        }
    }
#else
    void Scenario::giveRewards() {
        for (int i = 0; i < size(); ++i) {
            if (getCreature(i).getState() == alive && rewards_[i] != 0.f) {
                getCreature(i).addEnergy(rewards_[i]);
            }
        }
    }
#endif

    void saveWorld(const char* path, Scenario* current_field, const unsigned int& seed) {
        std::ofstream safe_file(path, std::ios::binary);
        if (!safe_file) {
            safe_file.close();
            return;
        }
        int x = current_field->sizeX();
        int y = current_field->sizeY();

        int saved = 1;
        safe_file << saved << ' ';
        safe_file << x << ' ' << y << ' ' << seed << ' ';
        for (int i = 0; i < coefficients_count; ++i) {
            safe_file << Creature::coeff_[i] << ' ';
        }
        safe_file << Creature::is_breedable << " ";

        constexpr int creature_size = sizeof(Genome) + sizeof(float) * 2 + sizeof(int) * 5;
        char* converted = new char[creature_size];
        for (int i = 0; i < x * y; ++i) {
            Creature& current = current_field->getCreature(i);
            std::memcpy(converted, &current, creature_size - sizeof(NeuronNetwork));
            std::memcpy(converted + creature_size - sizeof(NeuronNetwork), current.networkPtr(), sizeof(NeuronNetwork));
            safe_file.write(converted, creature_size);
        }
        delete[] converted;


        converted = new char[4 * current_field->size()];

        std::memcpy(converted, &current_field->rewards_[0], 4 * current_field->size());
        safe_file.write(converted, 4 * current_field->size());

        std::memcpy(converted, &current_field->rewards_backup_[0], 4 * current_field->size());
        safe_file.write(converted, 4 * current_field->size());

        delete[] converted;
        safe_file << ' ' << current_field->cycle_len_ << ' ' << current_field->iteration_ << ' ' << current_field->initial_population_ << ' ' << current_field->alive_count_ << ' ' << current_field->dead_count_ << '\n';
        safe_file << generator_ << " " << dis;
        safe_file.close();
    }

    void loadWorld(const char* path, Scenario* current_field, unsigned int& seed) {
        Scenario tmp = std::move(*current_field);
        std::ifstream safe_file(path, std::ios::binary);
        if (!safe_file) {
            safe_file.close();
            return;
        }
        int x, y;
        int check;
        safe_file >> check;
        if (check != 1) {
            safe_file.close();
            return;
        }

        safe_file >> x >> y >> seed;
        for (int i = 0; i < coefficients_count; ++i) {
            safe_file >> Creature::coeff_[i];
        }
        safe_file >> Creature::is_breedable;


        *current_field = Scenario(x, y);
        constexpr int creature_size = sizeof(Genome) + sizeof(float) * 2 + sizeof(int) * 5;
        char* converted = new char[creature_size];
        safe_file.read(converted, 1);
        for (int i = 0; i < x * y; ++i) {
            Creature& current = current_field->getCreature(i);
            safe_file.read(converted, creature_size);
            std::memcpy(&current, converted, creature_size - sizeof(NeuronNetwork));
            std::memcpy(current.networkPtr(), converted + creature_size - sizeof(NeuronNetwork), sizeof(NeuronNetwork));
            current.buildIO();
        }
        delete[] converted;
        


        converted = new char[4 * current_field->size()];

        safe_file.read(converted, 4 * current_field->size());
        std::memcpy(&current_field->rewards_[0], converted, 4 * current_field->size());
        safe_file.read(converted, 4 * current_field->size());
        std::memcpy(&current_field->rewards_backup_[0], converted, 4 * current_field->size());

        delete[] converted;

        safe_file >> current_field->cycle_len_ >> current_field->iteration_ >> current_field->initial_population_ >> current_field->alive_count_ >> current_field->dead_count_;
        safe_file >> generator_ >> dis;
        safe_file.close();
    }
    

}




