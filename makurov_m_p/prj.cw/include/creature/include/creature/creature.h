

#ifndef EVOLVING_WORLD_2023_Q2
#define EVOLVING_WORLD_2023_Q2



#define GL_SILENCE_DEPRECATION
#include<glad/glad.h>

#define EIGEN_NO_DEBUG
#include <Eigen/Core>

#include <array>
#include <fstream>
#include <random>
#include <vector>










namespace cellworld
{
    /*!
    *
    *\brief ����� ����������������� ��������� ��������� �����.
    *����� ��������� ���������� ����� ������ ���� ������� � ������� ������� ����������(�� ����������� ��������� ����).
    */
    inline std::mt19937 generator_;
    
    /*!
    *\ingroup ���������
    *\brief ��������� ��������� float � ��������� [-4,4].
    *���������� ������� �������� � ����� ���������.
    */

    inline std::uniform_real_distribution<float> dis(-4.0f, 4.0f);



/*!
*\brief ��� ��������� ��������� ��������.
* 
*/
    enum State {
        not_exist,///< �������� ���
        dead,///< ������ ��������
        alive,///< ����� ��������
    };


    enum Directions {
        up,
        down,
        left,
        right,
    };


/*!
*\defgroup ������������
*/

/*!
*\ingroup ������������
*\brief ����� �������������, ������� ���������� ������� ����.
*/
///����� �������������, ������� ���������� ������� ����.
    enum Coefficients {
/*!
����������� �������������� ����� � �������.\n
������� ������� ��������� ��� �������� ������� �����.
������������ ���:
    + ������ ��������,\n    ������� �����+= �����*mass_into_energy.
    + �������� ��������,\n  ������� ��������-=����� ������*mass_into_energy.
*/
        mass_into_energy,


/*!
����������� ����������� �����.
������� ������� ����� ������� � ���� ������� �����.\n
���� ������� = mass_capacity * ����� ��������.
*/
        mass_capacity,


/*!
����������� ��������� �������.
���������� ������� ������� ����� �������� ��� ������(�� ��������).\n
������� = ���� ������� * starting_energy.
*/
        starting_energy,

/*!
����������� ��������� �����.
���������� ������� ������� ����� �������� ������ �� ��� �� ����������� ������� �����.\n
�������-=�����*mass_cost.
*/
        mass_cost,


/*!
����������� ��������� ��������� ������ ��������.
���������� ������� ������� �������� ������ �� ��� �� ��������� ������ ��������.\n
�������-=(��������� ������ ��������)^2*change_speed_module_cost.

*/
        change_speed_module_cost,


/*!
����������� ���� ����������.
���������� �� ������� ����������� ������ �������� � ����� ������� ����.\n
������ ��������-=braking_force.
*/
        braking_force,


/*!
����������� �������.
���������� �� ������� ������ ����� ��������� ����� ������ �� ��������.
    -   <=0 - ����� ��������.
    -   >=1 - ����� �� ������� �� �������� ������.\n
��� ������=��� ��������*(1-mutation_strength) + ��������� ���*mutation_strength.
*/
        mutation_strength,


///���������� �������������
        coefficients_count

    };

/**@}*/

/*!
*\defgroup ���������
*\brief ��� ���������� ��������� �����
*@{
*/

/*!
*\brief �������� ������� ������� �������� �������� ���������� � ���������� ��������� ��������
*/
    enum InputNeurons {
        /// ���������� X
        pos_x,

        /// ���������� Y
        pos_y,

        /// ������ ��������
        speed_module,

        /// �������
        energy,

        /// C�������� ������
        bias,

        /// ���������� ������� ��������
        input_neurons_count
    };



/*!
*\brief �������� ������� ��������
*
*/
    enum OutputNeurons {
        ///�������� ������ ��������
        change_speed_module,

        ///�� ����� ������ ������������ ��������, ������������ ��� ��������������
        vertical_or_horizontal,

        ///� ����� ������� ������������ ��������, �� ���������� ��� ���������� ����������
        decrease_or_increase,

