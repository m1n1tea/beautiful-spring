

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
    \brief ����� �������� - ����������� ����� ����

    � ����������� ���������� ������:
        + �������� ����� ������� �� ������;
        + ���������� � �������� �����;
        + ����������� ��� - ����� x �����(x - ����� �����), ��� ����������� � ��������� ���������, ������ ����� ����� ������� ���������� �� ������ �������, �������� � ����� ���������� ��������;
        + ������� ������ - ����� ������� ��������� ����� ������ ����������� ������� �� ������������ �� �������, ����� �������� �������� ����������� ���������� ������� ������ ���;
    */
    class Scenario: public Field {

        /*!
        \brief ������� ���������� ����
        */
        friend void saveWorld(const char* path, Scenario* current_field, const unsigned int& seed);

        /*!
        \brief ������� �������� ����
        */
        friend void loadWorld(const char* path, Scenario* current_field, unsigned int& seed);
    public:
        

        Scenario(int size_x=0, int size_y=0);

        ///�������� ����
        void makeNew();

        ///������� 1 ��� ���������
        void makeOneStep();

        ///������� ������ ���������� ������� �� ��������� �������� ����, ������������ ������ �� ������ ����. ����� ����� ��������� ���������.
        void spawnCreatures(int amount);
        
        ///������� ����� ����
        void newCycle();


        /*! @name �������
        */
        ///@{
        int getInitialPopulation() { return initial_population_; }
        long long getIteration() { return iteration_; }
        ///@}


        /*!
        *\defgroup �������
        *\brief �������� ������� ������. ��� ������� ��������� � ��������.

        ���� ������� ������ - ����� ������� ��������� ����� ������ ����������� ������� �� ������������ �� �������, ����� �������� �������� ����������� �������(�������) ������ ���.
        *@{
        */


        ///������ ������������� ����� ���������, �� ���������� �� ������� �������� ������ ��� �������� �������=strength
        void makeRewards(Position,Position, float strength);

        ///������� �������
        void giveRewards();

        ///��������� � ����������� ��������� ������� ������.
        void CancelRewardsChange() { std::swap(rewards_, rewards_backup_); }

        ///������� ��� �������.
        void resetRewards();

        ///�������� ����� ������������ ���������� ��������� ������, ��������� �� ������ ���� �������� �������������, �� ������� ����� �������.
        void rewardsEditor(ImVec2 window_pos, ImVec2 window_size, float strength, ImTextureID texture);

        ///�������������� ����������� ������������� ������ � ������� ���������� ������
        void updateRewardsTexture();
        /**@}*/

        ///����� �����, ��������� � public, ��������� ����� ������ ��� ��� ������ ��� � ��� ������.
        int cycle_len_;
    private:

        ///brief ��������� ���������� ������� ������ � ���������� �������
        Position convertInput(ImVec2 begin, ImVec2 input, int square_size);

        ///��������� ���������
        int initial_population_;
            
        ///���������� ��������� ����� ���������.
        long long iteration_;

        ///������� ��������� ������
        std::vector<float> rewards_;

        ///���������� ��������� ������
        std::vector<float> rewards_backup_;

        ///��������� �� �������� �������� � ����� �����
        std::vector<Creature*> survivors_;

        ///�������������� ��� ������� ����, ������������ ��� ������ ������� �� ��������� ��������.
        std::vector<int> positions_;
    };
    
    
}

#endif