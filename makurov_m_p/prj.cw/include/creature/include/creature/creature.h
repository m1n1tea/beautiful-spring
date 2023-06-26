

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
    *\brief Общий детерминированный генератор случайных чисел.
    *Любая генерация случайного числа должна быть сделана с помощью данного генератора(за исключением генерации сида).
    */
    inline std::mt19937 generator_;
    
    /*!
    *\ingroup Нейросеть
    *\brief Генератор случайных float в диапазоне [-4,4].
    *Определяет разброс значений у весов нейросети.
    */

    inline std::uniform_real_distribution<float> dis(-4.0f, 4.0f);



/*!
*\brief Все возможные состояния существа.
* 
*/
    enum State {
        not_exist,///< существа нет
        dead,///< мёртвое существо
        alive,///< живое существо
    };


    enum Directions {
        up,
        down,
        left,
        right,
    };


/*!
*\defgroup Коэффициенты
*/

/*!
*\ingroup Коэффициенты
*\brief Набор коэффициентов, которые определяют правила мира.
*/
///Набор коэффициентов, которые определяют правила мира.
    enum Coefficients {
/*!
Коэффициент преобразования массы в энергию.\n
Сколько энергии требуется для создания единица массы.
Используется при:
    + смерти сущности,\n    энергия трупа+= масса*mass_into_energy.
    + рождении сущности,\n  энергия родителя-=масса ребёнка*mass_into_energy.
*/
        mass_into_energy,


/*!
Коэффициент вместимости массы.
Сколько энергии может хранить в себе единица массы.\n
Макс энергия = mass_capacity * масса существа.
*/
        mass_capacity,


/*!
Коэффициент начальной энергии.
Определяет сколько энергии имеет существо при спавне(не рождении).\n
Энергия = Макс энергия * starting_energy.
*/
        starting_energy,

/*!
Коэффициент стоимости массы.
Определяет сколько энергии живое существо тратит за ход на поддержание единицы массы.\n
Энергия-=масса*mass_cost.
*/
        mass_cost,


/*!
Коэффициент стоимости изменения модуля скорости.
Определяет сколько энергии существо тратит за ход на изменение модуля скорости.\n
Энергия-=(изменение модуля скорости)^2*change_speed_module_cost.

*/
        change_speed_module_cost,


/*!
Коэффициент силы торможения.
Определяет на сколько уменьшается модуль скорости в конце каждого хода.\n
Модуль скорости-=braking_force.
*/
        braking_force,


/*!
Коэффициент мутации.
Определяет на сколько сильно будет отличатся геном ребёнка от родителя.
    -   <=0 - копия родителя.
    -   >=1 - геном не зависит от родителя совсем.\n
Ген ребёнка=ген родителя*(1-mutation_strength) + случайный ген*mutation_strength.
*/
        mutation_strength,


///количество коэффициентов
        coefficients_count

    };

/**@}*/

/*!
*\defgroup Нейросеть
*\brief Все упоминания нейронных сетей
*@{
*/

/*!
*\brief Основные входные нейроны существа Содержит информацию о внутреннем состоянии существа
*/
    enum InputNeurons {
        /// Координата X
        pos_x,

        /// Координата Y
        pos_y,

        /// Модуль скорости
        speed_module,

        /// Энергия
        energy,

        /// Cмещающий нейрон
        bias,

        /// Количество входных нейронов
        input_neurons_count
    };



/*!
*\brief Выходные нейроны существа
*
*/
    enum OutputNeurons {
        ///Изменить модуль скорости
        change_speed_module,

        ///По какой прямой перемещается существо, вертикальной или горизонтальной
        vertical_or_horizontal,

        ///В какую сторону перемещается существо, по увеличению или уменьшению координаты
        decrease_or_increase,

        ///Желание существа размножиться
        reproduce,
        
        /// Количество выходных нейронов
        output_neurons_count
    };


    /*!
    *\brief Дополнительные входные нейроны существа.
    * 
    *Содержит информацию об внешнем окружении существа.\n
    *Для каждого из 4 направлений(верх,низ,право,лево) существо имеет отдельный набор нейронов. \n
    *Поэтому общее количество входных нейронов равно 4 * look_input_count + input_neurons_count.
    */
    enum LookInput {
        /// расстояние от существа до ближайшего существа в одном из направлений
        distance,
        /// уровень красного цвета ближайшего существа
        color_red,
        /// уровень зелёного цвета ближайшего существа
        color_green,
        /// уровень синего цвета ближайшего существа
        color_blue,


        /// Количество входных нейронов
        look_input_count
    };


///Хранит веса нейросети существа
using NeuronNetwork = Eigen::Matrix<float,output_neurons_count , (4 * look_input_count + input_neurons_count)>;

/**@}*/
using Position = std::pair<int,int>;
constexpr Position bad_position={-1,-1};



