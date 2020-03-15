#include <cstdlib>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>

namespace bi = boost::interprocess;
typedef bi::allocator<char, bi::managed_mapped_file::segment_manager> char_allocator;
typedef bi::vector<char, char_allocator> mapped_vector;
typedef bi::basic_string<char, std::char_traits<char>, char_allocator> mapped_string;

void writeFileInC(const char *path, const std::string &_str) {
    FILE *fp = fopen(path, "wb");
    if(!fp) {printf("File opening failed");}
    fwrite(&_str[0], sizeof(char), _str.size(), fp);
    fclose(fp);
}

std::vector<char> readFileInC(const char *path, size_t _dataSize) {
    FILE *fp = fopen(path, "rb");
    if(!fp) {printf("File opening failed");}
    std::vector<char> vec;
    vec.resize(_dataSize);
    size_t s = fread(&vec[0], sizeof(std::vector<char>::value_type), vec.size(), fp);
    (void)s;
    fclose(fp);
    return vec;
}

void writeFileInCpp(const char *path, const std::string &_str) {
    std::ofstream os { path, std::ios::out | std::ios::binary};
    os.write(reinterpret_cast<const char*>(&_str[0]), _str.size() * sizeof(char));
    os.close();
}

std::vector<char> readFileInCpp(const char *path, size_t _dataSize) {
    std::ifstream is { path, std::ios::in | std::ios::binary};
    std::vector<char> vec {};
    vec.resize(_dataSize);
    is.read(reinterpret_cast<char*>(&vec[0]), vec.size() * sizeof(char));
    is.close();
    return vec;
}

void writeFileInMMF(const char *path, const std::string &_str) {
    using namespace boost::interprocess;

    managed_mapped_file segment(create_only, path, _str.size() * 2);
    const char_allocator alloc_inst (segment.get_segment_manager());
    mapped_vector *myvector = segment.construct<mapped_vector>
                                               ("mydata")
                                               (_str.c_str()
                                               ,_str.end().base()
                                               ,alloc_inst);
}

void readFileInMMF(const char *path) {
    using namespace boost::interprocess;
    managed_mapped_file mapfile(open_read_only,path);
    mapped_vector * vec = mapfile.find<mapped_vector>("mydata").first;

    std::cerr << "Output datasize = " << vec->size() << " ("
                                      << vec->at(vec->size()-1) << ")\n";
}

// Linux desktop with an Intel® Core™ i7-8550U CPU @ 1.80GHz × 8  processor
// and GCC 8.3.0 with the -O3 flag for tests.
int main(int argc, char *argv[])
{
    using namespace std;
    const auto path = "file121.bin";
    constexpr size_t dataSize {1024 * 1024 * 1024};

    std::string str {};
    str.resize(dataSize, '!');
    std::cerr << "Input data size = " << str.size() <<  "\n";

    auto beginTime = chrono::high_resolution_clock::now();

    // 956962363ns ~ 0,956962363sec
    if((argc > 0) && (!strcmp(argv[1], "c"))) {
        writeFileInC(path, str);
        auto vec = readFileInC(path, dataSize);
        std::cerr << "Output datasize = " << vec.size() << " ("
                                          << vec.at(vec.size()-1) << ")\n";
    }

    // 965643923ns ~ 0,965643923sec
    if((argc > 0) && (!strcmp(argv[1], "cpp"))) {
        writeFileInCpp(path, str);
        std::vector<char> vec = readFileInCpp(path, dataSize);
        std::cerr << "Output datasize = " << vec.size() << " ("
                                          << vec.at(vec.size()-1) << ")\n";
    }

    // 579833105ns ~ 0,579833105sec
    if((argc > 0) && (!strcmp(argv[1], "mmf"))) {
        writeFileInMMF(path, str);
        readFileInMMF(path);
    }

    auto endTime = chrono::high_resolution_clock::now();
    cerr << chrono::duration_cast<chrono::nanoseconds>(endTime-beginTime).count()<<"ns"<< endl;

    return EXIT_SUCCESS;
}
