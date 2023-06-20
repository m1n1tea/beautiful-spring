

#ifndef EVOLVING_WORLD_2023_Q2_UI
#define EVOLVING_WORLD_2023_Q2_UI

#include "imgui.h"
#include<creature/creature.h>
#include<save_system/save_system.h>
#include<scenario/scenario.h>
#include "imgui_stdlib.h"
namespace cellworld {


    /*!
    *\brief Набор существующих сцен. Все используемы сцены должны храниться здесь.
    */
    enum Scene {
        ///Стартовый экран
        start_screen,

        ///Экран настройки/создания мира
        creation_of_the_world,

        ///Экран выбора мира для загрузки
        load_world_screen,

        ///Экран симуляции мира
        simulation_of_the_world,
    };



    /*!
    *\brief Часто используемые флаги для окон ImGui
    */
    enum WindowTemplates {
        ///Невидимое окно
        invisible_window = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize,
        ///Невидимое окно с полосой прокрутки
        scroll_bar_only = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize
    };


    /*!
    *\brief Класс графического интерфейса.

    Является синглтоном.
    Содержит набор функций, которые представляют собой отдельные сцены.
    Сцены самостоятельны внутри себя(за исключением записи и чтения данных, которые доступны для всех сцен), должны представлять возмность сменить сцену.
    */
    class UI {
    public:
        void operator=(const UI&) = delete;
        UI(UI&) = delete;

        ///Доступ к единственному экземпляру
        static UI& GetInstance() {
            static UI* singleUI = new UI();
            return *singleUI;
        }

        /// Синхронизировать с размером окна
        void updateWindowSize(int width, int height){ width_ =width; height_= height;}

        ///Загрузить текущую сцену
        void loadScene();
        
    private:

        

        ///Сменить сцену на данную
        void sceneUpdate(int scene);


        /*!
        \brief Функция для сцены стартого экрана
* 
*       Возможности:
            - перейти к сцене загрузки мира;
            - перейти к сцене создания мира;
        */
        void startScreen();


        /*!
        \brief Функция для сцены настройки/создания мира
        
        Возможности:
            - вернуться на стартовый экран;
            - задать стартовые параметры мира;
            - перейти к сцене симуляции мира;

        При смене сцен выбранные стартовые настройки мира сохраняются, на случай необходимости небольших изменений.
        */
        void CreationOfTheWorld();


        /*!
        \brief Функция для сцены выбора мира для загрузки
           
        Возможности:
            - вернуться на стартовый экран;
            - выбрать мир и перейти к сцене симуляции мира;
            - удалить любой из существующих миров;
        
        */
        void loadWorldScreen();


        /*!
        \brief Функция для сцены симуляция мира
 
        Возможности:
            - вернуться на стартовый экран;
            - поставить на паузу/продолжить симуляцию;
            - сделать 1 шаг симуляции;
            - включить/выключить визуализацию симуляции мира;
            - включить/выключить визуализацию наград;
            - задать имя миру и сохранить мир с ним;
            -посмотреть fps, количество сделанных шагов симуляции, количество живых и мёртвых существ;
            
        */
        void SimulationOfTheWorld();
        

        ///Текущая сцена
        int scene_;
        ///Предыдущая сцена
        int previous_scene_;
        ///1 если текущая сцена не равна предыдущей, иначе 0.
        bool scene_is_changed_;

        ///Ширина окна
        int width_;
        ///Высота окна
        int height_;
    
        /// @name Общие данные для всех сцен 

        ///@{
        FileSystem file_names_;
        Scenario scenario_;
        ///Сид для генератора рандомных чисел.
        unsigned int seed_;
        ///@}
        
        UI() : scene_(start_screen), previous_scene_(start_screen), scene_is_changed_(0), width_(1), height_(1), seed_(0), scenario_(100, 50) {}
    };  
}

#endif