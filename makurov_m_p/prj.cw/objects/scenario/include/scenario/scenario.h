

#ifndef EVOLVING_WORLD_2023_Q2_SCENARIO
#define EVOLVING_WORLD_2023_Q2_SCENARIO

#include<creature/creature.h>
#include"imgui.h"
#include<array>
#include<vector>
#include<random>
#include<omp.h>
namespace cellworld {



    /*!
    \brief Класс сценария - расширенный класс поля

    В расширенных функционал входят:
        + создание живых существ со старта;
        + сохранение и загрузка миров;
        + циклический мир - после x шагов(x - длина цикла), мир оказывается в начальном состоянии, однако геном новых существ базируется на геноме существ, выживших в конце предыдущей итерации;
        + система наград - перед началом симуляции можно задать определённые позиции за расположение на которых, живое существо получает определённое количество энергии каждый ход;
    */
    class Scenario: public Field {

        /*!
        \brief Функция сохранение мира
        */
        friend void saveWorld(const char* path, Scenario* current_field, const unsigned int& seed);

        /*!
        \brief Функция загрузки мира
        */
        friend void loadWorld(const char* path, Scenario* current_field, unsigned int& seed);
    public:
        

        Scenario(int size_x=0, int size_y=0);

        ///Очистить поле
        void makeNew();

        ///Сделать 1 шаг симуляции
        void makeOneStep();

        ///Создать данное количество существ на случайных позициях поля, использовать только на пустом поле. Также задаёт начальную популяцию.
        void spawnCreatures(int amount);
        
        ///Сделать новый цикл
        void newCycle();


        /*! @name Геттеры
        */
        ///@{
        int getInitialPopulation() { return initial_population_; }
        long long getIteration() { return iteration_; }
        ///@}


        /*!
        *\defgroup Награды
        *\brief Описание системы наград. Все функции связанные с наградми.

        Идея системы наград - перед началом симуляции можно задать определённые позиции за расположение на которых, живое существо получает определённую награду(энергию) каждый ход.
        *@{
        */


        ///Задать прямоугольник двумя позициями, за нахождение на котором существо каждый ход получает энергию=strength
        void makeRewards(Position,Position, float strength);

        ///Раздать награды
        void giveRewards();

        ///Вернуться к предыдущему состоянию системы наград.
        void CancelRewardsChange() { std::swap(rewards_, rewards_backup_); }

        ///Удалить все награды.
        void resetRewards();

        ///Ключевая часть графического интерфейса настройки наград, позволяет на пустом поле задавать прямоугольнки, на которых будут награды.
        void rewardsEditor(ImVec2 window_pos, ImVec2 window_size, float strength, ImTextureID texture);

        ///Синхронизирует графическое представление наград с текущим состоянием наград
        void updateRewardsTexture();
        /**@}*/

        ///Длина цикла, находится в public, поскольку нужен доступ как для чтения так и для записи.
        int cycle_len_;
    private:

        ///brief Переводит координату пикселя экрана в логическую позицию
        Position convertInput(ImVec2 begin, ImVec2 input, int square_size);

        ///Начальная популяция
        int initial_population_;
            
        ///Количество прошедших шагов симуляции.
        long long iteration_;

        ///Текущее состояние наград
        std::vector<float> rewards_;

        ///Предыдущее состояние наград
        std::vector<float> rewards_backup_;

        ///Указатели на выжившие существа в конце цикла
        std::vector<Creature*> survivors_;

        ///Перетасованные все индексы поля, используется для спавна существ на рандомных позициях.
        std::vector<int> positions_;
    };
    
    
}

#endif