        ///������� �������� ������������
        reproduce,
        
        /// ���������� �������� ��������
        output_neurons_count
    };


    /*!
    *\brief �������������� ������� ������� ��������.
    * 
    *�������� ���������� �� ������� ��������� ��������.\n
    *��� ������� �� 4 �����������(����,���,�����,����) �������� ����� ��������� ����� ��������. \n
    *������� ����� ���������� ������� �������� ����� 4 * look_input_count + input_neurons_count.
    */
    enum LookInput {
        /// ���������� �� �������� �� ���������� �������� � ����� �� �����������
        distance,
        /// ������� �������� ����� ���������� ��������
        color_red,
        /// ������� ������� ����� ���������� ��������
        color_green,
        /// ������� ������ ����� ���������� ��������
        color_blue,


        /// ���������� ������� ��������
        look_input_count
    };


///������ ���� ��������� ��������
using NeuronNetwork = Eigen::Matrix<float,output_neurons_count , (4 * look_input_count + input_neurons_count)>;

/**@}*/
using Position = std::pair<int,int>;
constexpr Position bad_position={-1,-1};



/*!
*\brief ������ ����� ��������.

 � ����� ������: 
    - ����
    - �����
    - ���� ��������� ��������
*/
struct Genome {
    Genome() :color(),mass(),neuron_network() {}
    explicit Genome(unsigned int in_color):color(in_color),mass(),neuron_network() {}
    unsigned int color;
    unsigned int mass;
    NeuronNetwork neuron_network;
};



/*!
*\brief ����� ��������.
*/



class Creature {

    friend bool operator==(const Creature&, const Creature&);
public:
    
    /// ������� ��������� �����
    static Genome generateGenome();

    /// ������� ������ ����� ���������
    static void generateGenome(Genome&);

    /*!
    \brief ���������� �����
    */
    static void mixGen(float& gen1, const float& gen2);


    /*!
    \brief ���������� �����
    */
    static unsigned int mixGen(const unsigned int& gen1, const unsigned int& gen2);

    /*!
    \brief ������� �����, ���������� �� ������ ������.
    */
    static Genome createGenome(const Genome& ancestor);

    /*!
    \brief ���� �������. ���� ������� �������� ������� �� ��� ������� �������.
    */
    static unsigned int energyColor(int energy);


    /// ������, ������� ������ �������� ���� �������������.
    inline static std::array<float, coefficients_count> coeff_{ 0 };

    ///���������� �������� �� ����������� �����������, ���� 0 - ���������, ����� - ��������.
    inline static bool is_breedable = 1;

    ///���� ��������������� ��������.
    inline static unsigned int base_color_ = 0xAfAfAfff;


/*! @name ����������� ������������ ��������
*�� ��������� ������ �������� ����� 0, �������� ���������� �����.
*/

///@{

    /*!
    \brief �������������� ����� ��������. ��������� ������� ������� �� ����(��. \ref ������������)
    */
    Creature(const Genome& gen, const Position& pos);


    ///�������������� �������������� ��������
    Creature();


    /*!
    \brief �������������� ������� ��������.
    */
    Creature(float energy, const Position& pos);
///@}



    Creature(const Creature&)=default;
    Creature(Creature&&) = default;
    Creature& operator=(const Creature&) = default;
    Creature& operator=(Creature&&) = default;
    


    /*! @name ��������� ��������� ��������
    ������������ ��� ��������� ������ �������������
    */
///@{


    /*!
    \brief �������� ���������� �����. ����� �������� ������������ �� ������.
    */
    void makeAlive(Creature& ancestor, const Position& pos);


    /*!
    \brief �������� ���������� �����. ����� ���������.
    */
    void makeAlive(const Position& pos);


    /*!
    \brief �������� ���������� ������. ��� ����������������� � �������(��. \ref ������������).
    */
    void die();


    /*!
    \brief �������� �������� ������������.
    */
    void stopExisting() { state_ = not_exist; creatures_genome_.color = base_color_; energy_ = 0; speed_module_ = 0; output_neurons_.coeffRef(reproduce)=0; }
///@}
    

