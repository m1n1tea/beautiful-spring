
#ifndef EVOLVING_WORLD_2023_Q2_FILESYSYTEM
#define EVOLVING_WORLD_2023_Q2_FILESYSYTEM

#include<fstream>
#include<vector>
#include<string>
#include<cstdio>
#include<utility>
#include<filesystem>
namespace cellworld {


    

    /*!
    *\brief ����� �������� ������.

    ������� �� 2 ������:
    - ����� � �������, ���������� ����������.
    - ��������� ����, �������� ����� ������ � �����������.
    */
    class FileSystem {
    public:
        /// ������ ���������, � ������, �������� �����, � ������ txt_name+".txt" � ������, �������� ����� � ������ folder_name.
        FileSystem(std::string txt_name, std::string folder_name);
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
        ///������ � ������ ����� ����� � �������
        const std::string& getFolderName() { return folder_name_; }
    private:
        ///��������� ���������� ����� � ������ ��������.
        void checkFileName(int index);

        ///��� ���������� �����, �������� ����� ������ � �����������.
        std::string store_names_file_;
        ///��� �����,�������� ����
        std::string folder_name_;

        ///������ ���.
        std::vector<std::string> files_;
    };
}

#endif