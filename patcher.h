#ifndef PATCHER_H
#define PATCHER_H

#include <array>

constexpr std::streamsize  MAX_READ_SIZE(1024 * 32);

/**
 * @brief The Patcher class
 */
class Patcher
{
    public:
        Patcher(const std::string &digestToSearch, const std::string &digest);

        bool patchCompiler();

    private:
        std::streampos findOffset(std::ifstream &file, const std::streamsize &length);
        bool makeBackup(std::ifstream &file, const std::string &fileName, const std::streamsize &lenght);
        bool patchFile(const std::string &fileName, const std::streamsize &offset);

        const std::string &mDigest;
        const std::string &mDigestToSearch;
        static const std::array<std::string, 4>  filesToPatch;
};

#endif // PATCHER_H
