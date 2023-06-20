

#ifndef EVOLVING_WORLD_2023_Q2_UI
#define EVOLVING_WORLD_2023_Q2_UI

#include "imgui.h"
#include<creature/creature.h>
#include<save_system/save_system.h>
#include<scenario/scenario.h>
#include "imgui_stdlib.h"
namespace cellworld {


    /*!
    *\brief ����� ������������ ����. ��� ����������� ����� ������ ��������� �����.
    */
    enum Scene {
        ///��������� �����
        start_screen,

        ///����� ���������/�������� ����
        creation_of_the_world,

        ///����� ������ ���� ��� ��������
        load_world_screen,

        ///����� ��������� ����
        simulation_of_the_world,
    };



    /*!
    *\brief ����� ������������ ����� ��� ���� ImGui
    */
    enum WindowTemplates {
        ///��������� ����
        invisible_window = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize,
        ///��������� ���� � ������� ���������
        scroll_bar_only = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize
    };


    /*!
    *\brief ����� ������������ ����������.

    �������� ����������.
    �������� ����� �������, ������� ������������ ����� ��������� �����.
    ����� �������������� ������ ����(�� ����������� ������ � ������ ������, ������� �������� ��� ���� ����), ������ ������������ ��������� ������� �����.
    */
    class UI {
    public:
        void operator=(const UI&) = delete;
        UI(UI&) = delete;

        ///������ � ������������� ����������
        static UI& GetInstance() {
            static UI* singleUI = new UI();
            return *singleUI;
        }

        /// ���������������� � �������� ����
        void updateWindowSize(int width, int height){ width_ =width; height_= height;}

        ///��������� ������� �����
        void loadScene();
        
    private:

        

        ///������� ����� �� ������
        void sceneUpdate(int scene);


        /*!
        \brief ������� ��� ����� �������� ������
* 
*       �����������:
            - ������� � ����� �������� ����;
            - ������� � ����� �������� ����;
        */
        void startScreen();


        /*!
        \brief ������� ��� ����� ���������/�������� ����
        
        �����������:
            - ��������� �� ��������� �����;
            - ������ ��������� ��������� ����;
            - ������� � ����� ��������� ����;

        ��� ����� ���� ��������� ��������� ��������� ���� �����������, �� ������ ������������� ��������� ���������.
        */
        void CreationOfTheWorld();


        /*!
        \brief ������� ��� ����� ������ ���� ��� ��������
           
        �����������:
            - ��������� �� ��������� �����;
            - ������� ��� � ������� � ����� ��������� ����;
            - ������� ����� �� ������������ �����;
        
        */
        void loadWorldScreen();


        /*!
        \brief ������� ��� ����� ��������� ����
 
        �����������:
            - ��������� �� ��������� �����;
            - ��������� �� �����/���������� ���������;
            - ������� 1 ��� ���������;
            - ��������/��������� ������������ ��������� ����;
            - ��������/��������� ������������ ������;
            - ������ ��� ���� � ��������� ��� � ���;
            -���������� fps, ���������� ��������� ����� ���������, ���������� ����� � ������ �������;
            
        */
        void SimulationOfTheWorld();
        

        ///������� �����
        int scene_;
        ///���������� �����
        int previous_scene_;
        ///1 ���� ������� ����� �� ����� ����������, ����� 0.
        bool scene_is_changed_;

        ///������ ����
        int width_;
        ///������ ����
        int height_;
    
        /// @name ����� ������ ��� ���� ���� 

        ///@{
        FileSystem file_names_;
        Scenario scenario_;
        ///��� ��� ���������� ��������� �����.
        unsigned int seed_;
        ///@}
        
        UI() : scene_(start_screen), previous_scene_(start_screen), scene_is_changed_(0), width_(1), height_(1), seed_(0), scenario_(100, 50) {}
    };  
}

#endif