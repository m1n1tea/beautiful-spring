
#ifndef EVOLVING_WORLD_2023_Q2_FILESYSYTEM
#define EVOLVING_WORLD_2023_Q2_FILESYSYTEM



#include<fstream>
#include<vector>
#include<string>
#include<cstdio>
#include<utility>

namespace cellworld {


    

    /*!
    *\brief ����� �������� ������.

    ������� �� 2 ������:
    - �����, ���������� ����������.
    - ��������� ����, �������� ����� ������ � �����������.
    */
    class FileSystem {
    public:
        /// ������ ���������, � ������, �������� �����, � ������ name+".txt". "file_names.txt"
        FileSystem(std::string name);
        /// ������ ���������, � ������, �������� �����, � ������ "file_names.txt".
        FileSystem();

        /// �������� � ���������, ���� � ����������� � ������ file_name.
        void addFileName(const char* file_name);

        /// ������� ��� ����� �� ������� ��� � ������ ��������, � ������� ��� ���� �� ������������ �������.
        void removeFileName(int index);

        ///��������� ������ ��� � ��������� ����. 
        void saveFileNames();
        ///��������� ������ ��� �� ���������� �����.
        void loadFileNames();


        ///��������� ���������� ������� �� ��� ������.
        void checkFileNames();
        
        ///��������� ������� ������� ����� � �������.
        bool findFileName(const char* file_name);
        
        /// �������� �������������� ���, ������ � ������.
        std::string getValidFileName(std::string file_name);

        /// ������ � ������ ������� ���.
        const std::vector<std::string>& getFileNames() { return files_; }
    private:
        ///��������� ���������� ����� � ������ ��������.
        void checkFileName(int index);

        ///��� ���������� �����, �������� ����� ������ � �����������.
        std::string store_names_file_;

        ///������ ���.
        std::vector<std::string> files_;
    };
}

#endif