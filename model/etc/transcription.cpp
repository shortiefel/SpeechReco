#include <fstream>
#include <string>
#include <iostream>

int main()
{
    std::fstream fs;
    fs.open("transcriptiontest.txt", std::ios::out);
    for (int i = 1; i < 76 ; ++i)
    {
        //for each speaker
        for (int j=1; j < 4 ; ++j)
        {
            if (j == 1)
            {
                fs << "<s> JIANBURUFEI </s> ";
            }
            if (j == 2)
            {
                fs << "<s> HUNSHUIMOYU </s> ";
            }
            if (j == 3)
            {
                fs << "<s> HUOMAOSANZHANG </s> ";
            }
            fs << "(" << i << "_0" << j << ")\n";
        }
    }
    if (!fs) std::cout << "file not creted\n";
    fs.close();

    std::fstream nfs;
    nfs.open("fileidtest.txt", std::ios::out);
    for (int i = 1; i < 76 ; ++i)
    {
        //for each speaker
        //speaker_i/i_0j
        for (int j=1; j < 4 ; ++j)
        {
            nfs << "speaker_" << i << "/" << i << "_0" << j << "\n";
        }
    }
    if (!nfs) std::cout << "file not creted\n";
    nfs.close();
    std::cout << "test!\n";
    return 0;
}