    /*! @name �������
    */
    ///@{
    const int& getState() const { return state_; }
    const int& getDirection() const {return speed_direction_;}
    const int& getSpeed() const {return speed_module_;}


    /*!
    \brief ���������� ������ �� ���������� ��������, ������, �� �� ���������� ����������� X.
    */
    int& getX();


    /*!
    \brief ���������� ������ �� ���������� ��������, ������, �� �� ���������� ����������� Y.
    */
    int& getY();


    const float& getEnergy() const { return energy_; }
    const float& getEnergyLimit() const { return energy_limit_; }
    const int& getMass() const { return (creatures_genome_.mass); }
    const unsigned int& getColor() const { return (creatures_genome_.color); }
    unsigned int getBlue() const { return((getColor() >> 8) & 0xff); }
    unsigned int getGreen() const { return((getColor() >> 16) & 0xff); }
    unsigned int getRed() const { return((getColor() >> 24) & 0xff); }
    const Genome& getGenome() { return creatures_genome_; }

    ///@}


    ///���������� ������� ������� ��������. ������� ����������� �� ������ �������.
    float Leftover();

    
    ///�������� �� ������� ������������
    bool wantToReproduce()const { return (output_neurons_.size() != 0) && is_breedable && (output_neurons_.coeff(reproduce) > 0); }
    

    /*!
    \brief ��������� ���������� � �������� �������� � ����� �� �����������.
    ��� ������� ���������� ��. \ref LookInput "LookInput"
    */
    void look(Creature& found, int direction);


    /*!
    \brief ��������� ���������� � ����. ��� ������� ���������� ��. \ref InputNeurons "InputNeuron"
    */
    void getInfo();


    /*!
    \brief ������������ ������� ������. ��������� � �������� [0;1] ���� ��������, �� ��������� ������� �� ���� ����.
    */
    void reverseInput();


    /*!
    \brief ������ ���������. ������������ ������� ������� � ������� �����, ������������� � ������ ��������, � ������� ��������� � �������� �������.
    */
    void think();

    /*!
    \brief ������ � ��������� ���������.

    ���� �������� �����, �� � ����������� �� �������� �������� ������ ������ �������� � �����������.
    ����� ���� ����������� ��������� ��������.
    � ����� ������� ������ �������� ����������� �� ����������� ����������.
    */
    void act();

    /*!
    \brief �������� �������� ��� ������� ������� ��������.
    */
    void eat(Creature&);
    /*!
    \brief �������� �������� ������� � ������� energy.
    */
    void addEnergy(const float& energy);
    

    ///�������������� ����������� �������� �������� ��� ������ �� �����.
    void buildIO();
    ///������/������ ����� ��������� ����.
    void* networkPtr() { return &(creatures_genome_.neuron_network(0)); }
    


    /*! @name ������� ��������
     \brief ��������� � public, ����� ���������������� �������, ��� ���������� ��������� ��������, � �������, ��� ������ �������.
    */

    ///@{
    int pos_x_;
    int pos_y_;
    ///@}


    
private:


    ///��������� ��������
    int state_;

    /// ������� ��������
    float energy_;

    /// ������������ ������� ��������
    float energy_limit_;

    ///������ ��������
    int speed_module_;

    ///����������� ��������
    int speed_direction_;
    
    ///����� ��������
    Genome creatures_genome_;
    
    ///������� ������� ��������
    Eigen::MatrixXf input_neurons_;

    ///�������� ������� ��������
    Eigen::MatrixXf output_neurons_;

    std::tuple<int,int,float> tmp_;
    
};









/*!
*\brief ����� ����, ������ �������.

������ ����:
    - ������� ���������� ���������
    - ��������� ���������
    - �������������� ���������, ���������, �������������� �������
    - ��������������� ���������

* 
*/
class Field{

    /*! @name ��������� �������� (��������, ����� 2 �������� ���������� �� 1 �������)
    */

