#include <iostream>
#include <filesystem>
#include <vector>
#include <openssl/sha.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

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
    char hash[SHA_DIGEST_LENGTH];
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
	
	vector<filesystem::path> pathes;
	vector<string> hashes;
	
	for (auto& entry : filesystem::directory_iterator(directory)) 
	{
		if (filesystem::is_regular_file(entry)) 
		{
			pathes.push_back(entry.path());
			hashes.push_back(CalculateHash(entry.path()));
		}
	}
	for (int i = 0; i < pathes.size(); i++) 
	{
	    for (int j = i + 1; j < pathes.size(); j++) 
	    {
		if (hashes[i] == hashes[j]) 
		{
		    cout << "Found same hashes\n";
		    filesystem::remove(pathes[j]);
		    filesystem::create_hard_link(pathes[i], pathes[j]);
		    pathes.erase(pathes.begin() + j);
		    hashes.erase(hashes.begin() + j);
		    j -= 1;
		    continue;
		}
	    }
	}
	cout<< "Done\n";
	return 0;
}
