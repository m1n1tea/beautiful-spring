
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
    *\brief Класс хранения файлов.

    Состоит из 2 частей:
    - Папка с файлами, содержащие информацию.
    - Текстовый файл, хранящий имена файлов с информацией.
    */
    class FileSystem {
    public:
        /// Создаёт экземпляр, с файлом, хранящим имена, с именем txt_name+".txt" и папкой, хранящей файлы с именем folder_name.
        FileSystem(std::string txt_name, std::string folder_name);
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
        ///Доступ к чтение имени папки с файлами
        const std::string& getFolderName() { return folder_name_; }
    private:
        ///Проверить валидность имени с данным индексом.
        void checkFileName(int index);

        ///Имя текстового файла, хранящий имена файлов с информацией.
        std::string store_names_file_;
        ///Имя папки,хранящяя миры
        std::string folder_name_;

        ///Массив имён.
        std::vector<std::string> files_;
    };
}

#endif