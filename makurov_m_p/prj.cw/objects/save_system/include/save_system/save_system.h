
#ifndef EVOLVING_WORLD_2023_Q2_FILESYSYTEM
#define EVOLVING_WORLD_2023_Q2_FILESYSYTEM



#include<fstream>
#include<vector>
#include<string>
#include<cstdio>
#include<utility>

namespace cellworld {


    

    /*!
    *\brief Класс хранения файлов.

    Состоит из 2 частей:
    - Файлы, содержащие информацию.
    - Текстовый файл, хранящий имена файлов с информацией.
    */
    class FileSystem {
    public:
        /// Создаёт экземпляр, с файлом, хранящим имена, с именем name+".txt". "file_names.txt"
        FileSystem(std::string name);
        /// Создаёт экземпляр, с файлом, хранящим имена, с именем "file_names.txt".
        FileSystem();

        /// Добавить в экземпляр, файл с информацией с именем file_name.
        void addFileName(const char* file_name);

        /// Удалить имя файла из массива имён с данным индексом, и удалить сам файл из операционной системы.
        void removeFileName(int index);

        ///Сохранить массив имён в текстовый файл. 
        void saveFileNames();
        ///Загрузить массив имён из текстового файла.
        void loadFileNames();


        ///Проверить валидность каждого из имён файлов.
        void checkFileNames();
        
        ///Проверить наличие данного имени в массиве.
        bool findFileName(const char* file_name);
        
        /// Получить неиспользуемое имя, схожее с данным.
        std::string getValidFileName(std::string file_name);

        /// Доступ к чтению массива имён.
        const std::vector<std::string>& getFileNames() { return files_; }
    private:
        ///Проверить валидность имени с данным индексом.
        void checkFileName(int index);

        ///Имя текстового файла, хранящий имена файлов с информацией.
        std::string store_names_file_;

        ///Массив имён.
        std::vector<std::string> files_;
    };
}

#endif