/*!
*\brief Хранит геном существа.

 В геном входят: 
    - цвет
    - масса
    - веса нейросети существа
*/
struct Genome {
    Genome() :color(),mass(),neuron_network() {}
    explicit Genome(unsigned int in_color):color(in_color),mass(),neuron_network() {}
    unsigned int color;
    unsigned int mass;
    NeuronNetwork neuron_network;
};



/*!
*\brief Класс существа.
*/



class Creature {

    friend bool operator==(const Creature&, const Creature&);
public:
    
    /// Создать случайным геном
    static Genome generateGenome();

    /// Сделать данный геном случайным
    static void generateGenome(Genome&);

    /*!
    \brief Смешивание генов
    */
    static void mixGen(float& gen1, const float& gen2);


    /*!
    \brief Смешивание генов
    */
    static unsigned int mixGen(const unsigned int& gen1, const unsigned int& gen2);

    /*!
    \brief Создать геном, основанный на геноме предка.
    */
    static Genome createGenome(const Genome& ancestor);

    /*!
    \brief Цвет энергии. Цвет мёртвого существа зависит от его текущей энергии.
    */
    static unsigned int energyColor(int energy);


    /// Массив, который хранит значения всех коэффициентов.
    inline static std::array<float, coefficients_count> coeff_{ 0 };

    ///Определяет включена ли возможность размножения, если 0 - выключена, иначе - включена.
    inline static bool is_breedable = 1;

    ///Цвет несуществующего существа.
    inline static unsigned int base_color_ = 0xAfAfAfff;


/*! @name Специальные конструкторы существа
*По умолчанию модуль скорости равен 0, существо направлено вверх.
*/

///@{

    /*!
    \brief Конструируется живое существо. Начальная энергия зависит от веса(см. \ref Коэффициенты)
    */
    Creature(const Genome& gen, const Position& pos);


    ///Конструируется несуществующее существо
    Creature();


    /*!
    \brief Конструируется неживое существо.
    */
    Creature(float energy, const Position& pos);
///@}



    Creature(const Creature&)=default;
    Creature(Creature&&) = default;
    Creature& operator=(const Creature&) = default;
    Creature& operator=(Creature&&) = default;
    


    /*! @name Изменение состояния существа
    Используются для избежания лишней инициализации
    */
///@{


    /*!
    \brief Существо становится живым. Геном существа основывается на предке.
    */
    void makeAlive(Creature& ancestor, const Position& pos);


    /*!
    \brief Существо становится живым. Геном рандомный.
    */
    void makeAlive(const Position& pos);


    /*!
    \brief Существо становится мёртвым. Вес преобразовывается в энергию(см. \ref Коэффициенты).
    */
    void die();


    /*!
    \brief Существо перестаёт существовать.
    */
    void stopExisting() { state_ = not_exist; creatures_genome_.color = base_color_; energy_ = 0; speed_module_ = 0; output_neurons_.coeffRef(reproduce)=0; }
///@}
    

    /*! @name Геттеры
    */
    ///@{
    const int& getState() const { return state_; }
    const int& getDirection() const {return speed_direction_;}
    const int& getSpeed() const {return speed_module_;}


    /*!
    \brief Возвращает ссылку на изменяемое значение, равное, но не являющееся координатой X.
    */
    int& getX();


    /*!
    \brief Возвращает ссылку на изменяемое значение, равное, но не являющееся координатой Y.
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


    ///Возвращает избыток энергии существа. Энергия уменьшается до лимита энергии.
    float Leftover();

    
    ///проверка на желание размножиться
    bool wantToReproduce()const { return (output_neurons_.size() != 0) && is_breedable && (output_neurons_.coeff(reproduce) > 0); }
    

    /*!
    \brief Получение информации о соседнем существе в одном из направлений.
    Для большей информации см. \ref LookInput "LookInput"
    */
    void look(Creature& found, int direction);


    /*!
    \brief Получение информации о себе. Для большей информации см. \ref InputNeurons "InputNeuron"
    */
    void getInfo();


    /*!
    \brief Нормализация входных данных. Переводит в диапазон [0;1] путём быстрого, но неточного деления на само себя.
    */
    void reverseInput();


    /*!
    \brief Работа нейросети. Обрабатывает входные нейроны с помощью весов, расположенных в геноме существа, и выводит результат в выходные нейроны.
    */
    void think();

    /*!
    \brief Работа с выходными нейронами.

    Если существо живое, то в зависимости от выходных нейронов меняет модуль скорости и направление.
    После чего обновляется положение существа.
    В конце функции модуль скорости уменьшается на коэффициент торможения.
    */
    void act();

    /*!
    \brief Существо забирает всю энергию другого существа.
    */
    void eat(Creature&);
    /*!
    \brief Существо получает энергию в размере energy.
    */
    void addEnergy(const float& energy);
    

    ///Инициализирует недостающие элементы существа при чтении из файла.
    void buildIO();
    ///Чтение/запись весов нейронной сети.
    void* networkPtr() { return &(creatures_genome_.neuron_network(0)); }
    