    ///@{
    /*!
    \param master - c�������, �������� ��� ������� ������
    \param candidate - c�������, �������� ��� ������� ������
    */
    friend void conjoin(Creature*& master, Creature*& candidate);
    /*!
        \param master - c�������, �������� ��� ������� ������
        \param candidate - c�������, �������� ��� ������� ������
    */
    friend void conjoin(Creature& master, Creature& candidate);
    ///@}

    //�� ����� ���� �� �� ���� ���� �������, �� ��� ������������ �������

public:
    /*!
    \brief ����������� ������, �������� ������ � ����� ����.
    */
    Field(int size_x=0, int size_y=0);


    /*!
    \brief ��������� ������� �������.

    ���� ������ � ���������.
    ���������� ��� ������ ����������.
    �������� ������� � ������� �� ������������.
    ��������� �� ���������� ������� ������� �������.
    ����������� �������� �������.
    �������� � ������������� ������, ��� �������������.
    
    */
    void updatePositions();


    /*!
    \brief ��������� ��������� �������.
    ���� � ������ �������� <0 ������� - ��� �������.
    ���� �������� ����� ������������, �� ���� �� �������� ������� ���������� �����, � �����, ����������� �� ������.
    �� �������������� �������.
    
    */
    void updateStates();


    /*! @name ������ � �������� �� 1 ��� 2 ��������(�������) ����
    */
    ///@{
    const Creature& getCreature(const Position& pos) const;
    Creature& getCreature(const Position& pos);

    const Creature& getCreature(const int& index) const { return *zoo_ptr_[index]; }
    Creature& getCreature(const int& index) { return *zoo_ptr_[index]; }

    const Creature& operator[](const int& index) const {return *zoo_ptr_[index];}
    Creature& operator[](const int& index) { return *zoo_ptr_[index]; }
    ///@}



    /*! @name �������
    */
    ///@{
    unsigned int getColor(const Position& pos)const {return getCreature(pos).getColor();}
    unsigned int getColor(const int& index) const { return getCreature(index).getColor(); }
    int sizeX() const { return size_x_; }
    int sizeY() const { return size_y_; }
    int size() const { return size_; }
    const int& getAlive() { return alive_count_; };
    const int& getDead() { return dead_count_; };
    ///@}

    /*! @name �������� ���������� �� ����������
    */
    ///@{
    bool validX(const int& x) const;
    bool validY(const int& y) const;
    ///@}

    /*! \brief �������� ������� ������������ ��������, ��������� ������������, ������� ���������� �����. ���� ������� �� �������, ���������� bad_position.
    */
    Position findClosePosition(Creature* ancestor);

    /*! \brief ����� �������� �� ������������ �������� � ����������� �����������.
    */
    Creature& findCreature(Creature* finder, int direction);


    /// ������� ����
    void clear();


    /*! @name ��������� ������������
    */
    
    ///@{
    ///������� ��������
    void createTexture();
    /// �������� �������� ��� ������ � ImGui
    void* getTexture() { return (void*)(texture_); }
    /// �������� �������� ��� ������ � OpenGL
    const GLuint& getGLTexture() {return texture_;}
    ///���������������� �������� � ������� ���������� ����.
    void updateTexture();
    ///��������� ������������� � �����.
    void unbindTexture();
    ///@}


    inline static Creature bad_creature=Creature();
    
protected:

    ///������
    int size_x_;
    ///������
    int size_y_;
    ///�������
    int size_;

    ///��������� �� ������� ���������  ����
    std::vector<Creature*> zoo_ptr_;
    ///��������� �� �������� ��������� ����
    std::vector<Creature*> empty_zoo_ptr_;
    ///��������� �������
    std::vector<Creature> storage_;
    ///������ ����� ������� ��������, ������������ ��� �������� ��������.
    std::vector<unsigned int> colors_;
    ///��������� �� ��������
    GLuint texture_;

    ///���������� ����� ������� � ����
    int alive_count_;

    ///���������� ������ ������� � ����
    int dead_count_;

    
};









};

#endif
