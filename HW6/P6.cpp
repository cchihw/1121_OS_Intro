/*
Student No.:
Student Name:
Email: 
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not supposed to be posted to a
public server, such as a public GitHub repository or a public web page.
*/
#include<filesystem>
#include<fstream>
#include<iostream>
#include<string>
#include<unistd.h>
#include<unordered_map>
#include<openssl/sha.h>

namespace fs=std::filesystem;
std::string Hash(const std::string& path){
    std::ifstream f(path,std::ios::binary);
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);
    const int bufSize=16384;
    char* buffer=new char[bufSize];
    while(f.read(buffer,bufSize)) {
        // std::cout<<"Read: "<<f.gcount()<<" bytes"<<std::endl;
        SHA1_Update(&sha1,buffer,f.gcount());
        // std::cout<<f.gcount()<<std::endl;
    }
    SHA1_Update(&sha1,buffer,f.gcount());
    // std::cout<<"Read: "<<f.gcount()<<" bytes" <<std::endl;

    delete[] buffer;
    SHA1_Final(hash,&sha1);
    char res[SHA_DIGEST_LENGTH*2+1];
    for(int i=0;i<SHA_DIGEST_LENGTH;i++){
        sprintf(res+i*2,"%02x",hash[i]);
    }
    return std::string(res);
}
int main(int argc, char** argv){
    //implement file deduplication
    //and link or unlink files
    //using SHA1
    // fs::path currentPath = fs::current_path();
    // std::cout <<"Current Working Directory: "<<currentPath<<'\n';
    //testcase path=argv[1]
    std::string path = fs::absolute(argv[1]);
    std::unordered_map<std::string,std::string> m;
    for(const auto& dir_entry:fs::recursive_directory_iterator(path)){
        if(fs::is_regular_file(dir_entry)){
            // std::cout<<dir_entry << '\n';
            std::string hash = Hash(dir_entry.path());
            // std::cout<<hash <<'\n';
            if(m[hash].empty()){
                m[hash] = dir_entry.path();
            }
            else{
                //link and unlink
                std::string path=dir_entry.path();
                std::string link_path=m[hash];
                // std::cout << dir_entry << '\n';
                // std::cout << "link " << path << " to " << link_path << "\n\n";
                unlink(path.c_str());
                link(link_path.c_str(),path.c_str());            
            }
        }
    }

    return 0;
}

