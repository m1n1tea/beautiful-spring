#include<save_system/save_system.h>

namespace cellworld {

    bool findFile(std::string path) {
        std::ifstream safe_file(path, std::ios::binary);
        if (!safe_file) {
            safe_file.close();
            return false;
        }
        int check;
        safe_file >> check;
        safe_file.close();
        return (check == 1);
    }


    FileSystem::FileSystem(std::string txt_name, std::string folder_name) : store_names_file_(txt_name), folder_name_(folder_name){
        store_names_file_ += ".txt";
        std::filesystem::create_directory(folder_name_);
        loadFileNames();
    }
    FileSystem::FileSystem() : store_names_file_("file_names.txt"), folder_name_("worlds") {
        std::filesystem::create_directory(folder_name_);
        loadFileNames();
    }

    void FileSystem::addFileName(const char* file_name) {
        files_.emplace_back(file_name);
    }

    void FileSystem::removeFileName(int index) {
        std::string path = folder_name_ + '/' + files_[index];
        std::remove(path.c_str());
        files_.erase(files_.begin()+index);
    }


    void FileSystem::saveFileNames() {
        std::ofstream safe_file(store_names_file_);
        safe_file << files_.size() << "\n";
        for (int i=0; i<files_.size();++i)
            safe_file << files_[i] << "\n";
        safe_file.close();
    }

    void FileSystem::loadFileNames() {
        std::ifstream safe_file(store_names_file_);
        if (!safe_file) {
            safe_file.close();
            return;
        }
        int worlds_count;
        safe_file  >> worlds_count;
        char skip;
        safe_file.read(&skip,1);
        files_=std::vector<std::string>(worlds_count);
        for (int i = 0; i < worlds_count; ++i) {
            std::getline(safe_file,files_[i]);
        }
        safe_file.close();
    }

    void FileSystem::checkFileName(int index) {
        std::string path= folder_name_ + '/' + files_[index];
        bool found= findFile(path);
        //std::cout << path << " " << found << "\n";
        if (!found) {
            files_.erase(files_.begin() + index);
        }
    }

    bool FileSystem::findFileName(const char* file_name)
    {   
        for (std::string existing:files_)
            if (file_name==existing)
                return true;
        return false;
    }

    std::string FileSystem::getValidFileName(std::string file_name)
    {
        int number=0;
        std::string base(file_name);
        std::string addition;
        std::string result;
        do {
            ++number;
            addition='('+std::to_string(number)+')';
            result= base + addition;
        }while(findFileName( result.c_str() ));

        return result;
    }

    void FileSystem::checkFileNames() {
        for (int i = files_.size() - 1; i >= 0; --i) {
            checkFileName(i);
        }
    }
}