    /*! @name Позиция существа
     \brief Находится в public, чтобы синхронизировать позицию, как внутреннее состояние сущности, и позицию, как индекс массива.
    */

    ///@{
    int pos_x_;
    int pos_y_;
    ///@}


    
private:


    ///Состояние существа
    int state_;

    /// Энергия существа
    float energy_;

    /// Максимальная энергию существа
    float energy_limit_;

    ///Модуль скорости
    int speed_module_;

    ///Направление скорости
    int speed_direction_;
    
    ///Геном существа
    Genome creatures_genome_;
    
    ///Входные нейроны существа
    Eigen::MatrixXf input_neurons_;

    ///Выходные нейроны существа
    Eigen::MatrixXf output_neurons_;

    std::tuple<int,int,float> tmp_;
    
};









/*!
*\brief Класс Поля, хранит существ.

Задачи поля:
    - Хранить конкретное состояние
    - Обновлять состояние
    - Контролировать поведение, состояние, взаимодействие существ
    - Визуализировать симуляцию

* 
*/
class Field{

    /*! @name Обработка коллизий (ситуаций, когда 2 существа претендуют на 1 позицию)
    */

    ///@{
    /*!
    \param master - cущество, занявшее эту позицию первым
    \param candidate - cущество, занявшее эту позицию вторым
    */
    friend void conjoin(Creature*& master, Creature*& candidate);
    /*!
        \param master - cущество, занявшее эту позицию первым
        \param candidate - cущество, занявшее эту позицию вторым
    */
    friend void conjoin(Creature& master, Creature& candidate);
    ///@}

    //на самом деле им не надо быть друзьми, но для документации красиво

public:
    /*!
    \brief Конструктор класса, задающий ширину и длину поля.
    */
    Field(int size_x=0, int size_y=0);


    /*!
    \brief Обновляет позиции существ.

    Ввод данных в нейросеть.
    Происходит вся работа нейросетей.
    Забирает энергии у существ за передвижение.
    Оставляет на предыдущей позиции остатки энергии.
    Обрабатывет коллизию существ.
    Работает в многопоточном режиме, без инициализаций.
    
    */
    void updatePositions();


    /*!
    \brief Обновляет состояние существ.
    Если у живого существа <0 энергии - оно умирает.
    Если существо хочет размножаться, то одно из соседних существ становится живым, с геном, наследуемым от предка.
    Не инициализирует объекты.
    
    */
    void updateStates();


    /*! @name Доступ к существу по 1 или 2 индексам(позиции) поля
    */
    ///@{
    const Creature& getCreature(const Position& pos) const;
    Creature& getCreature(const Position& pos);

    const Creature& getCreature(const int& index) const { return *zoo_ptr_[index]; }
    Creature& getCreature(const int& index) { return *zoo_ptr_[index]; }

    const Creature& operator[](const int& index) const {return *zoo_ptr_[index];}
    Creature& operator[](const int& index) { return *zoo_ptr_[index]; }
    ///@}



    /*! @name Геттеры
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

    /*! @name Проверка координаты на валидность
    */
    ///@{
    bool validX(const int& x) const;
    bool validY(const int& y) const;
    ///@}

    /*! \brief Соседняя позиция относительно существа, желающего размножиться, которая становится живой. Если позиция не найдена, возвращает bad_position.
    */
    Position findClosePosition(Creature* ancestor);

    /*! \brief Поиск существа из определённого существа в определённом направлении.
    */
    Creature& findCreature(Creature* finder, int direction);


    /// Очистка поля
    void clear();


    /*! @name Интерфейс визуализации
    */
    
    ///@{
    ///Создать текстуру
    void createTexture();
    /// Получить текстуру для работы с ImGui
    void* getTexture() { return (void*)(texture_); }
    /// Получить текстуру для работы с OpenGL
    const GLuint& getGLTexture() {return texture_;}
    ///Синхронизировать текстуру с текущим состоянием поля.
    void updateTexture();
    ///Закончить синхронизацию с полем.
    void unbindTexture();
    ///@}


    inline static Creature bad_creature=Creature();
    
protected:

    ///Ширина
    int size_x_;
    ///Высота
    int size_y_;
    ///Площадь
    int size_;

    ///Указатели на текущее состояние  поля
    std::vector<Creature*> zoo_ptr_;
    ///Указатели на запасное состояние поля
    std::vector<Creature*> empty_zoo_ptr_;
    ///Хранилище существ
    std::vector<Creature> storage_;
    ///Хранит цвета каждого существа, используется при создании текстуры.
    std::vector<unsigned int> colors_;
    ///Указатель на текстуру
    GLuint texture_;

    ///Количество живых существ в мире
    int alive_count_;

    ///Количество мёртвых существ в мире
    int dead_count_;

    
};









};

#endif
