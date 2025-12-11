#include <iostream>
#include <filesystem>
#include <vector>
#include <openssl/sha.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>

using namespace std;

string CalculateHash(const filesystem::path& file_path) 
{
    ifstream file(file_path.string(), ios::binary);
    SHA_CTX context;
    SHA1_Init(&context);
    char buffer[4096];
    while (file.read(buffer, sizeof(buffer)) || file.gcount()) 
    {
	SHA1_Update(&context, buffer, file.gcount());
    }
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1_Final(hash, &context);
    stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) 
    {
        ss << hex << setw(2) << setfill('0') << hash[i];
    }
    return ss.str();
}

int main(int argc, char** argv) 
{ 
	filesystem::path directory = argv[1];
	
	
	map<string, filesystem::path> hashesPathes;
	
	for (auto& entry : filesystem::recursive_directory_iterator(directory)) 
	{
		if (filesystem::is_regular_file(entry)) 
		{
		    string hash = CalculateHash(entry.path());
		    auto mapVal = hashesPathes.find(hash);
                    if (mapVal != hashesPathes.end()) 
                    {
                        cout << "Creating hard link\n";
                        filesystem::remove(entry.path());
                        filesystem::create_hard_link(mapVal->second,entry.path());
                    } else 
                    {
                        hashesPathes[hash] = entry.path();
                    }
               }
	}
	cout<< "Done\n";
	return 0;
}
