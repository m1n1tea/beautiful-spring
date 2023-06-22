#include "creature/creature.h"
namespace cellworld{








Genome Creature::generateGenome() {
    
    Genome res;
        
        res.mass = generator_();
        res.mass >>= 24; // вес в диапозоне от 0 до 255
        res.mass += 1;
        res.color = generator_();
        res.color |= 0x0000ffff; // убираем прозрачность + у всех живых существ синий цвет на 100%
        for (int i = 0; i < res.neuron_network.size(); ++i) {
            res.neuron_network(i) = dis(generator_);
        }
    return res;
}

void Creature::generateGenome(Genome& res) {
        res.mass = generator_();
        res.mass >>= 24;
        res.mass += 1;
        res.color = generator_();
        res.color |= 0x0000ffff;
        for (int i = 0; i < res.neuron_network.size(); ++i) {
            res.neuron_network(i) = dis(generator_);
        }
}


Creature::Creature(const Genome& genome, const Position& pos):
  state_(alive),creatures_genome_(genome),
  pos_x_(pos.first), pos_y_(pos.second), speed_direction_(0), speed_module_(0), 
  energy_limit_(genome.mass*coeff_[mass_capacity]),
  energy_(genome.mass* coeff_[mass_capacity] * coeff_[starting_energy]),
  input_neurons_(Eigen::MatrixXf::Zero((4 * look_input_count + input_neurons_count), 1)), output_neurons_() {}

Creature::Creature(float energy, const Position& pos):
  state_(dead), creatures_genome_(energyColor(energy)),
  pos_x_(pos.first), pos_y_(pos.second), speed_direction_(0),
  speed_module_(0), energy_limit_(0), energy_(energy),
  input_neurons_(), output_neurons_() {}

Creature::Creature(): 
  state_(not_exist), creatures_genome_(base_color_),
  pos_x_(0), pos_y_(0), speed_direction_(0),
  speed_module_(0), energy_limit_(0), energy_(0),
    input_neurons_(), output_neurons_() {buildIO();}









int& Creature::getX() {
    std::get<0>(tmp_)=pos_x_;
    return std::get<0>(tmp_);
}

int& Creature::getY() {
    std::get<1>(tmp_) = pos_y_;
    return std::get<1>(tmp_);
}


void Creature::mixGen(float& gen1,const float& gen2) {
    gen1*=(1.0f-coeff_[mutation_strength]);
    gen1+=gen2* coeff_[mutation_strength];
}


unsigned int Creature::mixGen(const unsigned int& gen1, const unsigned int& gen2) {
    return gen1 * (1.0f - coeff_[mutation_strength]) + gen2 * coeff_[mutation_strength];
}

Genome Creature::createGenome(const Genome& ancestor) {
    Genome kids_genome= ancestor;
    if (coeff_[mutation_strength]<=0){
        return kids_genome;
    }
    Genome mutations=generateGenome();
    if (coeff_[mutation_strength] >= 1) {
        return mutations;
    }
    unsigned int kids_red = ((kids_genome.color >> 24) & 0xff);
    unsigned int kids_green = ((kids_genome.color >> 16) & 0xff);
    unsigned int mutation_red = ((mutations.color >> 24) & 0xff);
    unsigned int mutation_green = ((mutations.color >> 16) & 0xff);
    kids_red= mixGen(kids_red, mutation_red);
    kids_red&=0xff;
    kids_green = mixGen(kids_green, mutation_green);
    kids_green &= 0xff;
    kids_genome.color=(kids_red << 24) + (kids_green << 16);
    kids_genome.color |= 0x0000ffff;
       kids_genome.mass = mixGen(kids_genome.mass, mutations.mass);
    for (int i = 0; i < kids_genome.neuron_network.size(); ++i) {
             mixGen(kids_genome.neuron_network(i), mutations.neuron_network(i));
    }
    return kids_genome;
}


void Creature::die() {
    if(state_==alive){
        state_= dead;
        energy_+=creatures_genome_.mass * coeff_[mass_into_energy];
        creatures_genome_.color=energyColor(energy_);
    }
}

void Creature::eat(Creature& victim) {
    energy_+=victim.energy_;
    victim.energy_=0;
    if (state_ == dead)
        creatures_genome_.color = energyColor(energy_);
}

void Creature::addEnergy(const float& energy) {
    energy_ += energy;
    if (state_ == dead) {
        creatures_genome_.color = energyColor(energy_);
    }
    if (state_ == not_exist) {
        state_=dead;
        creatures_genome_.color = energyColor(energy_);
    }
        
}

unsigned int Creature::energyColor(int energy) {
    if (energy > 1023)
        energy = 1023;
    if (energy < -1023)
        energy = -1023;
    energy /=8;
    return (((127 - energy) << 24) + ((127 + energy) << 16) | 0x000000ff);
}


bool operator==(const Creature& lhs, const Creature& rhs)
{
    if (lhs.pos_x_ != rhs.pos_x_)
        return 0;
    if (lhs.pos_y_ != rhs.pos_y_)
        return 0;
    if (lhs.state_ != rhs.state_)
        return 0;
    if (lhs.energy_ != rhs.energy_)
        return 0;
    if (lhs.energy_limit_ != rhs.energy_limit_)
        return 0;
    if (lhs.speed_module_ != rhs.speed_module_)
        return 0;
    if (lhs.speed_direction_ != rhs.speed_direction_)
        return 0;
    if (lhs.creatures_genome_.color != rhs.creatures_genome_.color)
        return 0;
    if (lhs.creatures_genome_.mass != rhs.creatures_genome_.mass)
        return 0;
    if (lhs.creatures_genome_.neuron_network != rhs.creatures_genome_.neuron_network)
        return 0;


    return 1;
}

//Плохо работает в многопотоке
void conjoin( Creature*& champion, Creature*& candidate) {
        
    if (champion->getState() == not_exist) {
        std::swap(champion,candidate);
        return;
    } 
    if (candidate->getState() == alive && (champion->getState() == dead || (champion->getMass() * champion->getSpeed() < candidate->getMass() * candidate->getSpeed()))) {
        std::swap(champion, candidate);
    }
    champion->eat(*candidate);
}

void conjoin(Creature& champion, Creature& candidate) {

    if (champion.getState() == not_exist) {
        std::swap(champion, candidate);
        return;
    }
    if (candidate.getState() == alive && (champion.getState() == dead || (champion.getMass() * champion.getSpeed() < candidate.getMass() * candidate.getSpeed()))) {
        champion.die();
        candidate.eat(champion);
        std::swap(champion, candidate);
        return;
    }
    champion.eat(candidate);
}

void Creature::buildIO(){
    input_neurons_ = Eigen::MatrixXf::Zero((4 * look_input_count + input_neurons_count), 1);
    output_neurons_= Eigen::MatrixXf::Zero(output_neurons_count, 1);
}




void Creature::reverseInput() {
    std::vector<unsigned int> byte_copy(input_neurons_.size());
    std::memcpy(&byte_copy[0], &input_neurons_.coeffRef(0), input_neurons_.size()*4);

    for (int i = 0; i < input_neurons_.size(); i++) {
        if (input_neurons_.coeffRef(i)<1)
            continue;
        byte_copy[i] = 0x7EEEEEEE - byte_copy[i];
    }

    std::memcpy(&input_neurons_.coeffRef(0), &byte_copy[0], input_neurons_.size()*4);
}



void Creature::getInfo() {
        input_neurons_.coeffRef(pos_x) = pos_x_;
        input_neurons_.coeffRef(pos_y) = pos_y_;
        input_neurons_.coeffRef(speed_module)=speed_module_;
        input_neurons_.coeffRef(energy) = energy_;
        input_neurons_.coeffRef(bias) = 1;
}

void Creature::look(Creature& found, int direction){
    if (direction == up) {
        input_neurons_.coeffRef(direction* look_input_count+input_neurons_count)=getY()-found.getY();
        
    }
    if (direction == down) {
        input_neurons_.coeffRef(direction * look_input_count+ input_neurons_count) = found.getY()-getY();
    }
    if (direction == left) {
        input_neurons_.coeffRef(direction * look_input_count+ input_neurons_count) = getX() - found.getX();
    }
    if (direction == right) {
        input_neurons_.coeffRef(direction * look_input_count+ input_neurons_count) = found.getX() - getX();
    }
    input_neurons_.coeffRef(direction * look_input_count + color_red+ input_neurons_count) = found.getRed();
    input_neurons_.coeffRef(direction * look_input_count + color_green+ input_neurons_count) = found.getGreen();
    input_neurons_.coeffRef(direction * look_input_count + color_blue+ input_neurons_count) = found.getBlue();
}

void Creature::think(){
    output_neurons_.noalias() = creatures_genome_.neuron_network*input_neurons_;
    
}

void Creature::act(){
    if (state_==alive){
        speed_direction_=0;
        if (output_neurons_.coeff(vertical_or_horizontal) > 0) {// <=0 вертикально, >0 горизонтально
            speed_direction_+=2;
        }
        if (output_neurons_.coeff(decrease_or_increase) > 0) {// <=0 уменьшаем, >0 увеличиваем
            ++speed_direction_;
        }
        int& actual_speed_change=std::get<0>(tmp_);
        actual_speed_change= output_neurons_.coeff(change_speed_module);
        speed_module_+= actual_speed_change;
        if (speed_module_ < 0) {
            actual_speed_change-=speed_module_;
            speed_module_=0;
        }
        energy_-= actual_speed_change * actual_speed_change* coeff_[change_speed_module_cost];
        energy_ -= creatures_genome_.mass * coeff_[mass_cost];
    }
    if (speed_direction_ == up) {
        pos_y_-=speed_module_;
    }
    if (speed_direction_ == right) {
        pos_x_ += speed_module_;
    }
    if (speed_direction_ == down) {
        pos_y_ += speed_module_;
    }
    if (speed_direction_ == left) {
        pos_x_ -= speed_module_;
    }
    speed_module_ -= coeff_[braking_force];
    if (speed_module_ < 0) {
        speed_module_ = 0;
    }
    
    

}
//Плохо работает в многопотоке
void Creature::makeAlive(Creature& ancestor,const Position& pos) {
    state_=alive;
    pos_x_=pos.first;
    pos_y_=pos.second;
    Genome& kids_genome=creatures_genome_;
    Genome& ancestors_genome= ancestor.creatures_genome_;
    if (coeff_[mutation_strength] > 0 && coeff_[mutation_strength] < 1) {
        generateGenome(kids_genome);
        unsigned int kids_red = ((ancestors_genome.color >> 24) & 0xff);
        unsigned int kids_green = ((ancestors_genome.color >> 16) & 0xff);
        unsigned int mutation_red = ((kids_genome.color >> 24) & 0xff);
        unsigned int mutation_green = ((kids_genome.color >> 16) & 0xff);
        kids_red = mixGen(kids_red, mutation_red);
        kids_green = mixGen(kids_green, mutation_green);
        kids_red &= 0xff;
        
        kids_green &= 0xff;
        kids_genome.color = (kids_red << 24) | (kids_green << 16);
        kids_genome.color |= 0x0000ffff;
        kids_genome.mass = mixGen(kids_genome.mass, ancestors_genome.mass);
        for (int i = 0; i < kids_genome.neuron_network.size(); ++i) {
            mixGen(kids_genome.neuron_network(i), ancestors_genome.neuron_network(i));
        }
    }
    else if (coeff_[mutation_strength] <= 0) {
        kids_genome=ancestors_genome;
    }
    else if (coeff_[mutation_strength] >=1) {
        generateGenome(kids_genome);
    }
    ancestor.energy_ -= kids_genome.mass * coeff_[mass_into_energy];
    ancestor.energy_ /= 2.0f;
    energy_+= ancestor.energy_;
    energy_limit_ = kids_genome.mass * coeff_[mass_capacity];
    output_neurons_.coeffRef(reproduce) = 0;
}

void Creature::makeAlive(const Position& pos) {
    state_ = alive;
    pos_x_ = pos.first;
    pos_y_ = pos.second;
    Genome& kids_genome = creatures_genome_;
    generateGenome(kids_genome);
    energy_limit_ = kids_genome.mass * coeff_[mass_capacity];
    energy_=energy_limit_*coeff_[starting_energy];
    input_neurons_ = Eigen::MatrixXf::Zero((4 * look_input_count + input_neurons_count), 1);
}

float Creature::Leftover() {
    std::get<2>(tmp_)= energy_- energy_limit_;
    energy_=energy_limit_;
    return std::get<2>(tmp_);
}







Field::Field(int size_x, int size_y) : size_x_(size_x), size_y_(size_y), size_(size_x* size_y), zoo_ptr_(size_x* size_y), empty_zoo_ptr_(size_x* size_y),
storage_(2*size_x* size_y), colors_(size_x* size_y, Creature::base_color_), dead_count_(0),alive_count_(0) {
    for (int i=0;i<size_;++i) {
        zoo_ptr_[i]=&storage_[i];
    }
        
    for (int i = 0; i < size_; ++i) {
        empty_zoo_ptr_[i] = &storage_[i+size_];
    }

    for (int i = 0; i < size_x_; ++i) {
        for (int j = 0; j < size_y_; ++j) {
            zoo_ptr_[i * size_y + j]->pos_x_ = i;
            zoo_ptr_[i * size_y + j]->pos_y_ = j;
            empty_zoo_ptr_[i * size_y + j]->pos_x_ = i;
            empty_zoo_ptr_[i * size_y + j]->pos_y_ = j;
        }
    }
}





Creature& Field::getCreature(const Position& pos) { 
    if (pos!=bad_position && validX(pos.first) && validY(pos.second))
        return *(zoo_ptr_[pos.first*size_y_+pos.second]); 
    return bad_creature;
}

const Creature& Field::getCreature(const Position& pos) const {
    if (pos != bad_position && validX(pos.first) && validY(pos.second))
        return *(zoo_ptr_[pos.first * size_y_ + pos.second]);
    return bad_creature;
}


Position Field::findClosePosition(Creature* ancestor) {
    int& x=ancestor->getX();
    int& y=ancestor->getY();
    const int& direction= ancestor->getDirection();

    if (direction == down) {
        if (y!=0){
            if (zoo_ptr_[x*size_y_+y-1]->getState()!=alive)
                return {x,y-1};
        }
        else{
            if (zoo_ptr_[x * size_y_ + size_y_ - 1]->getState() != alive)
                return { x,size_y_ - 1 };
        }
    }

    if (direction == up) {
        if (y != size_y_ - 1){
            if (zoo_ptr_[x * size_y_ + y + 1]->getState() != alive)
                return { x,y + 1 };
        }
        else{
            if (zoo_ptr_[x * size_y_]->getState() != alive)
                return { x, 0};
        }
    }

    if (direction == right) {
        if (x!=0){
            if (zoo_ptr_[(x-1) * size_y_ + y]->getState() != alive)
                return { x-1,y };
        }
        else{
            if (zoo_ptr_[(size_x_ - 1) * size_y_ + y]->getState() != alive)
                return { size_x_ - 1,y };
        }
    }

    if (direction == left) {
        if (x != size_x_ - 1){
            if (zoo_ptr_[(x+1) * size_y_ + y]->getState() != alive)
                return { x+1,y };
        }
        else{
            if (zoo_ptr_[y]->getState() != alive)
                return { 0,y };
        }
    }
    
    return bad_position;
}


bool Field::validX(const int& x) const{
    return (x >= 0 &&  x < size_x_);
}

bool Field::validY(const int& y) const  {
    return (y >= 0 && y < size_y_);
}






Creature& Field::findCreature(Creature* finder, int direction) {
    int& x = finder->getX();
    int& y = finder->getY();
    if (direction == up) {
        do{
            --y;
            if (y<0)
                y=size_y_-1;
        }while (zoo_ptr_[x * size_y_ + y]->getState() == not_exist);
            
    }
    if (direction == down) {
        do{
            ++y;
            if (y == size_y_)
                y = 0;
        }while (zoo_ptr_[x * size_y_ + y]->getState() == not_exist);
            
    }
    if (direction == left) {
        do{
            --x;
            if (x < 0)
                x = size_x_-1;
        }while (zoo_ptr_[x * size_y_ + y]->getState() == not_exist);
    }
    if (direction == right) {
        do{
            ++x;
            if (x == size_x_)
                x = 0;
        } while (zoo_ptr_[x * size_y_ + y]->getState() == not_exist);
            
    }
    return *zoo_ptr_[x * size_y_ + y];
}

void Field::clear() {
    *this=Field(size_x_,size_y_);
}

#if OPEN_MP_FOUND == 1
void Field::updatePositions(){
//auto start = std::chrono::steady_clock::now();
//auto end = std::chrono::steady_clock::now();
//std::chrono::duration<double> time = end - start;
//std::cout << "\n" << time.count();
    #pragma omp parallel for
    for (int i = 0; i < size_; ++i){
        Creature& current = *zoo_ptr_[i];
        if (current.getState() == alive){
            current.look(findCreature(zoo_ptr_[i],up), up);
            current.look(findCreature(zoo_ptr_[i], down), down);
            current.look(findCreature(zoo_ptr_[i], left), left);
            current.look(findCreature(zoo_ptr_[i], right), right);
            current.getInfo();
            current.reverseInput();
        }
        
    }
    
    #pragma omp parallel for
    for (int i = 0; i < size_; ++i) {
        
        if (zoo_ptr_[i]->getState() == alive) {
            zoo_ptr_[i]->think();
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < size_; ++i) {

        if (zoo_ptr_[i]->getState()== not_exist)
            continue;

        zoo_ptr_[i]->act();

        if (!validX(zoo_ptr_[i]->pos_x_)) {
            zoo_ptr_[i]->pos_x_ %=size_x_;
            if (zoo_ptr_[i]->pos_x_ <0)
                zoo_ptr_[i]->pos_x_ +=size_x_;
        }
        if (!validY(zoo_ptr_[i]->pos_y_)) {
            zoo_ptr_[i]->pos_y_ %= size_y_;
            if (zoo_ptr_[i]->pos_y_ < 0)
                zoo_ptr_[i]->pos_y_ += size_y_;
        }
        
    }

    #pragma omp parallel for
    for (int i = 0; i < size_; ++i) {
        if (zoo_ptr_[i]->getState()==alive && (zoo_ptr_[i]->getEnergy() > zoo_ptr_[i]->getEnergyLimit())){
            empty_zoo_ptr_[i]->addEnergy(zoo_ptr_[i]->Leftover());
        }
    }

        #pragma omp barrier

    for (int i = 0; i < size_; ++i) {
        if (zoo_ptr_[i]->getState() != not_exist) {
            conjoin(empty_zoo_ptr_[zoo_ptr_[i]->pos_x_ * size_y_ + zoo_ptr_[i]->pos_y_], zoo_ptr_[i]);
        }
            
    }

    std::swap(empty_zoo_ptr_,zoo_ptr_);
    for (int i = 0; i < size_x_; ++i) {
        for (int j = 0; j < size_y_; ++j) {
            empty_zoo_ptr_[i * size_y_ + j]->stopExisting();
            empty_zoo_ptr_[i * size_y_ + j]->pos_x_ = i;
            empty_zoo_ptr_[i * size_y_ + j]->pos_y_ = j;
        }
    }
            

}

#else


void Field::updatePositions() {
    //auto start = std::chrono::steady_clock::now();
    //auto end = std::chrono::steady_clock::now();
    //std::chrono::duration<double> time = end - start;
    //std::cout << "\n" << time.count();
    for (int i = 0; i < size_; ++i) {
        Creature& current = *zoo_ptr_[i];
        if (current.getState() == alive) {
            current.look(findCreature(zoo_ptr_[i], up), up);
            current.look(findCreature(zoo_ptr_[i], down), down);
            current.look(findCreature(zoo_ptr_[i], left), left);
            current.look(findCreature(zoo_ptr_[i], right), right);
            current.getInfo();
            current.reverseInput();
        }

    }

    for (int i = 0; i < size_; ++i) {

        if (zoo_ptr_[i]->getState() == alive) {
            zoo_ptr_[i]->think();
        }
    }

    for (int i = 0; i < size_; ++i) {

        if (zoo_ptr_[i]->getState() == not_exist)
            continue;

        zoo_ptr_[i]->act();

        if (!validX(zoo_ptr_[i]->pos_x_)) {
            zoo_ptr_[i]->pos_x_ %= size_x_;
            if (zoo_ptr_[i]->pos_x_ < 0)
                zoo_ptr_[i]->pos_x_ += size_x_;
        }
        if (!validY(zoo_ptr_[i]->pos_y_)) {
            zoo_ptr_[i]->pos_y_ %= size_y_;
            if (zoo_ptr_[i]->pos_y_ < 0)
                zoo_ptr_[i]->pos_y_ += size_y_;
        }

    }

    for (int i = 0; i < size_; ++i) {
        if (zoo_ptr_[i]->getState() == alive && (zoo_ptr_[i]->getEnergy() > zoo_ptr_[i]->getEnergyLimit())) {
            empty_zoo_ptr_[i]->addEnergy(zoo_ptr_[i]->Leftover());
        }
    }


    for (int i = 0; i < size_; ++i) {
        if (zoo_ptr_[i]->getState() != not_exist) {
            conjoin(empty_zoo_ptr_[zoo_ptr_[i]->pos_x_ * size_y_ + zoo_ptr_[i]->pos_y_], zoo_ptr_[i]);
        }

    }

    std::swap(empty_zoo_ptr_, zoo_ptr_);
    for (int i = 0; i < size_x_; ++i) {
        for (int j = 0; j < size_y_; ++j) {
            empty_zoo_ptr_[i * size_y_ + j]->stopExisting();
            empty_zoo_ptr_[i * size_y_ + j]->pos_x_ = i;
            empty_zoo_ptr_[i * size_y_ + j]->pos_y_ = j;
        }
    }


}


#endif

void Field::updateStates(){
    alive_count_ = 0;
    dead_count_ = 0;
    //#pragma omp parallel for ordered
    for (int i = 0; i < size_; ++i) {

        if (zoo_ptr_[i]->getState() == not_exist) {
            continue;
        }
                
        if (zoo_ptr_[i]->getState() == dead) {
            continue;
        }

        if (zoo_ptr_[i]->getEnergy()<0) {
            zoo_ptr_[i]->die();
            continue;
        }

        if (zoo_ptr_[i]->wantToReproduce()) {
            Position child_pos = findClosePosition(zoo_ptr_[i]);
                if (child_pos != bad_position) {
                    //#pragma omp critical
                    {
                        getCreature(child_pos).makeAlive(*zoo_ptr_[i], child_pos);
                    }
                }
        }
        
    }

    //#pragma omp parallel for
    for (int i = 0; i < size_; ++i) {
        if (zoo_ptr_[i]->getState() == dead) {
            //#pragma omp atomic
            ++dead_count_;
        }

        if (zoo_ptr_[i]->getState() == alive) {
            //#pragma omp atomic
            ++alive_count_;
        }
    }

}


void Field::createTexture() {

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    texture_=texture;
}


void Field::updateTexture() {
    for (int i = 0; i < size_x_; ++i) {
        for (int j = 0; j < size_y_; ++j) {
            colors_[j*size_x_+i]=getColor({i,j});
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size_x_, size_y_, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &(colors_[0]));
}

void Field::unbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}











}


